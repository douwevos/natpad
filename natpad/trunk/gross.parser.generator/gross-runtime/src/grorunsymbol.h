/*
   File:    grorunsymbol.h
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

#ifndef GRORUNSYMBOL_H_
#define GRORUNSYMBOL_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_SYMBOL              (grorun_symbol_get_type())
#define GRORUN_SYMBOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_symbol_get_type(), GroRunSymbol))
#define GRORUN_SYMBOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_SYMBOL, GroRunSymbolClass))
#define GRORUN_IS_SYMBOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_SYMBOL))
#define GRORUN_IS_SYMBOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_SYMBOL))
#define GRORUN_SYMBOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_SYMBOL, GroRunSymbolClass))


typedef struct _GroRunSymbol               GroRunSymbol;
typedef struct _GroRunSymbolPrivate        GroRunSymbolPrivate;
typedef struct _GroRunSymbolClass          GroRunSymbolClass;


struct _GroRunSymbol {
	GObject parent;
};

struct _GroRunSymbolClass {
	GObjectClass parent_class;
};


GType grorun_symbol_get_type();

GroRunSymbol *grorun_symbol_new(gboolean is_non_terminal, const CatStringWo *name, int id);

const CatStringWo *grorun_symbol_get_name(GroRunSymbol *symbol);

int grorun_symbol_hash(GroRunSymbol *symbol);

gboolean grorun_symbol_equal(const GroRunSymbol *symbol_a, const GroRunSymbol *symbol_b);


G_END_DECLS

#endif /* GRORUNSYMBOL_H_ */
