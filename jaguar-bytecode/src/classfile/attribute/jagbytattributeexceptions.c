/*
   File:    jagbytattributeexceptions.c
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

#include "jagbytattributeexceptions.h"
#include "../../jagjobjectinputstream.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeExceptions"
#include <logging/catlog.h>

CatS jag_txt_attribute_exceptions  = CAT_S_DEF("Exceptions");

struct _JagBytAttributeExceptionsPrivate {
	int exceptionsCount;
	int *exceptionIndexTable;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytAttributeExceptions, jag_byt_attribute_exceptions, JAG_BYT_TYPE_ATTRIBUTE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_exceptions_class_init(JagBytAttributeExceptionsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_exceptions_init(JagBytAttributeExceptions *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytAttributeExceptions *instance = JAG_BYT_ATTRIBUTE_EXCEPTIONS(object);
	JagBytAttributeExceptionsPrivate *priv = jag_byt_attribute_exceptions_get_instance_private(instance);
	cat_free_ptr(priv->exceptionIndexTable);
	G_OBJECT_CLASS(jag_byt_attribute_exceptions_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_attribute_exceptions_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeExceptions *jag_byt_attribute_exceptions_new(CatStringWo *e_attribute_data) {
	JagBytAttributeExceptions *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS, NULL);
	cat_ref_anounce(result);
	JagBytAttributeExceptionsPrivate *priv = jag_byt_attribute_exceptions_get_instance_private(result);
	jag_byt_attribute_construct((JagBytAttribute *) result);
	JagJObjectInputStream *obj_istream = jag_jobject_input_stream_new_from_array(e_attribute_data);

	int exceptionsCount = jag_jobject_input_stream_read_short(obj_istream) & 0xFFFF;
	priv->exceptionsCount = exceptionsCount;

	priv->exceptionIndexTable = g_malloc(sizeof(int)*exceptionsCount);
	int idx;
	for(idx=0; idx<exceptionsCount; idx++) {
		priv->exceptionIndexTable[idx] = jag_jobject_input_stream_read_short(obj_istream) & 0xFFFF;
	}
	cat_unref_ptr(obj_istream);
	return result;
}

int jag_byt_attribute_exceptions_count(JagBytAttributeExceptions *attribute_exceptions) {
	JagBytAttributeExceptionsPrivate *priv = jag_byt_attribute_exceptions_get_instance_private(attribute_exceptions);
	return priv->exceptionsCount;
}

int jag_byt_attribute_exceptions_get(JagBytAttributeExceptions *attribute_exceptions, int index) {
	JagBytAttributeExceptionsPrivate *priv = jag_byt_attribute_exceptions_get_instance_private(attribute_exceptions);
	return ((int) (priv->exceptionIndexTable[index])-1) & 0xFFFF;
}
