/*
   File:    vippath.c
   Project: viper
   Author:  Douwe Vos
   Date:    Jun 6, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "vippath.h"

#include <natdate.h>
#include <stdio.h>

#include "vipicdprovider.h"
//#include "vippathprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipPath"
#include <logging/catlog.h>

#ifdef G_OS_WIN32
#define SEP_CHAR '\\'
#else
#define SEP_CHAR '/'
#endif

struct _VipPathPrivate {
	VipPathDriveType drive_type;
	CatStringWo *drive;

	CatArrayWo *a_entries;
	gboolean is_canonical;
	gboolean is_map;

	int hash_code;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(VipPath, vip_path, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void vip_path_class_init(VipPathClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(VipPathPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void vip_path_init(VipPath *instance) {
	VipPathPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, VIP_TYPE_PATH, VipPathPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%o", object);
	VipPath *instance = VIP_PATH(object);
	VipPathPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_entries);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatS vip_s_txt_current_path = CAT_S_DEF(".");
CatS vip_s_txt_parent_path = CAT_S_DEF("..");


#ifdef G_OS_WIN32
static CatS l_s_txt_unc_prefix = CAT_S_DEF("\\\\");
#else
static CatS l_s_txt_root = CAT_S_DEF("/");
#endif

VipPath *vip_path_new(const CatStringWo *c_full_path) {
	cat_log_debug("full_path=%s", cat_string_wo_getchars(c_full_path));
	VipPath *result = g_object_new(VIP_TYPE_PATH, NULL);
	cat_ref_anounce(result);
	VipPathPrivate *priv = result->priv;

	priv->drive = NULL;
	CatArrayWo *e_split = NULL;
	priv->drive_type = VIP_PATH_DRIVE_CD;
	priv->is_canonical = FALSE;
#ifdef G_OS_WIN32
	if (cat_string_wo_beginswith(c_full_path, CAT_S(l_s_txt_unc_prefix))) {
		priv->is_canonical = TRUE;
		priv->drive_type = VIP_PATH_DRIVE_UNC;
		int sep_idx = cat_string_wo_index_of_from(c_full_path, SEP_CHAR, 2);
		if (sep_idx<0) {
			e_split = cat_array_wo_new();
			priv->drive = cat_ref_ptr(c_full_path);
		} else {
			priv->drive = cat_string_wo_new_sub(c_full_path, 0, sep_idx);
			sep_idx++;
			CatStringWo *new_full_path = cat_string_wo_new_sub(c_full_path, sep_idx, cat_string_wo_length(c_full_path)-sep_idx);
			e_split = cat_string_wo_split(new_full_path, SEP_CHAR, FALSE);
		}
	} else {
		int colon_idx = cat_string_wo_index_of(c_full_path, ':');
		if (colon_idx>0) {
			cat_log_debug("colon_idx=%d", colon_idx);
			priv->drive_type = VIP_PATH_DRIVE_ROOT;
			if (cat_string_wo_char_at(c_full_path, colon_idx+1) == SEP_CHAR) {
				priv->drive_type = VIP_PATH_DRIVE_ROOT_CD;
				priv->drive = cat_string_wo_new_sub(c_full_path, 0, colon_idx+2);
			} else {
				priv->is_canonical = TRUE;
				priv->drive = cat_string_wo_new_sub(c_full_path, 0, colon_idx+1);
			}
			colon_idx++;
			CatStringWo *new_full_path = cat_string_wo_new_sub(c_full_path, colon_idx, cat_string_wo_length(c_full_path)-colon_idx);
			e_split = cat_string_wo_split(new_full_path, SEP_CHAR, FALSE);
			cat_log_debug("drive_letter=%o, rest_path=%o", priv->drive, new_full_path);
			cat_unref_ptr(new_full_path);
		}
	}
#else
	priv->drive = cat_ref_ptr(CAT_S(l_s_txt_root));

	if (cat_string_wo_beginswith_char(c_full_path, SEP_CHAR)) {
		priv->is_canonical = TRUE;
		priv->drive_type = VIP_PATH_DRIVE_ROOT;
	}
#endif


	priv->is_map = cat_string_wo_endswith_char(c_full_path, SEP_CHAR);

	if (e_split == NULL) {
		e_split = cat_string_wo_split(c_full_path, SEP_CHAR, FALSE);
	}

	int hash_code = 0;
	int idx;
	for(idx=0; idx<cat_array_wo_size(e_split); idx++) {
		CatStringWo *sub = (CatStringWo *) cat_array_wo_get(e_split, idx);

		cat_log_trace("sub[%d]=%o", idx, sub);


		if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_current_path))) {
			priv->is_canonical = FALSE;
		} else if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_parent_path))) {
			priv->is_canonical = FALSE;
		}
		hash_code = hash_code*7+cat_string_wo_hash(sub);
	}

	priv->a_entries = cat_array_wo_anchor(e_split, 0);
	priv->hash_code = hash_code;

	cat_unref_ptr(c_full_path);
	cat_log_debug("result=%o", result);
	return result;
}


VipPath *vip_path_new_for_uri(CatStringWo *a_full_uri) {
	if (!cat_string_wo_beginswith(a_full_uri, cat_string_wo_new_with("file://"))) {
		cat_log_error("not a valid URI:%s", cat_string_wo_getchars(a_full_uri));
	} else {
		CatStringWo *sub_uri = cat_string_wo_new_sub(a_full_uri, 7, cat_string_wo_length(a_full_uri)-7);
		a_full_uri = cat_string_wo_anchor(sub_uri, 0);
	}
#ifdef G_OS_WIN32
	CatStringWo *e_converted = cat_string_wo_new();
	int chidx;
	for(chidx=0; chidx<cat_string_wo_length(a_full_uri); chidx++) {
		char ch = cat_string_wo_char_at(a_full_uri, chidx);
		if (ch=='/') {
			cat_string_wo_append_char(e_converted, '\\');
		} else {
			cat_string_wo_append_char(e_converted, ch);
		}
	}
	a_full_uri = cat_string_wo_anchor(e_converted, 0);
#endif

	VipPath *result = vip_path_new(a_full_uri);
	return result;
}

static VipPath *l_new_with(VipPathDriveType drive_type, CatStringWo *drive, CatArrayWo *c_entries, gboolean is_map) {
	VipPath *result = g_object_new(VIP_TYPE_PATH, NULL);
	cat_ref_anounce(result);
	VipPathPrivate *priv = result->priv;
	priv->drive_type = drive_type;
	priv->drive = cat_ref_ptr(drive);
	if (c_entries == NULL) {
		c_entries = cat_array_wo_new();
	}
	priv->a_entries = c_entries;
	priv->a_entries = cat_array_wo_anchor(priv->a_entries, 0);
	priv->is_map = is_map;
	gboolean is_canonical = TRUE;
	CatIIterator *iter = cat_array_wo_iterator(c_entries);
	int hash_code = 0;
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *sub = (CatStringWo *) cat_iiterator_next(iter);
		if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_current_path))) {
			is_canonical = FALSE;
		} else if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_parent_path))) {
			is_canonical = FALSE;
		}
		hash_code = hash_code*7+cat_string_wo_hash(sub);
	}
	cat_unref_ptr(iter);
#ifdef G_OS_WIN32
	if (drive_type == VIP_PATH_DRIVE_ROOT_CD) {
		is_canonical = FALSE;
	}
#endif

	priv->is_canonical = is_canonical;
	priv->hash_code = hash_code;
	cat_log_debug("result=%o", result);

	return result;
}

VipPath *vip_path_create_canonical(VipPath *path, VipICdProvider *cd_provider) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	if (priv->is_canonical) {
		return cat_ref_ptr(path);
	}

//	cat_log_debug("cwd=%s", cat_string_wo_getchars(vip_path_to_string(cwd)));

	CatArrayWo *new_entries = cat_array_wo_new();
	CatStringWo *drive = priv->drive;

	VipPathDriveType drive_type = priv->drive_type;
	VipPath *cwd = NULL;
	switch(drive_type) {
		case VIP_PATH_DRIVE_CD : {
			cwd = vip_icd_provider_get_cd(cd_provider, priv->drive);
		} break;
		case VIP_PATH_DRIVE_ROOT : {
			cwd = vip_icd_provider_get_root(cd_provider, priv->drive);
		} break;
#ifdef G_OS_WIN32
		case VIP_PATH_DRIVE_UNC : {
		} break;
#endif
	}



	if (cwd!=NULL) {
		CatArrayWo *entries = VIP_PATH_GET_PRIVATE(cwd)->a_entries;
		cat_array_wo_append_all(new_entries, entries);
		drive_type = vip_path_get_directory_type(cwd);
		drive = vip_path_get_drive(cwd);
	}


	CatIIterator *iter = cat_array_wo_iterator(priv->a_entries);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *sub = (CatStringWo *) cat_iiterator_next(iter);
		if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_current_path))) {
		} else if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_parent_path))) {
			if (cat_array_wo_size(new_entries)>0) {
				cat_array_wo_remove_last(new_entries, NULL);
			}
		} else {
			cat_array_wo_append(new_entries, (GObject *) sub);
		}
	}
	cat_unref_ptr(iter);
	return l_new_with(drive_type, drive, new_entries, priv->is_map);
}


VipPath *vip_path_new_with(VipPathDriveType drive_type, CatStringWo *drive, CatArrayWo *entries, gboolean is_map) {
	return l_new_with(drive_type, drive, cat_ref_ptr(entries), is_map);
}

VipPath *vip_path_new_from_entries(CatArrayWo *entries) {
	if (cat_array_wo_size(entries)==0) {
		return l_new_with(VIP_PATH_DRIVE_CD, NULL, cat_ref_ptr(entries), TRUE);
	}
	CatStringWo *drive = (CatStringWo *) cat_array_wo_get_first(entries);
	CatArrayWo *new_entries = cat_array_wo_clone(entries, CAT_CLONE_DEPTH_MAIN);
	cat_array_wo_remove_first(new_entries, NULL);
	return l_new_with(VIP_PATH_DRIVE_ROOT, drive, new_entries, TRUE);
}


VipPath *vip_path_create_drive_path(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return l_new_with(priv->drive_type, priv->drive, NULL, TRUE);
}

CatStringWo *vip_path_get_drive(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return priv->drive;
}

CatStringWo *vip_path_get_drive_descriptor(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	CatStringWo *buf = cat_string_wo_new();
#ifdef G_OS_WIN32
	switch(priv->drive_type) {
		case VIP_PATH_DRIVE_CD : {
			cat_string_wo_append_char(buf, '.');
		} break;
		case VIP_PATH_DRIVE_ROOT_CD :
		case VIP_PATH_DRIVE_ROOT :
		case VIP_PATH_DRIVE_UNC : {
			cat_string_wo_append(buf, priv->drive);
		} break;
	}
#else
	switch(priv->drive_type) {
		case VIP_PATH_DRIVE_CD : {
			cat_string_wo_append_char(buf, '.');
		} break;
		case VIP_PATH_DRIVE_ROOT : {
			cat_string_wo_append_char(buf, '/');
		} break;
	}
#endif
	return buf;
}


gboolean vip_path_is_canonical(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return priv->is_canonical;
}

gboolean vip_path_is_a_child(VipPath *path, VipPath *potential_child) {
	g_return_val_if_fail(vip_path_is_canonical(path), FALSE);
	g_return_val_if_fail(vip_path_is_canonical(potential_child), FALSE);
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	VipPathPrivate *priv_child = VIP_PATH_GET_PRIVATE(potential_child);
	if (priv->drive_type!=priv_child->drive_type) {
		return FALSE;
	}
	if (!cat_string_wo_equal(priv->drive, priv_child->drive)) {
		return FALSE;
	}

	int size = cat_array_wo_size(priv->a_entries);
	gboolean result = FALSE;
	if (cat_array_wo_size(priv_child->a_entries)>=size) {
		CatArrayWo *child_trimmed = cat_array_wo_new_sub(priv_child->a_entries, 0, size);
		result = cat_array_wo_equal(priv->a_entries, child_trimmed, (GEqualFunc) cat_string_wo_equal);
		cat_unref_ptr(child_trimmed);
	}
	return result;
}


gboolean vip_path_get_level(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return cat_array_wo_size(priv->a_entries);
}

VipPathDriveType vip_path_get_directory_type(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return priv->drive_type;
}


VipPath *vip_path_create_parent_path(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	if (cat_array_wo_size(priv->a_entries)>1) {
		CatArrayWo *new_entries = cat_array_wo_clone(priv->a_entries, CAT_CLONE_DEPTH_MAIN);
		cat_array_wo_remove_last(new_entries, NULL);
		return l_new_with(priv->drive_type, priv->drive, new_entries, priv->is_map);
	}
	return NULL;
}

VipPath *vip_path_create_child(VipPath *path, const CatStringWo *new_leaf) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	CatArrayWo *new_entries = cat_array_wo_clone(priv->a_entries, CAT_CLONE_DEPTH_MAIN);
	cat_array_wo_append(new_entries, (GObject *) new_leaf);
	return l_new_with(priv->drive_type, priv->drive, new_entries, FALSE);
}


int vip_path_count(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return cat_array_wo_size(priv->a_entries);
}

CatStringWo *vip_path_get_at(VipPath *path, int index) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return (CatStringWo *) cat_array_wo_get(priv->a_entries, index);
}


CatIIterator *vip_path_iterator(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return cat_array_wo_iterator(priv->a_entries);
}


int vip_path_hash(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	return priv->hash_code;
}

CatStringWo *vip_path_get_leaf(VipPath *path) {
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	if (cat_array_wo_size(priv->a_entries)>0) {
		return (CatStringWo *) cat_array_wo_get_last(priv->a_entries);
	}
	return NULL;
}

gboolean vip_path_equal(VipPath *path_a, VipPath *path_b) {
	if (path_a==path_b) {
		return TRUE;
	}
	if (path_a==NULL || path_b==NULL) {
		return FALSE;
	}
	VipPathPrivate *priv_a = VIP_PATH_GET_PRIVATE(path_a);
	VipPathPrivate *priv_b = VIP_PATH_GET_PRIVATE(path_b);

	if (priv_a->is_canonical!=priv_b->is_canonical) {
		return FALSE;
	}
	if (priv_a->drive_type!=priv_b->drive_type) {
		return FALSE;
	}
	if (priv_a->is_map!=priv_b->is_map) {
		return FALSE;
	}
	if (priv_a->hash_code!=priv_b->hash_code) {
		return FALSE;
	}
	if (!cat_string_wo_equal(priv_a->drive, priv_b->drive)) {
		return FALSE;
	}

	if (cat_array_wo_size(priv_a->a_entries)!=cat_array_wo_size(priv_b->a_entries)) {
		return FALSE;
	}
	int idx;
	for(idx=cat_array_wo_size(priv_a->a_entries)-1; idx>=0; idx--) {
		CatStringWo *sub_a = (CatStringWo *) cat_array_wo_get(priv_a->a_entries, idx);
		CatStringWo *sub_b = (CatStringWo *) cat_array_wo_get(priv_b->a_entries, idx);
		if (!cat_string_wo_equal(sub_a, sub_b)) {
			return FALSE;
		}
	}
	return TRUE;
}

CatStringWo *vip_path_to_string(VipPath *path) {
	if (path==NULL) {
		return cat_string_wo_new_with(".");
	}
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);
	cat_log_trace("type:%d, path=%p, entries=%p", priv->drive_type, path, priv->a_entries);
	CatStringWo *e_buf = cat_string_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->a_entries);
	switch(priv->drive_type) {
		case VIP_PATH_DRIVE_CD : {
			cat_string_wo_append_char(e_buf, '.');
		} break;
		case VIP_PATH_DRIVE_ROOT : {
			cat_string_wo_append(e_buf, priv->drive);
		} break;
#ifdef G_OS_WIN32
		case VIP_PATH_DRIVE_ROOT_CD : {
			cat_string_wo_append(e_buf, priv->drive);
		} break;
		case VIP_PATH_DRIVE_UNC : {
			cat_string_wo_append(e_buf, priv->drive);
		} break;
#endif
	}

	if(cat_iiterator_has_next(iter)) {
		CatStringWo *sub = (CatStringWo *) cat_iiterator_next(iter);
		cat_string_wo_append(e_buf, sub);
	}

	while(cat_iiterator_has_next(iter)) {
		CatStringWo *sub = (CatStringWo *) cat_iiterator_next(iter);
		cat_string_wo_append_char(e_buf, SEP_CHAR);
		cat_string_wo_append(e_buf, sub);
	}
	cat_unref_ptr(iter);

	if (priv->is_map) {
		if (!cat_string_wo_endswith_char(e_buf, SEP_CHAR)) {
			cat_string_wo_append_char(e_buf, SEP_CHAR);
		}
	}

	return e_buf;
}



// TODO: what to do if path is not canonical
CatStringWo *vip_path_as_uri(VipPath *path) {
	if (path==NULL) {
		return cat_string_wo_new_with("file:///");
	}
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(path);

	if (!priv->is_canonical) {
		CatStringWo *ps = vip_path_to_string(path);
		cat_log_error("path is not canonical:%p %s", path, cat_string_wo_getchars(ps));
		cat_unref_ptr(ps);
	}
	CatIIterator *iter = cat_array_wo_iterator(priv->a_entries);
	CatStringWo *e_buf = cat_string_wo_new_with("file://");
//	cat_string_wo_append_char(e_buf, '/');
	gboolean is_first = TRUE;
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *sub = (CatStringWo *) cat_iiterator_next(iter);
		if (is_first) {


// FIXME: does not support windows UNC end drive letters.
			is_first = FALSE;
			if (cat_string_wo_equal(sub, CAT_S(vip_s_txt_current_path)) || (cat_string_wo_equal(sub, CAT_S(vip_s_txt_parent_path)))) {
			} else {
				cat_string_wo_append_char(e_buf, '/');
			}
		} else {
			cat_string_wo_append_char(e_buf, '/');
		}
		cat_string_wo_append(e_buf, sub);
	}
	cat_unref_ptr(iter);
	return e_buf;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	VipPath *instance = VIP_PATH(self);
	VipPathPrivate *priv = VIP_PATH_GET_PRIVATE(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	CatStringWo *fpt = vip_path_to_string(instance);
	cat_string_wo_format(append_to, "%s[%p:canonical=%s, path=%s, drive-type=%d, drive=%o]", iname, self, priv->is_canonical ? "true" : "false", cat_string_wo_getchars(fpt), priv->drive_type, priv->drive);
	cat_unref_ptr(fpt);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

