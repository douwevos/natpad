/*
   File:    elkiservice.h
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


#ifndef ELKISERVICE_H_
#define ELKISERVICE_H_

#include <caterpillar.h>
#include <viper.h>
#include <moose.h>

#define ELK_TYPE_ISERVICE                 (elk_iservice_get_type())
#define ELK_ISERVICE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), ELK_TYPE_ISERVICE, ElkIService))
#define ELK_IS_ISERVICE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), ELK_TYPE_ISERVICE))
#define ELK_ISERVICE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), ELK_TYPE_ISERVICE, ElkIServiceInterface))


typedef struct _ElkIService               ElkIService; /* dummy object */
typedef struct _ElkIServiceInterface      ElkIServiceInterface;

struct _ElkIServiceInterface {
	GTypeInterface parent_iface;

	gboolean (*exit)(ElkIService *self);
	void (*showPreferences)(ElkIService *self);
	void (*selectAndOpenResources)(ElkIService *self);

	/*
	 * resource_list    CatArray<CatReadableTreeNode<VipNode *>> *
	 */
	void (*openResourceList)(ElkIService *self, CatArrayWo *resource_list);
	void (*openNewFileEditor)(ElkIService *self, MooNodeWo *moo_node);
	GtkWidget *(*createEmptyEditor)(ElkIService *self);


	gboolean (*closeMultipleResourceEditors)(ElkIService *self, CatArrayWo *selected_editors); /* selected_editors is optional */

	gboolean (*saveAllResourceEditors)(ElkIService *self, CatArrayWo *selected_editors); /* selected_editors is optional */
};

GType elk_iservice_get_type(void);

gboolean elk_iservice_exit(ElkIService *self);

void elk_iservice_show_preferences(ElkIService *self);

void elk_iservice_select_and_open_resources(ElkIService *self);


GtkWidget *elk_iservice_create_empty_editor(ElkIService *self);


gboolean elk_iservice_save_all_resource_editors(ElkIService *self, CatArrayWo *selected_editors);

gboolean elk_iservice_close_multiple_resource_editors(ElkIService *self, CatArrayWo *selected_editors);


#endif /* ELKISERVICE_H_ */
