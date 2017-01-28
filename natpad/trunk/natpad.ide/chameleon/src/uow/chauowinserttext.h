/*
   File:    chauowinserttext.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 28, 2015
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

#ifndef CHAUOWINSERTTEXT_H_
#define CHAUOWINSERTTEXT_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_INSERT_TEXT              (cha_uow_insert_text_get_type())
#define CHA_UOW_INSERT_TEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_insert_text_get_type(), ChaUowInsertText))
#define CHA_UOW_INSERT_TEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_INSERT_TEXT, ChaUowInsertTextClass))
#define CHA_IS_UOW_INSERT_TEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_INSERT_TEXT))
#define CHA_IS_UOW_INSERT_TEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_INSERT_TEXT))
#define CHA_UOW_INSERT_TEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_INSERT_TEXT, ChaUowInsertTextClass))


typedef struct _ChaUowInsertText               ChaUowInsertText;
typedef struct _ChaUowInsertTextPrivate        ChaUowInsertTextPrivate;
typedef struct _ChaUowInsertTextClass          ChaUowInsertTextClass;


struct _ChaUowInsertText {
	ChaUow parent;
};

struct _ChaUowInsertTextClass {
	ChaUowClass parent_class;
};


GType cha_uow_insert_text_get_type();

ChaUowInsertText *cha_uow_insert_text_new(CatStringWo *ca_text);

G_END_DECLS

#endif /* CHAUOWINSERTTEXT_H_ */
