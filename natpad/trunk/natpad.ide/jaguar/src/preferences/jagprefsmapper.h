/*
   File:    jagprefsmapper.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 17, 2014
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

#ifndef JAGPREFSMAPPER_H_
#define JAGPREFSMAPPER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PREFS_MAPPER              (jag_prefs_mapper_get_type())
#define JAG_PREFS_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_prefs_mapper_get_type(), JagPrefsMapper))
#define JAG_PREFS_MAPPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFS_MAPPER, JagPrefsMapperClass))
#define JAG_IS_PREFS_MAPPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFS_MAPPER))
#define JAG_IS_PREFS_MAPPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFS_MAPPER))
#define JAG_PREFS_MAPPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFS_MAPPER, JagPrefsMapperClass))


typedef struct _JagPrefsMapper               JagPrefsMapper;
typedef struct _JagPrefsMapperPrivate        JagPrefsMapperPrivate;
typedef struct _JagPrefsMapperClass          JagPrefsMapperClass;


struct _JagPrefsMapper {
	GObject parent;
};

struct _JagPrefsMapperClass {
	GObjectClass parent_class;
};


GType jag_prefs_mapper_get_type();

JagPrefsMapper *jag_prefs_mapper_new();

G_END_DECLS

#endif /* JAGPREFSMAPPER_H_ */
