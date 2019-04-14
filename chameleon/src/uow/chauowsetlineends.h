/*
   File:    chauowsetlineends.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 14, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef UOW_CHAUOWSETLINEENDS_H_
#define UOW_CHAUOWSETLINEENDS_H_

#include "chauow.h"
#include "../document/chalinewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_SET_LINE_ENDS              (cha_uow_set_line_ends_get_type())
#define CHA_UOW_SET_LINE_ENDS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_set_line_ends_get_type(), ChaUowSetLineEnds))
#define CHA_UOW_SET_LINE_ENDS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_SET_LINE_ENDS, ChaUowSetLineEndsClass))
#define CHA_IS_UOW_SET_LINE_ENDS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_SET_LINE_ENDS))
#define CHA_IS_UOW_SET_LINE_ENDS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_SET_LINE_ENDS))
#define CHA_UOW_SET_LINE_ENDS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_SET_LINE_ENDS, ChaUowSetLineEndsClass))

struct _ChaUowSetLineEnds {
	ChaUow parent;
};

struct _ChaUowSetLineEndsClass {
	ChaUowClass parent_class;
};

typedef struct _ChaUowSetLineEnds               ChaUowSetLineEnds;
typedef struct _ChaUowSetLineEndsPrivate        ChaUowSetLineEndsPrivate;
typedef struct _ChaUowSetLineEndsClass          ChaUowSetLineEndsClass;


GType cha_uow_set_line_ends_get_type();

ChaUowSetLineEnds *cha_uow_set_line_ends_new(ChaLineEnd line_end);

G_END_DECLS

#endif /* UOW_CHAUOWSETLINEENDS_H_ */
