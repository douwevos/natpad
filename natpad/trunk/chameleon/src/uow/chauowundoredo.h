/*
   File:    chauowundoredo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jun 22, 2015
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

#ifndef UOW_CHAUOWUNDOREDO_H_
#define UOW_CHAUOWUNDOREDO_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_UOW_UNDO_REDO              (cha_uow_undo_redo_get_type())
#define CHA_UOW_UNDO_REDO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_undo_redo_get_type(), ChaUowUndoRedo))
#define CHA_UOW_UNDO_REDO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_UNDO_REDO, ChaUowUndoRedoClass))
#define CHA_IS_UOW_UNDO_REDO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_UNDO_REDO))
#define CHA_IS_UOW_UNDO_REDO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_UNDO_REDO))
#define CHA_UOW_UNDO_REDO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_UNDO_REDO, ChaUowUndoRedoClass))


typedef struct _ChaUowUndoRedo               ChaUowUndoRedo;
typedef struct _ChaUowUndoRedoPrivate        ChaUowUndoRedoPrivate;
typedef struct _ChaUowUndoRedoClass          ChaUowUndoRedoClass;


struct _ChaUowUndoRedo {
	ChaUow parent;
};

struct _ChaUowUndoRedoClass {
	ChaUowClass parent_class;
};


GType cha_uow_undo_redo_get_type();

ChaUowUndoRedo *cha_uow_undo_redo_new(gboolean redo);

G_END_DECLS

#endif /* UOW_CHAUOWUNDOREDO_H_ */
