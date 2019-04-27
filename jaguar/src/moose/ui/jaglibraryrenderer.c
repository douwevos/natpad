/*
   File:    jaglibraryrenderer.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 22, 2014
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

#include "jaglibraryrenderer.h"
#include "jagjarnoderenderer.h"
#include "../srcfolder/jagsrcfolderrenderer.h"
#include <moose.h>
#include <mooseexplorer.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLibraryRenderer"
#include <logging/catlog.h>


static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_renderer_iface_init(MooINodeRendererInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagLibraryRenderer, jag_library_renderer, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_RENDERER, l_renderer_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_library_renderer_class_init(JagLibraryRendererClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_library_renderer_init(JagLibraryRenderer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_library_renderer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_library_renderer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagLibraryRenderer *jag_library_renderer_new() {
	JagLibraryRenderer *result = g_object_new(JAG_TYPE_LIBRARY_RENDERER, NULL);
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
	int size = moo_node_layout_get_height(node_layout);

	layout_x += size;
	jag_jar_renderer_draw_jar(cairo, layout_x, layout_y, size, TRUE);

	double psize = (double) size*0.65;

	gboolean has_children = moo_node_wo_child_count((MooNodeWo *) node)>0;
	jag_src_folder_renderer_draw_package(cairo, layout_x+psize*0.5, layout_y+size-psize, psize, has_children);
}

static void l_renderer_iface_init(MooINodeRendererInterface *iface) {
	iface->updateLayout = l_update_layout;
	iface->paint = l_paint;
}

/********************* end MooINodeRendererFactory implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
