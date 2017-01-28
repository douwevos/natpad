/*
   File:    chaprintline.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 6, 2015
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

#include "chaprintline.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPrintLine"
#include <logging/catlog.h>

struct _ChaPrintLinePrivate {
	ChaLineLayout *line_layout;
	int sub_line_idx;
	int view_height;
	long long line_nr;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPrintLine, cha_print_line, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaPrintLine)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_print_line_class_init(ChaPrintLineClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_print_line_init(ChaPrintLine *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPrintLine *instance = CHA_PRINT_LINE(object);
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(instance);
	cat_unref_ptr(priv->line_layout);
	G_OBJECT_CLASS(cha_print_line_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_print_line_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPrintLine *cha_print_line_new(ChaLineLayout *line_layout, int sub_line_idx, int view_height, long long line_nr) {
	ChaPrintLine *result = g_object_new(CHA_TYPE_PRINT_LINE, NULL);
	cat_ref_anounce(result);
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(result);
	priv->line_layout = cat_ref_ptr(line_layout);
	priv->sub_line_idx = sub_line_idx;
	priv->view_height = view_height;
	priv->line_nr = line_nr;
	return result;
}

int cha_print_line_get_view_height(ChaPrintLine *print_line) {
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(print_line);
	return priv->view_height;
}

ChaLineLayout *cha_print_line_get_line_layout(ChaPrintLine *print_line) {
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(print_line);
	return priv->line_layout;
}

int cha_print_line_get_sub_line_index(ChaPrintLine *print_line) {
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(print_line);
	return priv->sub_line_idx;
}

long long cha_print_line_get_line_nr(ChaPrintLine *print_line) {
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(print_line);
	return priv->line_nr;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPrintLine *instance = CHA_PRINT_LINE(self);
	ChaPrintLinePrivate *priv = cha_print_line_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p view_height=%d]", iname, self, priv->view_height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

