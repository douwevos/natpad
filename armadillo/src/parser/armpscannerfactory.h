/*
   File:    armpscannerfactory.h
   Project: armadillo
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

#ifndef ARMPSCANNERFACTORY_H_
#define ARMPSCANNERFACTORY_H_

#include "armpscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define ARMP_TYPE_SCANNER_FACTORY              (armp_scanner_factory_get_type())
#define ARMP_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), armp_scanner_factory_get_type(), ArmPScannerFactory))
#define ARMP_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARMP_TYPE_SCANNER_FACTORY, ArmPScannerFactoryClass))
#define ARMP_IS_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARMP_TYPE_SCANNER_FACTORY))
#define ARMP_IS_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARMP_TYPE_SCANNER_FACTORY))
#define ARMP_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARMP_TYPE_SCANNER_FACTORY, ArmPScannerFactoryClass))


typedef struct _ArmPScannerFactory               ArmPScannerFactory;
typedef struct _ArmPScannerFactoryPrivate        ArmPScannerFactoryPrivate;
typedef struct _ArmPScannerFactoryClass          ArmPScannerFactoryClass;


struct _ArmPScannerFactory {
	GObject parent;
};

struct _ArmPScannerFactoryClass {
	GObjectClass parent_class;
};


GType armp_scanner_factory_get_type();

ArmPScannerFactory *armp_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

ArmPScanner *armp_scanner_factory_create_scanner(ArmPScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* ARMPSCANNERFACTORY_H_ */
