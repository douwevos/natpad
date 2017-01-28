/*
   File:    jagjrerenderer.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#include "jagjrerenderer.h"
#include <moose.h>
#include <mooseexplorer.h>
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJreRenderer"
#include <logging/catlog.h>

static void l_renderer_iface_init(MooINodeRendererInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagJreRenderer, jag_jre_renderer, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_renderer_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_jre_renderer_class_init(JagJreRendererClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_jre_renderer_init(JagJreRenderer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_jre_renderer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_jre_renderer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagJreRenderer *jag_jre_renderer_new() {
	JagJreRenderer *result = g_object_new(JAG_TYPE_JRE_RENDERER, NULL);
	cat_ref_anounce(result);
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagJreRenderer *instance = JAG_JRE_RENDERER(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) instance);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



static void l_draw_compiler(cairo_t *cairo, double xoffset, int yoffset, double size) {

	double nn = size*0.08;
	xoffset += nn;
	yoffset += nn;
	size = size-nn*2.0;

//	double x1  = 0.5+xoffset+(size*0.15);
//	double x2  = 0.5+xoffset+(size*0.35);
//	double y2 = 0.5+yoffset+(size*0.10);


	cairo_set_source_rgb(cairo, 0.4, 0.6, 0.9);
//	cairo_move_to(cairo, 0.5+0.4364888*size, 0.5+0.9202277*size);
//	cairo_line_to(cairo, 0.5+0.5635112*size, 0.5+0.9202277*size);
//	cairo_line_to(cairo, 0.5+0.58981556*size, 0.5+0.812343*size);
//	cairo_line_to(cairo, 0.5+0.6882005*size, 0.5+0.7649633*size);
//	cairo_line_to(cairo, 0.5+0.78894866*size, 0.5+0.81166273*size);
//	cairo_line_to(cairo, 0.5+0.8681458*size, 0.5+0.71235263*size);
//	cairo_line_to(cairo, 0.5+0.80019873*size, 0.5+0.62452203*size);
//	cairo_line_to(cairo, 0.5+0.82449776*size, 0.5+0.51806086*size);
//	cairo_line_to(cairo, 0.5+0.9238243*size, 0.5+0.4684094*size);
//	cairo_line_to(cairo, 0.5+0.89555913*size, 0.5+0.34457168*size);
//	cairo_line_to(cairo, 0.5+0.78452605*size, 0.5+0.34293342*size);
//	cairo_line_to(cairo, 0.5+0.71644163*size, 0.5+0.2575582*size);
//	cairo_line_to(cairo, 0.5+0.7395516*size, 0.5+0.1489444*size);
//	cairo_line_to(cairo, 0.5+0.62510836*size, 0.5+0.093831435*size);
//	cairo_line_to(cairo, 0.5+0.5545995*size, 0.5+0.17961915*size);
//	cairo_line_to(cairo, 0.5+0.44540048*size, 0.5+0.17961915*size);
//	cairo_line_to(cairo, 0.5+0.37489164*size, 0.5+0.093831435*size);
//	cairo_line_to(cairo, 0.5+0.2604484*size, 0.5+0.1489444*size);
//	cairo_line_to(cairo, 0.5+0.2835584*size, 0.5+0.2575582*size);
//	cairo_line_to(cairo, 0.5+0.21547392*size, 0.5+0.34293342*size);
//	cairo_line_to(cairo, 0.5+0.10444085*size, 0.5+0.34457168*size);
//	cairo_line_to(cairo, 0.5+0.076175705*size, 0.5+0.4684094*size);
//	cairo_line_to(cairo, 0.5+0.17550223*size, 0.5+0.51806086*size);
//	cairo_line_to(cairo, 0.5+0.1998013*size, 0.5+0.62452203*size);
//	cairo_line_to(cairo, 0.5+0.13185416*size, 0.5+0.71235263*size);
//	cairo_line_to(cairo, 0.5+0.21105133*size, 0.5+0.81166273*size);
//	cairo_line_to(cairo, 0.5+0.3117995*size, 0.5+0.7649633*size);
//	cairo_line_to(cairo, 0.5+0.4101844*size, 0.5+0.812343*size);


	cairo_move_to(cairo, xoffset+0.5+0.4364888*size, yoffset+0.5+0.9202277*size);
	cairo_line_to(cairo, xoffset+0.5+0.5635112*size, yoffset+0.5+0.9202277*size);
	cairo_line_to(cairo, xoffset+0.5+0.58981556*size, yoffset+0.5+0.812343*size);
	cairo_line_to(cairo, xoffset+0.5+0.65735066*size, yoffset+0.5+0.78436905*size);
	cairo_line_to(cairo, xoffset+0.5+0.75223666*size, yoffset+0.5+0.8420551*size);
	cairo_line_to(cairo, xoffset+0.5+0.8420551*size, yoffset+0.5+0.75223666*size);
	cairo_line_to(cairo, xoffset+0.5+0.78436905*size, yoffset+0.5+0.65735066*size);
	cairo_line_to(cairo, xoffset+0.5+0.812343*size, yoffset+0.5+0.58981556*size);
	cairo_line_to(cairo, xoffset+0.5+0.9202277*size, yoffset+0.5+0.5635112*size);
	cairo_line_to(cairo, xoffset+0.5+0.9202277*size, yoffset+0.5+0.4364888*size);
	cairo_line_to(cairo, xoffset+0.5+0.812343*size, yoffset+0.5+0.4101844*size);
	cairo_line_to(cairo, xoffset+0.5+0.78436905*size, yoffset+0.5+0.34264934*size);
	cairo_line_to(cairo, xoffset+0.5+0.8420551*size, yoffset+0.5+0.24776334*size);
	cairo_line_to(cairo, xoffset+0.5+0.75223666*size, yoffset+0.5+0.15794493*size);
	cairo_line_to(cairo, xoffset+0.5+0.65735066*size, yoffset+0.5+0.21563092*size);
	cairo_line_to(cairo, xoffset+0.5+0.58981556*size, yoffset+0.5+0.18765698*size);
	cairo_line_to(cairo, xoffset+0.5+0.5635112*size, yoffset+0.5+0.07977229*size);
	cairo_line_to(cairo, xoffset+0.5+0.4364888*size, yoffset+0.5+0.07977229*size);
	cairo_line_to(cairo, xoffset+0.5+0.4101844*size, yoffset+0.5+0.18765698*size);
	cairo_line_to(cairo, xoffset+0.5+0.34264934*size, yoffset+0.5+0.21563092*size);
	cairo_line_to(cairo, xoffset+0.5+0.24776334*size, yoffset+0.5+0.15794493*size);
	cairo_line_to(cairo, xoffset+0.5+0.15794493*size, yoffset+0.5+0.24776334*size);
	cairo_line_to(cairo, xoffset+0.5+0.21563092*size, yoffset+0.5+0.34264934*size);
	cairo_line_to(cairo, xoffset+0.5+0.18765698*size, yoffset+0.5+0.4101844*size);
	cairo_line_to(cairo, xoffset+0.5+0.07977229*size, yoffset+0.5+0.4364888*size);
	cairo_line_to(cairo, xoffset+0.5+0.07977229*size, yoffset+0.5+0.5635112*size);
	cairo_line_to(cairo, xoffset+0.5+0.18765698*size, yoffset+0.5+0.58981556*size);
	cairo_line_to(cairo, xoffset+0.5+0.21563092*size, yoffset+0.5+0.65735066*size);
	cairo_line_to(cairo, xoffset+0.5+0.15794493*size, yoffset+0.5+0.75223666*size);
	cairo_line_to(cairo, xoffset+0.5+0.24776334*size, yoffset+0.5+0.8420551*size);
	cairo_line_to(cairo, xoffset+0.5+0.34264934*size, yoffset+0.5+0.78436905*size);
	cairo_line_to(cairo, xoffset+0.5+0.4101844*size, yoffset+0.5+0.812343*size);


	cairo_fill_preserve(cairo);

	cairo_set_line_width(cairo, 1.0);
	cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);
	cairo_stroke(cairo);


	double xc  = 0.5+xoffset+(size*0.5);
	double yc = 0.5+yoffset+(size*0.5);


	cairo_move_to(cairo, xc, yc);
	cairo_arc(cairo, xc, yc, size*0.1, 0, 2*M_PI);
	cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);
	cairo_stroke_preserve(cairo);
	cairo_set_source_rgb(cairo, 1.0, 1.0, 1.0);
	cairo_fill(cairo);
}



/********************* begin MooINodeRendererFactory implementation *********************/


static void l_update_layout(MooINodeRenderer *self, struct _MooNodeLayout *node_layout) {

}

static void l_paint(MooINodeRenderer *self, cairo_t *cairo, struct _MooNodeLayout *node_layout) {
	int layout_x = moo_node_layout_get_x(node_layout);
	int layout_y = moo_node_layout_get_y(node_layout);
	int size = moo_node_layout_get_height(node_layout);

	layout_x += size;
	l_draw_compiler(cairo, layout_x, layout_y, size);
}



static void l_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_update_layout;
	iface->paint = l_paint;
}

/********************* end MooINodeRendererFactory implementation *********************/
