/*
   File:    jagclassfilecontent.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 23, 2013
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

#include "jagsrcfilecontentwo.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcfileContentWo"
#include <logging/catlog.h>

struct _WoInfo {
	JagSrcfileContentWo *original;
	gboolean marked;
};

typedef struct _WoInfo WoInfo;

struct _JagSrcfileContentWoPrivate {
	int version;
	MooIdPath *srcfilePath;
	VipIFile *linked_file;

	JagIndexSourceContent *index_source_content;
	WoInfo *wo_info;
};


static CatS jag_s_srcfile_content_key = CAT_S_DEF("JagSrcfileContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagSrcfileContentWo, jag_srcfile_content_wo, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});


static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_srcfile_content_wo_class_init(JagSrcfileContentWoClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagSrcfileContentWoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_srcfile_content_wo_init(JagSrcfileContentWo *instance) {
	JagSrcfileContentWoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_SRCFILE_CONTENT_WO, JagSrcfileContentWoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagSrcfileContentWo *instance = JAG_SRCFILE_CONTENT_WO(object);
	JagSrcfileContentWoPrivate *priv = instance->priv;
	cat_unref_ptr(priv->srcfilePath);
	cat_unref_ptr(priv->linked_file);
	cat_unref_ptr(priv->index_source_content);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagSrcfileContentWo *jag_srcfile_content_wo_new(MooIdPath *classfilePath, VipIFile *linked_file) {
	JagSrcfileContentWo *result = g_object_new(JAG_TYPE_SRCFILE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagSrcfileContentWoPrivate *priv = result->priv;
	priv->version = 1;
	priv->srcfilePath = cat_ref_sink_ptr(classfilePath);
	priv->linked_file = cat_ref_ptr(linked_file);
	priv->index_source_content = jag_index_source_content_new();

	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->original = NULL;
	priv->wo_info->marked = FALSE;
	return result;
}



JagSrcfileContentWo *jag_srcfile_content_wo_ensure_editable(JagSrcfileContentWo *content) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content);
	if (priv->wo_info) {
		return cat_ref_ptr(content);
	}

	JagSrcfileContentWo *result = g_object_new(JAG_TYPE_SRCFILE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagSrcfileContentWoPrivate *dpriv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(result);
	dpriv->srcfilePath = cat_ref_ptr(priv->srcfilePath);
	dpriv->linked_file = cat_ref_ptr(priv->linked_file);
	dpriv->index_source_content = cat_ref_ptr(priv->index_source_content);
	dpriv->version = priv->version;
	dpriv->wo_info = g_new0(WoInfo, 1);
	dpriv->wo_info->original = cat_ref_ptr(content);
	dpriv->wo_info->marked = FALSE;
	return result;
}



gboolean jag_srcfile_content_wo_is_fixed(JagSrcfileContentWo *content) {
	return JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content)->wo_info==NULL;
}


int jag_srcfile_content_wo_get_version(JagSrcfileContentWo *content) {
	return JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content)->version;
}


JagSrcfileContentWo *jag_srcfile_content_wo_anchor(JagSrcfileContentWo *content, int version) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content);
	if (priv->wo_info==NULL) {
		return content;
	}
	gboolean was_modified = TRUE;
	if (priv->wo_info->original) {
		JagSrcfileContentWoPrivate *opriv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(priv->wo_info->original);
		was_modified = priv->wo_info->marked
				|| priv->linked_file != opriv->linked_file;
	}

	if (was_modified) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
		priv->version = version;
		return content;
	}

	JagSrcfileContentWo *result = priv->wo_info->original;
	priv->wo_info->original = NULL;
	cat_unref_ptr(content);
	return result;
}



#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", e_content); \
			return rval; \
		} \

void jag_srcfile_content_wo_mark(JagSrcfileContentWo *e_content) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(e_content);
	CHECK_IF_WRITABLE()
	priv->wo_info->marked = TRUE;
}




MooIdPath *jag_srcfile_content_wo_get_file_path(JagSrcfileContentWo *content) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content);
	return priv->srcfilePath;
}

VipIFile *jag_srcfile_content_wo_get_file(JagSrcfileContentWo *content) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content);
	return priv->linked_file;
}

void jag_srcfile_content_wo_set_file(JagSrcfileContentWo *e_content, VipIFile *file) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(e_content);
	CHECK_IF_WRITABLE()
	cat_ref_swap(priv->linked_file, file);

}


JagIndexSourceContent *jag_srcfile_content_wo_get_index_context(JagSrcfileContentWo *content) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(content);
	return priv->index_source_content;
}


CatStringWo *jag_srcfile_content_wo_key() {
	return CAT_S(jag_s_srcfile_content_key);
}


/********************* start MooIContent implementation *********************/


static CatStringWo *l_content_get_key(MooIContent *self) {
	return jag_srcfile_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) jag_srcfile_content_wo_anchor((JagSrcfileContentWo *) self, version);
}



static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagSrcfileContentWoPrivate *priv = JAG_SRCFILE_CONTENT_WO_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:version=%d,linked-file=%p, %s]", iname, self, priv->version, priv->linked_file, priv->wo_info ? "editable" : "anchored");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
