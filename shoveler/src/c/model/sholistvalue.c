/*
   File:    sholistvalue.c
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "sholistvalue.h"
#include "shosimplevalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoListValue"
#include <logging/catlog.h>

struct _ShoListValuePrivate {
	CatArrayWo *value_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_value_iface_init(ShoIValueInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ShoListValue, sho_list_value, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ShoListValue)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(SHO_TYPE_IVALUE, l_value_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_list_value_class_init(ShoListValueClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_list_value_init(ShoListValue *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoListValue *instance = SHO_LIST_VALUE(object);
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(instance);
	cat_unref_ptr(priv->value_list);
	G_OBJECT_CLASS(sho_list_value_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_list_value_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ShoListValue *sho_list_value_new() {
	ShoListValue *result = g_object_new(SHO_TYPE_LIST_VALUE, NULL);
	cat_ref_anounce(result);
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(result);
	priv->value_list = cat_array_wo_new();
	return result;
}


void sho_list_value_add_value(ShoListValue *list_value, ShoIValue *value) {
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(list_value);
	cat_array_wo_append(priv->value_list, (GObject *) value);
}

void sho_list_value_add_string(ShoListValue *list_value, CatStringWo *text) {
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(list_value);
	ShoSimpleValue *value = sho_simple_value_new_string(text);
	cat_array_wo_append(priv->value_list, (GObject *) value);
	cat_unref_ptr(value);
}

int sho_list_value_count(ShoListValue *list_value) {
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(list_value);
	return cat_array_wo_size(priv->value_list);
}

ShoIValue *sho_list_value_get_at(ShoListValue *list_value, int index) {
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(list_value);
	return (ShoIValue *) cat_array_wo_get(priv->value_list, index);
}

CatIIterator *sho_list_value_iterator(ShoListValue *list_value) {
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(list_value);
	return cat_array_wo_iterator(priv->value_list);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ShoListValue *instance = SHO_LIST_VALUE(self);
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %p]", iname, self, priv->value_list);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

/********************* start ShoIValue implementation *********************/

static ShoIValue *l_value_deep_copy(ShoIValue *self) {
	ShoListValue *result = g_object_new(SHO_TYPE_LIST_VALUE, NULL);
	cat_ref_anounce(result);
	ShoListValuePrivate *priv = sho_list_value_get_instance_private(result);
	ShoListValuePrivate *spriv = sho_list_value_get_instance_private((ShoListValue *) self);
	priv->value_list = cat_array_wo_new(cat_array_wo_size(spriv->value_list));

	if (cat_array_wo_size(spriv->value_list)>0) {
		CatIIterator *iter = cat_array_wo_iterator(spriv->value_list);
		while(cat_iiterator_has_next(iter)) {
			ShoIValue *value = (ShoIValue *) cat_iiterator_next(iter);
			ShoIValue *new_value = sho_ivalue_deep_copy(value);
			cat_array_wo_append(priv->value_list, (GObject *) new_value);
			cat_unref_ptr(new_value);
		}
		cat_unref_ptr(iter);
	}

	return (ShoIValue *) result;
}

static void l_value_iface_init(ShoIValueInterface *iface) {
	iface->deepCopy = l_value_deep_copy;
}

/********************* end ShoIValue implementation *********************/
