/*
   File:    worspawnrequest.h
   Project: worm
   Author:  Douwe Vos
   Date:    Feb 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef WORSPAWNREQUEST_H_
#define WORSPAWNREQUEST_H_

#include "../worirequestpostbox.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define WOR_TYPE_SPAWN_REQUEST              (wor_spawn_request_get_type())
#define WOR_SPAWN_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_spawn_request_get_type(), WorSpawnRequest))
#define WOR_SPAWN_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_SPAWN_REQUEST, WorSpawnRequestClass))
#define WOR_IS_SPAWN_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_SPAWN_REQUEST))
#define WOR_IS_SPAWN_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_SPAWN_REQUEST))
#define WOR_SPAWN_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_SPAWN_REQUEST, WorSpawnRequestClass))
#define WOR_SPAWN_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), WOR_TYPE_SPAWN_REQUEST, WorSpawnRequestPrivate))

typedef struct _WorSpawnRequest               WorSpawnRequest;
typedef struct _WorSpawnRequestPrivate        WorSpawnRequestPrivate;
typedef struct _WorSpawnRequestClass          WorSpawnRequestClass;


struct _WorSpawnRequest {
	WorRequest parent;
	WorSpawnRequestPrivate *priv;
};

struct _WorSpawnRequestClass {
	WorRequestClass parent_class;

	void (*startSpawn)(WorSpawnRequest *request);

	WorRequest *(*nextSpawn)(WorSpawnRequest *request);
};


GType wor_spawn_request_get_type();

void wor_spawn_request_construct(WorSpawnRequest *spawn_request, WorIRequestPostBox *request_post_box);

G_END_DECLS


#endif /* WORSPAWNREQUEST_H_ */
