/*
   File:    chaselectionprivate.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 18, 2015
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

#ifndef CHASELECTIONPRIVATE_H_
#define CHASELECTIONPRIVATE_H_

#include "chaselection.h"

struct _ChaSelectionPrivate {
	CatAtomicInteger *sequence;
	ChaCursorWo *a_start;
	ChaCursorWo *a_end;
	int modification_count;

	int top_page_index;
	int top_page_line_index;
	int top_x_cursor_bytes;
	int bottom_page_index;
	int bottom_page_line_index;
	int bottom_x_cursor_bytes;
};

//static inline gpointer cha_selection_get_instance_private(ChaSelection *self);

ChaSelectionPrivate *cha_selection_get_protected(ChaSelection *selection);


#endif /* CHASELECTIONPRIVATE_H_ */
