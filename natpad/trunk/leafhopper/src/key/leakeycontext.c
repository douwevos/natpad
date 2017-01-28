/*
   File:    leakeycontext.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#include "leakeycontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKeyContext"
#include <logging/catlog.h>

struct _LeaKeyContextPrivate {
	CatStringWo *a_context_name;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaKeyContext, lea_key_context, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_key_context_class_init(LeaKeyContextClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaKeyContextPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_key_context_init(LeaKeyContext *instance) {
	LeaKeyContextPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_KEY_CONTEXT, LeaKeyContextPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKeyContext *instance = LEA_KEY_CONTEXT(object);
	LeaKeyContextPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_context_name);
	G_OBJECT_CLASS(lea_key_context_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_key_context_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaKeyContext *lea_key_context_new(CatStringWo *ca_context_name) {
	LeaKeyContext *result = g_object_new(LEA_TYPE_KEY_CONTEXT, NULL);
	cat_ref_anounce(result);
	LeaKeyContextPrivate *priv = result->priv;
	priv->a_context_name = ca_context_name;
	return result;
}



CatStringWo *lea_key_context_get_name(LeaKeyContext *key_context) {
	return LEA_KEY_CONTEXT_GET_PRIVATE(key_context)->a_context_name;

}

int lea_key_context_hash(LeaKeyContext *key_context) {
	LeaKeyContextPrivate *priv = LEA_KEY_CONTEXT_GET_PRIVATE(key_context);
	return cat_string_wo_hash(priv->a_context_name);
}

gboolean lea_key_context_equal(LeaKeyContext *key_context_a, LeaKeyContext *key_context_b) {
	if (key_context_a==key_context_b) {
		return TRUE;
	}
	if ((key_context_a==NULL) || (key_context_b==NULL)) {
		return FALSE;
	}
	LeaKeyContextPrivate *priv_a = LEA_KEY_CONTEXT_GET_PRIVATE(key_context_a);
	LeaKeyContextPrivate *priv_b = LEA_KEY_CONTEXT_GET_PRIVATE(key_context_b);
	return cat_string_wo_equal(priv_a->a_context_name, priv_b->a_context_name);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaKeyContextPrivate *priv = LEA_KEY_CONTEXT_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%o]", iname, priv->a_context_name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/



