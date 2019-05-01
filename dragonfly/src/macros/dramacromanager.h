/*
   File:    dramacromanager.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 21, 2016
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

#ifndef MACROS_DRAMACROMANAGER_H_
#define MACROS_DRAMACROMANAGER_H_

#include "draimacromanagerlistener.h"
#include "../draeditor.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_MACRO_MANAGER              (dra_macro_manager_get_type())
#define DRA_MACRO_MANAGER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_macro_manager_get_type(), DraMacroManager))
#define DRA_MACRO_MANAGER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_MACRO_MANAGER, DraMacroManagerClass))
#define DRA_IS_MACRO_MANAGER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_MACRO_MANAGER))
#define DRA_IS_MACRO_MANAGER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_MACRO_MANAGER))
#define DRA_MACRO_MANAGER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_MACRO_MANAGER, DraMacroManagerClass))

typedef struct _DraMacroManager               DraMacroManager;
typedef struct _DraMacroManagerPrivate        DraMacroManagerPrivate;
typedef struct _DraMacroManagerClass          DraMacroManagerClass;


struct _DraMacroManager {
	GObject parent;
};

struct _DraMacroManagerClass {
	GObjectClass parent_class;
};


GType dra_macro_manager_get_type();

DraMacroManager *dra_macro_manager_new();

void dra_macro_manager_add_listener(DraMacroManager *macro_manager, DraIMacroManagerListener *listener);
void dra_macro_manager_remove_listener(DraMacroManager *macro_manager, DraIMacroManagerListener *listener);

void dra_macro_manager_set_editor(DraMacroManager *macro_manager, DraEditor *new_editor);

void dra_macro_manager_start_recording(DraMacroManager *macro_manager);
void dra_macro_manager_stop_recording(DraMacroManager *macro_manager);
void dra_macro_manager_replay(DraMacroManager *macro_manager);

G_END_DECLS

#endif /* MACROS_DRAMACROMANAGER_H_ */
