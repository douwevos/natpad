/*
   File:    jagbytattributelocalvariabletable.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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

#include "jagbytattributelocalvariabletable.h"
#include "jagbytlocalvariabletableentry.h"
#include "../../jagjobjectinputstream.h"
#include "../jagbytfield.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeLocalVariableTable"
#include <logging/catlog.h>

CatS jag_txt_attribute_local_variable_table = CAT_S_DEF("LocalVariableTable");

struct _JagBytAttributeLocalVariableTablePrivate {
	JagBytLocalVariableTable *table;
};

G_DEFINE_TYPE (JagBytAttributeLocalVariableTable, jag_byt_attribute_local_variable_table, JAG_BYT_TYPE_ATTRIBUTE)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_local_variable_table_class_init(JagBytAttributeLocalVariableTableClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytAttributeLocalVariableTablePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_local_variable_table_init(JagBytAttributeLocalVariableTable *instance) {
	JagBytAttributeLocalVariableTablePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE, JagBytAttributeLocalVariableTablePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytAttributeLocalVariableTable *instance = JAG_BYT_ATTRIBUTE_LOCAL_VARIABLE_TABLE(object);
	JagBytAttributeLocalVariableTablePrivate *priv = instance->priv;
	cat_unref_ptr(priv->table);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeLocalVariableTable *jag_byt_attribute_local_variable_table_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider) {
	JagBytAttributeLocalVariableTable *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE, NULL);
	cat_ref_anounce(result);
	JagBytAttributeLocalVariableTablePrivate *priv = result->priv;
	jag_byt_attribute_construct((JagBytAttribute *) result);


	JagJObjectInputStream *obj_istream = jag_jobject_input_stream_new_from_array(e_attribute_data);

	int tableLength = jag_jobject_input_stream_read_unsigned_short(obj_istream);
	priv->table = jag_byt_local_variable_table_new();
	int idx;
	for(idx=0; idx<tableLength; idx++) {
		int startPc = jag_jobject_input_stream_read_unsigned_short(obj_istream);
		int length = jag_jobject_input_stream_read_unsigned_short(obj_istream);
		int nameIndex = jag_jobject_input_stream_read_unsigned_short(obj_istream);
		int descriptorIndex = jag_jobject_input_stream_read_unsigned_short(obj_istream);
		int index = jag_jobject_input_stream_read_unsigned_short(obj_istream);


		CatStringWo *a_nameText = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, nameIndex-1);
		CatStringWo *a_descriptorText = jag_byt_iconstant_provider_get_constant_utf8_text(constant_provider, descriptorIndex-1);
		JagAstDeclarationType *declarationType = jag_byt_field_parse_descriptor(a_descriptorText);

		JagBytLocalVariableTableEntry *entry = jag_byt_local_variable_table_entry_new(startPc, length, nameIndex, a_nameText, descriptorIndex, declarationType, index);
		jag_byt_local_variable_table_add(priv->table, entry);
		cat_unref_ptr(entry);
		cat_unref_ptr(declarationType);
	}

	cat_unref_ptr(obj_istream);
	return result;
}






JagBytLocalVariableTable *jag_byt_attribute_local_variable_table_get_table(JagBytAttributeLocalVariableTable *attr_local_variable_table) {
	JagBytAttributeLocalVariableTablePrivate *priv = JAG_BYT_ATTRIBUTE_LOCAL_VARIABLE_TABLE_GET_PRIVATE(attr_local_variable_table);
	return priv->table;
}



