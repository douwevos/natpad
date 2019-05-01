/*
   File:    draimacromanagerlistener.c
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

#include "draimacromanagerlistener.h"

G_DEFINE_INTERFACE(DraIMacroManagerListener, dra_imacro_manager_listener, G_TYPE_OBJECT);

static void dra_imacro_manager_listener_default_init (DraIMacroManagerListenerInterface *iface) {
	iface->macroState = NULL;
}

void dra_imacro_manager_listener_macro_state(DraIMacroManagerListener *self, gboolean isRecording, gboolean macroAvailable) {
	DRA_IMACRO_MANAGER_LISTENER_GET_INTERFACE(self)->macroState(self, isRecording, macroAvailable);
}
