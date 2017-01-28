/*
   File:    terprefsclazzdescription.h
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
#ifndef TERPREFSCLAZZDESCRIPTION_H_
#define TERPREFSCLAZZDESCRIPTION_H_

#include "../terprefsclazzwo.h"
#include <caterpillar.h>
#include <cow.h>

G_BEGIN_DECLS

#define TER_TYPE_PREFS_CLAZZ_DESCRIPTION              (ter_prefs_clazz_description_get_type())
#define TER_PREFS_CLAZZ_DESCRIPTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_prefs_clazz_description_get_type(), TerPrefsClazzDescription))
#define TER_PREFS_CLAZZ_DESCRIPTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_PREFS_CLAZZ_DESCRIPTION, TerPrefsClazzDescriptionClass))
#define TER_IS_PREFS_CLAZZ_DESCRIPTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_PREFS_CLAZZ_DESCRIPTION))
#define TER_IS_PREFS_CLAZZ_DESCRIPTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_PREFS_CLAZZ_DESCRIPTION))
#define TER_PREFS_CLAZZ_DESCRIPTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_PREFS_CLAZZ_DESCRIPTION, TerPrefsClazzDescriptionClass))


typedef struct _TerPrefsClazzDescription               TerPrefsClazzDescription;
typedef struct _TerPrefsClazzDescriptionPrivate        TerPrefsClazzDescriptionPrivate;
typedef struct _TerPrefsClazzDescriptionClass          TerPrefsClazzDescriptionClass;


struct _TerPrefsClazzDescription {
	CowPanelDescription parent;
};

struct _TerPrefsClazzDescriptionClass {
	CowPanelDescriptionClass parent_class;
};


GType ter_prefs_clazz_description_get_type();

TerPrefsClazzDescription *ter_prefs_clazz_description_new(TerPrefsClazzWo *prefs_clazz);

void ter_prefs_clazz_description_set(TerPrefsClazzDescription *description, TerPrefsClazzWo *prefs_clazz);
TerPrefsClazzWo *ter_prefs_clazz_description_get(TerPrefsClazzDescription *description);


G_END_DECLS

#endif /* TERPREFSCLAZZDESCRIPTION_H_ */
