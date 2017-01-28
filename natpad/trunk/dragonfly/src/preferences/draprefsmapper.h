/*
   File:    draprefsmapper.h
   Project: astarte
   Author:  Douwe Vos
   Date:    Oct 19, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef DRAPREFSMAPPER_H_
#define DRAPREFSMAPPER_H_

#include "drapreferenceswo.h"
#include <chameleon.h>
#include <caterpillar.h>
#include <shoveler.h>

G_BEGIN_DECLS

#define DRA_TYPE_PREFS_MAPPER              (dra_prefs_mapper_get_type())
#define DRA_PREFS_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_prefs_mapper_get_type(), DraPrefsMapper))
#define DRA_PREFS_MAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFS_MAPPER, DraPrefsMapperClass))
#define DRA_IS_PREFS_MAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFS_MAPPER))
#define DRA_IS_PREFS_MAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFS_MAPPER))
#define DRA_PREFS_MAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFS_MAPPER, DraPrefsMapperClass))


typedef struct _DraPrefsMapper               DraPrefsMapper;
typedef struct _DraPrefsMapperPrivate        DraPrefsMapperPrivate;
typedef struct _DraPrefsMapperClass          DraPrefsMapperClass;


struct _DraPrefsMapper {
	GObject parent;
};

struct _DraPrefsMapperClass {
	GObjectClass parent_class;
};


GType dra_prefs_mapper_get_type();

DraPrefsMapper *dra_prefs_mapper_new();

void dra_prefs_mapper_model_to_prefs(ShoModel *model, DraPreferencesWo *e_prefs);
void dra_prefs_mapper_prefs_to_model(DraPreferencesWo *prefs, ShoModel *model);

G_END_DECLS

#endif /* DRAPREFSMAPPER_H_ */
