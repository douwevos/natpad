/*
   File:    jagilinklistener.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#ifndef JAGILINKLISTENER_H_
#define JAGILINKLISTENER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_ILINK_LISTENER                 (jag_ilink_listener_get_type())
#define JAG_ILINK_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_TYPE_ILINK_LISTENER, JagILinkListener))
#define JAG_IS_ILINK_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_TYPE_ILINK_LISTENER))
#define JAG_ILINK_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_TYPE_ILINK_LISTENER, JagILinkListenerInterface))


typedef struct _JagILinkListener               JagILinkListener; /* dummy object */
typedef struct _JagILinkListenerInterface      JagILinkListenerInterface;

struct _JagLink;

struct _JagILinkListenerInterface {
	GTypeInterface parent_iface;

	void (*notify)(JagILinkListener *self, struct _JagLink *link, GObject *notify_data);
};

GType jag_ilink_listener_get_type(void);

void jag_ilink_listener_notify(JagILinkListener *self, struct _JagLink *link, GObject *notify_data);

G_END_DECLS

#endif /* JAGILINKLISTENER_H_ */
