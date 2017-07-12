/*
   File:    jagpsimplediagnosticposition.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jul 3, 2017
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

#include "jagpsimplediagnosticposition.h"
#include "jagpidiagnosticposition.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPSimpleDiagnosticPosition"
#include <logging/catlog.h>

struct _JagPSimpleDiagnosticPositionPrivate {
	JagPCursor *at;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_diagnostic_position_iface_init(JagPIDiagnosticPositionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPSimpleDiagnosticPosition, jagp_simple_diagnostic_position, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPSimpleDiagnosticPosition)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(JAGP_TYPE_IDIAGNOSTIC_POSITION, l_diagnostic_position_iface_init);
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_simple_diagnostic_position_class_init(JagPSimpleDiagnosticPositionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_simple_diagnostic_position_init(JagPSimpleDiagnosticPosition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPSimpleDiagnosticPosition *instance = JAGP_SIMPLE_DIAGNOSTIC_POSITION(object);
	JagPSimpleDiagnosticPositionPrivate *priv = jagp_simple_diagnostic_position_get_instance_private(instance);
	cat_unref_ptr(priv->at);
	G_OBJECT_CLASS(jagp_simple_diagnostic_position_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_simple_diagnostic_position_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPSimpleDiagnosticPosition *jagp_simple_diagnostic_position_new(JagPCursor *at) {
	JagPSimpleDiagnosticPosition *result = g_object_new(JAGP_TYPE_SIMPLE_DIAGNOSTIC_POSITION, NULL);
	cat_ref_anounce(result);
	JagPSimpleDiagnosticPositionPrivate *priv = jagp_simple_diagnostic_position_get_instance_private(result);
	priv->at = cat_ref_ptr(at);
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPSimpleDiagnosticPosition *instance = JAGP_SIMPLE_DIAGNOSTIC_POSITION(self);
	JagPSimpleDiagnosticPositionPrivate *priv = jagp_simple_diagnostic_position_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

/********************* start JagPIDiagnosticPosition implementation *********************/

static JagPCursor *l_diagnostic_position_get_start_cursor(JagPIDiagnosticPosition *position) {
	JagPSimpleDiagnosticPositionPrivate *priv = jagp_simple_diagnostic_position_get_instance_private((JagPSimpleDiagnosticPosition *) position);
	return priv->at;
}

static JagPCursor *l_diagnostic_position_get_end_cursor(JagPIDiagnosticPosition *position) {
	JagPSimpleDiagnosticPositionPrivate *priv = jagp_simple_diagnostic_position_get_instance_private((JagPSimpleDiagnosticPosition *) position);
	return priv->at;
}

static JagPCursor *l_diagnostic_position_get_preferred_cursor(JagPIDiagnosticPosition *position) {
	JagPSimpleDiagnosticPositionPrivate *priv = jagp_simple_diagnostic_position_get_instance_private((JagPSimpleDiagnosticPosition *) position);
	return priv->at;
}

static struct _JagPJCTree *l_diagnostic_position_get_tree(JagPIDiagnosticPosition *position) {
	return NULL;
}

static void l_diagnostic_position_iface_init(JagPIDiagnosticPositionInterface *iface) {
	iface->getStartCursor = l_diagnostic_position_get_start_cursor;
	iface->getEndCursor = l_diagnostic_position_get_end_cursor;
	iface->getPreferredCursor = l_diagnostic_position_get_preferred_cursor;
	iface->getTree = l_diagnostic_position_get_tree;
}

/********************* end JagPIDiagnosticPosition implementation *********************/

