/*
   File:    jagdeccombineitem.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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

#include "jagdeccombineitem.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecCombineItem"
#include <logging/catlog.h>


G_DEFINE_TYPE (JagDecCombineItem, jag_dec_combine_item, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_combine_item_class_init(JagDecCombineItemClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_combine_item_init(JagDecCombineItem *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecCombineItem *instance = JAG_DEC_COMBINE_ITEM(object);
	cat_unref_ptr(instance->groupOne);
	cat_unref_ptr(instance->groupTwo);
	cat_unref_ptr(instance->output);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecCombineItem *jag_dec_combine_item_new() {
	JagDecCombineItem *result = g_object_new(JAG_DEC_TYPE_COMBINE_ITEM, NULL);
	cat_ref_anounce(result);
	result->doBreak = FALSE;
	result->lastIndex = 0;
	result->groupOne = NULL;
	result->groupTwo = NULL;
	result->output = NULL;
	return result;
}




void jag_dec_combine_item_prepend_combined(JagDecCombineItem *combine_item, JagDecIIntermediateNode *nodeOne, JagDecIIntermediateNode *nodeTwo) {
	if (nodeOne==NULL || nodeTwo==NULL) {
		return;
	}
	if (combine_item->output==NULL) {
		combine_item->output = jag_dec_intermediate_group_new();
	}
	jag_dec_intermediate_group_add_at(combine_item->output, 0, nodeOne);
}

void jag_dec_combine_item_prepend(JagDecCombineItem *combine_item, JagDecIntermediateGroup *group) {
	if (group==NULL) {
		return;
	}
	if (combine_item->output==NULL) {
		combine_item->output = jag_dec_intermediate_group_new();
	}
	jag_dec_intermediate_group_add_at(combine_item->output, 0, (JagDecIIntermediateNode *) group);
}



