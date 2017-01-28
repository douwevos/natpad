/*
   File:    elkiservice.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 17, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#include "elkiservice.h"

static void elk_iservice_interface_init(gpointer klass) {
//	ElkIServiceInterface *iface = (ElkIServiceInterface*) klass;
}


GType elk_iservice_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (ElkIServiceInterface),
				elk_iservice_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};


		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
				g_intern_static_string ("ElkIService"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}


gboolean elk_iservice_exit(ElkIService *self) {
	g_return_val_if_fail(ELK_IS_ISERVICE(self), FALSE);
	return ELK_ISERVICE_GET_INTERFACE(self)->exit(self);
}


void elk_iservice_show_preferences(ElkIService *self) {
	g_return_if_fail(ELK_IS_ISERVICE(self));
	ELK_ISERVICE_GET_INTERFACE(self)->showPreferences(self);
}



void elk_iservice_select_and_open_resources(ElkIService *self) {
	g_return_if_fail(ELK_IS_ISERVICE(self));
	ELK_ISERVICE_GET_INTERFACE(self)->selectAndOpenResources(self);
}



GtkWidget *elk_iservice_create_empty_editor(ElkIService *self) {
	g_return_val_if_fail(ELK_IS_ISERVICE(self), FALSE);
	return ELK_ISERVICE_GET_INTERFACE(self)->createEmptyEditor(self);
}



gboolean elk_iservice_save_all_resource_editors(ElkIService *self, CatArrayWo *selected_editors) {
	g_return_val_if_fail(ELK_IS_ISERVICE(self), FALSE);
	return ELK_ISERVICE_GET_INTERFACE(self)->saveAllResourceEditors(self, selected_editors);
}


gboolean elk_iservice_close_multiple_resource_editors(ElkIService *self, CatArrayWo *selected_editors) {
	g_return_val_if_fail(ELK_IS_ISERVICE(self), FALSE);
	return ELK_ISERVICE_GET_INTERFACE(self)->closeMultipleResourceEditors(self, selected_editors);
}
