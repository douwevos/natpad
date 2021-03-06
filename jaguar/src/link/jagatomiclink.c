/*
   File:    jagatomiclink.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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


#include "jagatomiclink.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAtomicLink"
#include <logging/catlog.h>

struct _JagAtomicLinkPrivate {
	CatAtomicReference *value_ref;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAtomicLink, jag_atomic_link, JAG_TYPE_LINK,
		G_ADD_PRIVATE(JagAtomicLink)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_link_set_value(JagLink *link, GObject *value);
static GObject *l_link_get_value_ref(JagLink *link);

static void jag_atomic_link_class_init(JagAtomicLinkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagLinkClass *link_class = JAG_LINK_CLASS(clazz);
	link_class->setValue = l_link_set_value;
	link_class->getValueRef = l_link_get_value_ref;
}

static void jag_atomic_link_init(JagAtomicLink *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAtomicLink *instance = JAG_ATOMIC_LINK(object);
	JagAtomicLinkPrivate *priv = jag_atomic_link_get_instance_private(instance);
	cat_unref_ptr(priv->value_ref);
	G_OBJECT_CLASS(jag_atomic_link_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_atomic_link_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagAtomicLink *jag_atomic_link_new() {
	JagAtomicLink *result = g_object_new(JAG_TYPE_ATOMIC_LINK, NULL);
	cat_ref_anounce(result);
	JagAtomicLinkPrivate *priv = jag_atomic_link_get_instance_private(result);
	priv->value_ref = cat_atomic_reference_new();
	jag_link_construct((JagLink *) result);
	return result;
}


static void l_link_set_value(JagLink *link, GObject *value) {
	JagAtomicLinkPrivate *priv = jag_atomic_link_get_instance_private((JagAtomicLink *) link);
	cat_atomic_reference_set(priv->value_ref, value);
}

static GObject *l_link_get_value_ref(JagLink *link) {
	JagAtomicLinkPrivate *priv = jag_atomic_link_get_instance_private((JagAtomicLink *) link);
	return cat_atomic_reference_get(priv->value_ref);
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagAtomicLink *instance = JAG_ATOMIC_LINK(self);
	JagAtomicLinkPrivate *priv = jag_atomic_link_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p], value=%o", iname, self, cat_atomic_reference_get(priv->value_ref));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







