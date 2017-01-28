/*
   File:    mulphpscannerfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 11, 2016
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

#ifndef PHP_PARSER_MULPHPSCANNERFACTORY_H_
#define PHP_PARSER_MULPHPSCANNERFACTORY_H_

#include "mulphpscanner.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MUL_PHP_TYPE_SCANNER_FACTORY              (mul_php_scanner_factory_get_type())
#define MUL_PHP_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_php_scanner_factory_get_type(), MulPhpScannerFactory))
#define MUL_PHP_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_PHP_TYPE_SCANNER_FACTORY, MulPhpScannerFactoryClass))
#define MUL_PHP_IS_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_PHP_TYPE_SCANNER_FACTORY))
#define MUL_PHP_IS_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_PHP_TYPE_SCANNER_FACTORY))
#define MUL_PHP_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_PHP_TYPE_SCANNER_FACTORY, MulPhpScannerFactoryClass))


typedef struct _MulPhpScannerFactory               MulPhpScannerFactory;
typedef struct _MulPhpScannerFactoryPrivate        MulPhpScannerFactoryPrivate;
typedef struct _MulPhpScannerFactoryClass          MulPhpScannerFactoryClass;


struct _MulPhpScannerFactory {
	GObject parent;
};

struct _MulPhpScannerFactoryClass {
	GObjectClass parent_class;
};


GType mul_php_scanner_factory_get_type();

MulPhpScannerFactory *mul_php_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

MulPhpScanner *mul_php_scanner_factory_create_scanner(MulPhpScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* PHP_PARSER_MULPHPSCANNERFACTORY_H_ */
