/*
   File:    chaprintline.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 6, 2015
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

#ifndef PRINTING_CHAPRINTLINE_H_
#define PRINTING_CHAPRINTLINE_H_

#include "../layout/chalinelayout.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PRINT_LINE              (cha_print_line_get_type())
#define CHA_PRINT_LINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_print_line_get_type(), ChaPrintLine))
#define CHA_PRINT_LINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PRINT_LINE, ChaPrintLineClass))
#define CHA_IS_PRINT_LINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PRINT_LINE))
#define CHA_IS_PRINT_LINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PRINT_LINE))
#define CHA_PRINT_LINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PRINT_LINE, ChaPrintLineClass))


typedef struct _ChaPrintLine               ChaPrintLine;
typedef struct _ChaPrintLinePrivate        ChaPrintLinePrivate;
typedef struct _ChaPrintLineClass          ChaPrintLineClass;


struct _ChaPrintLine {
	GObject parent;
};

struct _ChaPrintLineClass {
	GObjectClass parent_class;
};


GType cha_print_line_get_type();

ChaPrintLine *cha_print_line_new(ChaLineLayout *line_layout, int sub_line_idx, int view_height, long long line_nr);

int cha_print_line_get_view_height(ChaPrintLine *print_line);

ChaLineLayout *cha_print_line_get_line_layout(ChaPrintLine *print_line);
int cha_print_line_get_sub_line_index(ChaPrintLine *print_line);

long long cha_print_line_get_line_nr(ChaPrintLine *print_line);

G_END_DECLS

#endif /* PRINTING_CHAPRINTLINE_H_ */
