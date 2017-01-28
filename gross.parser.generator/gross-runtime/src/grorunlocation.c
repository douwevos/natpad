/*
   File:    grorunlocation.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#include "grorunlocation.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunLocation"
#include <logging/catlog.h>

struct _GroRunLocationPrivate {
	int begin_column;
	long begin_row;
	int end_column;
	long end_row;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunLocation, grorun_location, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunLocation)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_location_class_init(GroRunLocationClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_location_init(GroRunLocation *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroRunLocation *instance = GRORUN_LOCATION(object);
//	GroRunLocationPrivate *priv = grorun_location_get_instance_private(instance);
	G_OBJECT_CLASS(grorun_location_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_location_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunLocation *grorun_location_new(int begin_column, long begin_row, int end_column, long end_row) {
	GroRunLocation *result = g_object_new(GRORUN_TYPE_LOCATION, NULL);
	cat_ref_anounce(result);
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(result);
	priv->begin_column = begin_column;
	priv->begin_row = begin_row;
	priv->end_column = end_column;
	priv->end_row = end_row;
	return result;
}

GroRunLocation *grorun_location_set_end(GroRunLocation *location, int end_column, long end_row) {
	GroRunLocation *result = g_object_new(GRORUN_TYPE_LOCATION, NULL);
	cat_ref_anounce(result);
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(result);
	GroRunLocationPrivate *spriv = grorun_location_get_instance_private(location);
	priv->begin_column = spriv->begin_column;
	priv->begin_row = spriv->begin_row;
	priv->end_column = end_column;
	priv->end_row = end_row;
	return result;
}

void grorun_location_get_all(GroRunLocation *location, int *begin_column, long *begin_row, int *end_column, long *end_row) {
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(location);
	*begin_column = priv->begin_column;
	*begin_row = priv->begin_row;
	*end_column = priv->end_column;
	*end_row = priv->end_row;
}

void grorun_location_get_begin(GroRunLocation *location, int *begin_column, long *begin_row) {
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(location);
	*begin_column = priv->begin_column;
	*begin_row = priv->begin_row;
}

void grorun_location_get_end(GroRunLocation *location, int *end_column, long *end_row) {
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(location);
	*end_column = priv->end_column;
	*end_row = priv->end_row;
}

int grorun_location_get_width(GroRunLocation *location) {
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(location);
	return priv->end_column-priv->begin_column;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunLocation *instance = GRORUN_LOCATION(self);
	GroRunLocationPrivate *priv = grorun_location_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p col:row=%d, %d - %d, %d]", iname, self, priv->begin_column, priv->begin_row, priv->end_column, priv->end_row);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
