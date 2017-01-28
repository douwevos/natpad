/*
   File:    jagbytlocalvariabletable.c
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

#include "jagbytlocalvariabletable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytLocalVariableTable"
#include <logging/catlog.h>

struct _JagBytLocalVariableTablePrivate {
	CatArrayWo *e_entries;
};

G_DEFINE_TYPE (JagBytLocalVariableTable, jag_byt_local_variable_table, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_local_variable_table_class_init(JagBytLocalVariableTableClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytLocalVariableTablePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_local_variable_table_init(JagBytLocalVariableTable *instance) {
	JagBytLocalVariableTablePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE, JagBytLocalVariableTablePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytLocalVariableTable *instance = JAG_BYT_LOCAL_VARIABLE_TABLE(object);
	JagBytLocalVariableTablePrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_entries);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytLocalVariableTable *jag_byt_local_variable_table_new() {
	JagBytLocalVariableTable *result = g_object_new(JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE, NULL);
	cat_ref_anounce(result);
	JagBytLocalVariableTablePrivate *priv = result->priv;
	priv->e_entries = cat_array_wo_new();
	return result;
}






CatStringWo *jag_byt_local_variable_table_find_name(JagBytLocalVariableTable *local_variable_table, int frameIndex, int index) {
//	JagBytLocalVariableTablePrivate *priv = JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(local_variable_table);
	JagBytLocalVariableTableEntry *entry = jag_byt_local_variable_table_find_entry(local_variable_table, frameIndex, index);
	if (entry != NULL) {
		return jag_byt_local_variable_table_entry_get_name(entry);
	}
	return NULL;
}


JagBytLocalVariableTableEntry *jag_byt_local_variable_table_find_entry(JagBytLocalVariableTable *local_variable_table, int frameIndex, int index) {
	JagBytLocalVariableTablePrivate *priv = JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(local_variable_table);
	JagBytLocalVariableTableEntry *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_entries);
	while(cat_iiterator_has_next(iter)) {
		JagBytLocalVariableTableEntry *entry = (JagBytLocalVariableTableEntry *) cat_iiterator_next(iter);
		int e_start_pc = jag_byt_local_variable_table_entry_get_start_pc(entry);
		if (jag_byt_local_variable_table_entry_get_index(entry)==frameIndex
				&& index>=e_start_pc
				&& index<=(e_start_pc+jag_byt_local_variable_table_entry_get_length(entry))) {
			result = entry;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}



void jag_byt_local_variable_table_add(JagBytLocalVariableTable *local_variable_table, JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTablePrivate *priv = JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(local_variable_table);
	cat_array_wo_append(priv->e_entries, (GObject *) entry);
}


void jag_byt_local_variable_table_remove(JagBytLocalVariableTable *local_variable_table, JagBytLocalVariableTableEntry *entry) {
	JagBytLocalVariableTablePrivate *priv = JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(local_variable_table);
	int idx = cat_array_wo_find_index(priv->e_entries, entry, -1);
	if (idx>0) {
		cat_array_wo_remove(priv->e_entries, idx, NULL);
	}
}

gboolean jag_byt_local_variable_table_isEmpty(JagBytLocalVariableTable *local_variable_table) {
	JagBytLocalVariableTablePrivate *priv = JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(local_variable_table);
	return cat_array_wo_size(priv->e_entries)==0;
}

int jag_byt_local_variable_table_count(JagBytLocalVariableTable *local_variable_table) {
	JagBytLocalVariableTablePrivate *priv = JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(local_variable_table);
	return cat_array_wo_size(priv->e_entries);
}

