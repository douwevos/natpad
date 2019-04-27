/*
   File:    jagbytlocalvariabletableentry.c
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

#include "jagbytlocalvariabletableentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytLocalVariableTableEntry"
#include <logging/catlog.h>

struct _JagBytLocalVariableTableEntryPrivate {
	int start_pc;
	int length;
	int name_index;
	CatStringWo *a_name_text;
	int descriptor_index;
	JagAstDeclarationType *declaration_type;
	int index;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagBytLocalVariableTableEntry, jag_byt_local_variable_table_entry, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_local_variable_table_entry_class_init(JagBytLocalVariableTableEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_local_variable_table_entry_init(JagBytLocalVariableTableEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytLocalVariableTableEntry *instance = JAG_BYT_LOCAL_VARIABLE_TABLE_ENTRY(object);
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(instance);
	cat_unref_ptr(priv->declaration_type);
	cat_unref_ptr(priv->a_name_text);
	G_OBJECT_CLASS(jag_byt_local_variable_table_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_local_variable_table_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagBytLocalVariableTableEntry *jag_byt_local_variable_table_entry_new(int start_pc, int length, int name_index, CatStringWo *a_name_text, int descriptor_index, JagAstDeclarationType *declaration_type, int index) {
	JagBytLocalVariableTableEntry *result = g_object_new(JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE_ENTRY, NULL);
	cat_ref_anounce(result);
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(result);
	priv->start_pc = start_pc;
	priv->length = length;
	priv->name_index = name_index;
	priv->a_name_text = cat_ref_ptr(a_name_text);
	priv->descriptor_index = descriptor_index;
	priv->declaration_type = cat_ref_ptr(declaration_type);
	priv->index = index;
	return result;
}

int jag_byt_local_variable_table_entry_get_start_pc(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->start_pc;
}

int jag_byt_local_variable_table_entry_get_length(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->length;
}

int jag_byt_local_variable_table_entry_get_name_index(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->name_index;
}

CatStringWo *jag_byt_local_variable_table_entry_get_name(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->a_name_text;
}

int jag_byt_local_variable_table_entry_get_descriptor_index(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->descriptor_index;
}

JagAstDeclarationType *jag_byt_local_variable_table_entry_get_declaration_type(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->declaration_type;
}

int jag_byt_local_variable_table_entry_get_index(JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTableEntryPrivate *priv = jag_byt_local_variable_table_entry_get_instance_private(entry);
	return priv->index;
}
