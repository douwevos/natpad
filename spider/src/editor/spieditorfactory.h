/*
   File:    spieditorfactory.h
   Project: spider
   Author:  Douwe Vos
   Date:    Jul 18, 2016
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

#ifndef EDITOR_SPIEDITORFACTORY_H_
#define EDITOR_SPIEDITORFACTORY_H_

#include "spieditorconnector.h"
#include <caterpillar.h>
#include <elk.h>
#include <leafhopper.h>
#include <dragonfly.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define SPI_TYPE_EDITOR_FACTORY              (spi_editor_factory_get_type())
#define SPI_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_editor_factory_get_type(), SpiEditorFactory))
#define SPI_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_EDITOR_FACTORY, SpiEditorFactoryClass))
#define SPI_IS_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_EDITOR_FACTORY))
#define SPI_IS_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_EDITOR_FACTORY))
#define SPI_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_EDITOR_FACTORY, SpiEditorFactoryClass))


typedef struct _SpiEditorFactory               SpiEditorFactory;
typedef struct _SpiEditorFactoryPrivate        SpiEditorFactoryPrivate;
typedef struct _SpiEditorFactoryClass          SpiEditorFactoryClass;


struct _SpiEditorFactory {
	GObject parent;
};

struct _SpiEditorFactoryClass {
	GObjectClass parent_class;
};


GType spi_editor_factory_get_type();

SpiEditorFactory *spi_editor_factory_new(LeaIPanelOwner *panel_owner, SpiEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* EDITOR_SPIEDITORFACTORY_H_ */
