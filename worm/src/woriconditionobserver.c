/*
   File:    woriconditionobserver.c
   Project: worm
   Author:  Douwe Vos
   Date:    Dec 29, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "woriconditionobserver.h"

static void wor_icondition_observer_interface_init(gpointer klass) {
//	WorIConditionObserverInterface *iface = (WorIConditionObserverInterface*) klass;
}


GType wor_icondition_observer_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (WorIConditionObserverInterface),
				wor_icondition_observer_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};

		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("WorIConditionObserver"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}


void wor_icondition_observer_condition_reporting(WorIConditionObserver *self) {
	g_return_if_fail(WOR_IS_ICONDITION_OBSERVER(self));
	return WOR_ICONDITION_OBSERVER_GET_INTERFACE(self)->condition_reporting(self);
}
