/*
   File:    chaidocumentviewlistener.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jun 21, 2015
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


#include "chaidocumentviewlistener.h"

G_DEFINE_INTERFACE(ChaIDocumentViewListener, cha_idocument_view_listener, G_TYPE_OBJECT);

static void cha_idocument_view_listener_default_init(ChaIDocumentViewListenerInterface *iface) {
	iface->selectionChanged = NULL;
}

void cha_idocument_view_listener_selection_changed(ChaIDocumentViewListener *self, ChaSelection *selection) {
	ChaIDocumentViewListenerInterface *iface = CHA_IDOCUMENT_VIEW_LISTENER_GET_INTERFACE(self);
	if (iface->selectionChanged) {
		iface->selectionChanged(self, selection);
	}
}

void cha_idocument_view_listener_edit_mode_changed(ChaIDocumentViewListener *self, ChaEditMode edit_mode) {
	ChaIDocumentViewListenerInterface *iface = CHA_IDOCUMENT_VIEW_LISTENER_GET_INTERFACE(self);
	if (iface->editModeChanged) {
		iface->editModeChanged(self, edit_mode);
	}
}

