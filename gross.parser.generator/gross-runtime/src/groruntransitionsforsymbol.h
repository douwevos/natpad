/*
   File:    groruntransitionsforsymbol.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 10, 2016
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

#ifndef GRORUNTRANSITIONSFORSYMBOL_H_
#define GRORUNTRANSITIONSFORSYMBOL_H_

#include "grorunsymbol.h"
#include "groruntransition.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_TRANSITIONS_FOR_SYMBOL              (grorun_transitions_for_symbol_get_type())
#define GRORUN_TRANSITIONS_FOR_SYMBOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_transitions_for_symbol_get_type(), GroRunTransitionsForSymbol))
#define GRORUN_TRANSITIONS_FOR_SYMBOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_TRANSITIONS_FOR_SYMBOL, GroRunTransitionsForSymbolClass))
#define GRORUN_IS_TRANSITIONS_FOR_SYMBOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_TRANSITIONS_FOR_SYMBOL))
#define GRORUN_IS_TRANSITIONS_FOR_SYMBOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_TRANSITIONS_FOR_SYMBOL))
#define GRORUN_TRANSITIONS_FOR_SYMBOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_TRANSITIONS_FOR_SYMBOL, GroRunTransitionsForSymbolClass))


typedef struct _GroRunTransitionsForSymbol               GroRunTransitionsForSymbol;
typedef struct _GroRunTransitionsForSymbolPrivate        GroRunTransitionsForSymbolPrivate;
typedef struct _GroRunTransitionsForSymbolClass          GroRunTransitionsForSymbolClass;


struct _GroRunTransitionsForSymbol {
	GObject parent;
};

struct _GroRunTransitionsForSymbolClass {
	GObjectClass parent_class;
};


GType grorun_transitions_for_symbol_get_type();

GroRunTransitionsForSymbol *grorun_transitions_for_symbol_new(GroRunSymbol *symbol);

void grorun_transitions_for_symbol_add(GroRunTransitionsForSymbol *trans_for_sym, GroRunTransition *transition);

int grorun_transitions_for_symbol_count(GroRunTransitionsForSymbol *trans_for_sym);
GroRunTransition *grorun_transitions_for_symbol_get(GroRunTransitionsForSymbol *trans_for_sym, int index);



G_END_DECLS

#endif /* GRORUNTRANSITIONSFORSYMBOL_H_ */
