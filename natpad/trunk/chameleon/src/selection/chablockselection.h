/*
   File:    chablockselection.h
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

#ifndef CHABLOCKSELECTION_H_
#define CHABLOCKSELECTION_H_

#include "chaselection.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_BLOCK_SELECTION              (cha_block_selection_get_type())
#define CHA_BLOCK_SELECTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_block_selection_get_type(), ChaBlockSelection))
#define CHA_BLOCK_SELECTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_BLOCK_SELECTION, ChaBlockSelectionClass))
#define CHA_IS_BLOCK_SELECTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_BLOCK_SELECTION))
#define CHA_IS_BLOCK_SELECTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_BLOCK_SELECTION))
#define CHA_BLOCK_SELECTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_BLOCK_SELECTION, ChaBlockSelectionClass))


typedef struct _ChaBlockSelection               ChaBlockSelection;
typedef struct _ChaBlockSelectionPrivate        ChaBlockSelectionPrivate;
typedef struct _ChaBlockSelectionClass          ChaBlockSelectionClass;


struct _ChaBlockSelection {
	ChaSelection parent;
};

struct _ChaBlockSelectionClass {
	ChaSelectionClass parent_class;
};


GType cha_block_selection_get_type();

ChaBlockSelection *cha_block_selection_new(CatAtomicInteger *sequence, ChaCursorWo *cursor);

ChaCursorWo *cha_block_selection_paste(struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision, CatStringWo *text);

G_END_DECLS

#endif /* CHABLOCKSELECTION_H_ */
