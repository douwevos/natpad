/*
   File:    draforeground.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 8, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "draforeground.h"
#include "draiattributebuilder.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraForeground"
#include <logging/catlog.h>

struct _DraForegroundPrivate {
	int color_index;
	int begin_idx;
	int end_idx;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_attribute_builder_iface_init(DraIAttributeBuilderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraForeground, dra_foreground, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraForeground)
		G_IMPLEMENT_INTERFACE(DRA_TYPE_IATTRIBUTE_BUILDER, l_attribute_builder_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_foreground_class_init(DraForegroundClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_foreground_init(DraForeground *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraForeground *instance = DRA_FOREGROUND(object);
//	DraForegroundPrivate *priv = dra_foreground_get_instance_private(instance);
	G_OBJECT_CLASS(dra_foreground_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_foreground_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraForeground *dra_foreground_new(int color_index) {
	DraForeground *result = g_object_new(DRA_TYPE_FOREGROUND, NULL);
	cat_ref_anounce(result);
	DraForegroundPrivate *priv = dra_foreground_get_instance_private(result);
	priv->color_index = color_index;
	priv->begin_idx = 0;
	priv->end_idx = 0;
	return result;
}

int dra_foreground_get_color_index(DraForeground *foreground) {
	DraForegroundPrivate *priv = dra_foreground_get_instance_private(foreground);
	return priv->color_index;
}


/********************* start DraIAttributeBuilder implementation *********************/

static DraIAttributeBuilder *l_ab_create_copy(DraIAttributeBuilder *self) {
	DraForegroundPrivate *spriv = dra_foreground_get_instance_private((DraForeground *) self);
	DraForeground *result = dra_foreground_new(spriv->color_index);
	DraForegroundPrivate *priv = dra_foreground_get_instance_private((DraForeground *) result);
	priv->begin_idx = spriv->begin_idx;
	priv->end_idx = spriv->end_idx;
	return (DraIAttributeBuilder *) result;
}

static void l_ab_set_begin_and_end(DraIAttributeBuilder *self, int begin_idx, int end_idx) {
	DraForegroundPrivate *priv = dra_foreground_get_instance_private((DraForeground *) self);
	priv->begin_idx = begin_idx;
	priv->end_idx = end_idx;
}

static void l_ab_get_begin_and_end(DraIAttributeBuilder *self, int *begin_idx, int *end_idx) {
	DraForegroundPrivate *priv = dra_foreground_get_instance_private((DraForeground *) self);
	*begin_idx = priv->begin_idx;
	*end_idx = priv->end_idx;
}


static PangoAttribute *l_ab_build(DraIAttributeBuilder *self, ChaPrefsColorMapWo *color_map) {
	DraForegroundPrivate *priv = dra_foreground_get_instance_private((DraForeground *) self);
	ChaColorEntry cce = cha_prefs_color_map_wo_get_color(color_map, priv->color_index);
	guint16 lred = (guint) (cce.red*0xFFFF);
	guint16 lgreen = (guint) (cce.green*0xFFFF);
	guint16 lblue = (guint) (cce.blue*0xFFFF);

	PangoAttribute *pango_attr = pango_attr_foreground_new(lred, lgreen, lblue);
	pango_attr->start_index = priv->begin_idx;
	pango_attr->end_index = priv->end_idx;
	return pango_attr;
}

static gboolean l_ab_equal(const DraIAttributeBuilder *ab_a, const DraIAttributeBuilder *ab_b) {
	if (ab_a==ab_b) {
		return TRUE;
	}
	if (ab_a==NULL || ab_b==NULL) {
		return FALSE;
	}
	if (DRA_IS_FOREGROUND(ab_b)) {
		DraForegroundPrivate *priv_a = dra_foreground_get_instance_private((DraForeground *) ab_a);
		DraForegroundPrivate *priv_b = dra_foreground_get_instance_private((DraForeground *) ab_b);
		return (priv_a->color_index == priv_b->color_index)
				&& (priv_a->begin_idx== priv_b->begin_idx)
				&& (priv_a->end_idx== priv_b->end_idx);
	}
	return FALSE;
}



static void l_attribute_builder_iface_init(DraIAttributeBuilderInterface *iface) {
	iface->createCopy = l_ab_create_copy;
	iface->setBeginAndEnd = l_ab_set_begin_and_end;
	iface->getBeginAndEnd = l_ab_get_begin_and_end;
	iface->build = l_ab_build;
	iface->equal = l_ab_equal;
}


/********************* end DraIAttributeBuilder implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraForeground *instance = DRA_FOREGROUND(self);
	DraForegroundPrivate *priv = dra_foreground_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p color_index=%d, begin=%d, end=%d]", iname, self, priv->color_index, priv->begin_idx, priv->end_idx);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
