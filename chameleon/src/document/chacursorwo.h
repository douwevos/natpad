/*
   File:    chacursorwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 20, 2015
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

#ifndef CHACURSORWO_H_
#define CHACURSORWO_H_

#include "chacursor.h"
#include "chalinelocationwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_CURSOR_WO              (cha_cursor_wo_get_type())
#define CHA_CURSOR_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_cursor_wo_get_type(), ChaCursorWo))
#define CHA_CURSOR_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_CURSOR_WO, ChaCursorWoClass))
#define CHA_IS_CURSOR_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_CURSOR_WO))
#define CHA_IS_CURSOR_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_CURSOR_WO))
#define CHA_CURSOR_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_CURSOR_WO, ChaCursorWoClass))


typedef struct _ChaCursorWo               ChaCursorWo;
typedef struct _ChaCursorWoPrivate        ChaCursorWoPrivate;
typedef struct _ChaCursorWoClass          ChaCursorWoClass;


struct _ChaCursorWo {
	CatWo parent;
};

struct _ChaCursorWoClass {
	CatWoClass parent_class;
};


GType cha_cursor_wo_get_type();

ChaCursorWo *cha_cursor_wo_new();
ChaCursorWo *cha_cursor_wo_new_ll_offset(ChaLineLocationWo *line_location, int x_cursor);

int cha_cursor_wo_get_x_cursor_bytes(const ChaCursorWo *cursor);
int cha_cursor_wo_get_x_sub(const ChaCursorWo *cursor);
ChaLineLocationWo *cha_cursor_wo_get_line_location(const ChaCursorWo *cursor);

ChaLineLocationWo *cha_cursor_wo_get_editable_line_location(ChaCursorWo *e_cursor);
void cha_cursor_wo_set_x_cursor_bytes(ChaCursorWo *e_cursor, int new_x_cursor);
void cha_cursor_wo_set_x_sub(ChaCursorWo *e_cursor, int x_sub);
void cha_cursor_wo_set_line_location(ChaCursorWo *e_cursor, ChaLineLocationWo *location);

/* result:
 *  -2  instance-a is referring to a line which precedes the line referred by instance-b
 *  -1  instance-a and instance-b refer to the same line but the x position of a is left of b
 *  0   instance-a and instance-b are identical
 *  1   instance-a and instance-b refer to the same line but the x position of a is right of b
 *  2   instance-a is referring to a line which comes after the line referred by instance-b
 */
ChaCursorOrder cha_cursor_wo_compare(const ChaCursorWo *instance_a, const ChaCursorWo *instance_b);

CAT_WO_BASE_H(ChaCursorWo,cha_cursor_wo);

G_END_DECLS

#endif /* CHACURSORWO_H_ */
