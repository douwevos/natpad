/*
   File:    turline.h
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 12, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef LAYOUT_TURLINE_H_
#define LAYOUT_TURLINE_H_

#include "../model/turinode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define TUR_TYPE_LINE              (tur_line_get_type())
#define TUR_LINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_line_get_type(), TurLine))
#define TUR_LINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_LINE, TurLineClass))
#define TUR_IS_LINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_LINE))
#define TUR_IS_LINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_LINE))
#define TUR_LINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_LINE, TurLineClass))


typedef struct _TurLine               TurLine;
typedef struct _TurLinePrivate        TurLinePrivate;
typedef struct _TurLineClass          TurLineClass;


struct _TurLine {
	GObject parent;
};

struct _TurLineClass {
	GObjectClass parent_class;
};


GType tur_line_get_type();

TurLine *tur_line_new(TurINode *node);

TurINode *tur_line_get_node(TurLine *line);

G_END_DECLS

#endif /* LAYOUT_TURLINE_H_ */
