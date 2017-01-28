/*
   File:    cushighlightermap.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 6, 2016
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

#ifndef EDITOR_CUSHIGHLIGHTERMAP_H_
#define EDITOR_CUSHIGHLIGHTERMAP_H_

#include <caterpillar.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define CUS_TYPE_HIGHLIGHTER_MAP              (cus_highlighter_map_get_type())
#define CUS_HIGHLIGHTER_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_highlighter_map_get_type(), CusHighlighterMap))
#define CUS_HIGHLIGHTER_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_HIGHLIGHTER_MAP, CusHighlighterMapClass))
#define CUS_IS_HIGHLIGHTER_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_HIGHLIGHTER_MAP))
#define CUS_IS_HIGHLIGHTER_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_HIGHLIGHTER_MAP))
#define CUS_HIGHLIGHTER_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_HIGHLIGHTER_MAP, CusHighlighterMapClass))


typedef struct _CusHighlighterMap               CusHighlighterMap;
typedef struct _CusHighlighterMapPrivate        CusHighlighterMapPrivate;
typedef struct _CusHighlighterMapClass          CusHighlighterMapClass;


struct _CusHighlighterMap {
	DraHighlighterMap parent;
};

struct _CusHighlighterMapClass {
	DraHighlighterMapClass parent_class;
};


GType cus_highlighter_map_get_type();

CusHighlighterMap *cus_highlighter_map_new(WorService *wor_service);

G_END_DECLS

#endif /* EDITOR_CUSHIGHLIGHTERMAP_H_ */
