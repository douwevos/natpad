/*
   File:    jagclassfileexpander.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 28, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "jagclassfileexpander.h"
#include "jagreadclassfilerequest.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagClassfileExpander"
#include <logging/catlog.h>

struct _JagClassfileExpanderPrivate {
	WorQueue *queue;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_node_expander_iface_init(MooINodeExpanderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagClassfileExpander, jag_classfile_expander, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagClassfileExpander)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_EXPANDER, l_node_expander_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_classfile_expander_class_init(JagClassfileExpanderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_classfile_expander_init(JagClassfileExpander *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagClassfileExpander *instance = JAG_CLASSFILE_EXPANDER(object);
	JagClassfileExpanderPrivate *priv = jag_classfile_expander_get_instance_private(instance);
	cat_unref_ptr(priv->queue);
	G_OBJECT_CLASS(jag_classfile_expander_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_classfile_expander_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagClassfileExpander *jag_classfile_expander_new(WorQueue *queue) {
	JagClassfileExpander *result = g_object_new(JAG_TYPE_CLASSFILE_EXPANDER, NULL);
	cat_ref_anounce(result);
	JagClassfileExpanderPrivate *priv = jag_classfile_expander_get_instance_private(result);
	priv->queue = cat_ref_ptr(queue);
	return result;
}

/********************* start MooINodeExpander implementation *********************/


static gboolean l_expander_expand(MooINodeExpander *self, struct _MooNodeWo *moo_node) {
//	JagClassfileExpanderPrivate *priv = jag_classfile_expander_get_instance_private(self);

//	JagReadClassfileRequest *request = jag_read_classfile_request_new(moo_node);
//	wor_queue_post(priv->queue, (WorRequest *) request);
//	cat_unref_ptr(request);

//	jag_read_cla

//	moo_node_wo_get_content(moo_node, )

	//				if (cat_string_wo_endswith(child_name,CAT_S(l_s_txt_dot_class))) {
	//					VipIFile *res_file = (VipIFile *) vip_node_get_content(child_vip_node);
	//					MooNodeWo *e_classfile_indexed = l_index_classfile(priv, res_file);
	//					if (e_classfile_indexed) {
	//						moo_node_wo_append_child(e_child_moo_node, e_classfile_indexed);
	//						cat_unref_ptr(e_classfile_indexed);
	//					}
	//				}


	return TRUE;
}

static void l_expander_collapse(MooINodeExpander *self, struct _MooNodeWo *moo_node) {

}


static void l_node_expander_iface_init(MooINodeExpanderInterface *iface) {
	iface->expand = l_expander_expand;
	iface->collapse = l_expander_collapse;
}


/********************* end CatIStringable implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






