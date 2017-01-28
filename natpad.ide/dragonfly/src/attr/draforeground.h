/*
   File:    draforeground.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 8, 2015
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

#ifndef ATTR_DRAFOREGROUND_H_
#define ATTR_DRAFOREGROUND_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_FOREGROUND              (dra_foreground_get_type())
#define DRA_FOREGROUND(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_foreground_get_type(), DraForeground))
#define DRA_FOREGROUND_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_FOREGROUND, DraForegroundClass))
#define DRA_IS_FOREGROUND(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_FOREGROUND))
#define DRA_IS_FOREGROUND_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_FOREGROUND))
#define DRA_FOREGROUND_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_FOREGROUND, DraForegroundClass))


typedef struct _DraForeground               DraForeground;
typedef struct _DraForegroundPrivate        DraForegroundPrivate;
typedef struct _DraForegroundClass          DraForegroundClass;


struct _DraForeground {
	GObject parent;
};

struct _DraForegroundClass {
	GObjectClass parent_class;
};


GType dra_foreground_get_type();

DraForeground *dra_foreground_new(int color_index);

int dra_foreground_get_color_index(DraForeground *foreground);

G_END_DECLS

#endif /* ATTR_DRAFOREGROUND_H_ */
