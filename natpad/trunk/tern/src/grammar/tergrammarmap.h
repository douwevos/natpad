/*
   File:    tergrammarmap.h
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


#ifndef TERGRAMMARMAP_H_
#define TERGRAMMARMAP_H_

#include "scanner/tersyntax.h"
#include <caterpillar.h>
#include <elk.h>
#include <viper.h>
#include <worm.h>

G_BEGIN_DECLS

#define TER_TYPE_GRAMMAR_MAP              (ter_grammar_map_get_type())
#define TER_GRAMMAR_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_grammar_map_get_type(), TerGrammarMap))
#define TER_GRAMMAR_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_GRAMMAR_MAP, TerGrammarMapClass))
#define TER_IS_GRAMMAR_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_GRAMMAR_MAP))
#define TER_IS_GRAMMAR_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_GRAMMAR_MAP))
#define TER_GRAMMAR_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_GRAMMAR_MAP, TerGrammarMapClass))
#define TER_GRAMMAR_MAP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TER_TYPE_GRAMMAR_MAP, TerGrammarMapPrivate))

typedef struct _TerGrammarMap               TerGrammarMap;
typedef struct _TerGrammarMapPrivate        TerGrammarMapPrivate;
typedef struct _TerGrammarMapClass          TerGrammarMapClass;


struct _TerGrammarMap {
	GObject parent;
	TerGrammarMapPrivate *priv;
};

struct _TerGrammarMapClass {
	GObjectClass parent_class;
};


GType ter_grammar_map_get_type();

TerGrammarMap *ter_grammar_map_new(ElkPreferencesService *preference_service, VipService *vip_service, WorService *wor_service);

TerSyntax *ter_grammar_map_get_syntax(TerGrammarMap *grammar_map, CatStringWo *clazz_name);

G_END_DECLS


#endif /* TERGRAMMARMAP_H_ */
