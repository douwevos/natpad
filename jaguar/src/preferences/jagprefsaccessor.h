/*
   File:    jagprefsaccessor.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 12, 2015
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

#ifndef SRC_PREFERENCES_JAGPREFSACCESSOR_H_
#define SRC_PREFERENCES_JAGPREFSACCESSOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PREFS_ACCESSOR              (jag_prefs_accessor_get_type())
#define JAG_PREFS_ACCESSOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_prefs_accessor_get_type(), JagPrefsAccessor))
#define JAG_PREFS_ACCESSOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFS_ACCESSOR, JagPrefsAccessorClass))
#define JAG_IS_PREFS_ACCESSOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFS_ACCESSOR))
#define JAG_IS_PREFS_ACCESSOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFS_ACCESSOR))
#define JAG_PREFS_ACCESSOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFS_ACCESSOR, JagPrefsAccessorClass))


typedef struct _JagPrefsAccessor               JagPrefsAccessor;
typedef struct _JagPrefsAccessorClass          JagPrefsAccessorClass;


struct _JagPrefsAccessor {
	GObject parent;
};

struct _JagPrefsAccessorClass {
	GObjectClass parent_class;
};


GType jag_prefs_accessor_get_type();

JagPrefsAccessor *jag_prefs_accessor_new();

G_END_DECLS

#endif /* SRC_PREFERENCES_JAGPREFSACCESSOR_H_ */
