/*
   File:    mooresourcenamematcher.c
   Project: moose
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

#include "mooresourcenamematcher.h"
#include "../mooresourcecontentwo.h"
#include "../moonodewo.h"
#include "mooinodematcher.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooResourceNameMatcher"
#include <logging/catlog.h>


static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooResourceNameMatcher, moo_resource_name_matcher, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_resource_name_matcher_class_init(MooResourceNameMatcherClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_resource_name_matcher_init(MooResourceNameMatcher *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(moo_resource_name_matcher_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_resource_name_matcher_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooResourceNameMatcher *moo_resource_name_matcher_new() {
	MooResourceNameMatcher *result = g_object_new(MOO_TYPE_RESOURCE_NAME_MATCHER, NULL);
	cat_ref_anounce(result);
	return result;
}


/********************* start MooIContentMap implementation *********************/

static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata) {
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	gboolean result = FALSE;
	if (resource_content!=NULL) {
		CatStringWo *a_res_name = (CatStringWo *) userdata;
		VipNode *viper_node = moo_resource_content_wo_get_viper_node(resource_content);
		if (viper_node) {
			CatStringWo *a_vip_node_name = vip_node_get_name(viper_node);
			result = cat_string_wo_equal(a_vip_node_name, a_res_name);
		}
	}
	return result;
}

static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooIContentMap implementation *********************/
