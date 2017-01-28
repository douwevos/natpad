/*
   File:    jagbyttrycatch.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#include "jagbyttrycatch.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytTryCatch"
#include <logging/catlog.h>

struct _JagBytTryCatchPrivate {
	int start_pc;
	int end_pc;
	CatArrayWo *e_catch_list;
};

G_DEFINE_TYPE (JagBytTryCatch, jag_byt_try_catch, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_try_catch_class_init(JagBytTryCatchClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytTryCatchPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_try_catch_init(JagBytTryCatch *instance) {
	JagBytTryCatchPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_TRY_CATCH, JagBytTryCatchPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytTryCatch *instance = JAG_BYT_TRY_CATCH(object);
	JagBytTryCatchPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_catch_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytTryCatch *jag_byt_try_catch_new(int start_pc, int end_pc, CatArrayWo *e_catch_list) {
	JagBytTryCatch *result = g_object_new(JAG_BYT_TYPE_TRY_CATCH, NULL);
	cat_ref_anounce(result);
	JagBytTryCatchPrivate *priv = result->priv;
	priv->start_pc = start_pc;
	priv->end_pc = end_pc;
	priv->e_catch_list = cat_ref_ptr(e_catch_list);
	return result;
}


int jag_byt_try_catch_get_start_pc(JagBytTryCatch *try_catch) {
	JagBytTryCatchPrivate *priv = JAG_BYT_TRY_CATCH_GET_PRIVATE(try_catch);
	return priv->start_pc;
}

int jag_byt_try_catch_get_end_pc(JagBytTryCatch *try_catch) {
	JagBytTryCatchPrivate *priv = JAG_BYT_TRY_CATCH_GET_PRIVATE(try_catch);
	return priv->end_pc;
}

int jag_byt_try_catch_get_catch_count(JagBytTryCatch *try_catch) {
	JagBytTryCatchPrivate *priv = JAG_BYT_TRY_CATCH_GET_PRIVATE(try_catch);
	return cat_array_wo_size(priv->e_catch_list);
}

JagBytICatchOrFinally *jag_byt_try_catch_get_catch_at(JagBytTryCatch *try_catch, int index) {
	JagBytTryCatchPrivate *priv = JAG_BYT_TRY_CATCH_GET_PRIVATE(try_catch);
	return (JagBytICatchOrFinally *) cat_array_wo_get(priv->e_catch_list, index);
}


CatIIterator *jag_byt_try_catch_iterator(JagBytTryCatch *try_catch) {
	JagBytTryCatchPrivate *priv = JAG_BYT_TRY_CATCH_GET_PRIVATE(try_catch);
	return cat_array_wo_iterator(priv->e_catch_list);
}



