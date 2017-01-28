/*
   File:    jagprefsjrewo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 9, 2014
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

#ifndef JAGPREFSJREWO_H_
#define JAGPREFSJREWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_PREFS_JRE_WO              (jag_prefs_jre_wo_get_type())
#define JAG_PREFS_JRE_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_prefs_jre_wo_get_type(), JagPrefsJreWo))
#define JAG_PREFS_JRE_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFS_JRE_WO, JagPrefsJreWoClass))
#define JAG_IS_PREFS_JRE_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFS_JRE_WO))
#define JAG_IS_PREFS_JRE_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFS_JRE_WO))
#define JAG_PREFS_JRE_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFS_JRE_WO, JagPrefsJreWoClass))


typedef struct _JagPrefsJreWo               JagPrefsJreWo;
typedef struct _JagPrefsJreWoPrivate        JagPrefsJreWoPrivate;
typedef struct _JagPrefsJreWoClass          JagPrefsJreWoClass;


struct _JagPrefsJreWo {
	CatWo parent;
};

struct _JagPrefsJreWoClass {
	CatWoClass parent_class;
};


GType jag_prefs_jre_wo_get_type();

JagPrefsJreWo *jag_prefs_jre_wo_new();

CatStringWo *jag_prefs_jre_wo_get_name(const JagPrefsJreWo *prefs);
CatStringWo *jag_prefs_jre_wo_get_path(const JagPrefsJreWo *prefs);
CatArrayWo *jag_prefs_jre_wo_get_library_paths(const JagPrefsJreWo *prefs);

void jag_prefs_jre_wo_set_name(JagPrefsJreWo *e_prefs, CatStringWo *new_name);
void jag_prefs_jre_wo_set_path(JagPrefsJreWo *e_prefs, CatStringWo *new_path);
void jag_prefs_jre_wo_set_library_paths(JagPrefsJreWo *e_prefs, CatArrayWo *new_paths);

CAT_WO_BASE_H(JagPrefsJreWo, jag_prefs_jre_wo);

G_END_DECLS

#endif /* JAGPREFSJREWO_H_ */
