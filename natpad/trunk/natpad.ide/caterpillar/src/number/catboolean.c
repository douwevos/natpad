/*
   File:    catboolean.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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



#include "catboolean.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatBoolean"
#include "../logging/catlog.h"

struct _CatBooleanPrivate {
	gboolean value;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatBoolean, cat_boolean, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_boolean_class_init(CatBooleanClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(CatBooleanPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_boolean_init(CatBoolean *instance) {
	CatBooleanPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_BOOLEAN, CatBooleanPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	CatBoolean *instance = CAT_BOOLEAN(object);
//	CatBooleanPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatBoolean *cat_boolean_new(gboolean init_value) {
	CatBoolean *result = g_object_new(CAT_TYPE_BOOLEAN, NULL);
	cat_ref_anounce(result);
	CatBooleanPrivate *priv = result->priv;
	priv->value = init_value;
	return result;
}




int cat_boolean_value(CatBoolean *bobj) {
	return CAT_BOOLEAN_GET_PRIVATE(bobj)->value;
}

int cat_boolean_hash(CatBoolean *bobj) {
	return CAT_BOOLEAN_GET_PRIVATE(bobj)->value;
}

gboolean cat_boolean_equal(CatBoolean *bobj_a, CatBoolean *bobj_b) {
	if (bobj_a==bobj_b) {
		return TRUE;
	}
	if (bobj_a==NULL || bobj_b==NULL) {
		return FALSE;
	}

	gboolean val_a = CAT_BOOLEAN_GET_PRIVATE(bobj_a)->value;
	gboolean val_b = CAT_BOOLEAN_GET_PRIVATE(bobj_b)->value;
	return val_a==val_b;
}



int cat_boolean_compare(CatBoolean *bobj_a, CatBoolean *bobj_b) {
	if (bobj_a==bobj_b) {
		return TRUE;
	}
	if (bobj_a==NULL) {
		return -1;
	}
	if (bobj_b==NULL) {
		return 1;
	}

	gboolean val_a = CAT_BOOLEAN_GET_PRIVATE(bobj_a)->value;
	gboolean val_b = CAT_BOOLEAN_GET_PRIVATE(bobj_b)->value;
	return val_a==val_b ? 0 : (val_a<val_b ? -1 : 1);
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatBooleanPrivate *bobj = CAT_BOOLEAN_GET_PRIVATE(self);
	cat_string_wo_append_chars(append_to, bobj->value ? "true" : "false");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/




