/*
   File:    elkactionsave.h
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 12, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef ELKACTIONSAVE_H_
#define ELKACTIONSAVE_H_

#include <dragonfly.h>
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_SAVE              (elk_action_save_get_type())
#define ELK_ACTION_SAVE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_save_get_type(), ElkActionSave))
#define ELK_ACTION_SAVE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_SAVE, ElkActionSaveClass))
#define ELK_IS_ACTION_SAVE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_SAVE))
#define ELK_IS_ACTION_SAVE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_SAVE))
#define ELK_ACTION_SAVE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_SAVE, ElkActionSaveClass))

typedef struct _ElkActionSave               ElkActionSave;
typedef struct _ElkActionSavePrivate        ElkActionSavePrivate;
typedef struct _ElkActionSaveClass          ElkActionSaveClass;


struct _ElkActionSave {
	LeaAction parent;
};

struct _ElkActionSaveClass {
	LeaActionClass parent_class;
};


GType elk_action_save_get_type();

ElkActionSave *elk_action_save_new();

void elk_action_save_set_editor_panel(ElkActionSave *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ELKACTIONSAVE_H_ */
