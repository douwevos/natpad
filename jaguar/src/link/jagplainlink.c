/*
   File:    jagplainlink.c
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


#include "jagplainlink.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPlainLink"
#include <logging/catlog.h>

struct _JagPlainLinkPrivate {
	GObject *value;
};

G_DEFINE_TYPE_WITH_CODE(JagPlainLink, jag_plain_link, JAG_TYPE_LINK,
		G_ADD_PRIVATE(JagPlainLink)
);


static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_link_set_value(JagLink *link, GObject *value);
static GObject *l_link_get_value_ref(JagLink *link);

static void jag_plain_link_class_init(JagPlainLinkClass *clazz) {

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagLinkClass *link_class = JAG_LINK_CLASS(clazz);
	link_class->setValue = l_link_set_value;
	link_class->getValueRef = l_link_get_value_ref;
}

static void jag_plain_link_init(JagPlainLink *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPlainLink *instance = JAG_PLAIN_LINK(object);
	JagPlainLinkPrivate *priv = jag_plain_link_get_instance_private(instance);
	cat_unref_ptr(priv->value);
	G_OBJECT_CLASS(jag_plain_link_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_plain_link_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPlainLink *jag_plain_link_new() {
	JagPlainLink *result = g_object_new(JAG_TYPE_PLAIN_LINK, NULL);
	cat_ref_anounce(result);
	JagPlainLinkPrivate *priv = jag_plain_link_get_instance_private(result);
	priv->value = NULL;
	jag_link_construct((JagLink *) result);
	return result;
}


static void l_link_set_value(JagLink *link, GObject *value) {
	JagPlainLinkPrivate *priv = jag_plain_link_get_instance_private(JAG_PLAIN_LINK(link));
	cat_ref_swap(priv->value, value);
}

static GObject *l_link_get_value_ref(JagLink *link) {
	JagPlainLinkPrivate *priv = jag_plain_link_get_instance_private(JAG_PLAIN_LINK(link));
	return cat_ref_ptr(priv->value);
}

