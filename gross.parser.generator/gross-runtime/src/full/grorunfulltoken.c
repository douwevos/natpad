/*
   File:    grorunfulltoken.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 12, 2016
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

#include "grorunfulltoken.h"
#include "../grorunitoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunFullToken"
#include <logging/catlog.h>

struct _GroRunFullTokenPrivate {
	GroRunLeftRight indexes;
	GroRunSymbol *symbol;
	GObject *value;
	GroRunLocation *location;
	CatStringWo *symbol_description;
	int user_index;
};

static void l_token_iface_init(GroRunITokenInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunFullToken, grorun_full_token, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunFullToken)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ITOKEN, l_token_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_full_token_class_init(GroRunFullTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_full_token_init(GroRunFullToken *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunFullToken *instance = GRORUN_FULL_TOKEN(object);
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(instance);
	cat_unref_ptr(priv->location);
	cat_unref_ptr(priv->symbol);
	cat_unref_ptr(priv->symbol_description);
	cat_unref_ptr(priv->value);
	G_OBJECT_CLASS(grorun_full_token_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_full_token_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunFullToken *grorun_full_token_new(GroRunLeftRight indexes, GroRunSymbol *symbol, GObject *value, GroRunLocation *location, CatStringWo *symbol_description) {
	GroRunFullToken *result = g_object_new(GRORUN_TYPE_FULL_TOKEN, NULL);
	cat_ref_anounce(result);
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(result);
	priv->indexes = indexes;
	priv->symbol = cat_ref_ptr(symbol);
	priv->value = cat_ref_ptr(value);
	priv->location = cat_ref_ptr(location);
	priv->symbol_description = cat_ref_ptr(symbol_description);
	return result;
}


GroRunLocation *grorun_full_token_get_location(GroRunFullToken *token) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(token);
	return priv->location;
}

void grorun_full_token_set_user_index(GroRunFullToken *token, int index) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(token);
	priv->user_index = index;
}

int grorun_full_token_get_user_index(GroRunFullToken *token) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(token);
	return priv->user_index;
}


/********************* start GroRunIToken implementation *********************/

static GroRunSymbol *l_token_get_symbol(GroRunIToken *self) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(GRORUN_FULL_TOKEN(self));
	return priv->symbol;
}

static GObject *l_token_get_value(GroRunIToken *self) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(GRORUN_FULL_TOKEN(self));
	return priv->value;
}

static GroRunLeftRight l_token_get_indexes(GroRunIToken *self) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(GRORUN_FULL_TOKEN(self));
	return priv->indexes;
}

static GroRunLocation *l_token_get_location(GroRunIToken *self) {
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(GRORUN_FULL_TOKEN(self));
	return priv->location;
}

static void l_token_iface_init(GroRunITokenInterface *iface) {
	iface->getSymbol = l_token_get_symbol;
	iface->getValue = l_token_get_value;
	iface->getIndexes = l_token_get_indexes;
	iface->getLocation = l_token_get_location;
}

/********************* end GroRunIToken implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunFullToken *instance = GRORUN_FULL_TOKEN(self);
	GroRunFullTokenPrivate *priv = grorun_full_token_get_instance_private(instance);

	cat_string_wo_format(append_to, "FullToken[idx=%d..%d, sym=%O, location=%O, value=%O]", priv->indexes.left, priv->indexes.right, priv->symbol==NULL ? NULL : grorun_symbol_get_name(priv->symbol), priv->location, priv->value);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
