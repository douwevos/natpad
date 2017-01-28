
/*
   File:    spieditorpanel.h
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

#ifndef EDITOR_SPIEDITORPANEL_H_
#define EDITOR_SPIEDITORPANEL_H_

#include "spieditorconnector.h"
#include <elk.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define SPI_TYPE_EDITOR_PANEL              (spi_editor_panel_get_type())
#define SPI_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), spi_editor_panel_get_type(), SpiEditorPanel))
#define SPI_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SPI_TYPE_EDITOR_PANEL, SpiEditorPanelClass))
#define SPI_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SPI_TYPE_EDITOR_PANEL))
#define SPI_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SPI_TYPE_EDITOR_PANEL))
#define SPI_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SPI_TYPE_EDITOR_PANEL, SpiEditorPanelClass))


typedef struct _SpiEditorPanel               SpiEditorPanel;
typedef struct _SpiEditorPanelPrivate        SpiEditorPanelPrivate;
typedef struct _SpiEditorPanelClass          SpiEditorPanelClass;


struct _SpiEditorPanel {
	ElkEditorPanel parent;
};

struct _SpiEditorPanelClass {
	ElkEditorPanelClass parent_class;
};


GType spi_editor_panel_get_type();

SpiEditorPanel *spi_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, SpiEditorConnector *connector);

G_END_DECLS

#endif /* EDITOR_SPIEDITORPANEL_H_ */
