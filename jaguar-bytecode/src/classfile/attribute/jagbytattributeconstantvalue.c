/*
   File:    jagbytattributeconstantvalue.c
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

#include "jagbytattributeconstantvalue.h"
#include "../../jagjobjectinputstream.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeConstantValue"
#include <logging/catlog.h>

CatS jag_txt_attribute_constant_value = CAT_S_DEF("ConstantValue");

struct _JagBytAttributeConstantValuePrivate {
	uint16_t constant_value_index;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytAttributeConstantValue, jag_byt_attribute_constant_value, JAG_BYT_TYPE_ATTRIBUTE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_constant_value_class_init(JagBytAttributeConstantValueClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_constant_value_init(JagBytAttributeConstantValue *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_attribute_constant_value_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_attribute_constant_value_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeConstantValue *jag_byt_attribute_constant_value_new(CatStringWo *e_attribute_data) {
	JagBytAttributeConstantValue *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_CONSTANT_VALUE, NULL);
	cat_ref_anounce(result);
	JagBytAttributeConstantValuePrivate *priv = jag_byt_attribute_constant_value_get_instance_private(result);
	jag_byt_attribute_construct((JagBytAttribute *) result);
	JagJObjectInputStream *obj_istream = jag_jobject_input_stream_new_from_array(e_attribute_data);
	priv->constant_value_index = jag_jobject_input_stream_read_short(obj_istream);

	cat_unref_ptr(obj_istream);
	return result;
}

int jag_byt_attribute_constant_value_get_index(JagBytAttributeConstantValue *constant_value) {
	JagBytAttributeConstantValuePrivate *priv = jag_byt_attribute_constant_value_get_instance_private(constant_value);
	return ((int) priv->constant_value_index) & 0xFFFF;
}
