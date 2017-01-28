/*
   File:    mooiclipboardlistener.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Mar 10, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef MOOICLIPBOARDLISTENER_H_
#define MOOICLIPBOARDLISTENER_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_ICLIPBOARD_LISTENER                 (moo_iclipboard_listener_get_type())
#define MOO_ICLIPBOARD_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_ICLIPBOARD_LISTENER, MooIClipboardListener))
#define MOO_IS_ICLIPBOARD_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_ICLIPBOARD_LISTENER))
#define MOO_ICLIPBOARD_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_ICLIPBOARD_LISTENER, MooIClipboardListenerInterface))


typedef struct _MooIClipboardListener               MooIClipboardListener; /* dummy object */
typedef struct _MooIClipboardListenerInterface      MooIClipboardListenerInterface;


struct _MooIClipboardListenerInterface {
	GTypeInterface parent_iface;
	void (*stateChanged)(MooIClipboardListener *self, gboolean can_cut, gboolean can_copy, gboolean can_paste);
};

GType moo_iclipboard_listener_get_type(void);

G_END_DECLS


#endif /* MOOICLIPBOARDLISTENER_H_ */
