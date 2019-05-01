/*
   File:    draimacromanagerlistener.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    May 1, 2019
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

#ifndef MACROS_DRAIMACROMANAGERLISTENER_H_
#define MACROS_DRAIMACROMANAGERLISTENER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_IMACRO_MANAGER_LISTENER                 (dra_imacro_manager_listener_get_type ())
#define DRA_IMACRO_MANAGER_LISTENER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), DRA_TYPE_IMACRO_MANAGER_LISTENER, DraIMacroManagerListener))
#define DRA_IS_IMACRO_MANAGER_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_IMACRO_MANAGER_LISTENER))
#define DRA_IMACRO_MANAGER_LISTENER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), DRA_TYPE_IMACRO_MANAGER_LISTENER, DraIMacroManagerListenerInterface))


typedef struct _DraIMacroManagerListener               DraIMacroManagerListener; /* dummy object */
typedef struct _DraIMacroManagerListenerInterface      DraIMacroManagerListenerInterface;

struct _DraIMacroManagerListenerInterface {
  GTypeInterface parent_iface;
  void (*macroState) (DraIMacroManagerListener *self, gboolean isRecording, gboolean macroAvailable);
};

GType dra_imacro_manager_listener_get_type(void);

void dra_imacro_manager_listener_macro_state(DraIMacroManagerListener *self, gboolean isRecording, gboolean macroAvailable);


G_END_DECLS

#endif /* MACROS_DRAIMACROMANAGERLISTENER_H_ */
