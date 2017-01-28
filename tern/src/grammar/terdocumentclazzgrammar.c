/*
   File:    terdocumentclazzgrammar.c
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 16, 2012
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

#include "terdocumentclazzgrammar.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerDocumentClazzGrammar"
#include <logging/catlog.h>

struct _TerDocumentClazzGrammarPrivate {
	TerPrefsClazzWo *a_prefs_clazz;
	CatLong *viper_id;
};

G_DEFINE_TYPE (TerDocumentClazzGrammar, ter_document_clazz_grammar, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_document_clazz_grammar_class_init(TerDocumentClazzGrammarClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(TerDocumentClazzGrammarPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_document_clazz_grammar_init(TerDocumentClazzGrammar *instance) {
	TerDocumentClazzGrammarPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR, TerDocumentClazzGrammarPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerDocumentClazzGrammar *instance = TER_DOCUMENT_CLAZZ_GRAMMAR(object);
	TerDocumentClazzGrammarPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_prefs_clazz);
	cat_unref_ptr(priv->viper_id);

	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerDocumentClazzGrammar *ter_document_clazz_grammar_new(TerPrefsClazzWo *a_prefs_clazz, CatLong *viper_id) {
	TerDocumentClazzGrammar *result = g_object_new(TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR, NULL);
	cat_ref_anounce(result);
	TerDocumentClazzGrammarPrivate *priv = result->priv;

	priv->a_prefs_clazz = cat_ref_ptr(a_prefs_clazz);
	priv->viper_id = cat_ref_ptr(viper_id);
	return result;
}




CatLong *ter_document_clazz_grammar_get_viper_id(TerDocumentClazzGrammar *document_clazz_grammar) {
	TerDocumentClazzGrammarPrivate *priv = TER_DOCUMENT_CLAZZ_GRAMMAR_GET_PRIVATE(document_clazz_grammar);
	return priv->viper_id;
}


CatStringWo *ter_document_clazz_grammar_get_clazz_name(TerDocumentClazzGrammar *document_clazz_grammar) {
	TerDocumentClazzGrammarPrivate *priv = TER_DOCUMENT_CLAZZ_GRAMMAR_GET_PRIVATE(document_clazz_grammar);
	return ter_prefs_clazz_wo_get_name(priv->a_prefs_clazz);
}

gboolean ter_document_clazz_update_viper_id(TerDocumentClazzGrammar *document_clazz_grammar, CatLong *new_viper_id) {
	TerDocumentClazzGrammarPrivate *priv = TER_DOCUMENT_CLAZZ_GRAMMAR_GET_PRIVATE(document_clazz_grammar);
	gboolean result = FALSE;
	if (!cat_long_equal(priv->viper_id, new_viper_id)) {
		result = TRUE;
		cat_ref_swap(priv->viper_id, new_viper_id);
	}
	return result;
}
