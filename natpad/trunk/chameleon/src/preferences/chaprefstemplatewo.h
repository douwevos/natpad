/*
   File:    chaprefstemplatewo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 25, 2014
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

#ifndef CHAPREFSTEMPLATEWO_H_
#define CHAPREFSTEMPLATEWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PREFS_TEMPLATE_WO              (cha_prefs_template_wo_get_type())
#define CHA_PREFS_TEMPLATE_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_prefs_template_wo_get_type(), ChaPrefsTemplateWo))
#define CHA_PREFS_TEMPLATE_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PREFS_TEMPLATE_WO, ChaPrefsTemplateWoClass))
#define CHA_IS_PREFS_TEMPLATE_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PREFS_TEMPLATE_WO))
#define CHA_IS_PREFS_TEMPLATE_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PREFS_TEMPLATE_WO))
#define CHA_PREFS_TEMPLATE_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PREFS_TEMPLATE_WO, ChaPrefsTemplateWoClass))


typedef struct _ChaPrefsTemplateWo               ChaPrefsTemplateWo;
typedef struct _ChaPrefsTemplateWoPrivate        ChaPrefsTemplateWoPrivate;
typedef struct _ChaPrefsTemplateWoClass          ChaPrefsTemplateWoClass;


struct _ChaPrefsTemplateWo {
	CatWo parent;
};

struct _ChaPrefsTemplateWoClass {
	CatWoClass parent_class;
};


GType cha_prefs_template_wo_get_type();

ChaPrefsTemplateWo *cha_prefs_template_wo_new();

CatStringWo *cha_prefs_template_wo_get_name(const ChaPrefsTemplateWo *template);
CatStringWo *cha_prefs_template_wo_get_description(const ChaPrefsTemplateWo *template);
CatStringWo *cha_prefs_template_wo_get_template(const ChaPrefsTemplateWo *template);

void cha_prefs_template_wo_set_name(ChaPrefsTemplateWo *e_template, CatStringWo *new_name);
void cha_prefs_template_wo_set_description(ChaPrefsTemplateWo *e_template, CatStringWo *new_description);
void cha_prefs_template_wo_set_template(ChaPrefsTemplateWo *e_template, CatStringWo *new_template);

CAT_WO_BASE_H(ChaPrefsTemplateWo, cha_prefs_template_wo);

G_END_DECLS

#endif /* CHAPREFSTEMPLATEWO_H_ */
