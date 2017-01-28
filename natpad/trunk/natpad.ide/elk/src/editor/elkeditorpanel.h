/*
   File:    elkeditorpanel.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 5, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef EDITOR_ELKEDITORPANEL_H_
#define EDITOR_ELKEDITORPANEL_H_

#include "../documentio/elkdocumentbin.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <cow.h>
#include <moose.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define ELK_TYPE_EDITOR_PANEL              (elk_editor_panel_get_type())
#define ELK_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_editor_panel_get_type(), ElkEditorPanel))
#define ELK_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_EDITOR_PANEL, ElkEditorPanelClass))
#define ELK_IS_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_EDITOR_PANEL))
#define ELK_IS_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_EDITOR_PANEL))
#define ELK_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_EDITOR_PANEL, ElkEditorPanelClass))


typedef struct _ElkEditorPanel               ElkEditorPanel;
typedef struct _ElkEditorPanelPrivate        ElkEditorPanelPrivate;
typedef struct _ElkEditorPanelClass          ElkEditorPanelClass;


struct _ElkEditorPanel {
	DraEditorPanel parent;
};

struct _ElkEditorPanelClass {
	DraEditorPanelClass parent_class;
};


GType elk_editor_panel_get_type();

void elk_editor_panel_construct(ElkEditorPanel *editor_panel, LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, DraIConnectorRequestFactory *request_factory);

ElkEditorPanel *elk_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, DraIConnectorRequestFactory *request_factory);

void elk_editor_panel_set_moose_node(ElkEditorPanel *editor_panel, MooNodeWo *moo_node);

MooNodeWo *elk_editor_panel_get_moose_node(const ElkEditorPanel *editor_panel);

ElkDocumentBin *elk_editor_panel_get_document_bin(const ElkEditorPanel *editor_panel);

G_END_DECLS

#endif /* EDITOR_ELKEDITORPANEL_H_ */
