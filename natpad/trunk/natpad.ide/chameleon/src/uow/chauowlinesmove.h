/*
   File:    chauowlinesmove.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 16, 2015
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

#ifndef CHAUOWLINESMOVE_H_
#define CHAUOWLINESMOVE_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_LINES_MOVE              (cha_uow_lines_move_get_type())
#define CHA_UOW_LINES_MOVE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_lines_move_get_type(), ChaUowLinesMove))
#define CHA_UOW_LINES_MOVE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_LINES_MOVE, ChaUowLinesMoveClass))
#define CHA_IS_UOW_LINES_MOVE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_LINES_MOVE))
#define CHA_IS_UOW_LINES_MOVE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_LINES_MOVE))
#define CHA_UOW_LINES_MOVE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_LINES_MOVE, ChaUowLinesMoveClass))


typedef struct _ChaUowLinesMove               ChaUowLinesMove;
typedef struct _ChaUowLinesMovePrivate        ChaUowLinesMovePrivate;
typedef struct _ChaUowLinesMoveClass          ChaUowLinesMoveClass;


struct _ChaUowLinesMove {
	ChaUow parent;
};

struct _ChaUowLinesMoveClass {
	ChaUowClass parent_class;
};


GType cha_uow_lines_move_get_type();

ChaUowLinesMove *cha_uow_lines_move_new(gboolean down);

G_END_DECLS

#endif /* CHAUOWLINESMOVE_H_ */
