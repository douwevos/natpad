/*
   File:    mulluascannerfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Nov 9, 2016
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

#ifndef VHDL_MULVHDLSCANNERFACTORY_H_
#define VHDL_MULVHDLSCANNERFACTORY_H_

#include "mulvhdlscanner.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_TYPE_VHDL_SCANNER_FACTORY              (mul_vhdl_scanner_factory_get_type())
#define MUL_VHDL_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_vhdl_scanner_factory_get_type(), MulVhdlScannerFactory))
#define MUL_VHDL_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_VHDL_SCANNER_FACTORY, MulVhdlScannerFactoryClass))
#define MUL_IS_VHDL_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_VHDL_SCANNER_FACTORY))
#define MUL_IS_VHDL_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_VHDL_SCANNER_FACTORY))
#define MUL_VHDL_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_VHDL_SCANNER_FACTORY, MulVhdlScannerFactoryClass))


typedef struct _MulVhdlScannerFactory               MulVhdlScannerFactory;
typedef struct _MulVhdlScannerFactoryPrivate        MulVhdlScannerFactoryPrivate;
typedef struct _MulVhdlScannerFactoryClass          MulVhdlScannerFactoryClass;


struct _MulVhdlScannerFactory {
	GObject parent;
};

struct _MulVhdlScannerFactoryClass {
	GObjectClass parent_class;
};


GType mul_vhdl_scanner_factory_get_type();

MulVhdlScannerFactory *mul_vhdl_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

MulVhdlScanner *mul_vhdl_scanner_factory_create_scanner(MulVhdlScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* VHDL_MULVHDLSCANNERFACTORY_H_ */
