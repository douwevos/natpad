/*
   File:    jagpreferenceswo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#ifndef JAGPREFERENCESWO_H_
#define JAGPREFERENCESWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

extern CatS jag_s_jaguar_config;
extern CatS jag_s_java;
extern CatS jag_s_jdk_or_jre;
extern CatS jag_s_jar_path;

#define JAG_TYPE_PREFERENCES_WO              (jag_preferences_wo_get_type())
#define JAG_PREFERENCES_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_preferences_wo_get_type(), JagPreferencesWo))
#define JAG_PREFERENCES_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFERENCES_WO, JagPreferencesWoClass))
#define JAG_IS_PREFERENCES_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFERENCES_WO))
#define JAG_IS_PREFERENCES_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFERENCES_WO))
#define JAG_PREFERENCES_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFERENCES_WO, JagPreferencesWoClass))


typedef struct _JagPreferencesWo               JagPreferencesWo;
typedef struct _JagPreferencesWoPrivate        JagPreferencesWoPrivate;
typedef struct _JagPreferencesWoClass          JagPreferencesWoClass;


struct _JagPreferencesWo {
	CatWo parent;
};

struct _JagPreferencesWoClass {
	CatWoClass parent_class;
};


GType jag_preferences_wo_get_type();

JagPreferencesWo *jag_preferences_wo_new();

CatArrayWo *jag_preferences_wo_get_jre_list(const JagPreferencesWo *preferences);
CatArrayWo *jag_preferences_wo_get_editable_jre_list(JagPreferencesWo *e_preferences);

CAT_WO_BASE_H(JagPreferencesWo, jag_preferences_wo)

G_END_DECLS

#endif /* JAGPREFERENCESWO_H_ */
