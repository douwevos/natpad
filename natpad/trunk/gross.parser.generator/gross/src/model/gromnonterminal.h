
/*
   File:    gromnonterminal.h
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

#ifndef PARSER_MODEL_GROMNONTERMINAL_H_
#define PARSER_MODEL_GROMNONTERMINAL_H_

#include "gromsymbol.h"
#include "groterminalset.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_M_NON_TERMINAL              (gro_m_non_terminal_get_type())
#define GRO_M_NON_TERMINAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_m_non_terminal_get_type(), GroMNonTerminal))
#define GRO_M_NON_TERMINAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_M_NON_TERMINAL, GroMNonTerminalClass))
#define GRO_IS_M_NON_TERMINAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_M_NON_TERMINAL))
#define GRO_IS_M_NON_TERMINAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_M_NON_TERMINAL))
#define GRO_M_NON_TERMINAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_M_NON_TERMINAL, GroMNonTerminalClass))


typedef struct _GroMNonTerminal               GroMNonTerminal;
typedef struct _GroMNonTerminalPrivate        GroMNonTerminalPrivate;
typedef struct _GroMNonTerminalClass          GroMNonTerminalClass;


struct _GroMNonTerminal {
	GroMSymbol parent;
};

struct _GroMNonTerminalClass {
	GroMSymbolClass parent_class;
};

struct _GroMProduction;

GType gro_m_non_terminal_get_type();

GroMNonTerminal *gro_m_non_terminal_new(int index, GroBnfString *name);

void gro_m_non_terminal_add_production(GroMNonTerminal *non_term, struct _GroMProduction *production);
CatIIterator *gro_m_non_terminal_production_iterator(GroMNonTerminal *non_term);


GroTerminalSet *gro_m_non_terminal_get_first_set(GroMNonTerminal *non_term);

gboolean gro_m_non_terminal_compute_nullable(GroMNonTerminal *non_term);
gboolean gro_m_non_terminal_is_nullable(GroMNonTerminal *non_term);


G_END_DECLS

#endif /* PARSER_MODEL_GROMNONTERMINAL_H_ */
