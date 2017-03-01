/*
   File:    xanscannerfactory.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 11, 2017
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

#ifndef EDITOR_XANSCANNERFACTORY_H_
#define EDITOR_XANSCANNERFACTORY_H_

#include "xanscanner.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define XAN_TYPE_SCANNER_FACTORY              (xan_scanner_factory_get_type())
#define XAN_SCANNER_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_scanner_factory_get_type(), XanScannerFactory))
#define XAN_SCANNER_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_SCANNER_FACTORY, XanScannerFactoryClass))
#define XAN_IS_SCANNER_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_SCANNER_FACTORY))
#define XAN_IS_SCANNER_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_SCANNER_FACTORY))
#define XAN_SCANNER_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_SCANNER_FACTORY, XanScannerFactoryClass))


typedef struct _XanScannerFactory               XanScannerFactory;
typedef struct _XanScannerFactoryPrivate        XanScannerFactoryPrivate;
typedef struct _XanScannerFactoryClass          XanScannerFactoryClass;


struct _XanScannerFactory {
	GObject parent;
};

struct _XanScannerFactoryClass {
	GObjectClass parent_class;
};


GType xan_scanner_factory_get_type();

XanScannerFactory *xan_scanner_factory_new();

XanScanner *xan_scanner_factory_create_scanner(XanScannerFactory *factory, CatIUtf8Scanner *scanner);

G_END_DECLS

#endif /* EDITOR_XANSCANNERFACTORY_H_ */
