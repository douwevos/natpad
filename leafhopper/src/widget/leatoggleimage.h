/*
   File:    leatoggleimage.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jun 21, 2019
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

#ifndef WIDGET_LEATOGGLEIMAGE_H_
#define WIDGET_LEATOGGLEIMAGE_H_

#include "../action/leaaction.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LEA_TYPE_TOGGLE_IMAGE              (lea_toggle_image_get_type())
#define LEA_TOGGLE_IMAGE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_toggle_image_get_type(), LeaToggleImage))
#define LEA_TOGGLE_IMAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_TOGGLE_IMAGE, LeaToggleImageClass))
#define LEA_IS_TOGGLE_IMAGE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_TOGGLE_IMAGE))
#define LEA_IS_TOGGLE_IMAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_TOGGLE_IMAGE))
#define LEA_TOGGLE_IMAGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_TOGGLE_IMAGE, LeaToggleImageClass))

struct _LeaToggleImage {
	GtkWidget parent;
};

struct _LeaToggleImageClass {
	GtkWidgetClass parent_class;
};

typedef struct _LeaToggleImage               LeaToggleImage;
typedef struct _LeaToggleImagePrivate        LeaToggleImagePrivate;
typedef struct _LeaToggleImageClass          LeaToggleImageClass;


GType lea_toggle_image_get_type();

LeaToggleImage *lea_toggle_image_new(LeaAction *action, const char *image_file_name);

G_END_DECLS

#endif /* WIDGET_LEATOGGLEIMAGE_H_ */
