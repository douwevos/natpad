/*
   File:    chaiuowlistener.h
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

#ifndef UOW_CHAIUOWLISTENER_H_
#define UOW_CHAIUOWLISTENER_H_

#include "chauow.h"
#include "../layout/chadocumentview.h"
#include "../document/chadocument.h"
#include <caterpillar.h>

#define CHA_TYPE_IUOW_LISTENER                 (cha_iuow_listener_get_type())
#define CHA_IUOW_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CHA_TYPE_IUOW_LISTENER, ChaIUowListener))
#define CHA_IS_IUOW_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CHA_TYPE_IUOW_LISTENER))
#define CHA_IUOW_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CHA_TYPE_IUOW_LISTENER, ChaIUowListenerInterface))


typedef struct _ChaIUowListener               ChaIUowListener; /* dummy object */
typedef struct _ChaIUowListenerInterface      ChaIUowListenerInterface;


struct _ChaIUowListenerInterface {
	GTypeInterface parent_iface;
	void (*uowEvent)(ChaIUowListener *self, ChaUow *uow, ChaDocumentView *document_view, ChaDocument *document);
};

GType cha_iuow_listener_get_type(void);

void cha_iuow_listener_uow_event(ChaIUowListener *self, ChaUow *uow, ChaDocumentView *document_view, ChaDocument *document);

#endif /* UOW_CHAIUOWLISTENER_H_ */
