/*
   File:    terpreferenceswo.h
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

#ifndef TERPREFERENCESWO_H_
#define TERPREFERENCESWO_H_

#include "terprefsclazzwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define TER_TYPE_PREFERENCES_WO              (ter_preferences_wo_get_type())
#define TER_PREFERENCES_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_preferences_wo_get_type(), TerPreferencesWo))
#define TER_PREFERENCES_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_PREFERENCES_WO, TerPreferencesWoClass))
#define TER_IS_PREFERENCES_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_PREFERENCES_WO))
#define TER_IS_PREFERENCES_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_PREFERENCES_WO))
#define TER_PREFERENCES_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_PREFERENCES_WO, TerPreferencesWoClass))


typedef struct _TerPreferencesWo               TerPreferencesWo;
typedef struct _TerPreferencesWoPrivate        TerPreferencesWoPrivate;
typedef struct _TerPreferencesWoClass          TerPreferencesWoClass;


struct _TerPreferencesWo {
	CatWo parent;
};

struct _TerPreferencesWoClass {
	CatWoClass parent_class;
};


GType ter_preferences_wo_get_type();

TerPreferencesWo *ter_preferences_wo_new();

TerPrefsClazzWo *ter_preferences_wo_get_clazz(const TerPreferencesWo *preferences, const CatStringWo *clazz_name);
CatHashMapWo *ter_preferences_wo_get_document_clazzes(const TerPreferencesWo *preferences);

void ter_preferences_wo_set_clazz(TerPreferencesWo *e_preferences, TerPrefsClazzWo *clazz);

CAT_WO_BASE_H(TerPreferencesWo, ter_preferences_wo);

G_END_DECLS

#endif /* TERPREFERENCESWO_H_ */
