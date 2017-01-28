/*
   File:    lealayer.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 17, 2014
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

#include "lealayerprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaLayer"
#include <logging/catlog.h>

struct _LeaLayerPrivate {
	CatArrayWo *e_drawables;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaLayer, lea_layer, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(LeaLayer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr);

static void lea_layer_class_init(LeaLayerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->draw = l_widget_draw;
}

static void lea_layer_init(LeaLayer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaLayer *instance = LEA_LAYER(object);
	LeaLayerPrivate *priv = lea_layer_get_instance_private(instance);
	cat_unref_ptr(priv->e_drawables);
	G_OBJECT_CLASS(lea_layer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_layer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void lea_layer_construct(LeaLayer *layer) {
	LeaLayerPrivate *priv = lea_layer_get_instance_private(layer);
	gtk_widget_set_has_window(GTK_WIDGET(layer), FALSE);
	priv->e_drawables = cat_array_wo_new();

}

LeaLayer *lea_layer_new() {
	LeaLayer *result = g_object_new(LEA_TYPE_LAYER, NULL);
	cat_ref_anounce(result);
	lea_layer_construct(result);
	return result;
}

CatArrayWo *lea_layer_get_drawables(LeaLayer *layer) {
	LeaLayerPrivate *priv = lea_layer_get_instance_private(layer);
	return priv->e_drawables;
}


void lea_layer_add_drawable(LeaLayer *layer, LeaIDrawable *drawable) {
	LeaLayerPrivate *priv = lea_layer_get_instance_private(layer);
	cat_array_wo_append(priv->e_drawables, (GObject *) drawable);
	cairo_region_t *region = lea_idrawable_create_region(drawable);
	if (region) {
		GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(layer));
		if (window) {
			gdk_window_invalidate_region(window, region, TRUE);
		}
		cairo_region_destroy(region);
	}
}

void lea_layer_remove_drawable(LeaLayer *layer, LeaIDrawable *drawable) {
	LeaLayerPrivate *priv = lea_layer_get_instance_private(layer);
	int index = cat_array_wo_find_index(priv->e_drawables, drawable, -1);
	if (index>=0) {
		cairo_region_t *region = lea_idrawable_create_region(drawable);
		if (region) {
			GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(layer));
			if (window) {
				gdk_window_invalidate_region(window, region, TRUE);
			}
			cairo_region_destroy(region);
		}
		cat_array_wo_remove(priv->e_drawables, index, NULL);
	}
}


gboolean lea_layer_has_drawables(LeaLayer *layer) {
	LeaLayerPrivate *priv = lea_layer_get_instance_private(layer);
	return cat_array_wo_size(priv->e_drawables)>0;
}


static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	LeaLayerPrivate *priv = lea_layer_get_instance_private(LEA_LAYER(widget));

	GtkAllocation alloc;
	gtk_widget_get_allocation(widget, &alloc);
	cairo_save(cr);
	cairo_translate(cr, -alloc.x, -alloc.y);

	CatIIterator *iter = cat_array_wo_iterator(priv->e_drawables);
	while(cat_iiterator_has_next(iter)) {
		LeaIDrawable *drawable = (LeaIDrawable *) cat_iiterator_next(iter);
		lea_idrawable_draw(drawable, cr);
	}
	cat_unref_ptr(iter);


	cairo_restore(cr);
	return TRUE;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






