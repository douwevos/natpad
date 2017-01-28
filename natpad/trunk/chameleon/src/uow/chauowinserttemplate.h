/*
   File:    chauowinserttemplate.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 4, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef UOW_CHAUOWINSERTTEMPLATE_H_
#define UOW_CHAUOWINSERTTEMPLATE_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_INSERT_TEMPLATE              (cha_uow_insert_template_get_type())
#define CHA_UOW_INSERT_TEMPLATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_insert_template_get_type(), ChaUowInsertTemplate))
#define CHA_UOW_INSERT_TEMPLATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_INSERT_TEMPLATE, ChaUowInsertTemplateClass))
#define CHA_IS_UOW_INSERT_TEMPLATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_INSERT_TEMPLATE))
#define CHA_IS_UOW_INSERT_TEMPLATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_INSERT_TEMPLATE))
#define CHA_UOW_INSERT_TEMPLATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_INSERT_TEMPLATE, ChaUowInsertTemplateClass))


typedef struct _ChaUowInsertTemplate               ChaUowInsertTemplate;
typedef struct _ChaUowInsertTemplatePrivate        ChaUowInsertTemplatePrivate;
typedef struct _ChaUowInsertTemplateClass          ChaUowInsertTemplateClass;


struct _ChaUowInsertTemplate {
	ChaUow parent;
};

struct _ChaUowInsertTemplateClass {
	ChaUowClass parent_class;
};


GType cha_uow_insert_template_get_type();

ChaUowInsertTemplate *cha_uow_insert_template_new(CatStringWo *template_label);

G_END_DECLS

#endif /* UOW_CHAUOWINSERTTEMPLATE_H_ */
