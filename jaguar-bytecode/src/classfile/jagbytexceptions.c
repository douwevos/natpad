/*
   File:    jagbytexceptions.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagbytexceptions.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytExceptions"
#include <logging/catlog.h>

struct _JagBytExceptionsPrivate {
	CatArrayWo *e_names;
};

G_DEFINE_TYPE (JagBytExceptions, jag_byt_exceptions, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_exceptions_class_init(JagBytExceptionsClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytExceptionsPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_exceptions_init(JagBytExceptions *instance) {
	JagBytExceptionsPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_EXCEPTIONS, JagBytExceptionsPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytExceptions *instance = JAG_BYT_EXCEPTIONS(object);
	JagBytExceptionsPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_names);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytExceptions *jag_byt_exceptions_new() {
	JagBytExceptions *result = g_object_new(JAG_BYT_TYPE_EXCEPTIONS, NULL);
	cat_ref_anounce(result);
	JagBytExceptionsPrivate *priv = result->priv;
	priv->e_names = cat_array_wo_new();
	return result;
}

void jag_byt_exceptions_add(JagBytExceptions *exceptions, JagBytName *exception_type_name) {
	JagBytExceptionsPrivate *priv = JAG_BYT_EXCEPTIONS_GET_PRIVATE(exceptions);
	cat_array_wo_append(priv->e_names, (GObject *) exception_type_name);
}



int jag_byt_exceptions_count(JagBytExceptions *exceptions) {
	JagBytExceptionsPrivate *priv = JAG_BYT_EXCEPTIONS_GET_PRIVATE(exceptions);
	return cat_array_wo_size(priv->e_names);
}

JagBytName *jag_byt_exceptions_get(JagBytExceptions *exceptions, int index) {
	JagBytExceptionsPrivate *priv = JAG_BYT_EXCEPTIONS_GET_PRIVATE(exceptions);
	return (JagBytName *) cat_array_wo_get(priv->e_names, index);
}

gboolean jag_byt_exceptions_equal(JagBytExceptions *exceptions_a, JagBytExceptions *exceptions_b) {
	if (exceptions_a==exceptions_b) {
		return TRUE;
	}
	if (exceptions_a==NULL || exceptions_b==NULL) {
		return FALSE;
	}
	JagBytExceptionsPrivate *priv_a = JAG_BYT_EXCEPTIONS_GET_PRIVATE(exceptions_a);
	JagBytExceptionsPrivate *priv_b = JAG_BYT_EXCEPTIONS_GET_PRIVATE(exceptions_b);

	return cat_array_wo_equal(priv_a->e_names, priv_b->e_names, (GEqualFunc) jag_byt_name_equal);
}

