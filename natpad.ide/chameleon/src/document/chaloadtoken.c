/*
   File:    chaloadtoken.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jun 22, 2015
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

#include "chaloadtoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLoadToken"
#include <logging/catlog.h>

struct _ChaLoadTokenPrivate {
	ChaLoadToken *follow_up_token;
	CatArrayWo *loaded_pages;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLoadToken, cha_load_token, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaLoadToken)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_load_token_class_init(ChaLoadTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_load_token_init(ChaLoadToken *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaLoadToken *instance = CHA_LOAD_TOKEN(object);
	ChaLoadTokenPrivate *priv = cha_load_token_get_instance_private(instance);
	cat_unref_ptr(priv->follow_up_token);
	cat_unref_ptr(priv->loaded_pages);
	G_OBJECT_CLASS(cha_load_token_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_load_token_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaLoadToken *cha_load_token_new() {
	ChaLoadToken *result = g_object_new(CHA_TYPE_LOAD_TOKEN, NULL);
	cat_ref_anounce(result);
	ChaLoadTokenPrivate *priv = cha_load_token_get_instance_private(result);
	priv->follow_up_token = NULL;
	priv->loaded_pages = NULL;
	return result;
}

void cha_load_token_set_loaded_pages(ChaLoadToken *token, CatArrayWo *loaded_pages, ChaLoadToken *follow_up_token) {
	ChaLoadTokenPrivate *priv = cha_load_token_get_instance_private(token);
	cat_ref_swap(priv->loaded_pages, loaded_pages);
	cat_ref_swap(priv->follow_up_token, follow_up_token);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLoadToken *instance = CHA_LOAD_TOKEN(self);
	ChaLoadTokenPrivate *priv = cha_load_token_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p loaded_pages=%o]", iname, self, priv->loaded_pages);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
