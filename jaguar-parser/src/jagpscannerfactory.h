/*
   File:    jagpscannerfactory.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Mar 11, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef JAGPSCANNERFACTORY_H_
#define JAGPSCANNERFACTORY_H_

#include "jagpscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define JAGP_TYPE_SCANNER_FACTORY              (jagp_scanner_factory_get_type())
#define JAGP_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_scanner_factory_get_type(), JagPScannerFactory))
#define JAGP_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_SCANNER_FACTORY, JagPScannerFactoryClass))
#define JAGP_IS_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_SCANNER_FACTORY))
#define JAGP_IS_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_SCANNER_FACTORY))
#define JAGP_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_SCANNER_FACTORY, JagPScannerFactoryClass))


typedef struct _JagPScannerFactory               JagPScannerFactory;
typedef struct _JagPScannerFactoryPrivate        JagPScannerFactoryPrivate;
typedef struct _JagPScannerFactoryClass          JagPScannerFactoryClass;


struct _JagPScannerFactory {
	GObject parent;
};

struct _JagPScannerFactoryClass {
	GObjectClass parent_class;
};


GType jagp_scanner_factory_get_type();

JagPScannerFactory *jagp_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

JagPScanner *jagp_scanner_factory_create_scanner(JagPScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* JAGPSCANNERFACTORY_H_ */
