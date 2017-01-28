/*
   File:    jagbytattributecodeexception.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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

#include "jagbytattributecodeexception.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeCodeException"
#include <logging/catlog.h>

struct _JagBytAttributeCodeExceptionPrivate {
	uint16_t startPc;
	uint16_t endPc;
	uint16_t handlerPc;
	uint16_t catchType;
};

G_DEFINE_TYPE (JagBytAttributeCodeException, jag_byt_attribute_code_exception, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_code_exception_class_init(JagBytAttributeCodeExceptionClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytAttributeCodeExceptionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_code_exception_init(JagBytAttributeCodeException *instance) {
	JagBytAttributeCodeExceptionPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_ATTRIBUTE_CODE_EXCEPTION, JagBytAttributeCodeExceptionPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytAttributeCodeException *instance = JAG_BYT_ATTRIBUTE_CODE_EXCEPTION(object);
//	JagBytAttributeCodeExceptionPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeCodeException *jag_byt_attribute_code_exception_new(uint16_t startPc, uint16_t endPc, uint16_t handlerPc, uint16_t catchType) {
	JagBytAttributeCodeException *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_CODE_EXCEPTION, NULL);
	cat_ref_anounce(result);
	JagBytAttributeCodeExceptionPrivate *priv = result->priv;
	priv->startPc = startPc;
	priv->endPc = endPc;
	priv->handlerPc = handlerPc;
	priv->catchType = catchType;
	return result;
}



int jag_byt_attribute_code_exception_get_start_pc(JagBytAttributeCodeException *code_exception) {
	JagBytAttributeCodeExceptionPrivate *priv = JAG_BYT_ATTRIBUTE_CODE_EXCEPTION_GET_PRIVATE(code_exception);
	return priv->startPc;
}

int jag_byt_attribute_code_exception_get_end_pc(JagBytAttributeCodeException *code_exception) {
	JagBytAttributeCodeExceptionPrivate *priv = JAG_BYT_ATTRIBUTE_CODE_EXCEPTION_GET_PRIVATE(code_exception);
	return priv->endPc;
}

int jag_byt_attribute_code_exception_get_handler_pc(JagBytAttributeCodeException *code_exception) {
	JagBytAttributeCodeExceptionPrivate *priv = JAG_BYT_ATTRIBUTE_CODE_EXCEPTION_GET_PRIVATE(code_exception);
	return priv->handlerPc;
}


int jag_byt_attribute_code_exception_get_catch_type(JagBytAttributeCodeException *code_exception) {
	JagBytAttributeCodeExceptionPrivate *priv = JAG_BYT_ATTRIBUTE_CODE_EXCEPTION_GET_PRIVATE(code_exception);
	return priv->catchType;
}
