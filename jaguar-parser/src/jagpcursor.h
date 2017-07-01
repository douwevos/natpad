/*
   File:    jagpcursor.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef JAGPCURSOR_H_
#define JAGPCURSOR_H_

#include "code/jagpname.h"
#include "code/jagpcomment.h"
#include <caterpillar.h>
#include <grossruntime.h>

G_BEGIN_DECLS


#define JAGP_TYPE_CURSOR              (jagp_cursor_get_type())
#define JAGP_CURSOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_cursor_get_type(), JagPCursor))
#define JAGP_CURSOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_CURSOR, JagPCursorClass))
#define JAGP_IS_CURSOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_CURSOR))
#define JAGP_IS_CURSOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_CURSOR))
#define JAGP_CURSOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_CURSOR, JagPCursorClass))


typedef struct _JagPCursor               JagPCursor;
typedef struct _JagPCursorPrivate        JagPCursorPrivate;
typedef struct _JagPCursorClass          JagPCursorClass;

struct _JagPCursor {
	GObject parent;
};

struct _JagPCursorClass {
	GObjectClass parent_class;
};


GType jagp_cursor_get_type();

JagPCursor *jagp_cursor_new();


G_END_DECLS

#endif /* JAGPCURSOR_H_ */
