/*
   File:    chauowlinemarker.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 31, 2015
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

#ifndef CHAUOWLINEMARKER_H_
#define CHAUOWLINEMARKER_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_LINE_MARKER              (cha_uow_line_marker_get_type())
#define CHA_UOW_LINE_MARKER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_line_marker_get_type(), ChaUowLineMarker))
#define CHA_UOW_LINE_MARKER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_LINE_MARKER, ChaUowLineMarkerClass))
#define CHA_IS_UOW_LINE_MARKER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_LINE_MARKER))
#define CHA_IS_UOW_LINE_MARKER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_LINE_MARKER))
#define CHA_UOW_LINE_MARKER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_LINE_MARKER, ChaUowLineMarkerClass))

typedef enum _ChaUowMarkMode ChaUowMarkMode;

enum _ChaUowMarkMode {
	CHA_UOW_MARK_TOGGLE,
	CHA_UOW_MARK_SET,
	CHA_UOW_MARK_UNSET,
	CHA_UOW_MARK_SET_ALL,
	CHA_UOW_MARK_UNSET_ALL,
	CHA_UOW_MARK_TOGGLE_ALL,
	CHA_UOW_MARK_COPY
};

typedef struct _ChaUowLineMarker               ChaUowLineMarker;
typedef struct _ChaUowLineMarkerPrivate        ChaUowLineMarkerPrivate;
typedef struct _ChaUowLineMarkerClass          ChaUowLineMarkerClass;


struct _ChaUowLineMarker {
	ChaUow parent;
};

struct _ChaUowLineMarkerClass {
	ChaUowClass parent_class;
};


GType cha_uow_line_marker_get_type();

ChaUowLineMarker *cha_uow_line_marker_new(ChaUowMarkMode mode);

G_END_DECLS

#endif /* CHAUOWLINEMARKER_H_ */
