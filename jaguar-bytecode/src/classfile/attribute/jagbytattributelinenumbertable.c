/*
   File:    jagbytattributelinenumbertable.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 8, 2012
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

#include "jagbytattributelinenumbertable.h"
#include "jagbytlinenumbertableentry.h"
#include "../../jagjobjectinputstream.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAttributeLineNumberTable"
#include <logging/catlog.h>

CatS jag_txt_attribute_line_number_table = CAT_S_DEF("LineNumberTable");

struct _JagBytAttributeLineNumberTablePrivate {
	JagBytLineNumberTable *table;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytAttributeLineNumberTable, jag_byt_attribute_line_number_table, JAG_BYT_TYPE_ATTRIBUTE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_attribute_line_number_table_class_init(JagBytAttributeLineNumberTableClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_attribute_line_number_table_init(JagBytAttributeLineNumberTable *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytAttributeLineNumberTable *instance = JAG_BYT_ATTRIBUTE_LINE_NUMBER_TABLE(object);
	JagBytAttributeLineNumberTablePrivate *priv = jag_byt_attribute_line_number_table_get_instance_private(instance);
	cat_unref_ptr(priv->table);
	G_OBJECT_CLASS(jag_byt_attribute_line_number_table_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_attribute_line_number_table_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytAttributeLineNumberTable *jag_byt_attribute_line_number_table_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider) {
	JagBytAttributeLineNumberTable *result = g_object_new(JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE, NULL);
	cat_ref_anounce(result);
	JagBytAttributeLineNumberTablePrivate *priv = jag_byt_attribute_line_number_table_get_instance_private(result);
	jag_byt_attribute_construct((JagBytAttribute *) result);

	JagJObjectInputStream *obj_istream = jag_jobject_input_stream_new_from_array(e_attribute_data);

	int tableLength = jag_jobject_input_stream_read_unsigned_short(obj_istream);
	priv->table = jag_byt_line_number_table_new();

	int idx;
	for(idx=0; idx<tableLength; idx++) {
		int start_pc = jag_jobject_input_stream_read_unsigned_short(obj_istream);
		int line_number = jag_jobject_input_stream_read_unsigned_short(obj_istream);
		JagBytLineNumberTableEntry *entry = jag_byt_line_number_table_entry_new(start_pc, line_number);
		jag_byt_line_number_table_add(priv->table, entry);
		cat_unref_ptr(entry);
	}

	cat_unref_ptr(obj_istream);
	return result;
}


JagBytLineNumberTable *jag_byt_attribute_line_number_table_get_table(JagBytAttributeLineNumberTable *attr_line_number_table) {
	JagBytAttributeLineNumberTablePrivate *priv = jag_byt_attribute_line_number_table_get_instance_private(attr_line_number_table);
	return priv->table;
}
