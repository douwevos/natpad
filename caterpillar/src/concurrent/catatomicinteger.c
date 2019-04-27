/*
   File:    catatomicinteger.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 23, 2014
   Web:     http://www.natpad.net/
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

#include "catatomicinteger.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatAtomicInteger"
#include "../logging/catlog.h"

struct _CatAtomicIntegerPrivate {
	int value;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatAtomicInteger, cat_atomic_integer, G_TYPE_OBJECT,{
		G_ADD_PRIVATE(CatAtomicInteger)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_atomic_integer_class_init(CatAtomicIntegerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_atomic_integer_init(CatAtomicInteger *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cat_atomic_integer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_atomic_integer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatAtomicInteger *cat_atomic_integer_new() {
	CatAtomicInteger *result = g_object_new(CAT_TYPE_ATOMIC_INTEGER, NULL);
	cat_ref_anounce(result);
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(result);
	__atomic_store_n(&(priv->value), 0, __ATOMIC_SEQ_CST);
	return result;
}


CatAtomicInteger *cat_atomic_integer_new_val(int initial) {
	CatAtomicInteger *result = g_object_new(CAT_TYPE_ATOMIC_INTEGER, NULL);
	cat_ref_anounce(result);
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(result);
	__atomic_store_n(&(priv->value), initial, __ATOMIC_SEQ_CST);
	return result;
}



gboolean cat_atomic_integer_compare_set(CatAtomicInteger *int_atomic, int expect, int set) {
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(int_atomic);
	return __atomic_compare_exchange_n(&(priv->value), &expect, set, FALSE, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

int cat_atomic_integer_get(CatAtomicInteger *int_atomic) {
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(int_atomic);
	return __atomic_load_n(&(priv->value), __ATOMIC_SEQ_CST);
}

void cat_atomic_integer_set(CatAtomicInteger *int_atomic, int newVal) {
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(int_atomic);
	__atomic_store_n(&(priv->value), newVal, __ATOMIC_SEQ_CST);
}

int cat_atomic_integer_add(CatAtomicInteger *int_atomic, int delta) {
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(int_atomic);
	return __atomic_add_fetch(&(priv->value), delta, __ATOMIC_SEQ_CST);
}

int cat_atomic_integer_decrement(CatAtomicInteger *int_atomic) {
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(int_atomic);
	const int delta = 1;
	return __atomic_sub_fetch(&(priv->value), delta, __ATOMIC_SEQ_CST);
}

int cat_atomic_integer_increment(CatAtomicInteger *int_atomic) {
	CatAtomicIntegerPrivate *priv = cat_atomic_integer_get_instance_private(int_atomic);
	const int delta = 1;
	return __atomic_add_fetch(&(priv->value), delta, __ATOMIC_SEQ_CST);
}


/********************* begin CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	cat_string_wo_format(append_to, "%s[%d]", g_type_name_from_instance((GTypeInstance *) self), cat_atomic_integer_get((CatAtomicInteger *) self));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
