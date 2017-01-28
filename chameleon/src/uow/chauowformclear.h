/*
   File:    chauowformclear.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#ifndef UOW_CHAUOWFORMCLEAR_H_
#define UOW_CHAUOWFORMCLEAR_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_FORM_CLEAR              (cha_uow_form_clear_get_type())
#define CHA_UOW_FORM_CLEAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_form_clear_get_type(), ChaUowFormClear))
#define CHA_UOW_FORM_CLEAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_FORM_CLEAR, ChaUowFormClearClass))
#define CHA_IS_UOW_FORM_CLEAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_FORM_CLEAR))
#define CHA_IS_UOW_FORM_CLEAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_FORM_CLEAR))
#define CHA_UOW_FORM_CLEAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_FORM_CLEAR, ChaUowFormClearClass))


typedef struct _ChaUowFormClear               ChaUowFormClear;
typedef struct _ChaUowFormClearClass          ChaUowFormClearClass;


struct _ChaUowFormClear {
	ChaUow parent;
};

struct _ChaUowFormClearClass {
	ChaUowClass parent_class;
};


GType cha_uow_form_clear_get_type();

ChaUowFormClear *cha_uow_form_clear_new();

gboolean cha_uow_form_clear_run(ChaUowFormClear *form_clear, ChaDocumentView *document_view);

G_END_DECLS

#endif /* UOW_CHAUOWFORMCLEAR_H_ */
