/*
   File:    grorunmessage.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 15, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "grorunmessage.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunMessage"
#include <logging/catlog.h>

struct _GroRunMessagePrivate {
	GroRunMessage *parent_message;
	CatStringWo *text;
	GroRunLocation *location;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunMessage, grorun_message, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunMessage)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_message_class_init(GroRunMessageClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_message_init(GroRunMessage *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunMessage *instance = GRORUN_MESSAGE(object);
	GroRunMessagePrivate *priv = grorun_message_get_instance_private(instance);
	cat_unref_ptr(priv->parent_message);
	cat_unref_ptr(priv->text);
	cat_unref_ptr(priv->location);
	G_OBJECT_CLASS(grorun_message_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_message_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunMessage *grorun_message_new(GroRunMessage *parent_message, CatStringWo *text, GroRunLocation *location) {
	GroRunMessage *result = g_object_new(GRORUN_TYPE_MESSAGE, NULL);
	cat_ref_anounce(result);
	GroRunMessagePrivate *priv = grorun_message_get_instance_private(result);
	priv->parent_message = cat_ref_ptr(parent_message);
	priv->text = cat_ref_ptr(text);
	priv->location = cat_ref_ptr(location);
	return result;
}

GroRunMessage *grorun_message_get_parent_message(GroRunMessage *message) {
	GroRunMessagePrivate *priv = grorun_message_get_instance_private(message);
	return priv->parent_message;
}

CatStringWo *grorun_message_get_text(GroRunMessage *message) {
	GroRunMessagePrivate *priv = grorun_message_get_instance_private(message);
	return priv->text;
}

GroRunLocation *grorun_message_get_location(GroRunMessage *message) {
	GroRunMessagePrivate *priv = grorun_message_get_instance_private(message);
	return priv->location;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunMessage *instance = GRORUN_MESSAGE(self);
	GroRunMessagePrivate *priv = grorun_message_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p text=%O, location=%O]", iname, self, priv->text, priv->location);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
