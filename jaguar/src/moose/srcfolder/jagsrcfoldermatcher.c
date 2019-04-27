/*
   File:    jagsrcfoldermatcher.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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

#include "jagsrcfoldermatcher.h"
#include "jagsrcfoldercontentwo.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcFolderMatcher"
#include <logging/catlog.h>

static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagSrcFolderMatcher, jag_src_folder_matcher, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_src_folder_matcher_class_init(JagSrcFolderMatcherClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_src_folder_matcher_init(JagSrcFolderMatcher *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_src_folder_matcher_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_src_folder_matcher_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagSrcFolderMatcher *jag_src_folder_matcher_new() {
	JagSrcFolderMatcher *result = g_object_new(JAG_TYPE_SRC_FOLDER_MATCHER, NULL);
	cat_ref_anounce(result);
	return result;
}


/********************* start MooIContentMap implementation *********************/

static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata) {
	JagSrcFolderContentWo *src_folder_content = (JagSrcFolderContentWo *) moo_node_wo_get_content(node, jag_src_folder_content_wo_key());
	gboolean result = FALSE;
	if (src_folder_content!=NULL) {
		result = cat_string_wo_equal(moo_node_wo_get_name(node), (CatStringWo *) userdata);
	}
	return result;
}

static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooIContentMap implementation *********************/
