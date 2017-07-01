/*
   File:    jagpjcexpression.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 3, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "jagpjcexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCExpression"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPJCExpression, jagp_jcexpression, JAGP_TYPE_JCTREE,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_default_poly(JagPJCExpression *expression) { return FALSE; }
static gboolean l_default_standalone(JagPJCExpression *expression) { return TRUE; }

static void jagp_jcexpression_class_init(JagPJCExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->isPoly = l_default_poly;
	clazz->isStandalone = l_default_standalone;
}

static void jagp_jcexpression_init(JagPJCExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPJCExpression *instance = JAGP_JCEXPRESSION(object);
	G_OBJECT_CLASS(jagp_jcexpression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcexpression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCExpression *jagp_jcexpression_new() {
	JagPJCExpression *result = g_object_new(JAGP_TYPE_JCEXPRESSION, NULL);
	cat_ref_anounce(result);
	return result;
}

gboolean jagp_jcexpression_is_poly(JagPJCExpression *expression) {
	JagPJCExpressionClass *c = JAGP_JCEXPRESSION_GET_CLASS(expression);
	return c->isPoly(expression);
}

gboolean jagp_jcexpression_is_standalone(JagPJCExpression *expression) {
	JagPJCExpressionClass *c = JAGP_JCEXPRESSION_GET_CLASS(expression);
	return c->isStandalone(expression);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
