/*
   File:    natpad-window.h
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

#ifndef NATPAD_WINDOW_H_
#define NATPAD_WINDOW_H_

#include <caterpillar.h>
#include <viper.h>
#include <elk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NATPAD_TYPE_WINDOW              (natpad_window_get_type())
#define NATPAD_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), natpad_window_get_type(), NatpadWindow))
#define NATPAD_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), NATPAD_TYPE_WINDOW, NatpadWindowClass))
#define NATPAD_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NATPAD_TYPE_WINDOW))
#define NATPAD_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), NATPAD_TYPE_WINDOW))
#define NATPAD_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), NATPAD_TYPE_WINDOW, NatpadWindowClass))


typedef struct _NatpadWindow               NatpadWindow;
typedef struct _NatpadWindowPrivate        NatpadWindowPrivate;
typedef struct _NatpadWindowClass          NatpadWindowClass;


struct _NatpadWindow {
	GtkApplicationWindow parent;
};

struct _NatpadWindowClass {
	GtkApplicationWindowClass parent_class;
};


GType natpad_window_get_type();

NatpadWindow *natpad_window_new(GApplication *application, ElkPreferencesService *preference_service, VipCdProvider *cd_provider);

void natpad_window_new_open_file_list(NatpadWindow *window, CatArrayWo *file_list);

G_END_DECLS

#endif /* NATPAD_WINDOW_H_ */
