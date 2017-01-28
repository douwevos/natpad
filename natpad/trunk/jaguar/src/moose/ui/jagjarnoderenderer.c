/*
   File:    jagjarnoderenderer.c
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

#include "jagjarnoderenderer.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJarNodeRenderer"
#include <logging/catlog.h>

struct _JagJarNodeRendererPrivate {
	void *dummy;
};

static void l_renderer_iface_init(MooINodeRendererInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagJarNodeRenderer, jag_jar_node_renderer, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_renderer_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_jar_node_renderer_class_init(JagJarNodeRendererClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagJarNodeRendererPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_jar_node_renderer_init(JagJarNodeRenderer *instance) {
	JagJarNodeRendererPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_JAR_NODE_RENDERER, JagJarNodeRendererPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagJarNodeRenderer *jag_jar_node_renderer_new() {
	JagJarNodeRenderer *result = g_object_new(JAG_TYPE_JAR_NODE_RENDERER, NULL);
	cat_ref_anounce(result);
	return result;
}






void jag_jar_renderer_draw_jar(cairo_t *cairo, double xoffset, int yoffset, double size, gboolean with_blue) {

	double nn = size*0.08;
	xoffset += nn;
	yoffset += nn;
	size = size-nn*2.0;


//	double x2  = 0.5+xoffset+(size*0.260);
//	double y2 = 0.5+yoffset+(size*0.165);

//	double xc  = 0.5+xoffset+(size*0.5);
//	double yc = 0.5+yoffset+(size*0.5);

	double cap_diff = 0.08;

	double cap_top = 0.13;
	double cap_bottom = 0.30;



	double x10  = 0.5+xoffset+(size*0.05);
	double x1  = 0.5+xoffset+(size*0.10);
	double x11  = 0.5+xoffset+(size*0.15);

	double y1 = 0.5+yoffset+(size*(cap_top-cap_diff));
	double y2 = 0.5+yoffset+(size*cap_top);
	double y3 = 0.5+yoffset+(size*(cap_top+cap_diff));
	double y4 = 0.5+yoffset+(size*cap_bottom);
	double y5 = 0.5+yoffset+(size*(cap_bottom+cap_diff));

	double y6 = 0.5+yoffset+(size*0.72);
	double y7 = 0.5+yoffset+(size*0.76);

	double y8 = 0.5+yoffset+(size*0.83);

	double y9 = 0.5+yoffset+(size*0.90);

	double x20  = 0.5+xoffset+(size*0.95);
	double x2  = 0.5+xoffset+(size*0.90);
	double x21  = 0.5+xoffset+(size*0.85);




	cairo_set_line_width(cairo, 0.75);

	/* glass */
	cairo_move_to(cairo, x11, y4);
	cairo_line_to(cairo, x10, y5);

	cairo_line_to(cairo, x10, y6);
	cairo_curve_to(cairo, x10,y7, x10,y7, x11,y8);

	cairo_curve_to(cairo, x11,y9, x21,y9, x21,y8);

//	cairo_curve_to(cairo, x21,y7, x11,y7, x11,y8);
//
//	cairo_move_to(cairo, x21, y8);

	cairo_curve_to(cairo, x20,y7, x20,y7, x20,y6);
	cairo_line_to(cairo, x20, y5);

	cairo_line_to(cairo, x21, y4);
	cairo_close_path(cairo);

	if (with_blue) {
		cairo_set_source_rgb(cairo, 0.7,0.7,1.0);
	} else {
		cairo_set_source_rgb(cairo, 0.85, 0.85, 1.0);
	}


	cairo_fill_preserve(cairo);
//	cairo_set_source_rgb(cairo, 0.3, 0.3, 0.7);
	cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);
	cairo_stroke(cairo);

//
//	cairo_set_source_rgb(cairo, 0.5, 0.5, 1.0);
//	cairo_move_to(cairo, x11,y8);
//	cairo_curve_to(cairo, x11,y9, x21,y9, x21,y8);
//	cairo_curve_to(cairo, x21,y7, x11,y7, x11,y8);
//	cairo_stroke(cairo);

	/* cap */

	cairo_set_line_width(cairo, 1.0);


	cairo_move_to(cairo, x1,y2);
	cairo_curve_to(cairo, x1,y1, x2,y1, x2,y2);
	cairo_line_to(cairo, x2, y4);
	cairo_curve_to(cairo, x2,y5, x1,y5, x1,y4);
	cairo_line_to(cairo, x1, y2);

	cairo_curve_to(cairo, x1,y3, x2,y3, x2,y2);

	cairo_set_source_rgb(cairo, 0.3, 0.8, 0.3);
	cairo_fill_preserve(cairo);
	cairo_set_source_rgb(cairo, 0.0, 0.3, 0.0);
	cairo_stroke(cairo);

}



/********************* begin MooINodeRendererFactory implementation *********************/


static void l_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout) {

}

static void l_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout) {
	int layout_x = moo_node_layout_get_x(node_layout);
	int layout_y = moo_node_layout_get_y(node_layout);
	int size = moo_node_layout_get_height(node_layout);

	layout_x += size;
	jag_jar_renderer_draw_jar(cairo, layout_x, layout_y, size, FALSE);
}



static void l_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_update_layout;
	iface->paint = l_paint;
}

/********************* end MooINodeRendererFactory implementation *********************/
