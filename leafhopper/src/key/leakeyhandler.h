/*
   File:    leakeyhandler.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 12, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef LEAKEYHANDLER_H_
#define LEAKEYHANDLER_H_

#include <caterpillar.h>
#include "leakey.h"
#include "leaikeybinding.h"
#include "../layout/leapanel.h"

G_BEGIN_DECLS

#define LEA_TYPE_KEY_HANDLER              (lea_key_handler_get_type())
#define LEA_KEY_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_handler_get_type(), LeaKeyHandler))
#define LEA_KEY_HANDLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY_HANDLER, LeaKeyHandlerClass))
#define LEA_IS_KEY_HANDLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY_HANDLER))
#define LEA_IS_KEY_HANDLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY_HANDLER))
#define LEA_KEY_HANDLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY_HANDLER, LeaKeyHandlerClass))
#define LEA_KEY_HANDLER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_KEY_HANDLER, LeaKeyHandlerPrivate))


typedef struct _LeaKeyHandler	   		LeaKeyHandler;
typedef struct _LeaKeyHandlerClass	  	LeaKeyHandlerClass;
typedef struct _LeaKeyHandlerPrivate	LeaKeyHandlerPrivate;


struct _LeaKeyHandler {
	GObject parent;
};

struct _LeaKeyHandlerClass {
	GObjectClass parent_class;
};


GType lea_key_handler_get_type();

LeaKeyHandler *lea_key_handler_new();

void lea_key_handler_add_binding(LeaKeyHandler *handler, LeaIKeyBinding *key_binding);
void lea_key_handler_remove_binding(LeaKeyHandler *handler, LeaIKeyBinding *key_binding);

gboolean lea_key_handler_run(LeaKeyHandler *handler, LeaKey *key, LeaKeyContext *context);

void lea_key_handler_update(LeaKeyHandler *handler, LeaActionGroup *action_group);


G_END_DECLS

#endif /* LEAKEYHANDLER_H_ */
