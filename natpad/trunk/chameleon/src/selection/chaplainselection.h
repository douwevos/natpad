/*
   File:    chaplainselection.h
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

#ifndef CHAPLAINSELECTION_H_
#define CHAPLAINSELECTION_H_

#include "chaselection.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_PLAIN_SELECTION              (cha_plain_selection_get_type())
#define CHA_PLAIN_SELECTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_plain_selection_get_type(), ChaPlainSelection))
#define CHA_PLAIN_SELECTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PLAIN_SELECTION, ChaPlainSelectionClass))
#define CHA_IS_PLAIN_SELECTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PLAIN_SELECTION))
#define CHA_IS_PLAIN_SELECTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PLAIN_SELECTION))
#define CHA_PLAIN_SELECTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PLAIN_SELECTION, ChaPlainSelectionClass))


typedef struct _ChaPlainSelection               ChaPlainSelection;
typedef struct _ChaPlainSelectionPrivate        ChaPlainSelectionPrivate;
typedef struct _ChaPlainSelectionClass          ChaPlainSelectionClass;


struct _ChaPlainSelection {
	ChaSelection parent;
};

struct _ChaPlainSelectionClass {
	ChaSelectionClass parent_class;
};


GType cha_plain_selection_get_type();

ChaPlainSelection *cha_plain_selection_new(CatAtomicInteger *sequence, ChaCursorWo *cursor);

G_END_DECLS

#endif /* CHAPLAINSELECTION_H_ */
