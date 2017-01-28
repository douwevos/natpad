/*
   File:    mulrustscannerfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#ifndef RUST_MULRUSTSCANNERFACTORY_H_
#define RUST_MULRUSTSCANNERFACTORY_H_

#include "mulrustscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_TYPE_RUST_SCANNER_FACTORY              (mul_rust_scanner_factory_get_type())
#define MUL_RUST_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_rust_scanner_factory_get_type(), MulRustScannerFactory))
#define MUL_RUST_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_RUST_SCANNER_FACTORY, MulRustScannerFactoryClass))
#define MUL_IS_RUST_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_RUST_SCANNER_FACTORY))
#define MUL_IS_RUST_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_RUST_SCANNER_FACTORY))
#define MUL_RUST_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_RUST_SCANNER_FACTORY, MulRustScannerFactoryClass))


typedef struct _MulRustScannerFactory               MulRustScannerFactory;
typedef struct _MulRustScannerFactoryPrivate        MulRustScannerFactoryPrivate;
typedef struct _MulRustScannerFactoryClass          MulRustScannerFactoryClass;


struct _MulRustScannerFactory {
	GObject parent;
};

struct _MulRustScannerFactoryClass {
	GObjectClass parent_class;
};


GType mul_rust_scanner_factory_get_type();

MulRustScannerFactory *mul_rust_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

MulRustScanner *mul_rust_scanner_factory_create_scanner(MulRustScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* RUST_MULRUSTSCANNERFACTORY_H_ */
