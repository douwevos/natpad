/*
   File:    elkactionpreferences.h
   Project: elk
   Author:  Douwe Vos
   Date:    May 5, 2010
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

#ifndef ELKACTIONPREFERENCES_H_
#define ELKACTIONPREFERENCES_H_

#include "../elkiservice.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_PREFERENCES            (elk_action_preferences_get_type())
#define ELK_ACTION_PREFERENCES(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_preferences_get_type(), ElkActionPreferences))
#define ELK_ACTION_PREFERENCES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_PREFERENCES, ElkActionPreferencesClass))
#define ELK_IS_ACTION_PREFERENCES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_PREFERENCES))
#define ELK_IS_ACTION_PREFERENCES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_PREFERENCES))
#define ELK_ACTION_PREFERENCES_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_PREFERENCES, ElkActionPreferencesClass))

typedef struct _ElkActionPreferences	   		ElkActionPreferences;
typedef struct _ElkActionPreferencesPrivate	    ElkActionPreferencesPrivate;
typedef struct _ElkActionPreferencesClass	  	ElkActionPreferencesClass;


struct _ElkActionPreferences {
	LeaAction parent;
};

struct _ElkActionPreferencesClass {
	LeaActionClass parent_class;
};


GType elk_action_preferences_get_type();

ElkActionPreferences *elk_action_preferences_new(ElkIService *service);

G_END_DECLS

#endif /* ELKACTIONPREFERENCES_H_ */
