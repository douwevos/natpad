/*
   File:    jagclassfilerenderer.c
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

#include "jagclassfilerenderer.h"
#include <moose.h>
#include <mooseexplorer.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagClassfileRenderer"
#include <logging/catlog.h>

static void l_renderer_iface_init(MooINodeRendererInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagClassfileRenderer, jag_classfile_renderer, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_renderer_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_classfile_renderer_class_init(JagClassfileRendererClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_classfile_renderer_init(JagClassfileRenderer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_classfile_renderer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_classfile_renderer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagClassfileRenderer *jag_classfile_renderer_new() {
	JagClassfileRenderer *result = g_object_new(JAG_TYPE_CLASSFILE_RENDERER, NULL);
	cat_ref_anounce(result);
	return result;
}

static void l_draw_class_file(cairo_t *cairo, double xoffset, int yoffset, double size) {
	moo_default_renderer_draw_file(cairo, xoffset, yoffset, size);

	double x2  = 0.5+xoffset+(size*0.35);
	double y2 = 0.5+yoffset+(size*0.10);

	PangoLayout *layout = pango_cairo_create_layout(cairo);

	const PangoFontDescription *e_pango_description = pango_layout_get_font_description(layout);
	PangoFontDescription *pango_description = NULL;
	if (e_pango_description==NULL) {
		pango_description = pango_font_description_new();
	} else {
		pango_description = pango_font_description_copy(e_pango_description);
	}
//	int font_size = (int) ((0.4*size)*PANGO_SCALE);
	pango_font_description_set_absolute_size(pango_description, 0.55*size*PANGO_SCALE);
	pango_font_description_set_weight(pango_description, PANGO_WEIGHT_ULTRABOLD);
	pango_layout_set_font_description(layout, pango_description);

	pango_layout_set_text(layout, "J", 1);
	cairo_set_source_rgb(cairo, 0.0, 0.6, 0.0);
	cairo_move_to(cairo, x2, y2);
	pango_cairo_show_layout(cairo, layout);
}

/********************* begin MooINodeRendererFactory implementation *********************/

static void l_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout) {
}

static void l_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout) {
//	MooNodeWo *node = moo_node_layout_get_node(node_layout);
//	int fontHeight = moo_node_layout_get_font_height(node_layout);

	int layout_x = moo_node_layout_get_x(node_layout);
	int layout_y = moo_node_layout_get_y(node_layout);
	double size = moo_node_layout_get_height(node_layout);

	layout_x += size;
//	gboolean has_children = moo_node_wo_child_count(node)>0;

	l_draw_class_file(cairo, layout_x, layout_y, size);
//	jag_src_folder_renderer_draw_package(cairo, layout_x, layout_y, size, has_children);
}

static void l_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_update_layout;
	iface->paint = l_paint;
}

/********************* end MooINodeRendererFactory implementation *********************/
