/*
   File:    catlong.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 16, 2012
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

#include "catlong.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatLong"
#include "../logging/catlog.h"

struct _CatLongPrivate {
	int64_t value;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatLong, cat_long, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(CatLong)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_long_class_init(CatLongClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_long_init(CatLong *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cat_long_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_long_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

CatLong *cat_long_new(int64_t value) {
	CatLong *result = g_object_new(CAT_TYPE_LONG, NULL);
	cat_ref_anounce(result);
	CatLongPrivate *priv = cat_long_get_instance_private(result);
	priv->value = value;
	return result;
}


int64_t cat_long_value(CatLong *lobj) {
	CatLongPrivate *priv = cat_long_get_instance_private(lobj);
	return priv->value;
}

int cat_long_hash(CatLong *lobj) {
	CatLongPrivate *priv = cat_long_get_instance_private(lobj);
	int p1 = (int) priv->value;
	int p2 = (int) (priv->value>>32);
	return p1+p2;
}

gboolean cat_long_equal(CatLong *lobj_a, CatLong *lobj_b) {
	if (lobj_a==lobj_b) {
		return TRUE;
	}
	if (lobj_a==NULL || lobj_b==NULL) {
		return FALSE;
	}
	CatLongPrivate *priv_a = cat_long_get_instance_private(lobj_a);
	CatLongPrivate *priv_b = cat_long_get_instance_private(lobj_b);
	int64_t val_a = priv_a->value;
	int64_t val_b = priv_b->value;
	return val_a==val_b;

}

int cat_long_compare(CatLong *lobj_a, CatLong *lobj_b) {
	if (lobj_a==lobj_b) {
		return TRUE;
	}
	if (lobj_a==NULL) {
		return -1;
	}
	if (lobj_b==NULL) {
		return 1;
	}

	CatLongPrivate *priv_a = cat_long_get_instance_private(lobj_a);
	CatLongPrivate *priv_b = cat_long_get_instance_private(lobj_b);
	int64_t val_a = priv_a->value;
	int64_t val_b = priv_b->value;
	return val_a==val_b ? 0 : (val_a<val_b ? -1 : 1);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatLongPrivate *lobj = cat_long_get_instance_private(CAT_LONG(self));
	cat_string_wo_append_decimal(append_to, lobj->value);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
