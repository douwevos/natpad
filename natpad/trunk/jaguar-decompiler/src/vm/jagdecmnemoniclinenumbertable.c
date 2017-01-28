/*
   File:    jagdecmnemoniclinenumbertable.c
   Project: jaguar-decompiler
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

#include "jagdecmnemoniclinenumbertable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecMnemonicLineNumberTable"
#include <logging/catlog.h>

struct _JagDecMnemonicLineNumberTablePrivate {
	void *dummy;
};

G_DEFINE_TYPE (JagDecMnemonicLineNumberTable, jag_dec_mnemonic_line_number_table, JAG_BYT_TYPE_LINE_NUMBER_TABLE)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_mnemonic_line_number_table_class_init(JagDecMnemonicLineNumberTableClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecMnemonicLineNumberTablePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_mnemonic_line_number_table_init(JagDecMnemonicLineNumberTable *instance) {
	JagDecMnemonicLineNumberTablePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE, JagDecMnemonicLineNumberTablePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecMnemonicLineNumberTable *instance = JAG_DEC_MNEMONIC_LINE_NUMBER_TABLE(object);
//	JagDecMnemonicLineNumberTablePrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecMnemonicLineNumberTable *jag_dec_mnemonic_line_number_table_new(JagBytIMnemonicBlock *mnemonic_block, JagBytLineNumberTable *line_number_table) {
	JagDecMnemonicLineNumberTable *result = g_object_new(JAG_DEC_TYPE_MNEMONIC_LINE_NUMBER_TABLE, NULL);
	cat_ref_anounce(result);
	jag_byt_line_number_table_construct((JagBytLineNumberTable *) result);


	if (line_number_table) {
		CatIIterator *iter = jag_byt_line_number_table_iterator((JagBytLineNumberTable *) line_number_table);
		while(cat_iiterator_has_next(iter)) {
			JagBytLineNumberTableEntry *entry = (JagBytLineNumberTableEntry *) cat_iiterator_next(iter);
			int entry_start_pc = jag_byt_line_number_table_entry_get_start_pc(entry);
			int mnemonicIndex = jag_byt_imnemonic_block_find_by_bytecode_offset(mnemonic_block, entry_start_pc);
			int entry_line_number = jag_byt_line_number_table_entry_get_line_number(entry);
			JagBytLineNumberTableEntry *new_entry = jag_byt_line_number_table_entry_new(mnemonicIndex, entry_line_number);
			jag_byt_line_number_table_add((JagBytLineNumberTable *) result, new_entry);
			cat_unref_ptr(new_entry);
		}
		cat_unref_ptr(iter);
	}
	return result;
}

int jag_dec_mnemonic_line_number_table_get_least_line_number(JagDecMnemonicLineNumberTable *table) {
//	JagDecMnemonicLineNumberTablePrivate *priv = JAG_DEC_MNEMONIC_LINE_NUMBER_TABLE_GET_PRIVATE(table);
	int result = -1;
	CatIIterator *iter = jag_byt_line_number_table_iterator((JagBytLineNumberTable *) table);
	while(cat_iiterator_has_next(iter)) {
		JagBytLineNumberTableEntry *entry = (JagBytLineNumberTableEntry *) cat_iiterator_next(iter);
		int entry_line_number = jag_byt_line_number_table_entry_get_line_number(entry);
		if ((entry_line_number!=-1 && entry_line_number<result) || (result == -1)) {
			result = entry_line_number;
		}
	}
	cat_unref_ptr(iter);
	return result;
}





