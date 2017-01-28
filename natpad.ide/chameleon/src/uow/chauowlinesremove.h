/*
   File:    chauowlinesremove.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 30, 2015
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

#ifndef UOW_CHAUOWLINESREMOVE_H_
#define UOW_CHAUOWLINESREMOVE_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_LINES_REMOVE              (cha_uow_lines_remove_get_type())
#define CHA_UOW_LINES_REMOVE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_lines_remove_get_type(), ChaUowLinesRemove))
#define CHA_UOW_LINES_REMOVE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_LINES_REMOVE, ChaUowLinesRemoveClass))
#define CHA_IS_UOW_LINES_REMOVE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_LINES_REMOVE))
#define CHA_IS_UOW_LINES_REMOVE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_LINES_REMOVE))
#define CHA_UOW_LINES_REMOVE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_LINES_REMOVE, ChaUowLinesRemoveClass))


typedef struct _ChaUowLinesRemove               ChaUowLinesRemove;
typedef struct _ChaUowLinesRemoveClass          ChaUowLinesRemoveClass;


struct _ChaUowLinesRemove {
	ChaUow parent;
};

struct _ChaUowLinesRemoveClass {
	ChaUowClass parent_class;
};


GType cha_uow_lines_remove_get_type();

ChaUowLinesRemove *cha_uow_lines_remove_new();

G_END_DECLS

#endif /* UOW_CHAUOWLINESREMOVE_H_ */
