/*
   File:    grorunreducelayout.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Nov 3, 2016
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

#include "grorunreducelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunReduceLayout"
#include <logging/catlog.h>

struct _GroRunReduceLayoutPrivate {
	GroRunSymbol *lhs;
	int rhs_count;
	CatIntArrayWo *nullified;
	int production_id;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunReduceLayout, grorun_reduce_layout, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunReduceLayout)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_reduce_layout_class_init(GroRunReduceLayoutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_reduce_layout_init(GroRunReduceLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunReduceLayout *instance = GRORUN_REDUCE_LAYOUT(object);
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(instance);
	cat_unref_ptr(priv->lhs);
	cat_unref_ptr(priv->nullified);
	G_OBJECT_CLASS(grorun_reduce_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_reduce_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

GroRunReduceLayout *grorun_reduce_layout_new(GroRunSymbol *lhs, int rhs_count, CatIntArrayWo *nullified, int production_id) {
	GroRunReduceLayout *result = g_object_new(GRORUN_TYPE_REDUCE_LAYOUT, NULL);
	cat_ref_anounce(result);
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(result);
	priv->lhs = cat_ref_ptr(lhs);
	priv->rhs_count = rhs_count;
	priv->nullified = cat_ref_ptr(nullified);
	priv->production_id = production_id;
	return result;
}


GroRunSymbol *grorun_reduce_layout_get_lhs(GroRunReduceLayout *layout) {
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(layout);
	return priv->lhs;
}

int grorun_reduce_layout_get_production_id(GroRunReduceLayout *layout) {
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(layout);
	return priv->production_id;
}


int grorun_reduce_layout_get_stack_rhs_count(GroRunReduceLayout *layout) {
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(layout);
	int result = priv->rhs_count;
	if (priv->nullified) {
		result -= cat_int_array_wo_size(priv->nullified);
	}
	return result;
}

int grorun_reduce_layout_calculate_stack_offset(GroRunReduceLayout *layout, int offset) {
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(layout);
	int result = priv->rhs_count-offset;
	if (priv->nullified!=NULL) {
		int nlen = cat_int_array_wo_size(priv->nullified);
		int nidx;
		for(nidx=0; nidx<nlen; nidx++) {
			int off = cat_int_array_wo_get(priv->nullified, nidx);
			if (off==offset) {
				return -1;
			} else if (off>offset) {
				result--;
			}
		}
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunReduceLayout *instance = GRORUN_REDUCE_LAYOUT(self);
	GroRunReduceLayoutPrivate *priv = grorun_reduce_layout_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, lhs=%O, rhs-count=%d, prod-id=%d, nullified=%O]", iname, self, priv->lhs, priv->rhs_count, priv->production_id, priv->nullified);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
