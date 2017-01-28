/*
   File:    catatomiclong.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Apr 13, 2013
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

#include "catatomiclong.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatAtomicLong"
#include "../logging/catlog.h"

struct _CatAtomicLongPrivate {
	long long value;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatAtomicLong, cat_atomic_long, G_TYPE_OBJECT,{
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_atomic_long_class_init(CatAtomicLongClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(CatAtomicLongPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_atomic_long_init(CatAtomicLong *instance) {
	CatAtomicLongPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_ATOMIC_LONG, CatAtomicLongPrivate);
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


CatAtomicLong *cat_atomic_long_new() {
	CatAtomicLong *result = g_object_new(CAT_TYPE_ATOMIC_LONG, NULL);
	cat_ref_anounce(result);
	CatAtomicLongPrivate *priv = result->priv;
	__atomic_store_n(&(priv->value), 0, __ATOMIC_SEQ_CST);
	return result;
}


CatAtomicLong *cat_atomic_long_new_val(long long initial) {
	CatAtomicLong *result = g_object_new(CAT_TYPE_ATOMIC_LONG, NULL);
	cat_ref_anounce(result);
	CatAtomicLongPrivate *priv = result->priv;
	__atomic_store_n(&(priv->value), initial, __ATOMIC_SEQ_CST);
	return result;
}



gboolean cat_atomic_long_compare_set(CatAtomicLong *long_atomic, long long expect, long long set) {
	CatAtomicLongPrivate *priv = CAT_ATOMIC_LONG_GET_PRIVATE(long_atomic);
	return __atomic_compare_exchange_n(&(priv->value), &expect, set, FALSE, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

long long cat_atomic_long_get(CatAtomicLong *long_atomic) {
	CatAtomicLongPrivate *priv = CAT_ATOMIC_LONG_GET_PRIVATE(long_atomic);
	return __atomic_load_n(&(priv->value), __ATOMIC_SEQ_CST);
}

void cat_atomic_long_set(CatAtomicLong *long_atomic, long long newVal) {
	CatAtomicLongPrivate *priv = CAT_ATOMIC_LONG_GET_PRIVATE(long_atomic);
	__atomic_store_n(&(priv->value), newVal, __ATOMIC_SEQ_CST);
}

long long cat_atomic_long_add(CatAtomicLong *long_atomic, long long delta) {
	CatAtomicLongPrivate *priv = CAT_ATOMIC_LONG_GET_PRIVATE(long_atomic);
	return __atomic_add_fetch(&(priv->value), delta, __ATOMIC_SEQ_CST);
}

long long cat_atomic_long_decrement(CatAtomicLong *long_atomic) {
	CatAtomicLongPrivate *priv = CAT_ATOMIC_LONG_GET_PRIVATE(long_atomic);
	const long long delta = 1;
	return __atomic_sub_fetch(&(priv->value), delta, __ATOMIC_SEQ_CST);
}

long long cat_atomic_long_increment(CatAtomicLong *long_atomic) {
	CatAtomicLongPrivate *priv = CAT_ATOMIC_LONG_GET_PRIVATE(long_atomic);
	const long long delta = 1;
	return __atomic_add_fetch(&(priv->value), delta, __ATOMIC_SEQ_CST);
}


/********************* begin CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	cat_string_wo_format(append_to, "%s[%ld]", g_type_name_from_instance((GTypeInstance *) self), cat_atomic_long_get((CatAtomicLong *) self));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
