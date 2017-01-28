/*
   File:    natpad-app.h
   Project: natpad
   Author:  Douwe Vos
   Date:    Oct 23, 2015
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

#ifndef NATPAD_APP_H_
#define NATPAD_APP_H_

#include <gtk/gtk.h>
#include <caterpillar.h>
#include <viper.h>
#include <elk.h>


G_BEGIN_DECLS

#define NATPAD_TYPE_APP              (natpad_app_get_type())
#define NATPAD_APP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), natpad_app_get_type(), NatpadApp))
#define NATPAD_APP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), NATPAD_TYPE_APP, NatpadAppClass))
#define NATPAD_IS_APP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NATPAD_TYPE_APP))
#define NATPAD_IS_APP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), NATPAD_TYPE_APP))
#define NATPAD_APP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), NATPAD_TYPE_APP, NatpadAppClass))


typedef struct _NatpadApp               NatpadApp;
typedef struct _NatpadAppPrivate        NatpadAppPrivate;
typedef struct _NatpadAppClass          NatpadAppClass;


struct _NatpadApp {
	GtkApplication parent;
};

struct _NatpadAppClass {
	GtkApplicationClass parent_class;
};


GType natpad_app_get_type();

NatpadApp *natpad_app_new();

G_END_DECLS


#endif /* NATPAD_APP_H_ */
