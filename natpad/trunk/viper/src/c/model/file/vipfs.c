/*
   File:    vipfs.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 16, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "vipfs.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipFS"
#include <logging/catlog.h>

#ifdef G_OS_WIN32
#define SEP_CHAR '\\'
#else
#define SEP_CHAR '/'
#endif

struct _VipFSPrivate {
	VipPath *path;
	VipFSType type_of_file;
	gboolean is_readable;
	int mode;
#ifdef G_OS_WIN32
	time_t new_mtime;
#else
	struct timespec new_mtime;
#endif
	long long length;

	CatAtomicReference *dir_content_ref;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipFS, vip_fs, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_fs_class_init(VipFSClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipFSPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_fs_init(VipFS *instance) {
	VipFSPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_FS, VipFSPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	VipFS *instance = VIP_FS(object);
	VipFSPrivate *priv = instance->priv;
	cat_unref_ptr(priv->path);
	cat_unref_ptr(priv->dir_content_ref);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


VipFS *vip_fs_new(VipPath *fs_path) {
	VipFS *result = g_object_new(VIP_TYPE_FS, NULL);
	cat_ref_anounce(result);
	VipFSPrivate *priv = result->priv;
	priv->path = cat_ref_sink_ptr(fs_path);

	priv->type_of_file = VIP_FS_TYPE_NON_EXISTENT;
	priv->is_readable = FALSE;
	priv->mode = 0;
#ifdef G_OS_WIN32
	priv->new_mtime = 0;
#else
	priv->new_mtime.tv_nsec=0;
	priv->new_mtime.tv_sec=0;
#endif
	priv->length = 0;
	priv->dir_content_ref = NULL;
	vip_fs_refresh(result);
	return result;
}

gboolean vip_fs_refresh(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);

	cat_log_detail("path=%O", priv->path);

	CatStringWo *fs_name = vip_path_to_string(priv->path);
	cat_log_detail("fs_name=%O", fs_name);
	const char *fname = cat_string_wo_getchars(fs_name);
	cat_log_detail("fs=%p fname=%s", fs, fname);

	VipFSType new_type_of_file = VIP_FS_TYPE_NON_EXISTENT;
	gboolean rf_is_readable = FALSE;
	int rf_mode = 0;
#ifdef G_OS_WIN32
	time_t rf_new_mtime = {0};
#else
	struct timespec rf_new_mtime = {0};
#endif
	long long rf_length = 0;
	gboolean rf_has_dir_content = FALSE;


	struct stat statinfo;
	if (stat(fname, &statinfo)!=-1) {

#ifdef G_OS_UNIX
		if ((statinfo.st_mode & S_IFMT) == S_IFLNK) {
			gchar *hardfile = g_malloc0(2048);
			cat_log_detail("hardfile = %s", hardfile);
			int len = readlink(fname, hardfile, 2048-1);
			stat(hardfile, &statinfo);
			cat_free_ptr(hardfile);
		}
#endif



		cat_log_detail("mode = %d", statinfo.st_mode & S_IFMT);

		switch (statinfo.st_mode & S_IFMT) {
			case S_IFDIR:  {
				new_type_of_file = VIP_FS_TYPE_DIRECTORY;
			} break;
			case S_IFREG : {
				new_type_of_file = VIP_FS_TYPE_PLAIN;
			} break;
			default : {
			} break;
		}
		cat_log_detail("mode = %d", (statinfo.st_mode & S_IFMT));

		if (new_type_of_file!=VIP_FS_TYPE_NON_EXISTENT) {
			int read_access = access(fname, R_OK);
			if (read_access == 0) {
				rf_is_readable = TRUE;
			}

			rf_mode = statinfo.st_mode;
#ifdef G_OS_WIN32
			rf_new_mtime = statinfo.st_mtime;
#else
			rf_new_mtime = statinfo.st_mtim;
#endif
			rf_length = statinfo.st_size;

			if (new_type_of_file==VIP_FS_TYPE_DIRECTORY) {
				rf_has_dir_content = TRUE;
			}
		}
	} else {
		cat_log_detail("no stat available fot path:%s", fname);
	}
	cat_unref_ptr(fs_name);


	gboolean result = FALSE;
	if (priv->is_readable!=rf_is_readable) {
		priv->is_readable=rf_is_readable;
		result = TRUE;
	}
	if (priv->length != rf_length) {
		priv->length = rf_length;
		result = TRUE;
	}
	if (priv->mode != rf_mode) {
		priv->mode = rf_mode;
		result = TRUE;
	}

#ifdef G_OS_WIN32
	if (priv->new_mtime != rf_new_mtime) {
#else
	if ((priv->new_mtime.tv_nsec != rf_new_mtime.tv_nsec) ||
			(priv->new_mtime.tv_sec != rf_new_mtime.tv_sec)) {
#endif
		priv->new_mtime = rf_new_mtime;
		result = TRUE;
	}
	if (priv->type_of_file != new_type_of_file) {
		priv->type_of_file = new_type_of_file;
		result = TRUE;
	}


	if (priv->dir_content_ref==NULL) {
		if (rf_has_dir_content) {
			priv->dir_content_ref = cat_atomic_reference_new();
			result = TRUE;
		}
	} else {
		if (!rf_has_dir_content) {
			cat_unref_ptr(priv->dir_content_ref);
			result = TRUE;
		} else if (result) {
			cat_atomic_reference_set(priv->dir_content_ref, NULL);
		}
	}
	return result;
}


long long vip_fs_get_last_modified(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);
#ifdef G_OS_WIN32
	long long result = priv->new_mtime * 1000;
#else
	long long result = priv->new_mtime.tv_sec*1000;
	long long ms = priv->new_mtime.tv_nsec/1000000;
	result = result + (ms % 1000);
#endif
	return result;
}


long long vip_fs_get_length(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);
	return priv->length;
}

VipFSType vip_fs_get_type_of_file(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);
	return priv->type_of_file;
}


VipPath *vip_fs_get_path(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);
	return priv->path;
}


CatArrayWo *vip_fs_get_dir_content(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);
	CatArrayWo *result = NULL;
	if (priv->dir_content_ref) {
		result = (CatArrayWo *) cat_atomic_reference_get(priv->dir_content_ref);
		if (result==NULL) {
			result = cat_array_wo_new();
			CatStringWo *fs_name = vip_path_to_string(priv->path);
			const char *fname = cat_string_wo_getchars(fs_name);
			cat_log_debug("enlisting files in:%s", fname);
			DIR *dir = opendir(fname);
			if (dir) {
				while(TRUE) {
					struct dirent *direntry = readdir(dir);
					if (direntry==NULL) {
						break;
					}



#ifdef G_OS_UNIX
					cat_log_trace("direntry:%s, type=%c", direntry->d_name, direntry->d_type);
					if ((direntry->d_type==DT_DIR) ||
							(direntry->d_type==DT_REG) ||
							(direntry->d_type==DT_UNKNOWN)) {
#endif
						if(strcmp(direntry->d_name, "..")==0 || strcmp(direntry->d_name, ".")==0) {
							continue;
						}

						cat_log_trace("enlisted:%s", direntry->d_name);

						CatStringWo *child_name = cat_string_wo_new_with(direntry->d_name);
						cat_log_trace("child_name:%O", child_name);
						VipPath *child_path = vip_path_create_child(priv->path, child_name);
						VipFS *child_fs = vip_fs_new(child_path);
						cat_log_trace("child_fs:%O", child_fs);
						cat_unref_ptr(child_path);
						cat_array_wo_append(result, (GObject *) child_fs);
						cat_unref_ptr(child_fs);
						cat_unref_ptr(child_name);
#ifdef G_OS_UNIX
					} else if (direntry->d_type==DT_LNK) {
						CatStringWo *tmp_fp = cat_string_wo_new_with(fname);
						if (!cat_string_wo_endswith_char(tmp_fp, SEP_CHAR)) {
							cat_string_wo_append_char(tmp_fp, SEP_CHAR);
						}
						cat_string_wo_append_chars(tmp_fp, direntry->d_name);

						cat_log_trace("tmp_fp=%o", tmp_fp);

						gchar *hardfile = g_malloc0(2048);
						int len = readlink(cat_string_wo_getchars(tmp_fp), hardfile, 2048-1);
						if (len>=0) {
							cat_log_trace("hardfile=%s", hardfile);
							CatStringWo *hf_wo = cat_string_wo_new_with(hardfile);
							if (!cat_string_wo_beginswith(tmp_fp, hf_wo)) {
								CatStringWo *child_name = cat_string_wo_new_with(direntry->d_name);
								VipPath *child_path = vip_path_create_child(priv->path, child_name);
								VipFS *child_fs = vip_fs_new(child_path);
								cat_unref_ptr(child_path);
								cat_array_wo_append(result, (GObject *) child_fs);
								cat_unref_ptr(child_fs);
								cat_unref_ptr(child_name);
							}
							cat_unref_ptr(hf_wo);
						} else {
							cat_log_error("error when executing readlink() on %o", tmp_fp);
						}
						cat_free_ptr(hardfile);
						cat_unref_ptr(tmp_fp);
					}
#endif
				}
				closedir(dir);
			}
			cat_atomic_reference_set(priv->dir_content_ref, (GObject *) result);
			cat_unref_ptr(fs_name);
		}
	}
	return result;
}

VipFS *vip_fs_create_file(VipFS *fs, CatStringWo *new_file_name) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);

	VipPath *child_path = vip_path_create_child(priv->path, new_file_name);

	CatStringWo *fs_name = vip_path_to_string(child_path);
	const char *fname = cat_string_wo_getchars(fs_name);


	// TODO: looks like nonsense create and immediately closing a file
#ifdef G_OS_UNIX
	int res = open(fname, O_CREAT, DEFFILEMODE);
	if (res==-1) {
		cat_unref_ptr(fs_name);
		cat_unref_ptr(child_path);
		// TODO: error handling
		return NULL;
	}
	close(res);
#else
	HANDLE file_handle = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle==NULL) {
		cat_unref_ptr(fs_name);
		cat_unref_ptr(child_path);
		// TODO: error handling
		return NULL;
	}
	CloseHandle(file_handle);
#endif

	VipFS *child_fs = vip_fs_new(child_path);
	cat_unref_ptr(fs_name);
	cat_unref_ptr(child_path);
	cat_atomic_reference_set(priv->dir_content_ref, NULL);
	return child_fs;
}


void vip_fs_clear_dir_content_cache(VipFS *fs) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(fs);
	if (priv->dir_content_ref) {
		cat_atomic_reference_set(priv->dir_content_ref, NULL);
	}
}










CatFileInputStream *vip_fs_open_input_stream(CatStringWo *fs_name) {
	CatFileInputStream *result = NULL;
	result = cat_file_input_stream_new(fs_name);
	cat_file_input_stream_read_length(result, NULL, 0);
	CatStreamState status = cat_file_input_stream_get_status(result);
	if (status!=STREAM_OPENED) {
		cat_unref_ptr(result);
	}

	return result;
}

CatFileOutputStream *vip_fs_open_output_stream(CatStringWo *fs_name) {
	CatFileOutputStream *result = NULL;
	result = cat_file_output_stream_new(fs_name);
	int count = cat_file_output_stream_write_length(result, NULL, 0);
	if (count<0) {
		cat_unref_ptr(result);
	}
	return result;
}

gboolean vip_fs_equal(VipFS *fs_a, VipFS *fs_b) {
	if (fs_a==fs_b) {
		return TRUE;
	}
	if (fs_a==NULL || fs_b==NULL) {
		return FALSE;
	}
	VipFSPrivate *priv_a = VIP_FS_GET_PRIVATE(fs_a);
	VipFSPrivate *priv_b = VIP_FS_GET_PRIVATE(fs_b);
	return priv_a->is_readable==priv_b->is_readable
			&& priv_a->length==priv_b->length
			&& priv_a->mode==priv_b->mode
#ifdef G_OS_WIN32
			&& priv_a->new_mtime==priv_b->new_mtime
#else
			&& priv_a->new_mtime.tv_sec==priv_b->new_mtime.tv_sec
			&& priv_a->new_mtime.tv_nsec==priv_b->new_mtime.tv_nsec
#endif
			&& priv_a->type_of_file==priv_b->type_of_file
			&& vip_path_equal(priv_a->path, priv_b->path);

}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipFSPrivate *priv = VIP_FS_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:path=%o, tof=%d, length=%ld, mode=%d]", iname, self, vip_path_get_leaf(priv->path), priv->type_of_file, priv->length, priv->mode);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
