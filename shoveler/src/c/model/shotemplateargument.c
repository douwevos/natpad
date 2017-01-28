/*
   File:    shotemplateargument.c
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#include "shotemplateargument.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoTemplateArgument"
#include <logging/catlog.h>

struct _ShoTemplateArgumentPrivate {
	CatStringWo *id;
	ShoIValue *default_value;
};

G_DEFINE_TYPE (ShoTemplateArgument, sho_template_argument, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_template_argument_class_init(ShoTemplateArgumentClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ShoTemplateArgumentPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_template_argument_init(ShoTemplateArgument *instance) {
	ShoTemplateArgumentPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, SHO_TYPE_TEMPLATE_ARGUMENT, ShoTemplateArgumentPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoTemplateArgument *instance = SHO_TEMPLATE_ARGUMENT(object);
	ShoTemplateArgumentPrivate *priv = instance->priv;
	cat_unref_ptr(priv->id);
	cat_unref_ptr(priv->default_value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoTemplateArgument *sho_template_argument_new(CatStringWo *id) {
	ShoTemplateArgument *result = g_object_new(SHO_TYPE_TEMPLATE_ARGUMENT, NULL);
	cat_ref_anounce(result);
	ShoTemplateArgumentPrivate *priv = result->priv;
	priv->id = cat_ref_ptr(id);
	priv->default_value = NULL;
	return result;
}

ShoTemplateArgument *sho_template_argument_deep_copy(ShoTemplateArgument *source) {
	ShoTemplateArgument *result = g_object_new(SHO_TYPE_TEMPLATE_ARGUMENT, NULL);
	cat_ref_anounce(result);
	ShoTemplateArgumentPrivate *priv = result->priv;
	ShoTemplateArgumentPrivate *spriv = SHO_TEMPLATE_ARGUMENT_GET_PRIVATE(source);
	priv->id = cat_ref_ptr(spriv->id);
	priv->default_value = NULL;
	if (spriv->default_value) {
		priv->default_value = sho_ivalue_deep_copy(spriv->default_value);
	}
	return result;
}




void sho_template_argument_set_default(ShoTemplateArgument *argument, ShoIValue *default_value) {
	ShoTemplateArgumentPrivate *priv = SHO_TEMPLATE_ARGUMENT_GET_PRIVATE(argument);
	cat_ref_swap(priv->default_value, default_value);
}



