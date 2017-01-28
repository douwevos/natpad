/*
   File:    spieditorconnector.h
   Project: spider
   Author:  Douwe Vos
   Date:    Dec 28, 2016
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

#ifndef EDITOR_SPIEDITORCONNECTOR_H_
#define EDITOR_SPIEDITORCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define SPI_TYPE_EDITOR_CONNECTOR              (spi_editor_connector_get_type())
#define SPI_EDITOR_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_editor_connector_get_type(), SpiEditorConnector))
#define SPI_EDITOR_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_EDITOR_CONNECTOR, SpiEditorConnectorClass))
#define SPI_IS_EDITOR_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_EDITOR_CONNECTOR))
#define SPI_IS_EDITOR_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_EDITOR_CONNECTOR))
#define SPI_EDITOR_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_EDITOR_CONNECTOR, SpiEditorConnectorClass))


typedef struct _SpiEditorConnector               SpiEditorConnector;
typedef struct _SpiEditorConnectorPrivate        SpiEditorConnectorPrivate;
typedef struct _SpiEditorConnectorClass          SpiEditorConnectorClass;


struct _SpiEditorConnector {
	GObject parent;
};

struct _SpiEditorConnectorClass {
	GObjectClass parent_class;
};


GType spi_editor_connector_get_type();

SpiEditorConnector *spi_editor_connector_new();

G_END_DECLS

#endif /* EDITOR_SPIEDITORCONNECTOR_H_ */
