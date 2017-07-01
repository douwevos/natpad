/*
   File:    jagpjcfunctionalexpression.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 7, 2017
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

#include "jagpjcfunctionalexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPJCFunctionalExpression"
#include <logging/catlog.h>


G_DEFINE_TYPE(JagPJCFunctionalExpression, jagp_jcfunctional_expression, JAGP_TYPE_JCPOLY_EXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_jcfunctional_expression_class_init(JagPJCFunctionalExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_jcfunctional_expression_init(JagPJCFunctionalExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPJCFunctionalExpression *instance = JAGP_JCFUNCTIONAL_EXPRESSION(object);
	G_OBJECT_CLASS(jagp_jcfunctional_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcfunctional_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCFunctionalExpression *jagp_jcfunctional_expression_new() {
	JagPJCFunctionalExpression *result = g_object_new(JAGP_TYPE_JCFUNCTIONAL_EXPRESSION, NULL);
	cat_ref_anounce(result);
//	JAGP_JCPOLY_EXPRESSION_construct((JagPJCPolyExpression *) result);
	return result;
}


