/*
   File:    groscannerfactory.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 18, 2016
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

#ifndef PARSER_GROSCANNERFACTORY_H_
#define PARSER_GROSCANNERFACTORY_H_

#include "groscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define GRO_TYPE_SCANNER_FACTORY              (gro_scanner_factory_get_type())
#define GRO_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_scanner_factory_get_type(), GroScannerFactory))
#define GRO_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_SCANNER_FACTORY, GroScannerFactoryClass))
#define GRO_IS_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_SCANNER_FACTORY))
#define GRO_IS_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_SCANNER_FACTORY))
#define GRO_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_SCANNER_FACTORY, GroScannerFactoryClass))


typedef struct _GroScannerFactory               GroScannerFactory;
typedef struct _GroScannerFactoryPrivate        GroScannerFactoryPrivate;
typedef struct _GroScannerFactoryClass          GroScannerFactoryClass;


struct _GroScannerFactory {
	GObject parent;
};

struct _GroScannerFactoryClass {
	GObjectClass parent_class;
};


GType gro_scanner_factory_get_type();

GroScannerFactory *gro_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

GroScanner *gro_scanner_factory_create_scanner(GroScannerFactory *factory, CatIUtf8Scanner *scanner);


G_END_DECLS

#endif /* PARSER_GROSCANNERFACTORY_H_ */
