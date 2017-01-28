/*
   File:    leaipanellistener.c
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 26, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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


#include "leaipanellistener.h"

static void lea_ipanel_listener_interface_init(gpointer klass) {
	LeaIPanelListenerInterface *iface = (LeaIPanelListenerInterface*) klass;
	iface->panel_label_set = NULL;
}


GType lea_ipanel_listener_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (LeaIPanelListenerInterface),
				lea_ipanel_listener_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("LeaIPanelListener"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}

void lea_ipanel_label_set(LeaIPanelListener *self, gpointer panel, const CatStringWo *a_new_label) {
	g_return_if_fail(LEA_IS_IPANEL_LISTENER(self));
	LEA_IPANEL_LISTENER_GET_INTERFACE(self)->panel_label_set(self, panel, a_new_label);
}
