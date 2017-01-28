/*
   File:    grorunisymbolprovider.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#ifndef GRORUNISYMBOLPROVIDER_H_
#define GRORUNISYMBOLPROVIDER_H_

#include "grorunsymbol.h"
#include <caterpillar.h>

#define GRORUN_TYPE_ISYMBOL_PROVIDER                 (grorun_isymbol_provider_get_type())
#define GRORUN_ISYMBOL_PROVIDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRORUN_TYPE_ISYMBOL_PROVIDER, GroRunISymbolProvider))
#define GRORUN_IS_ISYMBOL_PROVIDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRORUN_TYPE_ISYMBOL_PROVIDER))
#define GRORUN_ISYMBOL_PROVIDER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRORUN_TYPE_ISYMBOL_PROVIDER, GroRunISymbolProviderInterface))


typedef struct _GroRunISymbolProvider               GroRunISymbolProvider; /* dummy object */
typedef struct _GroRunISymbolProviderInterface      GroRunISymbolProviderInterface;


struct _GroRunISymbolProviderInterface {
	GTypeInterface parent_iface;
	GroRunSymbol *(*getSymbolByName)(GroRunISymbolProvider *self, const CatStringWo *name);
};

GType grorun_isymbol_provider_get_type(void);

GroRunSymbol *grorun_isymbol_provider_get_symbol_by_name(GroRunISymbolProvider *self, const CatStringWo *name);

#endif /* GRORUNISYMBOLPROVIDER_H_ */
