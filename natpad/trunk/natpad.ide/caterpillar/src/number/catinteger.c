/*
   File:    catinteger.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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

#include "catinteger.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatInteger"
#include "../logging/catlog.h"

struct _CatIntegerPrivate {
	int value;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatInteger, cat_integer, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_integer_class_init(CatIntegerClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(CatIntegerPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_integer_init(CatInteger *instance) {
	CatIntegerPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_INTEGER, CatIntegerPrivate);
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

CatInteger *cat_integer_new(int value) {
	CatInteger *result = g_object_new(CAT_TYPE_INTEGER, NULL);
	cat_ref_anounce(result);
	CatIntegerPrivate *priv = result->priv;
	priv->value = value;
	return result;
}


int cat_integer_value(CatInteger *iobj) {
	return CAT_INTEGER_GET_PRIVATE(iobj)->value;
}

int cat_integer_hash(CatInteger *iobj) {
	return CAT_INTEGER_GET_PRIVATE(iobj)->value;
}

gboolean cat_integer_equal(const CatInteger *iobj_a, const CatInteger *iobj_b) {
	if (iobj_a==iobj_b) {
		return TRUE;
	}
	if (iobj_a==NULL || iobj_b==NULL) {
		return FALSE;
	}

	int val_a = CAT_INTEGER_GET_PRIVATE(iobj_a)->value;
	int val_b = CAT_INTEGER_GET_PRIVATE(iobj_b)->value;
	return val_a==val_b;
}



int cat_integer_compare(CatInteger *iobj_a, CatInteger *iobj_b) {
	if (iobj_a==iobj_b) {
		return TRUE;
	}
	if (iobj_a==NULL) {
		return -1;
	}
	if (iobj_b==NULL) {
		return 1;
	}

	int val_a = CAT_INTEGER_GET_PRIVATE(iobj_a)->value;
	int val_b = CAT_INTEGER_GET_PRIVATE(iobj_b)->value;
	return val_a==val_b ? 0 : (val_a<val_b ? -1 : 1);
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatIntegerPrivate *iobj = CAT_INTEGER_GET_PRIVATE(self);
	cat_string_wo_append_decimal(append_to, iobj->value);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
