/*
   File:    draactionstoprecord.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 23, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef ACTION_MACRO_DRAACTIONSTOPRECORD_H_
#define ACTION_MACRO_DRAACTIONSTOPRECORD_H_

#include "../../macros/dramacromanager.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_STOP_RECORD              (dra_action_stop_record_get_type())
#define DRA_ACTION_STOP_RECORD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_stop_record_get_type(), DraActionStopRecord))
#define DRA_ACTION_STOP_RECORD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_STOP_RECORD, DraActionStopRecordClass))
#define DRA_IS_ACTION_STOP_RECORD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_STOP_RECORD))
#define DRA_IS_ACTION_STOP_RECORD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_STOP_RECORD))
#define DRA_ACTION_STOP_RECORD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_STOP_RECORD, DraActionStopRecordClass))


typedef struct _DraActionStopRecord               DraActionStopRecord;
typedef struct _DraActionStopRecordPrivate        DraActionStopRecordPrivate;
typedef struct _DraActionStopRecordClass          DraActionStopRecordClass;


struct _DraActionStopRecord {
	LeaAction parent;
};

struct _DraActionStopRecordClass {
	LeaActionClass parent_class;
};


GType dra_action_stop_record_get_type();

DraActionStopRecord *dra_action_stop_record_new(DraMacroManager *macro_manager);

void dra_action_stop_record_set_recording(DraActionStopRecord *action_record, gboolean is_recording);

G_END_DECLS

#endif /* ACTION_MACRO_DRAACTIONSTOPRECORD_H_ */
