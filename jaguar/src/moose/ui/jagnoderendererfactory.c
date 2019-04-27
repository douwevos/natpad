/*
   File:    jagnoderendererfactory.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 6, 2013
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

#include "jagnoderendererfactory.h"
#include "jagjarnoderenderer.h"
#include "../srcfolder/jagsrcfolderrenderer.h"
#include "../srcfolder/jagsrcfoldercontentwo.h"
#include "jagjrerenderer.h"
#include "../package/jagpackagerenderer.h"
#include "../package/jagpackagecontent.h"
#include "jagclassfilerenderer.h"
#include "../srcfile/jagsrcfilecontentwo.h"
#include "../library/jaglibrarycontentwo.h"
#include "jaglibraryrenderer.h"
#include <moose.h>
#include <mooseexplorer.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagNodeRendererFactory"
#include <logging/catlog.h>

struct _JagNodeRendererFactoryPrivate {
	JagJarNodeRenderer *jarRenderer;
	JagSrcFolderRenderer *srcFolderRenderer;
	JagPackageRenderer *packageRenderer;
	JagClassfileRenderer *classfileRenderer;
	JagJreRenderer *jre_renderer;
	JagLibraryRenderer *library_renderer;
};

static void l_renderer_factory_iface_init(MooINodeRendererFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagNodeRendererFactory, jag_node_renderer_factory, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagNodeRendererFactory)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER_FACTORY, l_renderer_factory_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_node_renderer_factory_class_init(JagNodeRendererFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_node_renderer_factory_init(JagNodeRendererFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagNodeRendererFactory *instance = JAG_NODE_RENDERER_FACTORY(object);
	JagNodeRendererFactoryPrivate *priv = jag_node_renderer_factory_get_instance_private(instance);
	cat_unref_ptr(priv->classfileRenderer);
	cat_unref_ptr(priv->jarRenderer);
	cat_unref_ptr(priv->jre_renderer);
	cat_unref_ptr(priv->packageRenderer);
	cat_unref_ptr(priv->srcFolderRenderer);
	cat_unref_ptr(priv->library_renderer);
	G_OBJECT_CLASS(jag_node_renderer_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_node_renderer_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagNodeRendererFactory *jag_node_renderer_factory_new() {
	JagNodeRendererFactory *result = g_object_new(JAG_TYPE_NODE_RENDERER_FACTORY, NULL);
	cat_ref_anounce(result);
	JagNodeRendererFactoryPrivate *priv = jag_node_renderer_factory_get_instance_private(result);
	priv->jarRenderer = jag_jar_node_renderer_new();
	priv->srcFolderRenderer = jag_src_folder_renderer_new();
	priv->packageRenderer = jag_package_renderer_new();
	priv->classfileRenderer = jag_classfile_renderer_new();
	priv->jre_renderer = jag_jre_renderer_new();
	priv->library_renderer = jag_library_renderer_new();
	return result;
}

/********************* begin MooINodeRendererFactory implementation *********************/

static CatS l_s_txt_jar = CAT_S_DEF(".jar");
static CatS l_s_txt_ear = CAT_S_DEF(".ear");
static CatS l_s_txt_war = CAT_S_DEF(".war");
static CatS l_s_txt_java = CAT_S_DEF(".java");


static struct _MooINodeRenderer *l_request_renderer_for_node(MooINodeRendererFactory *self, struct _MooNodeWo *node) {
	struct _MooINodeRenderer *result = NULL;
	JagNodeRendererFactory *instance = JAG_NODE_RENDERER_FACTORY(self);
	JagNodeRendererFactoryPrivate *priv = jag_node_renderer_factory_get_instance_private(instance);

	if (moo_node_wo_get_content(node, moo_resource_content_wo_key())!=NULL) {
		CatStringWo *a_name = moo_node_wo_get_name(node);				// TODO name should be taken from resource-content
		if (cat_string_wo_endswith(a_name, CAT_S(l_s_txt_jar))
				|| cat_string_wo_endswith(a_name, CAT_S(l_s_txt_ear))
				|| cat_string_wo_endswith(a_name, CAT_S(l_s_txt_war))) {
			return (struct _MooINodeRenderer *) priv->jarRenderer;
		} else if (cat_string_wo_endswith(a_name, CAT_S(l_s_txt_java))) {
			return (struct _MooINodeRenderer *) priv->classfileRenderer;
		}
	} else if (moo_node_wo_get_content(node, jag_src_folder_content_wo_key())!=NULL) {
		return (struct _MooINodeRenderer *) priv->srcFolderRenderer;
	} else if (moo_node_wo_get_content(node, jag_package_content_key())!=NULL) {
		return (struct _MooINodeRenderer *) priv->packageRenderer;
	} else if (moo_node_wo_get_content(node, jag_srcfile_content_wo_key())!=NULL) {
		return (struct _MooINodeRenderer *) priv->classfileRenderer;
	} else if (moo_node_wo_get_content(node, jag_library_content_wo_key())!=NULL) {
		return (struct _MooINodeRenderer *) priv->library_renderer;
	} else {
		MooContentMapWo *content_map = moo_node_wo_get_content_map(node);
		GObject *obj = moo_content_map_wo_get_raw(content_map, cat_string_wo_new_with("JagJre"));
		if (obj) {
			return (struct _MooINodeRenderer *) priv->jre_renderer;
		}
	}
	return result;
}

static void l_renderer_factory_iface_init(MooINodeRendererFactoryInterface *iface) {
	iface->requestRendererForNode = l_request_renderer_for_node;
}

/********************* end MooINodeRendererFactory implementation *********************/
