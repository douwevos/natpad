/*
   File:    grorunfulltokenfactory.c
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

#include "grorunfulltokenfactory.h"
#include "grorunfulltoken.h"
#include "../grorunitokenfactory.h"
#include "../grorunsymbol.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunFullTokenFactory"
#include <logging/catlog.h>

struct _GroRunFullTokenFactoryPrivate {
	void *dummy;
};

static void l_token_factory_iface_init(GroRunITokenFactoryInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunFullTokenFactory, grorun_full_token_factory, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunFullTokenFactory)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ITOKEN_FACTORY, l_token_factory_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_full_token_factory_class_init(GroRunFullTokenFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void grorun_full_token_factory_init(GroRunFullTokenFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroRunFullTokenFactory *instance = GRORUN_FULL_TOKEN_FACTORY(object);
//	GroRunFullTokenFactoryPrivate *priv = grorun_full_token_factory_get_instance_private(instance);
	G_OBJECT_CLASS(grorun_full_token_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_full_token_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunFullTokenFactory *grorun_full_token_factory_new() {
	GroRunFullTokenFactory *result = g_object_new(GRORUN_TYPE_FULL_TOKEN_FACTORY, NULL);
	cat_ref_anounce(result);
//	GroRunFullTokenFactoryPrivate *priv = grorun_full_token_factory_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}


/********************* start GroRunITokenFactory implementation *********************/

static GroRunIToken *l_create_token(GroRunITokenFactory *self, GroRunLeftRight indexes, GroRunSymbol *symbol, GObject *value, GroRunLocation *location, CatStringWo *symbol_description) {
	return (GroRunIToken *) grorun_full_token_new(indexes, symbol, value, location, symbol_description);
}


static void l_token_factory_iface_init(GroRunITokenFactoryInterface *iface) {
	iface->createToken = l_create_token;
}

/********************* end GroRunITokenFactory implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	GroRunFullTokenFactory *instance = GRORUN_FULL_TOKEN_FACTORY(self);
//	GroRunFullTokenFactoryPrivate *priv = grorun_full_token_factory_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
