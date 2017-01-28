/*
   File:    jagdecimportnameentry.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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

#include "jagdecimportnameentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecImportNameEntry"
#include <logging/catlog.h>

struct _JagDecImportNameEntryPrivate {
	CatStringWo *a_short_type_name;
	CatHashSet *names;
};

G_DEFINE_TYPE (JagDecImportNameEntry, jag_dec_import_name_entry, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_import_name_entry_class_init(JagDecImportNameEntryClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecImportNameEntryPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_import_name_entry_init(JagDecImportNameEntry *instance) {
	JagDecImportNameEntryPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_IMPORT_NAME_ENTRY, JagDecImportNameEntryPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecImportNameEntry *instance = JAG_DEC_IMPORT_NAME_ENTRY(object);
	JagDecImportNameEntryPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_short_type_name);
	cat_unref_ptr(priv->names);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecImportNameEntry *jag_dec_import_name_entry_new(CatStringWo *a_short_type_name) {
	JagDecImportNameEntry *result = g_object_new(JAG_DEC_TYPE_IMPORT_NAME_ENTRY, NULL);
	cat_ref_anounce(result);
	JagDecImportNameEntryPrivate *priv = result->priv;
	priv->a_short_type_name = cat_ref_ptr(a_short_type_name);
	priv->names = cat_hash_set_new((GHashFunc) jag_byt_name_hash_code, (GEqualFunc) jag_byt_name_equal);
	return result;
}



JagBytName *jag_dec_import_name_entry_get_or_add(JagDecImportNameEntry *name_entry, JagBytName *name) {
	cat_log_debug("name_entry=%p, name=%p", name_entry, name);
	JagDecImportNameEntryPrivate *priv = JAG_DEC_IMPORT_NAME_ENTRY_GET_PRIVATE(name_entry);
	cat_log_debug("name_entry->set=%p", priv->names);
	JagBytName *result = NULL;
	if (!cat_hash_set_size(priv->names)==0) {
		CatIIterator *iter = cat_hash_set_iterator(priv->names);
		while(cat_iiterator_has_next(iter)) {
			JagBytName *test_name = (JagBytName *) cat_iiterator_next(iter);
			if (jag_byt_name_equal(test_name, name)) {
				result = test_name;
				break;
			}
		}
		cat_unref_ptr(iter);
	}
	if (result==NULL) {
		cat_hash_set_add(priv->names, (GObject *) name);
		result = name;
	}
	return result;
}

gboolean jag_dec_import_name_entry_allow_short_version(JagDecImportNameEntry *name_entry) {
	JagDecImportNameEntryPrivate *priv = JAG_DEC_IMPORT_NAME_ENTRY_GET_PRIVATE(name_entry);
	return cat_hash_set_size(priv->names)==1;
}

JagBytName *jag_dec_import_name_entry_get_first_name(JagDecImportNameEntry *name_entry) {
	JagDecImportNameEntryPrivate *priv = JAG_DEC_IMPORT_NAME_ENTRY_GET_PRIVATE(name_entry);
	if (cat_hash_set_size(priv->names)==0) {
		return NULL;
	}
	CatArrayWo *e_flat_list = cat_hash_set_enlist_all(priv->names);
	JagBytName *result = (JagBytName *) cat_array_wo_get(e_flat_list, 0);
	cat_unref_ptr(e_flat_list);
	return result;
}

