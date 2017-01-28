/*
   File:    moosnapshotwork.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 14, 2013
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


#ifndef MOOSNAPSHOTWORK_H_
#define MOOSNAPSHOTWORK_H_

#include "../mooservice.h"
#include "../model/moonodewo.h"
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_SNAPSHOT_WORK              (moo_snapshot_work_get_type())
#define MOO_SNAPSHOT_WORK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_snapshot_work_get_type(), MooSnapshotWork))
#define MOO_SNAPSHOT_WORK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_SNAPSHOT_WORK, MooSnapshotWorkClass))
#define MOO_IS_SNAPSHOT_WORK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_SNAPSHOT_WORK))
#define MOO_IS_SNAPSHOT_WORK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_SNAPSHOT_WORK))
#define MOO_SNAPSHOT_WORK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_SNAPSHOT_WORK, MooSnapshotWorkClass))
#define MOO_SNAPSHOT_WORK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_SNAPSHOT_WORK, MooSnapshotWorkPrivate))


typedef struct _MooSnapshotWork               MooSnapshotWork;
typedef struct _MooSnapshotWorkPrivate        MooSnapshotWorkPrivate;
typedef struct _MooSnapshotWorkClass          MooSnapshotWorkClass;


struct _MooSnapshotWork {
	GObject parent;
	MooSnapshotWorkPrivate *priv;
};

struct _MooSnapshotWorkClass {
	GObjectClass parent_class;
};


GType moo_snapshot_work_get_type();

MooSnapshotWork *moo_snapshot_work_new(VipISequence *moose_sequence, VipSnapshot *snapshot, MooNodeWo *editable_node);

G_END_DECLS

#endif /* MOOSNAPSHOTWORK_H_ */
