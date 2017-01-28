/*
   File:    chauownextformfield.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 14, 2015
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

#ifndef UOW_CHAUOWNEXTFORMFIELD_H_
#define UOW_CHAUOWNEXTFORMFIELD_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_NEXT_FORM_FIELD              (cha_uow_next_form_field_get_type())
#define CHA_UOW_NEXT_FORM_FIELD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_next_form_field_get_type(), ChaUowNextFormField))
#define CHA_UOW_NEXT_FORM_FIELD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_NEXT_FORM_FIELD, ChaUowNextFormFieldClass))
#define CHA_IS_UOW_NEXT_FORM_FIELD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_NEXT_FORM_FIELD))
#define CHA_IS_UOW_NEXT_FORM_FIELD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_NEXT_FORM_FIELD))
#define CHA_UOW_NEXT_FORM_FIELD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_NEXT_FORM_FIELD, ChaUowNextFormFieldClass))


typedef struct _ChaUowNextFormField               ChaUowNextFormField;
typedef struct _ChaUowNextFormFieldClass          ChaUowNextFormFieldClass;


struct _ChaUowNextFormField {
	ChaUow parent;
};

struct _ChaUowNextFormFieldClass {
	ChaUowClass parent_class;
};


GType cha_uow_next_form_field_get_type();

ChaUowNextFormField *cha_uow_next_form_field_new();

gboolean cha_uow_next_form_field_run(ChaUowNextFormField *next_ff, ChaDocumentView *document_view);

G_END_DECLS

#endif /* UOW_CHAUOWNEXTFORMFIELD_H_ */
