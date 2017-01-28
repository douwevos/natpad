/*
   File:    elkgroupmain.h
   Project: elk
   Author:  Douwe Vos
   Date:    Dec 23, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef ELKGROUPMAIN_H_
#define ELKGROUPMAIN_H_

#include "../elkiservice.h"
#include <dragonfly.h>
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_GROUP_MAIN              (elk_group_main_get_type())
#define ELK_GROUP_MAIN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_group_main_get_type(), ElkGroupMain))
#define ELK_GROUP_MAIN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_GROUP_MAIN, ElkGroupMainClass))
#define ELK_IS_GROUP_MAIN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_GROUP_MAIN))
#define ELK_IS_GROUP_MAIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_GROUP_MAIN))
#define ELK_GROUP_MAIN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_GROUP_MAIN, ElkGroupMainClass))
#define ELK_GROUP_MAIN_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ELK_TYPE_GROUP_MAIN, ElkGroupMainPrivate))

typedef struct _ElkGroupMain               ElkGroupMain;
typedef struct _ElkGroupMainPrivate        ElkGroupMainPrivate;
typedef struct _ElkGroupMainClass          ElkGroupMainClass;

struct _ElkGroupMain {
	LeaActionGroup parent;
	ElkGroupMainPrivate *priv;
};

struct _ElkGroupMainClass {
	LeaActionGroupClass parent_class;
};


GType elk_group_main_get_type();

ElkGroupMain *elk_group_main_new(ElkIService *service, VipService *vip_service, LeaFrame *frame);


void elk_group_main_set_editor_panel(ElkGroupMain *group, DraEditorPanel *editor_panel);

void elk_group_main_set_editor_list(ElkGroupMain *group, CatArrayWo *a_editor_list);

G_END_DECLS


#endif /* ELKGROUPMAIN_H_ */
