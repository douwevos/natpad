/*
   File:    turlayout.c
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 12, 2017
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

#include "turlayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "TurLayout"
#include <logging/catlog.h>

struct _TurLayoutPrivate {
	TurINode *root_node;
	TurLine *root_line;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TurLayout, tur_layout, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TurLayout)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void tur_layout_class_init(TurLayoutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void tur_layout_init(TurLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TurLayout *instance = TUR_LAYOUT(object);
//	TurLayoutPrivate *priv = tur_layout_get_instance_private(instance);
	G_OBJECT_CLASS(tur_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(tur_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TurLayout *tur_layout_new(TurINode *root_node) {
	TurLayout *result = g_object_new(TUR_TYPE_LAYOUT, NULL);
	cat_ref_anounce(result);
	TurLayoutPrivate *priv = tur_layout_get_instance_private(result);
	priv->root_node = cat_ref_ptr(root_node);
	priv->root_line = tur_line_new(priv->root_node);
	return result;
}

TurLine * tur_layout_get_root_line(TurLayout *layout) {
	TurLayoutPrivate *priv = tur_layout_get_instance_private(layout);
	return priv->root_line;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TurLayout *instance = TUR_LAYOUT(self);
	TurLayoutPrivate *priv = tur_layout_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/