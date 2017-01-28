/*
   File:    jagsrcfolderrenderer.c
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

#include "jagsrcfolderrenderer.h"
#include <moose.h>
#include <mooseexplorer.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagSrcFolderRenderer"
#include <logging/catlog.h>

struct _JagSrcFolderRendererPrivate {
	void *dummy;
};


static void l_renderer_iface_init(MooINodeRendererInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagSrcFolderRenderer, jag_src_folder_renderer, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_renderer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_src_folder_renderer_class_init(JagSrcFolderRendererClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagSrcFolderRendererPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_src_folder_renderer_init(JagSrcFolderRenderer *instance) {
	JagSrcFolderRendererPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_SRC_FOLDER_RENDERER, JagSrcFolderRendererPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagSrcFolderRenderer *instance = JAG_SRC_FOLDER_RENDERER(object);
//	JagSrcFolderRendererPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagSrcFolderRenderer *jag_src_folder_renderer_new() {
	JagSrcFolderRenderer *result = g_object_new(JAG_TYPE_SRC_FOLDER_RENDERER, NULL);
	cat_ref_anounce(result);
//	JagSrcFolderRendererPrivate *priv = result->priv;
//	moo_par_construct((MooPar *) result);
	return result;
}





void jag_src_folder_renderer_draw_package(cairo_t *cairo, double xoffset, int yoffset, double size, gboolean non_empty) {
//	double x1  = 0.5+xoffset+(int) (size*0.15);
//	double x2  = 0.5+xoffset+(int) (size*0.20);
//	double x3  = 0.5+xoffset+(int) (size*0.50);
//	double x4  = 0.5+xoffset+(int) (size*0.80);
//	double x5  = 0.5+xoffset+(int) (size*0.85);
//
//
//	double y1 = 0.5+yoffset+(int) (size*0.15);
//	double y2 = 0.5+yoffset+(int) (size*0.20);
//	double y2b = 0.5+yoffset+(int) (size*0.25);
//	double y3 = 0.5+yoffset+(int) (size*0.50);
//	double y4 = 0.5+yoffset+(int) (size*0.80);
//	double y5 = 0.5+yoffset+(int) (size*0.85);


	double x1  = 0.5+xoffset+(size*0.15);
	double x2  = 0.5+xoffset+(size*0.20);
	double x3  = 0.5+xoffset+(size*0.50);
	double x4  = 0.5+xoffset+(size*0.80);
	double x5  = 0.5+xoffset+(size*0.85);


	double y1 = 0.5+yoffset+(size*0.15);
	double y2 = 0.5+yoffset+(size*0.20);
	double y3 = 0.5+yoffset+(size*0.50);
	double y4 = 0.5+yoffset+(size*0.80);
	double y5 = 0.5+yoffset+(size*0.85);



	double radius = 0.15 *size;

	cairo_move_to  (cairo, x2, y2 + radius);
	cairo_curve_to (cairo, x2 , y2, x2 , y2, x2 + radius, y2);
	cairo_line_to (cairo, x4 - radius, y2);
	cairo_curve_to (cairo, x4, y2, x4, y2, x4, y2 + radius);
	cairo_line_to (cairo, x4 , y4 - radius);
	cairo_curve_to (cairo, x4, y4, x4, y4, x4 - radius, y4);
	cairo_line_to (cairo, x2 + radius, y4);
	cairo_curve_to (cairo, x2, y4, x2, y4, x2, y4- radius);
	cairo_close_path(cairo);
//
//
//
//
//	cairo_move_to(cairo, x2, y2b);
//	cairo_curve_to()
//
//	cairo_line_to(cairo, x4, y2);
//	cairo_line_to(cairo, x4, y4);
//	cairo_line_to(cairo, x2, y4);
//	cairo_line_to(cairo, x2, y2);

	if (non_empty) {
		cairo_set_source_rgb(cairo, 1.0,0.8,0.4);
	} else {
		cairo_set_source_rgb(cairo, 0.8,0.8,1.0);
	}

	cairo_fill_preserve(cairo);

	cairo_set_line_width(cairo, 1.5);

//	cairo_set_source_rgba(cairo, 0.7,0.5,0.1, 0.7);
	cairo_set_source_rgba(cairo, 0.4,0.2,0.0, 0.7);
	cairo_stroke(cairo);

	cairo_set_line_width(cairo, 0.5);

	cairo_set_source_rgb(cairo, 0.0,0.0,0.0);
	cairo_move_to(cairo, x1, y3);
	cairo_line_to(cairo, x5, y3);
	cairo_stroke(cairo);

	cairo_move_to(cairo, x3, y1);
	cairo_line_to(cairo, x3, y5);
	cairo_stroke(cairo);
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

	gboolean isExpanded = moo_node_layout_is_expanded(node_layout);

	moo_default_node_renderer_draw_map(cairo, layout_x, layout_y, size, isExpanded, TRUE);

	double psize = (double) size*0.65;


	gboolean has_children = moo_node_wo_child_count((MooNodeWo *) node)>0;
	jag_src_folder_renderer_draw_package(cairo, layout_x+psize*0.5, layout_y+size-psize, psize, has_children);
}



static void l_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_update_layout;
	iface->paint = l_paint;
}

/********************* end MooINodeRendererFactory implementation *********************/
