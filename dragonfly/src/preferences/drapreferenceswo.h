/*
   File:    drapreferenceswo.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jan 1, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef PREFERENCES_DRAPREFERENCESWO_H_
#define PREFERENCES_DRAPREFERENCESWO_H_

#include "draprefsspellingwo.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_PREFERENCES_WO              (dra_preferences_wo_get_type())
#define DRA_PREFERENCES_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_preferences_wo_get_type(), DraPreferencesWo))
#define DRA_PREFERENCES_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFERENCES_WO, DraPreferencesWoClass))
#define DRA_IS_PREFERENCES_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFERENCES_WO))
#define DRA_IS_PREFERENCES_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFERENCES_WO))
#define DRA_PREFERENCES_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFERENCES_WO, DraPreferencesWoClass))


typedef struct _DraPreferencesWo               DraPreferencesWo;
typedef struct _DraPreferencesWoPrivate        DraPreferencesWoPrivate;
typedef struct _DraPreferencesWoClass          DraPreferencesWoClass;


struct _DraPreferencesWo {
	ChaPreferencesWo parent;
};

struct _DraPreferencesWoClass {
	ChaPreferencesWoClass parent_class;
};


GType dra_preferences_wo_get_type();

DraPreferencesWo *dra_preferences_wo_new();

DraPrefsSpellingWo *dra_preferences_wo_get_spelling(DraPreferencesWo *prefs);

void dra_preferences_wo_set_spelling(DraPreferencesWo *e_prefs, DraPrefsSpellingWo *spelling);
DraPrefsSpellingWo *dra_preferences_wo_get_editable_spelling(DraPreferencesWo *e_prefs);


CAT_WO_BASE_H(DraPreferencesWo,dra_preferences_wo);

G_END_DECLS

#endif /* PREFERENCES_DRAPREFERENCESWO_H_ */
