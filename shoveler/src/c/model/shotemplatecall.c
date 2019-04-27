/*
   File:    shotemplatecall.c
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

#include "shotemplatecall.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoTemplateCall"
#include <logging/catlog.h>

struct _ShoTemplateCallPrivate {
	CatStringWo *template_name;
	ShoListValue *argument_list;
};

G_DEFINE_TYPE_WITH_PRIVATE(ShoTemplateCall, sho_template_call, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_template_call_class_init(ShoTemplateCallClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_template_call_init(ShoTemplateCall *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoTemplateCall *instance = SHO_TEMPLATE_CALL(object);
	ShoTemplateCallPrivate *priv = sho_template_call_get_instance_private(instance);
	cat_unref_ptr(priv->template_name);
	cat_unref_ptr(priv->argument_list);
	G_OBJECT_CLASS(sho_template_call_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_template_call_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoTemplateCall *sho_template_call_new(CatStringWo *template_name) {
	ShoTemplateCall *result = g_object_new(SHO_TYPE_TEMPLATE_CALL, NULL);
	cat_ref_anounce(result);
	ShoTemplateCallPrivate *priv = sho_template_call_get_instance_private(result);
	priv->template_name = cat_ref_ptr(template_name);
	priv->argument_list = NULL;
	return result;
}

ShoTemplateCall *sho_template_call_deep_copy(ShoTemplateCall *source) {
	ShoTemplateCall *result = g_object_new(SHO_TYPE_TEMPLATE_CALL, NULL);
	cat_ref_anounce(result);
	ShoTemplateCallPrivate *priv = sho_template_call_get_instance_private(result);
	ShoTemplateCallPrivate *spriv = sho_template_call_get_instance_private(source);
	priv->template_name = cat_ref_ptr(spriv->template_name);
	priv->argument_list = NULL;
	if (spriv->argument_list!=NULL) {
		priv->argument_list = (ShoListValue *) sho_ivalue_deep_copy((ShoIValue *) spriv->argument_list);
	}
	return result;
}

void sho_template_call_set_argument_list(ShoTemplateCall *template_call, ShoListValue *argument_list) {
	ShoTemplateCallPrivate *priv = sho_template_call_get_instance_private(template_call);
	cat_ref_swap(priv->argument_list, argument_list);
}
