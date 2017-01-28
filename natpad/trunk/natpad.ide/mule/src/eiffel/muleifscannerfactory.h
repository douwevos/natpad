/*
   File:    muleifscannerfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 14, 2016
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

#ifndef EIF_MULEIFSCANNERFACTORY_H_
#define EIF_MULEIFSCANNERFACTORY_H_

#include <caterpillar.h>
#include <grossruntime.h>
#include "muleifscanner.h"

G_BEGIN_DECLS

#define MUL_TYPE_EIF_SCANNER_FACTORY              (mul_eif_scanner_factory_get_type())
#define MUL_EIF_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_eif_scanner_factory_get_type(), MulEifScannerFactory))
#define MUL_EIF_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_EIF_SCANNER_FACTORY, MulEifScannerFactoryClass))
#define MUL_IS_EIF_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_EIF_SCANNER_FACTORY))
#define MUL_IS_EIF_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_EIF_SCANNER_FACTORY))
#define MUL_EIF_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_EIF_SCANNER_FACTORY, MulEifScannerFactoryClass))


typedef struct _MulEifScannerFactory               MulEifScannerFactory;
typedef struct _MulEifScannerFactoryPrivate        MulEifScannerFactoryPrivate;
typedef struct _MulEifScannerFactoryClass          MulEifScannerFactoryClass;


struct _MulEifScannerFactory {
	GObject parent;
};

struct _MulEifScannerFactoryClass {
	GObjectClass parent_class;
};


GType mul_eif_scanner_factory_get_type();

MulEifScannerFactory *mul_eif_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

MulEifScanner *mul_eif_scanner_factory_create_scanner(MulEifScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* EIF_MULEIFSCANNERFACTORY_H_ */
