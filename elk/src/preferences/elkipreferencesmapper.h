/*
   File:    elkipreferencesmapper.h
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 3, 2014
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

#ifndef ELKIPREFERENCESMAPPER_H_
#define ELKIPREFERENCESMAPPER_H_

#include <caterpillar.h>
#include <shoveler.h>
#include "elkpreferenceswo.h"


extern CatS elk_s_yes;
extern CatS elk_s_no;
extern CatS elk_s_true;
extern CatS elk_s_false;




#define ELK_TYPE_IPREFERENCES_MAPPER                 (elk_ipreferences_mapper_get_type())
#define ELK_IPREFERENCES_MAPPER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), ELK_TYPE_IPREFERENCES_MAPPER, ElkIPreferencesMapper))
#define ELK_IS_IPREFERENCES_MAPPER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), ELK_TYPE_IPREFERENCES_MAPPER))
#define ELK_IPREFERENCES_MAPPER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), ELK_TYPE_IPREFERENCES_MAPPER, ElkIPreferencesMapperInterface))


typedef struct _ElkIPreferencesMapper               ElkIPreferencesMapper; /* dummy object */
typedef struct _ElkIPreferencesMapperInterface      ElkIPreferencesMapperInterface;

struct _ElkIPreferencesMapperInterface {
	GTypeInterface parent_iface;
	void(*modelToPrefs)(ElkIPreferencesMapper *self, ShoModel *model, ElkPreferencesWo *e_prefs);
	void(*prefsToModel)(ElkIPreferencesMapper *self, ElkPreferencesWo *prefs, ShoModel *model);
};

GType elk_ipreferences_mapper_get_type(void);

void elk_ipreferences_mapper_model_to_prefs(ElkIPreferencesMapper *self, ShoModel *model, ElkPreferencesWo *e_prefs);
void elk_ipreferences_mapper_prefs_to_model(ElkIPreferencesMapper *self, ElkPreferencesWo *prefs, ShoModel *model);

#endif /* ELKIPREFERENCESMAPPER_H_ */
