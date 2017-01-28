/*
   File:    chauowselect.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 6, 2015
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

#ifndef CHAUOWSELECT_H_
#define CHAUOWSELECT_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS


typedef enum {
	CHA_UOW_SELECT_ALL, CHA_UOW_SELECT_WORD, CHA_UOW_SELECT_LINE
} ChaUowSelectKind;

#define CHA_TYPE_UOW_SELECT              (cha_uow_select_get_type())
#define CHA_UOW_SELECT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_select_get_type(), ChaUowSelect))
#define CHA_UOW_SELECT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_SELECT, ChaUowSelectClass))
#define CHA_IS_UOW_SELECT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_SELECT))
#define CHA_IS_UOW_SELECT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_SELECT))
#define CHA_UOW_SELECT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_SELECT, ChaUowSelectClass))


typedef struct _ChaUowSelect               ChaUowSelect;
typedef struct _ChaUowSelectPrivate        ChaUowSelectPrivate;
typedef struct _ChaUowSelectClass          ChaUowSelectClass;


struct _ChaUowSelect {
	ChaUow parent;
};

struct _ChaUowSelectClass {
	ChaUowClass parent_class;
};


GType cha_uow_select_get_type();

ChaUowSelect *cha_uow_select_new(ChaUowSelectKind kind);

G_END_DECLS


#endif /* CHAUOWSELECT_H_ */
