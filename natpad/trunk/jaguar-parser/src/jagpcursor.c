/*
   File:    jagpcursor.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Juni 28, 2017
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

#include "jagpcursor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPCursor"
#include <logging/catlog.h>

struct _JagPCursorPrivate {
	long long row;
	int column;
};


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPCursor, jagp_cursor, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPCursor)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_cursor_class_init(JagPCursorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_cursor_init(JagPCursor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPCursor *instance = JAGP_CURSOR(object);
	G_OBJECT_CLASS(jagp_cursor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_cursor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPCursor *jagp_cursor_new(long long row, int column) {
	JagPCursor *result = g_object_new(JAGP_TYPE_CURSOR, NULL);
	cat_ref_anounce(result);
	JagPCursorPrivate *priv = jagp_cursor_get_instance_private(result);
	priv->row = row;
	priv->column = column;
	return result;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPCursor *instance = JAGP_CURSOR(self);
	JagPCursorPrivate *priv = jagp_cursor_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p, row=%d, col=%d]", iname, self, priv->row, priv->column);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
