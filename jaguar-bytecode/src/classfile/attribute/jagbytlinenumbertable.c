/*
   File:    jagbytlinenumbertable.c
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

#include "jagbytlinenumbertable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytLineNumberTable"
#include <logging/catlog.h>

struct _JagBytLineNumberTablePrivate {
	CatArrayWo *e_entries;
};

G_DEFINE_TYPE (JagBytLineNumberTable, jag_byt_line_number_table, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_line_number_table_class_init(JagBytLineNumberTableClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytLineNumberTablePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_line_number_table_init(JagBytLineNumberTable *instance) {
	JagBytLineNumberTablePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_LINE_NUMBER_TABLE, JagBytLineNumberTablePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytLineNumberTable *instance = JAG_BYT_LINE_NUMBER_TABLE(object);
	JagBytLineNumberTablePrivate *priv = instance->priv;
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

void jag_byt_line_number_table_construct(JagBytLineNumberTable *line_number_table) {
	JagBytLineNumberTablePrivate *priv = JAG_BYT_LINE_NUMBER_TABLE_GET_PRIVATE(line_number_table);
	priv->e_entries = cat_array_wo_new();
}

JagBytLineNumberTable *jag_byt_line_number_table_new() {
	JagBytLineNumberTable *result = g_object_new(JAG_BYT_TYPE_LINE_NUMBER_TABLE, NULL);
	cat_ref_anounce(result);
	jag_byt_line_number_table_construct(result);
	return result;
}


void jag_byt_line_number_table_add(JagBytLineNumberTable *line_number_table, JagBytLineNumberTableEntry *entry) {
	JagBytLineNumberTablePrivate *priv = JAG_BYT_LINE_NUMBER_TABLE_GET_PRIVATE(line_number_table);
	cat_array_wo_append(priv->e_entries, (GObject *) entry);
}

CatIIterator *jag_byt_line_number_table_iterator(JagBytLineNumberTable *line_number_table) {
	JagBytLineNumberTablePrivate *priv = JAG_BYT_LINE_NUMBER_TABLE_GET_PRIVATE(line_number_table);
	return cat_array_wo_iterator(priv->e_entries);
}


