/*
   File:    chaiuowlistener.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 27, 2015
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

#include "chaiuowlistener.h"

G_DEFINE_INTERFACE(ChaIUowListener, cha_iuow_listener, G_TYPE_OBJECT);

static void cha_iuow_listener_default_init(ChaIUowListenerInterface *iface) {
	iface->uowEvent = NULL;
}

void cha_iuow_listener_uow_event(ChaIUowListener *self, ChaUow *uow, ChaDocumentView *document_view, ChaDocument *document) {
	ChaIUowListenerInterface *iface = CHA_IUOW_LISTENER_GET_INTERFACE(self);
	iface->uowEvent(self, uow, document_view, document);
}
