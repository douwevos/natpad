/*
   File:    draactionstartrecord.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 21, 2016
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

#ifndef ACTION_MACRO_DRAACTIONSTARTRECORD_H_
#define ACTION_MACRO_DRAACTIONSTARTRECORD_H_

#include "../../macros/dramacromanager.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_START_RECORD              (dra_action_start_record_get_type())
#define DRA_ACTION_START_RECORD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_start_record_get_type(), DraActionStartRecord))
#define DRA_ACTION_START_RECORD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_START_RECORD, DraActionStartRecordClass))
#define DRA_IS_ACTION_START_RECORD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_START_RECORD))
#define DRA_IS_ACTION_START_RECORD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_START_RECORD))
#define DRA_ACTION_START_RECORD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_START_RECORD, DraActionStartRecordClass))


typedef struct _DraActionStartRecord               DraActionStartRecord;
typedef struct _DraActionStartRecordPrivate        DraActionStartRecordPrivate;
typedef struct _DraActionStartRecordClass          DraActionStartRecordClass;


struct _DraActionStartRecord {
	LeaAction parent;
};

struct _DraActionStartRecordClass {
	LeaActionClass parent_class;
};


GType dra_action_start_record_get_type();

DraActionStartRecord *dra_action_start_record_new(DraMacroManager *macro_manager);

G_END_DECLS

#endif /* ACTION_MACRO_DRAACTIONSTARTRECORD_H_ */
