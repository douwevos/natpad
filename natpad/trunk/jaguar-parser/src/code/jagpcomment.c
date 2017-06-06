/*
   File:    jagpcomment.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 14, 2017
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

#include "jagpcomment.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPComment"
#include <logging/catlog.h>

struct _JagPCommentPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPComment, jagp_comment, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPComment)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_comment_class_init(JagPCommentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_comment_init(JagPComment *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPComment *instance = JAGP_COMMENT(object);
//	JagPCommentPrivate *priv = jagp_comment_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_comment_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_comment_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPComment *jagp_comment_new() {
	JagPComment *result = g_object_new(JAGP_TYPE_COMMENT, NULL);
	cat_ref_anounce(result);
	JagPCommentPrivate *priv = jagp_comment_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

CatStringWo *jagp_comment_get_text(JagPComment *comment) {
	return NULL;
}

int jagp_comment_get_source_pos(JagPComment *comment, int index) {
	return 0;
}

JagPCommentStyle jagp_comment_get_style(JagPComment *comment) {
	return JAGP_COMMENT_STYLE_JAVADOC;
}

gboolean jagp_comment_is_deprecated(JagPComment *comment) {
	return FALSE;
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPComment *instance = JAGP_COMMENT(self);
	JagPCommentPrivate *priv = jagp_comment_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
