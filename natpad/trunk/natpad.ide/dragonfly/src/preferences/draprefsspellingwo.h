/*
   File:    draprefsspellingwo.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 30, 2016
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

#ifndef PREFERENCES_DRAPREFSSPELLINGWO_H_
#define PREFERENCES_DRAPREFSSPELLINGWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_PREFS_SPELLING_WO              (dra_prefs_spelling_wo_get_type())
#define DRA_PREFS_SPELLING_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_prefs_spelling_wo_get_type(), DraPrefsSpellingWo))
#define DRA_PREFS_SPELLING_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_PREFS_SPELLING_WO, DraPrefsSpellingWoClass))
#define DRA_IS_PREFS_SPELLING_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_PREFS_SPELLING_WO))
#define DRA_IS_PREFS_SPELLING_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_PREFS_SPELLING_WO))
#define DRA_PREFS_SPELLING_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_PREFS_SPELLING_WO, DraPrefsSpellingWoClass))


typedef struct _DraPrefsSpellingWo               DraPrefsSpellingWo;
typedef struct _DraPrefsSpellingWoPrivate        DraPrefsSpellingWoPrivate;
typedef struct _DraPrefsSpellingWoClass          DraPrefsSpellingWoClass;


struct _DraPrefsSpellingWo {
	CatWo parent;
};

struct _DraPrefsSpellingWoClass {
	CatWoClass parent_class;
};


GType dra_prefs_spelling_wo_get_type();

DraPrefsSpellingWo *dra_prefs_spelling_wo_new();



gboolean dra_prefs_spelling_wo_is_enabled(const DraPrefsSpellingWo *spelling);
CatStringWo *dra_prefs_spelling_wo_get_dictionary_name(const DraPrefsSpellingWo *spelling);
int dra_prefs_spelling_wo_get_max_suggestions(const DraPrefsSpellingWo *spelling);

void dra_prefs_spelling_wo_set_enabled(DraPrefsSpellingWo *e_spelling, gboolean is_enabled);
void dra_prefs_spelling_wo_set_dictionary_name(DraPrefsSpellingWo *e_spelling, CatStringWo *dict_name);
void dra_prefs_spelling_wo_set_max_suggestions(DraPrefsSpellingWo *e_spelling, int max_suggestions);

CAT_WO_BASE_H(DraPrefsSpellingWo,dra_prefs_spelling_wo);

G_END_DECLS

#endif /* PREFERENCES_DRAPREFSSPELLINGWO_H_ */
