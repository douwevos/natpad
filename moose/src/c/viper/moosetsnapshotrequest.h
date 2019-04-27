/*
   File:    moosetsnapshotrequest.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 11, 2013
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

#ifndef MOOSETSNAPSHOTREQUEST_H_
#define MOOSETSNAPSHOTREQUEST_H_

#include "../mooservice.h"
#include <worm.h>
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_SET_SNAPSHOT_REQUEST              (moo_set_snapshot_request_get_type())
#define MOO_SET_SNAPSHOT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_set_snapshot_request_get_type(), MooSetSnapshotRequest))
#define MOO_SET_SNAPSHOT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_SET_SNAPSHOT_REQUEST, MooSetSnapshotRequestClass))
#define MOO_IS_SET_SNAPSHOT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_SET_SNAPSHOT_REQUEST))
#define MOO_IS_SET_SNAPSHOT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_SET_SNAPSHOT_REQUEST))
#define MOO_SET_SNAPSHOT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_SET_SNAPSHOT_REQUEST, MooSetSnapshotRequestClass))

typedef struct _MooSetSnapshotRequest               MooSetSnapshotRequest;
typedef struct _MooSetSnapshotRequestPrivate        MooSetSnapshotRequestPrivate;
typedef struct _MooSetSnapshotRequestClass          MooSetSnapshotRequestClass;

struct _MooSetSnapshotRequest {
	WorRequest parent;
};

struct _MooSetSnapshotRequestClass {
	WorRequestClass parent_class;
};

GType moo_set_snapshot_request_get_type();

MooSetSnapshotRequest *moo_set_snapshot_request_new(MooService *moo_service, CatAtomicReference *vip_snapshot_ref, VipSnapshot *snapshot);

G_END_DECLS

#endif /* MOOSETSNAPSHOTREQUEST_H_ */
