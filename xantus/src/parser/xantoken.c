/*
   File:    xantoken.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
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

#include "xantoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "XanToken"
#include <logging/catlog.h>

struct _XanTokenPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanToken, xan_token, G_TYPE_OBJECT, // @suppress("Unused static function")
		G_ADD_PRIVATE(XanToken)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_token_class_init(XanTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void xan_token_init(XanToken *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	XanToken *instance = XAN_TOKEN(object);
//	XanTokenPrivate *priv = xan_token_get_instance_private(instance);
	G_OBJECT_CLASS(xan_token_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_token_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanToken *xan_token_new() {
	XanToken *result = g_object_new(XAN_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
//	XanTokenPrivate *priv = xan_token_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	XanToken *instance = XAN_TOKEN(self);
//	XanTokenPrivate *priv = xan_token_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
