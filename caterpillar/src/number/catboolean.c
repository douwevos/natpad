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
		G_ADD_PRIVATE(CatBoolean)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_boolean_class_init(CatBooleanClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_boolean_init(CatBoolean *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cat_boolean_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_boolean_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatBoolean *cat_boolean_new(gboolean init_value) {
	CatBoolean *result = g_object_new(CAT_TYPE_BOOLEAN, NULL);
	cat_ref_anounce(result);
	CatBooleanPrivate *priv = cat_boolean_get_instance_private(result);
	priv->value = init_value;
	return result;
}


int cat_boolean_value(CatBoolean *bobj) {
	CatBooleanPrivate *priv = cat_boolean_get_instance_private(bobj);
	return priv->value;
}

int cat_boolean_hash(CatBoolean *bobj) {
	CatBooleanPrivate *priv = cat_boolean_get_instance_private(bobj);
	return priv->value;
}

gboolean cat_boolean_equal(CatBoolean *bobj_a, CatBoolean *bobj_b) {
	if (bobj_a==bobj_b) {
		return TRUE;
	}
	if (bobj_a==NULL || bobj_b==NULL) {
		return FALSE;
	}

	CatBooleanPrivate *priv_a = cat_boolean_get_instance_private(bobj_a);
	CatBooleanPrivate *priv_b = cat_boolean_get_instance_private(bobj_b);
	return priv_a->value == priv_b->value;
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

	CatBooleanPrivate *priv_a = cat_boolean_get_instance_private(bobj_a);
	CatBooleanPrivate *priv_b = cat_boolean_get_instance_private(bobj_b);
	gboolean val_a = priv_a->value;
	gboolean val_b = priv_b->value;
	return val_a==val_b ? 0 : (val_a<val_b ? -1 : 1);
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatBooleanPrivate *bobj = cat_boolean_get_instance_private(CAT_BOOLEAN(self));
	cat_string_wo_append_chars(append_to, bobj->value ? "true" : "false");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/




