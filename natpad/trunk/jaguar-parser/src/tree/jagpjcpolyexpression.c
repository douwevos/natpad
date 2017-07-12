/*
   File:    jagpjcpolyexpression.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 4, 2017
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

#include "jagpjcpolyexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPJCPolyExpression"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagPJCPolyExpression, jagp_jcpoly_expression, JAGP_TYPE_JCEXPRESSION);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_jcpoly_expression_class_init(JagPJCPolyExpressionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_jcpoly_expression_init(JagPJCPolyExpression *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPJCPolyExpression *instance = JAGP_JCPOLY_EXPRESSION(object);
	G_OBJECT_CLASS(jagp_jcpoly_expression_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_jcpoly_expression_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPJCPolyExpression *jagp_jcpoly_expression_new() {
	JagPJCPolyExpression *result = g_object_new(JAGP_TYPE_JCPOLY_EXPRESSION, NULL);
	cat_ref_anounce(result);
//	JAGP_JCEXPRESSION_construct((JagPJCExpression *) result);
	return result;
}


