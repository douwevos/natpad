/*
   File:    terdocumentclazzgrammar.h
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

#ifndef TERDOCUMENTCLAZZGRAMMAR_H_
#define TERDOCUMENTCLAZZGRAMMAR_H_

#include "../preferences/terprefsclazzwo.h"
#include "scanner/tersyntax.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR              (ter_document_clazz_grammar_get_type())
#define TER_DOCUMENT_CLAZZ_GRAMMAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_document_clazz_grammar_get_type(), TerDocumentClazzGrammar))
#define TER_DOCUMENT_CLAZZ_GRAMMAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR, TerDocumentClazzGrammarClass))
#define TER_IS_DOCUMENT_CLAZZ_GRAMMAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR))
#define TER_IS_DOCUMENT_CLAZZ_GRAMMAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR))
#define TER_DOCUMENT_CLAZZ_GRAMMAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_DOCUMENT_CLAZZ_GRAMMAR, TerDocumentClazzGrammarClass))

typedef struct _TerDocumentClazzGrammar               TerDocumentClazzGrammar;
typedef struct _TerDocumentClazzGrammarPrivate        TerDocumentClazzGrammarPrivate;
typedef struct _TerDocumentClazzGrammarClass          TerDocumentClazzGrammarClass;


struct _TerDocumentClazzGrammar {
	GObject parent;
};

struct _TerDocumentClazzGrammarClass {
	GObjectClass parent_class;
};

GType ter_document_clazz_grammar_get_type();

TerDocumentClazzGrammar *ter_document_clazz_grammar_new(TerPrefsClazzWo *a_prefs_clazz, CatLong *viper_id);

CatLong *ter_document_clazz_grammar_get_viper_id(TerDocumentClazzGrammar *document_clazz_grammar);

CatStringWo *ter_document_clazz_grammar_get_clazz_name(TerDocumentClazzGrammar *document_clazz_grammar);

gboolean ter_document_clazz_update_viper_id(TerDocumentClazzGrammar *document_clazz_grammar, CatLong *new_viper_id);

G_END_DECLS

#endif /* TERDOCUMENTCLAZZGRAMMAR_H_ */
