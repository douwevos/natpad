/*
   File:    dralinelayout.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 4, 2015
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

#ifndef DRALINELAYOUT_H_
#define DRALINELAYOUT_H_

#include "dralineinfowo.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_LINE_LAYOUT              (dra_line_layout_get_type())
#define DRA_LINE_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_line_layout_get_type(), DraLineLayout))
#define DRA_LINE_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_LINE_LAYOUT, DraLineLayoutClass))
#define DRA_IS_LINE_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_LINE_LAYOUT))
#define DRA_IS_LINE_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_LINE_LAYOUT))
#define DRA_LINE_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_LINE_LAYOUT, DraLineLayoutClass))


typedef struct _DraLineLayout               DraLineLayout;
typedef struct _DraLineLayoutPrivate        DraLineLayoutPrivate;
typedef struct _DraLineLayoutClass          DraLineLayoutClass;


struct _DraLineLayout {
	ChaLineLayout parent;
};

struct _DraLineLayoutClass {
	ChaLineLayoutClass parent_class;
};


GType dra_line_layout_get_type();

DraLineLayout *dra_line_layout_new();

void dra_line_layout_set_line_info(DraLineLayout *line_layout, DraLineInfoWo *a_line_info);

CatArrayWo *dra_line_layout_get_line_tags(DraLineLayout *line_layout);

void dra_line_layout_set_occurrences(DraLineLayout *line_layout, CatArrayWo *a_occurrences);


G_END_DECLS

#endif /* DRALINELAYOUT_H_ */
