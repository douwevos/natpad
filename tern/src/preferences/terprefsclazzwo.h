/*
   File:    terprefsclazzwo.h
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

#ifndef TERPREFSCLAZZWO_H_
#define TERPREFSCLAZZWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define TER_TYPE_PREFS_CLAZZ_WO              (ter_prefs_clazz_wo_get_type())
#define TER_PREFS_CLAZZ_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_prefs_clazz_wo_get_type(), TerPrefsClazzWo))
#define TER_PREFS_CLAZZ_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_PREFS_CLAZZ_WO, TerPrefsClazzWoClass))
#define TER_IS_PREFS_CLAZZ_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_PREFS_CLAZZ_WO))
#define TER_IS_PREFS_CLAZZ_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_PREFS_CLAZZ_WO))
#define TER_PREFS_CLAZZ_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_PREFS_CLAZZ_WO, TerPrefsClazzWoClass))


typedef struct _TerPrefsClazzWo               TerPrefsClazzWo;
typedef struct _TerPrefsClazzWoPrivate        TerPrefsClazzWoPrivate;
typedef struct _TerPrefsClazzWoClass          TerPrefsClazzWoClass;


struct _TerPrefsClazzWo {
	CatWo parent;
};

struct _TerPrefsClazzWoClass {
	CatWoClass parent_class;
};


GType ter_prefs_clazz_wo_get_type();

TerPrefsClazzWo *ter_prefs_clazz_wo_new(CatStringWo *name);

CatStringWo *ter_prefs_clazz_wo_get_name(const TerPrefsClazzWo *prefs_clazz);
CatStringWo *ter_prefs_clazz_wo_get_grammar_file(const TerPrefsClazzWo *prefs_clazz);
CatStringWo *ter_prefs_clazz_wo_get_extra_characters(const TerPrefsClazzWo *prefs_clazz);

CatArrayWo *ter_prefs_clazz_wo_get_pattern_list(const TerPrefsClazzWo *prefs_clazz);


void ter_prefs_clazz_wo_set_name(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *name);
void ter_prefs_clazz_wo_set_grammar_file(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *grammar_file);
void ter_prefs_clazz_wo_set_extra_characters(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *extra_characters);

void ter_prefs_clazz_wo_add_pattern(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *filename_pattern);
void ter_prefs_clazz_wo_set_pattern_at(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *filename_pattern, int at_index);
void ter_prefs_clazz_wo_remove_pattern(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *pattern_to_remove);

CAT_WO_BASE_H(TerPrefsClazzWo, ter_prefs_clazz_wo);

G_END_DECLS

#endif /* TERPREFSCLAZZWO_H_ */
