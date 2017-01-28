
/*
   File:    gromterminal.h
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

#ifndef PARSER_MODEL_GROMTERMINAL_H_
#define PARSER_MODEL_GROMTERMINAL_H_

#include "gromsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_M_TERMINAL              (gro_m_terminal_get_type())
#define GRO_M_TERMINAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_m_terminal_get_type(), GroMTerminal))
#define GRO_M_TERMINAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_M_TERMINAL, GroMTerminalClass))
#define GRO_IS_M_TERMINAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_M_TERMINAL))
#define GRO_IS_M_TERMINAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_M_TERMINAL))
#define GRO_M_TERMINAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_M_TERMINAL, GroMTerminalClass))


typedef struct _GroMTerminal               GroMTerminal;
typedef struct _GroMTerminalPrivate        GroMTerminalPrivate;
typedef struct _GroMTerminalClass          GroMTerminalClass;

#define GroMTerminalIterator CatIIterator
#define gro_mterminal_iterator_has_next(iter) cat_iiterator_has_next(iter)
#define gro_mterminal_iterator_next(iter) (GroMTerminal *) cat_iiterator_next(iter)

struct _GroMTerminal {
	GroMSymbol parent;
};

struct _GroMTerminalClass {
	GroMSymbolClass parent_class;
};


GType gro_m_terminal_get_type();

GroMTerminal *gro_m_terminal_new(int index, GroBnfString *name);

gboolean gro_m_terminal_equal(const GroMTerminal *term_a, const GroMTerminal *term_b);

int gro_m_terminal_hash(GroMTerminal *term);

int gro_m_terminal_get_precedence_num(GroMTerminal *term);
int gro_m_terminal_get_precedence_side(GroMTerminal *term);

G_END_DECLS

#endif /* PARSER_MODEL_GROMTERMINAL_H_ */
