
/*
   File:    grostateandsymbol.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
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

#ifndef GLR_GROSTATEANDSYMBOL_H_
#define GLR_GROSTATEANDSYMBOL_H_

#include "../groglibctypes.h"

G_BEGIN_DECLS

#define GRO_TYPE_STATE_AND_SYMBOL              (gro_state_and_symbol_get_type())
#define GRO_STATE_AND_SYMBOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_state_and_symbol_get_type(), GroStateAndSymbol))
#define GRO_STATE_AND_SYMBOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_STATE_AND_SYMBOL, GroStateAndSymbolClass))
#define GRO_IS_STATE_AND_SYMBOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_STATE_AND_SYMBOL))
#define GRO_IS_STATE_AND_SYMBOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_STATE_AND_SYMBOL))
#define GRO_STATE_AND_SYMBOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_STATE_AND_SYMBOL, GroStateAndSymbolClass))


typedef struct _GroStateAndSymbol               GroStateAndSymbol;
typedef struct _GroStateAndSymbolPrivate        GroStateAndSymbolPrivate;
typedef struct _GroStateAndSymbolClass          GroStateAndSymbolClass;


struct _GroStateAndSymbol {
	GObject parent;
};

struct _GroStateAndSymbolClass {
	GObjectClass parent_class;
};


GType gro_state_and_symbol_get_type();

GroStateAndSymbol *gro_state_and_symbol_new(GroStateAndSymbol *linked, int state, int symbol, gboolean is_terminal);

GroStateAndSymbol *gro_state_and_symbol_get_linked(GroStateAndSymbol *state_and_symbol);

int gro_state_and_symbol_get_state(GroStateAndSymbol *state_and_symbol);
int gro_state_and_symbol_get_symbol(GroStateAndSymbol *state_and_symbol);

gboolean gro_state_and_symbol_mergable(GroStateAndSymbol *state_and_symbol_a, GroStateAndSymbol *state_and_symbol_b);

G_END_DECLS

#endif /* GLR_GROSTATEANDSYMBOL_H_ */
