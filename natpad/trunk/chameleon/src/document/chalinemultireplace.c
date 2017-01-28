/*
   File:    chalinemultireplace.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 25, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "chalinemultireplace.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLineMultiReplace"
#include <logging/catlog.h>


struct _ChaLineMultiReplacePrivate {
	ChaLineMultiReplaceEntry *entries;
	int entry_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLineMultiReplace, cha_line_multi_replace, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaLineMultiReplace)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_line_multi_replace_class_init(ChaLineMultiReplaceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_line_multi_replace_init(ChaLineMultiReplace *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaLineMultiReplace *instance = CHA_LINE_MULTI_REPLACE(object);
	ChaLineMultiReplacePrivate *priv = cha_line_multi_replace_get_instance_private(instance);
	if (priv->entries) {
		int idx=0;
		for(idx=priv->entry_count-1; idx>=0; idx--) {
			cat_unref_ptr(priv->entries[idx].replacement);
		}
		cat_free_ptr(priv->entries);
	}
	priv->entry_count = 0;
	G_OBJECT_CLASS(cha_line_multi_replace_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_line_multi_replace_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaLineMultiReplace *cha_line_multi_replace_new() {
	ChaLineMultiReplace *result = g_object_new(CHA_TYPE_LINE_MULTI_REPLACE, NULL);
	cat_ref_anounce(result);
	ChaLineMultiReplacePrivate *priv = cha_line_multi_replace_get_instance_private(result);
	priv->entry_count = 0;
	priv->entries = NULL;
	return result;
}

void cha_line_multi_replace_append(ChaLineMultiReplace *multi_replace, CatStringWo *replacement, int start_offset, int end_offset) {
	ChaLineMultiReplacePrivate *priv = cha_line_multi_replace_get_instance_private(multi_replace);
	int off = priv->entry_count;
	priv->entry_count++;
	if (priv->entry_count==1) {
		priv->entries = g_malloc(sizeof(ChaLineMultiReplaceEntry));
	} else {
		priv->entries = g_realloc(priv->entries, sizeof(ChaLineMultiReplaceEntry)*priv->entry_count);
	}
	priv->entries[off].replacement = cat_ref_ptr(replacement);
	priv->entries[off].start_offset = start_offset;
	priv->entries[off].end_offset = end_offset;
}

int cha_line_multi_replace_entry_count(const ChaLineMultiReplace *multi_replace) {
	const ChaLineMultiReplacePrivate *priv = cha_line_multi_replace_get_instance_private((ChaLineMultiReplace *) multi_replace);
	return priv->entry_count;
}

const ChaLineMultiReplaceEntry *cha_line_multi_replace_entry_at(const ChaLineMultiReplace *multi_replace, int index) {
	const ChaLineMultiReplacePrivate *priv = cha_line_multi_replace_get_instance_private((ChaLineMultiReplace *) multi_replace);
	return priv->entries + index;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLineMultiReplace *instance = CHA_LINE_MULTI_REPLACE(self);
	ChaLineMultiReplacePrivate *priv = cha_line_multi_replace_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p entry_count=%d]", iname, self, priv->entry_count);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
