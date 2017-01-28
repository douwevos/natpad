/*
   File:    chauowreplacetext.h
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

#ifndef UOW_CHAUOWREPLACETEXT_H_
#define UOW_CHAUOWREPLACETEXT_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_REPLACE_TEXT              (cha_uow_replace_text_get_type())
#define CHA_UOW_REPLACE_TEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_replace_text_get_type(), ChaUowReplaceText))
#define CHA_UOW_REPLACE_TEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_REPLACE_TEXT, ChaUowReplaceTextClass))
#define CHA_IS_UOW_REPLACE_TEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_REPLACE_TEXT))
#define CHA_IS_UOW_REPLACE_TEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_REPLACE_TEXT))
#define CHA_UOW_REPLACE_TEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_REPLACE_TEXT, ChaUowReplaceTextClass))


typedef struct _ChaUowReplaceText               ChaUowReplaceText;
typedef struct _ChaUowReplaceTextPrivate        ChaUowReplaceTextPrivate;
typedef struct _ChaUowReplaceTextClass          ChaUowReplaceTextClass;


struct _ChaUowReplaceText {
	ChaUow parent;
};

struct _ChaUowReplaceTextClass {
	ChaUowClass parent_class;
};


GType cha_uow_replace_text_get_type();

ChaUowReplaceText *cha_uow_replace_text_new();

G_END_DECLS

#endif /* UOW_CHAUOWREPLACETEXT_H_ */
