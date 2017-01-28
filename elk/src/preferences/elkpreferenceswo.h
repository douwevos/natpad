/*
   File:    elkpreferenceswo.h
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 23, 2014
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

#ifndef ELKPREFERENCESWO_H_
#define ELKPREFERENCESWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

extern CatS elk_s_editor_prefs;


#define ELK_TYPE_PREFERENCES_WO              (elk_preferences_wo_get_type())
#define ELK_PREFERENCES_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_preferences_wo_get_type(), ElkPreferencesWo))
#define ELK_PREFERENCES_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_PREFERENCES_WO, ElkPreferencesWoClass))
#define ELK_IS_PREFERENCES_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_PREFERENCES_WO))
#define ELK_IS_PREFERENCES_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_PREFERENCES_WO))
#define ELK_PREFERENCES_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_PREFERENCES_WO, ElkPreferencesWoClass))

typedef struct _ElkPreferencesWo               ElkPreferencesWo;
typedef struct _ElkPreferencesWoPrivate        ElkPreferencesWoPrivate;
typedef struct _ElkPreferencesWoClass          ElkPreferencesWoClass;


struct _ElkPreferencesWo {
	CatWo parent;
};

struct _ElkPreferencesWoClass {
	CatWoClass parent_class;
};


GType elk_preferences_wo_get_type();

ElkPreferencesWo *elk_preferences_wo_new();

gboolean elk_preferences_wo_one_instance(ElkPreferencesWo *prefs);
gboolean elk_preferences_wo_full_screen(ElkPreferencesWo *prefs);
gboolean elk_preferences_wo_multiple_resource_editors(ElkPreferencesWo *prefs);
GObject *elk_preferences_wo_get_sub(ElkPreferencesWo *prefs, CatStringWo *key);

void elk_preferences_wo_set_one_instance(ElkPreferencesWo *e_prefs, gboolean one_instance);
void elk_preferences_wo_set_full_screen(ElkPreferencesWo *e_prefs, gboolean full_screen);
void elk_preferences_wo_set_multiple_resource_editors(ElkPreferencesWo *e_prefs, gboolean multiple_resource_editors);
void elk_preferences_wo_set_sub(ElkPreferencesWo *e_prefs, CatStringWo *key, GObject *sub);

void elk_preferences_wo_dump(ElkPreferencesWo *prefs);

CAT_WO_BASE_H(ElkPreferencesWo, elk_preferences_wo)

G_END_DECLS

#endif /* ELKPREFERENCESWO_H_ */
