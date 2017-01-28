/*
   File:    elkasyncmoonodeupdater.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#ifndef EDITOR_ELKASYNCMOONODEUPDATER_H_
#define EDITOR_ELKASYNCMOONODEUPDATER_H_

#include "elkeditorpanel.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define ELK_TYPE_ASYNC_MOO_NODE_UPDATER              (elk_async_moo_node_updater_get_type())
#define ELK_ASYNC_MOO_NODE_UPDATER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_async_moo_node_updater_get_type(), ElkAsyncMooNodeUpdater))
#define ELK_ASYNC_MOO_NODE_UPDATER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ASYNC_MOO_NODE_UPDATER, ElkAsyncMooNodeUpdaterClass))
#define ELK_IS_ASYNC_MOO_NODE_UPDATER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ASYNC_MOO_NODE_UPDATER))
#define ELK_IS_ASYNC_MOO_NODE_UPDATER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ASYNC_MOO_NODE_UPDATER))
#define ELK_ASYNC_MOO_NODE_UPDATER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ASYNC_MOO_NODE_UPDATER, ElkAsyncMooNodeUpdaterClass))


typedef struct _ElkAsyncMooNodeUpdater               ElkAsyncMooNodeUpdater;
typedef struct _ElkAsyncMooNodeUpdaterPrivate        ElkAsyncMooNodeUpdaterPrivate;
typedef struct _ElkAsyncMooNodeUpdaterClass          ElkAsyncMooNodeUpdaterClass;


struct _ElkAsyncMooNodeUpdater {
	ChaIOAsync parent;
};

struct _ElkAsyncMooNodeUpdaterClass {
	ChaIOAsyncClass parent_class;
};


GType elk_async_moo_node_updater_get_type();

ElkAsyncMooNodeUpdater *elk_async_moo_node_updater_new(ElkEditorPanel *editor_panel, ChaIOAsync *async_response);

G_END_DECLS

#endif /* EDITOR_ELKASYNCMOONODEUPDATER_H_ */
