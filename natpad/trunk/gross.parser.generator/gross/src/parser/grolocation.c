
/*
   File:    grolocation.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "grolocation.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroLocation"
#include <logging/catlog.h>

struct _GroLocationPrivate {
	long long row;
	int column;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroLocation, gro_location, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroLocation)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_location_class_init(GroLocationClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_location_init(GroLocation *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(gro_location_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_location_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroLocation *gro_location_new(long long row, int column) {
	GroLocation *result = g_object_new(GRO_TYPE_LOCATION, NULL);
	cat_ref_anounce(result);
	GroLocationPrivate *priv = gro_location_get_instance_private(result);
	priv->row = row;
	priv->column = column;
	return result;
}

long long gro_location_get_row(const GroLocation *location) {
	GroLocationPrivate *priv = gro_location_get_instance_private((GroLocation *) location);
	return priv->row;
}

int gro_location_get_column(const GroLocation *location) {
	GroLocationPrivate *priv = gro_location_get_instance_private((GroLocation *) location);
	return priv->column;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroLocation *instance = GRO_LOCATION(self);
	GroLocationPrivate *priv = gro_location_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, row=%ld, col=%d]", iname, self, priv->row, priv->column);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

