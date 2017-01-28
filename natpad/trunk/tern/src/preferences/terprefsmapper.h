/*
   File:    terprefsmapper.h
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
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

#ifndef TERPREFSMAPPER_H_
#define TERPREFSMAPPER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

extern CatS ter_s_textpad;


#define TER_TYPE_PREFS_MAPPER              (ter_prefs_mapper_get_type())
#define TER_PREFS_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_prefs_mapper_get_type(), TerPrefsMapper))
#define TER_PREFS_MAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_PREFS_MAPPER, TerPrefsMapperClass))
#define TER_IS_PREFS_MAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_PREFS_MAPPER))
#define TER_IS_PREFS_MAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_PREFS_MAPPER))
#define TER_PREFS_MAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_PREFS_MAPPER, TerPrefsMapperClass))


typedef struct _TerPrefsMapper               TerPrefsMapper;
typedef struct _TerPrefsMapperPrivate        TerPrefsMapperPrivate;
typedef struct _TerPrefsMapperClass          TerPrefsMapperClass;


struct _TerPrefsMapper {
	GObject parent;
};

struct _TerPrefsMapperClass {
	GObjectClass parent_class;
};


GType ter_prefs_mapper_get_type();

TerPrefsMapper *ter_prefs_mapper_new();

G_END_DECLS

#endif /* TERPREFSMAPPER_H_ */
