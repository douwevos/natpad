/*
   File:    cushighlighter.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 7, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef EDITOR_CUSHIGHLIGHTER_H_
#define EDITOR_CUSHIGHLIGHTER_H_

#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define CUS_TYPE_HIGHLIGHTER              (cus_highlighter_get_type())
#define CUS_HIGHLIGHTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_highlighter_get_type(), CusHighlighter))
#define CUS_HIGHLIGHTER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_HIGHLIGHTER, CusHighlighterClass))
#define CUS_IS_HIGHLIGHTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_HIGHLIGHTER))
#define CUS_IS_HIGHLIGHTER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_HIGHLIGHTER))
#define CUS_HIGHLIGHTER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_HIGHLIGHTER, CusHighlighterClass))


typedef struct _CusHighlighter               CusHighlighter;
typedef struct _CusHighlighterPrivate        CusHighlighterPrivate;
typedef struct _CusHighlighterClass          CusHighlighterClass;


struct _CusHighlighter {
	DraHighlighter parent;
};

struct _CusHighlighterClass {
	DraHighlighterClass parent_class;
};


GType cus_highlighter_get_type();

CusHighlighter *cus_highlighter_new();

G_END_DECLS

#endif /* EDITOR_CUSHIGHLIGHTER_H_ */
