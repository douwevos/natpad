/*
   File:    jagptoken.c
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

#include "jagptoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPToken"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPToken, jagp_token, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_token_class_init(JagPTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_token_init(JagPToken *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPToken *instance = JAGP_TOKEN(object);
//	JagPTokenPrivate *priv = jagp_token_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_token_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_token_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPToken *jagp_token_new() {
	JagPToken *result = g_object_new(JAGP_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

JagPName *jagp_token_name(JagPToken *token) {
	JagPTokenClass *tclass = JAGP_TOKEN_GET_CLASS(token);
	return tclass->name(token);
}

JagPComment *jagp_token_comment(JagPToken *token, JagPCommentStyle comment_style) {
	return jagp_comment_new();	// TODO
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPToken *instance = JAGP_TOKEN(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p, kind=%d, value=%O]", iname, self, instance->kind, instance->value);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
