/*
   File:    chaidocumentviewlistener.h
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

#ifndef LAYOUT_CHAIDOCUMENTVIEWLISTENER_H_
#define LAYOUT_CHAIDOCUMENTVIEWLISTENER_H_

#include "chaeditmode.h"
#include "../selection/chaselection.h"
#include <caterpillar.h>

#define CHA_TYPE_IDOCUMENT_VIEW_LISTENER                 (cha_idocument_view_listener_get_type())
#define CHA_IDOCUMENT_VIEW_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CHA_TYPE_IDOCUMENT_VIEW_LISTENER, ChaIDocumentViewListener))
#define CHA_IS_IDOCUMENT_VIEW_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CHA_TYPE_IDOCUMENT_VIEW_LISTENER))
#define CHA_IDOCUMENT_VIEW_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CHA_TYPE_IDOCUMENT_VIEW_LISTENER, ChaIDocumentViewListenerInterface))


typedef struct _ChaIDocumentViewListener               ChaIDocumentViewListener; /* dummy object */
typedef struct _ChaIDocumentViewListenerInterface      ChaIDocumentViewListenerInterface;


struct _ChaIDocumentViewListenerInterface {
	GTypeInterface parent_iface;
	void (*selectionChanged)(ChaIDocumentViewListener *self, ChaSelection *selection);
	void (*editModeChanged)(ChaIDocumentViewListener *self, ChaEditMode edit_mode);
};

GType cha_idocument_view_listener_get_type(void);

void cha_idocument_view_listener_selection_changed(ChaIDocumentViewListener *self, ChaSelection *selection);

void cha_idocument_view_listener_edit_mode_changed(ChaIDocumentViewListener *self, ChaEditMode edit_mode);

#endif /* LAYOUT_CHAIDOCUMENTVIEWLISTENER_H_ */
