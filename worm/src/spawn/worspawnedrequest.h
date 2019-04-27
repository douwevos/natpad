/*
   File:    worspawnedrequest.h
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

#ifndef WORSPAWNEDREQUEST_H_
#define WORSPAWNEDREQUEST_H_

#include "../worrequest.h"
#include "../worcondition.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define WOR_TYPE_SPAWNED_REQUEST              (wor_spawned_request_get_type())
#define WOR_SPAWNED_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_spawned_request_get_type(), WorSpawnedRequest))
#define WOR_SPAWNED_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_SPAWNED_REQUEST, WorSpawnedRequestClass))
#define WOR_IS_SPAWNED_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_SPAWNED_REQUEST))
#define WOR_IS_SPAWNED_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_SPAWNED_REQUEST))
#define WOR_SPAWNED_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_SPAWNED_REQUEST, WorSpawnedRequestClass))

typedef struct _WorSpawnedRequest               WorSpawnedRequest;
typedef struct _WorSpawnedRequestPrivate        WorSpawnedRequestPrivate;
typedef struct _WorSpawnedRequestClass          WorSpawnedRequestClass;


struct _WorSpawnedRequest {
	WorRequest parent;
};

struct _WorSpawnedRequestClass {
	WorRequestClass parent_class;
};


GType wor_spawned_request_get_type();

WorSpawnedRequest *wor_spawned_request_new(WorRequest *delegate_request);


WorCondition *wor_spawned_request_get_finished_condition(WorSpawnedRequest *spawned_request);

gboolean wor_spawned_request_get_is_finished(WorSpawnedRequest *spawned_request);

WorRequest *wor_spawned_request_get_delegate(WorSpawnedRequest *spawned_request);

G_END_DECLS

#endif /* WORSPAWNEDREQUEST_H_ */
