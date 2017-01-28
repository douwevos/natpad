/*
   File:    catfloat.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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

#include "catfloat.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatFloat"
#include "../logging/catlog.h"

struct _CatFloatPrivate {
	float value;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatFloat, cat_float, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_float_class_init(CatFloatClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(CatFloatPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_float_init(CatFloat *instance) {
	CatFloatPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_FLOAT, CatFloatPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	CatFloat *instance = CAT_FLOAT(object);
//	CatFloatPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatFloat *cat_float_new(float value) {
	CatFloat *result = g_object_new(CAT_TYPE_FLOAT, NULL);
	cat_ref_anounce(result);
	CatFloatPrivate *priv = result->priv;
	priv->value = value;
	return result;
}






float cat_float_value(CatFloat *fobj) {
	return CAT_FLOAT_GET_PRIVATE(fobj)->value;
}

int cat_float_hash(CatFloat *fobj) {
	return (int) (CAT_FLOAT_GET_PRIVATE(fobj)->value);
}

gboolean cat_float_equal(CatFloat *fobj_a, CatFloat *fobj_b) {
	if (fobj_a==fobj_b) {
		return TRUE;
	}
	if (fobj_a==NULL || fobj_b==NULL) {
		return FALSE;
	}

	float val_a = CAT_FLOAT_GET_PRIVATE(fobj_a)->value;
	float val_b = CAT_FLOAT_GET_PRIVATE(fobj_b)->value;
	return val_a==val_b;
}



int cat_float_compare(CatFloat *fobj_a, CatFloat *fobj_b) {
	if (fobj_a==fobj_b) {
		return TRUE;
	}
	if (fobj_a==NULL) {
		return -1;
	}
	if (fobj_b==NULL) {
		return 1;
	}

	float val_a = CAT_FLOAT_GET_PRIVATE(fobj_a)->value;
	float val_b = CAT_FLOAT_GET_PRIVATE(fobj_b)->value;
	return val_a==val_b ? 0 : (val_a<val_b ? -1 : 1);
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatFloatPrivate *fobj = CAT_FLOAT_GET_PRIVATE(self);
	char buf[100];
	sprintf(buf, "%f", fobj->value);
	cat_string_wo_append_chars(append_to, buf);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







