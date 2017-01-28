/*
   File:    jaglibrarypathmatcher.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
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

#include "jaglibrarypathmatcher.h"
#include "jaglibrarycontentwo.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLibraryPathMatcher"
#include <logging/catlog.h>

struct _JagLibraryPathMatcherPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagLibraryPathMatcher, jag_library_path_matcher, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagLibraryPathMatcher)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_library_path_matcher_class_init(JagLibraryPathMatcherClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_library_path_matcher_init(JagLibraryPathMatcher *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagLibraryPathMatcher *instance = JAG_LIBRARY_PATH_MATCHER(object);
//	JagLibraryPathMatcherPrivate *priv = jag_library_path_matcher_get_instance_private(instance);
	G_OBJECT_CLASS(jag_library_path_matcher_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_library_path_matcher_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagLibraryPathMatcher *jag_library_path_matcher_new() {
	JagLibraryPathMatcher *result = g_object_new(JAG_TYPE_LIBRARY_PATH_MATCHER, NULL);
	cat_ref_anounce(result);
	return result;
}


/********************* start MooIContentMap implementation *********************/

static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer vipnodeasobj) {
	cat_log_debug("matching:node=%o", node);
	JagLibraryContentWo *content = (JagLibraryContentWo *) moo_node_wo_get_content(node, jag_library_content_wo_key());
	gboolean result = FALSE;
	if (content!=NULL) {
		VipNode *vip_node_arg = VIP_NODE(vipnodeasobj);
		CatTreeNode *lib_tn = jag_library_content_wo_get_vip_node(content);
		VipNode *vip_node_lib = (VipNode *) cat_tree_node_get_content(lib_tn);
		if (vip_node_lib!=NULL && vip_node_arg!=NULL) {
			result = vip_node_get_id(vip_node_arg)==vip_node_get_id(vip_node_lib);
		}
	}
	return result;
}


static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooIContentMap implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







