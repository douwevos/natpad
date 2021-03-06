/*
   File:    chaviewcursor.h
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

#ifndef CHAVIEWCURSOR_H_
#define CHAVIEWCURSOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum {
	CHA_MOVE_CURSOR_NEXT_COLUMN,
	CHA_MOVE_CURSOR_NEXT_COLUMN_WORD,
	CHA_MOVE_CURSOR_PREV_COLUMN,
	CHA_MOVE_CURSOR_PREV_COLUMN_WORD,

	CHA_MOVE_CURSOR_UP, CHA_MOVE_CURSOR_PAGE_UP,
	CHA_MOVE_CURSOR_DOWN, CHA_MOVE_CURSOR_PAGE_DOWN,

	CHA_MOVE_CURSOR_BEGIN_OF_LINE, CHA_MOVE_CURSOR_BEGIN_OF_DOCUMENT,
	CHA_MOVE_CURSOR_END_OF_LINE, CHA_MOVE_CURSOR_END_OF_DOCUMENT
} ChaMoveCursorKind;

G_END_DECLS

#endif /* CHAVIEWCURSOR_H_ */
