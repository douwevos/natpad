/*
   File:    jagjrelink.c
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

#include "jagjrelink.h"
#include "jgijreentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJreLink"
#include <logging/catlog.h>

struct _JagJreLinkPrivate {
	CatStringWo *a_jre_name;
	CatAtomicReference *value_ref;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagJreLink, jag_jre_link, JAG_TYPE_LINK,
		G_ADD_PRIVATE(JagJreLink)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_link_set_value(JagLink *link, GObject *value);
static GObject *l_link_get_value_ref(JagLink *link);

static void jag_jre_link_class_init(JagJreLinkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagLinkClass *link_class = JAG_LINK_CLASS(clazz);
	link_class->setValue = l_link_set_value;
	link_class->getValueRef = l_link_get_value_ref;
}

static void jag_jre_link_init(JagJreLink *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagJreLink *instance = JAG_JRE_LINK(object);
	JagJreLinkPrivate *priv = jag_jre_link_get_instance_private(instance);
	cat_unref_ptr(priv->value_ref);
	cat_unref_ptr(priv->a_jre_name);
	G_OBJECT_CLASS(jag_jre_link_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_jre_link_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagJreLink *jag_jre_link_new(CatStringWo *a_jre_name) {
	JagJreLink *result = g_object_new(JAG_TYPE_JRE_LINK, NULL);
	cat_ref_anounce(result);
	JagJreLinkPrivate *priv = jag_jre_link_get_instance_private(result);
	priv->a_jre_name = cat_ref_ptr(a_jre_name);
	priv->value_ref = cat_atomic_reference_new();
	jag_link_construct((JagLink *) result);
	return result;
}

void jag_jre_link_set_entry(JagJreLink *link, struct _JgiJreEntry *entry) {
	JagJreLinkPrivate *priv = jag_jre_link_get_instance_private(link);
	cat_atomic_reference_set(priv->value_ref, (GObject *) entry);
}

struct _JgiJreEntry *jag_jre_link_get_entry_ref(JagJreLink *link) {
	JagJreLinkPrivate *priv = jag_jre_link_get_instance_private(link);
	return (JgiJreEntry *) cat_atomic_reference_get(priv->value_ref);
}



static void l_link_set_value(JagLink *link, GObject *value) {
	jag_jre_link_set_entry((JagJreLink *) link, (JgiJreEntry *) value);
}

static GObject *l_link_get_value_ref(JagLink *link) {
	return (GObject *) jag_jre_link_get_entry_ref((JagJreLink *) link);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagJreLink *instance = JAG_JRE_LINK(self);
	JagJreLinkPrivate *priv = jag_jre_link_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p, name=%o]", iname, self, priv->a_jre_name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






