/*
   File:    jagbytlinenumbertableentry.c
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

#include "jagbytlinenumbertableentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytLineNumberTableEntry"
#include <logging/catlog.h>

struct _JagBytLineNumberTableEntryPrivate {
	int start_pc;
	int line_number;
};

G_DEFINE_TYPE (JagBytLineNumberTableEntry, jag_byt_line_number_table_entry, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_line_number_table_entry_class_init(JagBytLineNumberTableEntryClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytLineNumberTableEntryPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_line_number_table_entry_init(JagBytLineNumberTableEntry *instance) {
	JagBytLineNumberTableEntryPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY, JagBytLineNumberTableEntryPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytLineNumberTableEntry *instance = JAG_BYT_LINE_NUMBER_TABLE_ENTRY(object);
//	JagBytLineNumberTableEntryPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytLineNumberTableEntry *jag_byt_line_number_table_entry_new(int startPc, int lineNumber) {
	JagBytLineNumberTableEntry *result = g_object_new(JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY, NULL);
	cat_ref_anounce(result);
	JagBytLineNumberTableEntryPrivate *priv = result->priv;
	priv->start_pc = startPc;
	priv->line_number = lineNumber;
	return result;
}


int jag_byt_line_number_table_entry_get_start_pc(JagBytLineNumberTableEntry *entry) {
	return JAG_BYT_LINE_NUMBER_TABLE_ENTRY_GET_PRIVATE(entry)->start_pc;
}

int jag_byt_line_number_table_entry_get_line_number(JagBytLineNumberTableEntry *entry) {
	return JAG_BYT_LINE_NUMBER_TABLE_ENTRY_GET_PRIVATE(entry)->line_number;
}






