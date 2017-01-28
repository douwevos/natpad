/*
   File:    chaprintpage.c
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

#include "chaprintpage.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPrintPage"
#include <logging/catlog.h>

struct _ChaPrintPagePrivate {
	CatArrayWo *lines;
	double lines_view_height;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPrintPage, cha_print_page, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaPrintPage)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_print_page_class_init(ChaPrintPageClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_print_page_init(ChaPrintPage *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPrintPage *instance = CHA_PRINT_PAGE(object);
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(instance);
	cat_unref_ptr(priv->lines);
	G_OBJECT_CLASS(cha_print_page_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_print_page_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPrintPage *cha_print_page_new() {
	ChaPrintPage *result = g_object_new(CHA_TYPE_PRINT_PAGE, NULL);
	cat_ref_anounce(result);
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(result);
	priv->lines = cat_array_wo_new();
	priv->lines_view_height = 0;
	return result;
}

void cha_print_page_append_line(ChaPrintPage *print_page, ChaPrintLine *print_line) {
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(print_page);
	cat_array_wo_append(priv->lines, (GObject *) print_line);
	priv->lines_view_height += cha_print_line_get_view_height(print_line);
}

double cha_print_page_get_lines_view_height(ChaPrintPage *print_page) {
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(print_page);
	return priv->lines_view_height;
}

int cha_print_page_line_count(ChaPrintPage *print_page) {
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(print_page);
	return cat_array_wo_size(priv->lines);
}

ChaPrintLine *cha_print_page_line_at(ChaPrintPage *print_page, int line_idx) {
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(print_page);
	return (ChaPrintLine *) cat_array_wo_get(priv->lines, line_idx);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPrintPage *instance = CHA_PRINT_PAGE(self);
	ChaPrintPagePrivate *priv = cha_print_page_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p lines_view_height=%f]", iname, self, priv->lines_view_height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
