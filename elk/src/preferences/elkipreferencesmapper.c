/*
   File:    elkipreferencesmapper.c
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

#include "elkipreferencesmapper.h"

CatS elk_s_yes = CAT_S_DEF("yes");
CatS elk_s_no = CAT_S_DEF("no");
CatS elk_s_true = CAT_S_DEF("true");
CatS elk_s_false = CAT_S_DEF("false");


G_DEFINE_INTERFACE(ElkIPreferencesMapper, elk_ipreferences_mapper, G_TYPE_OBJECT);

static void elk_ipreferences_mapper_default_init (ElkIPreferencesMapperInterface *iface) {
	iface->modelToPrefs = NULL;
	iface->prefsToModel = NULL;
}

void elk_ipreferences_mapper_model_to_prefs(ElkIPreferencesMapper *self, ShoModel *model, ElkPreferencesWo *e_prefs) {
	g_return_if_fail(ELK_IS_IPREFERENCES_MAPPER(self));
	ELK_IPREFERENCES_MAPPER_GET_INTERFACE(self)->modelToPrefs(self, model, e_prefs);

}

void elk_ipreferences_mapper_prefs_to_model(ElkIPreferencesMapper *self, ElkPreferencesWo *prefs, ShoModel *model) {
	g_return_if_fail(ELK_IS_IPREFERENCES_MAPPER(self));
	ELK_IPREFERENCES_MAPPER_GET_INTERFACE(self)->prefsToModel(self, prefs, model);
}
