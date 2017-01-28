/*
   File:    chauowmovecursor.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 21, 2015
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

#ifndef CHAUOWMOVECURSOR_H_
#define CHAUOWMOVECURSOR_H_

#include "chauow.h"
#include "../layout/chaviewcursor.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_MOVE_CURSOR              (cha_uow_move_cursor_get_type())
#define CHA_UOW_MOVE_CURSOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_move_cursor_get_type(), ChaUowMoveCursor))
#define CHA_UOW_MOVE_CURSOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_MOVE_CURSOR, ChaUowMoveCursorClass))
#define CHA_IS_UOW_MOVE_CURSOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_MOVE_CURSOR))
#define CHA_IS_UOW_MOVE_CURSOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_MOVE_CURSOR))
#define CHA_UOW_MOVE_CURSOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_MOVE_CURSOR, ChaUowMoveCursorClass))


typedef struct _ChaUowMoveCursor               ChaUowMoveCursor;
typedef struct _ChaUowMoveCursorPrivate        ChaUowMoveCursorPrivate;
typedef struct _ChaUowMoveCursorClass          ChaUowMoveCursorClass;


struct _ChaUowMoveCursor {
	ChaUow parent;
};

struct _ChaUowMoveCursorClass {
	ChaUowClass parent_class;
};


GType cha_uow_move_cursor_get_type();

ChaUowMoveCursor *cha_uow_move_cursor_new(ChaMoveCursorKind kind, gboolean extend_selection, gboolean alt_pressed);

G_END_DECLS

#endif /* CHAUOWMOVECURSOR_H_ */
