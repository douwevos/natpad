/*
   File:    mulpytscannerfactory.h
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

#ifndef PYT_MULPYTSCANNERFACTORY_H_
#define PYT_MULPYTSCANNERFACTORY_H_

#include "mulpytscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_TYPE_PYT_SCANNER_FACTORY              (mul_pyt_scanner_factory_get_type())
#define MUL_PYT_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_pyt_scanner_factory_get_type(), MulPytScannerFactory))
#define MUL_PYT_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_PYT_SCANNER_FACTORY, MulPytScannerFactoryClass))
#define MUL_IS_PYT_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_PYT_SCANNER_FACTORY))
#define MUL_IS_PYT_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_PYT_SCANNER_FACTORY))
#define MUL_PYT_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_PYT_SCANNER_FACTORY, MulPytScannerFactoryClass))


typedef struct _MulPytScannerFactory               MulPytScannerFactory;
typedef struct _MulPytScannerFactoryPrivate        MulPytScannerFactoryPrivate;
typedef struct _MulPytScannerFactoryClass          MulPytScannerFactoryClass;


struct _MulPytScannerFactory {
	GObject parent;
};

struct _MulPytScannerFactoryClass {
	GObjectClass parent_class;
};


GType mul_pyt_scanner_factory_get_type();

MulPytScannerFactory *mul_pyt_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

MulPytScanner *mul_pyt_scanner_factory_create_scanner(MulPytScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* PYT_MULPYTSCANNERFACTORY_H_ */
