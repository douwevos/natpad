/*
   File:    draiaccontextlistener.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 20, 2015
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

#ifndef AUTOCOMPLETE_DRAIACCONTEXTLISTENER_H_
#define AUTOCOMPLETE_DRAIACCONTEXTLISTENER_H_

#include <caterpillar.h>

#define DRA_TYPE_IAC_CONTEXT_LISTENER                 (dra_iac_context_listener_get_type())
#define DRA_IAC_CONTEXT_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), DRA_TYPE_IAC_CONTEXT_LISTENER, DraIAcContextListener))
#define DRA_IS_IAC_CONTEXT_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), DRA_TYPE_IAC_CONTEXT_LISTENER))
#define DRA_IAC_CONTEXT_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), DRA_TYPE_IAC_CONTEXT_LISTENER, DraIAcContextListenerInterface))


typedef struct _DraIAcContextListener               DraIAcContextListener; /* dummy object */
typedef struct _DraIAcContextListenerInterface      DraIAcContextListenerInterface;


struct _DraIAcContextListenerInterface {
	GTypeInterface parent_iface;
	void (*entryListChanged)(DraIAcContextListener *self);
};

GType dra_iac_context_listener_get_type(void);


#endif /* AUTOCOMPLETE_DRAIACCONTEXTLISTENER_H_ */
