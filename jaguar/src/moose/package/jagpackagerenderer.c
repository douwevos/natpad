/*
   File:    jagpackagerenderer.c
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

#include "jagpackagerenderer.h"
#include "../srcfolder/jagsrcfolderrenderer.h"
#include <moose.h>
#include <mooseexplorer.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPackageRenderer"
#include <logging/catlog.h>

static void l_renderer_iface_init(MooINodeRendererInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPackageRenderer, jag_package_renderer, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_renderer_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_package_renderer_class_init(JagPackageRendererClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_package_renderer_init(JagPackageRenderer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_package_renderer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_package_renderer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPackageRenderer *jag_package_renderer_new() {
	JagPackageRenderer *result = g_object_new(JAG_TYPE_PACKAGE_RENDERER, NULL);
	cat_ref_anounce(result);
	return result;
}

/********************* begin MooINodeRendererFactory implementation *********************/

static void l_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout) {

}

static void l_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout) {
	MooNodeWo *node = moo_node_layout_get_node(node_layout);

	int layout_x = moo_node_layout_get_x(node_layout);
	int layout_y = moo_node_layout_get_y(node_layout);
	double size = moo_node_layout_get_height(node_layout);

	layout_x += size;


	gboolean has_children = moo_node_wo_child_count(node)>0;
	jag_src_folder_renderer_draw_package(cairo, layout_x, layout_y, size, has_children);
}

static void l_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_update_layout;
	iface->paint = l_paint;
}

/********************* end MooINodeRendererFactory implementation *********************/
