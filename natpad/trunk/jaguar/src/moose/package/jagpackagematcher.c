/*
   File:    jagpackagematcher.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 20, 2013
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

#include "jagpackagematcher.h"
#include "jagpackagecontent.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPackageMatcher"
#include <logging/catlog.h>

struct _JagPackageMatcherPrivate {
	CatStringWo *a_package_name;
};

static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPackageMatcher, jag_package_matcher, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
});


static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_package_matcher_class_init(JagPackageMatcherClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagPackageMatcherPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_package_matcher_init(JagPackageMatcher *instance) {
	JagPackageMatcherPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_PACKAGE_MATCHER, JagPackageMatcherPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPackageMatcher *instance = JAG_PACKAGE_MATCHER(object);
	JagPackageMatcherPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_package_name);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPackageMatcher *jag_package_matcher_new() {
	JagPackageMatcher *result = g_object_new(JAG_TYPE_PACKAGE_MATCHER, NULL);
	cat_ref_anounce(result);
	JagPackageMatcherPrivate *priv = result->priv;
	priv->a_package_name = NULL;
	return result;
}

void jag_package_matcher_set_match_name(JagPackageMatcher *package_matcher, CatStringWo *a_newname) {
	JagPackageMatcherPrivate *priv = JAG_PACKAGE_MATCHER_GET_PRIVATE(package_matcher);
	cat_ref_swap(priv->a_package_name, a_newname);
}


/********************* start MooIContentMap implementation *********************/


static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata) {
	JagPackageMatcherPrivate *priv = JAG_PACKAGE_MATCHER_GET_PRIVATE(self);
//	cat_log_debug("node=%o", node);
	JagPackageContent *package_content = (JagPackageContent *) moo_node_wo_get_content(node, jag_package_content_key());
	gboolean result = FALSE;
	if (package_content!=NULL) {
		CatStringWo *a_package_name = moo_node_wo_get_name(node);
		result = cat_string_wo_equal(a_package_name, priv->a_package_name);
	}
	return result;
}


static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooIContentMap implementation *********************/