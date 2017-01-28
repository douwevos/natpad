/*
   File:    spiscannerfactory.h
   Project: spider
   Author:  Douwe Vos
   Date:    Nov 7, 2016
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

#ifndef EDITOR_SPISCANNERFACTORY_H_
#define EDITOR_SPISCANNERFACTORY_H_

#include "spiscannernew.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SPI_TYPE_SCANNER_FACTORY              (spi_scanner_factory_get_type())
#define SPI_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_scanner_factory_get_type(), SpiScannerFactory))
#define SPI_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_SCANNER_FACTORY, SpiScannerFactoryClass))
#define SPI_IS_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_SCANNER_FACTORY))
#define SPI_IS_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_SCANNER_FACTORY))
#define SPI_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_SCANNER_FACTORY, SpiScannerFactoryClass))


typedef struct _SpiScannerFactory               SpiScannerFactory;
typedef struct _SpiScannerFactoryPrivate        SpiScannerFactoryPrivate;
typedef struct _SpiScannerFactoryClass          SpiScannerFactoryClass;


struct _SpiScannerFactory {
	GObject parent;
};

struct _SpiScannerFactoryClass {
	GObjectClass parent_class;
};


GType spi_scanner_factory_get_type();

SpiScannerFactory *spi_scanner_factory_new(GroRunISymbolProvider *symbol_provider, GroRunITokenFactory *token_factory);

SpiScannerNew *spi_scanner_factory_create_scanner(SpiScannerFactory *factory, CatIUtf8Scanner *scanner);


G_END_DECLS

#endif /* EDITOR_SPISCANNERFACTORY_H_ */
