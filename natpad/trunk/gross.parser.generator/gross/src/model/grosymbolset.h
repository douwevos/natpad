
/*
   File:    grosymbolset.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 2, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef PARSER_MODEL_GROSYMBOLSET_H_
#define PARSER_MODEL_GROSYMBOLSET_H_

#include "gromsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_SYMBOL_SET              (gro_symbol_set_get_type())
#define GRO_SYMBOL_SET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_symbol_set_get_type(), GroSymbolSet))
#define GRO_SYMBOL_SET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_SYMBOL_SET, GroSymbolSetClass))
#define GRO_IS_SYMBOL_SET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_SYMBOL_SET))
#define GRO_IS_SYMBOL_SET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_SYMBOL_SET))
#define GRO_SYMBOL_SET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_SYMBOL_SET, GroSymbolSetClass))


typedef struct _GroSymbolSet               GroSymbolSet;
typedef struct _GroSymbolSetPrivate        GroSymbolSetPrivate;
typedef struct _GroSymbolSetClass          GroSymbolSetClass;


struct _GroSymbolSet {
	GObject parent;
};

struct _GroSymbolSetClass {
	GObjectClass parent_class;
};


GType gro_symbol_set_get_type();

GroSymbolSet *gro_symbol_set_new();

gboolean gro_symbol_set_add(GroSymbolSet *symbol_set, GroMSymbol *symbol);

CatIIterator *gro_symbol_set_value_iterator(GroSymbolSet *symbol_set);

G_END_DECLS

#endif /* PARSER_MODEL_GROSYMBOLSET_H_ */
