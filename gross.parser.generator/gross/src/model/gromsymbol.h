
/*
   File:    gromsymbol.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 25, 2016
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

#ifndef PARSER_MODEL_GROMSYMBOL_H_
#define PARSER_MODEL_GROMSYMBOL_H_

#include "../parser/file/grobnfstring.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_M_SYMBOL              (gro_m_symbol_get_type())
#define GRO_M_SYMBOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_m_symbol_get_type(), GroMSymbol))
#define GRO_M_SYMBOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_M_SYMBOL, GroMSymbolClass))
#define GRO_IS_M_SYMBOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_M_SYMBOL))
#define GRO_IS_M_SYMBOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_M_SYMBOL))
#define GRO_M_SYMBOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_M_SYMBOL, GroMSymbolClass))


typedef struct _GroMSymbol               GroMSymbol;
typedef struct _GroMSymbolPrivate        GroMSymbolPrivate;
typedef struct _GroMSymbolClass          GroMSymbolClass;


struct _GroMSymbol {
	GObject parent;
};

struct _GroMSymbolClass {
	GObjectClass parent_class;
};


GType gro_m_symbol_get_type();

void gro_m_symbol_construct(GroMSymbol *symbol, int index, GroBnfString *name);

int gro_m_symbol_get_index(GroMSymbol *symbol);
CatStringWo *gro_m_symbol_get_name(GroMSymbol *symbol);
GroBnfString *gro_m_symbol_get_string(GroMSymbol *symbol);
void gro_m_symbol_set_stack_type(GroMSymbol *symbol, CatStringWo *stack_type);
CatStringWo *gro_m_symbol_get_stack_type(GroMSymbol *symbol);

gboolean gro_m_symbol_equal(const GroMSymbol *symbol_a, const GroMSymbol *symbol_b);
int gro_m_symbol_hash(GroMSymbol *symbol);

G_END_DECLS

#endif /* PARSER_MODEL_GROMSYMBOL_H_ */
