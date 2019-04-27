/*
   File:    shotemplate.c
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

#include "shotemplate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoTemplate"
#include <logging/catlog.h>

struct _ShoTemplatePrivate {
	CatStringWo *name;
	CatArrayWo *arguments;
};

G_DEFINE_TYPE_WITH_PRIVATE(ShoTemplate, sho_template, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_template_class_init(ShoTemplateClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_template_init(ShoTemplate *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoTemplate *instance = SHO_TEMPLATE(object);
	ShoTemplatePrivate *priv = sho_template_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->arguments);
	G_OBJECT_CLASS(sho_template_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(sho_template_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoTemplate *sho_template_new(CatStringWo *name) {
	ShoTemplate *result = g_object_new(SHO_TYPE_TEMPLATE, NULL);
	cat_ref_anounce(result);
	ShoTemplatePrivate *priv = sho_template_get_instance_private(result);
	priv->name = cat_ref_ptr(name);
	priv->arguments = cat_array_wo_new();
	return result;
}

ShoTemplate *sho_template_deep_copy(ShoTemplate *source) {
	ShoTemplate *result = g_object_new(SHO_TYPE_TEMPLATE, NULL);
	cat_ref_anounce(result);
	ShoTemplatePrivate *priv = sho_template_get_instance_private(result);
	ShoTemplatePrivate *spriv = sho_template_get_instance_private(source);
	priv->name = cat_ref_ptr(spriv->name);
	priv->arguments = cat_array_wo_new_size(cat_array_wo_size(spriv->arguments));

	if (cat_array_wo_size(spriv->arguments)>0) {
		CatIIterator *iter = cat_array_wo_iterator(spriv->arguments);
		while(cat_iiterator_has_next(iter)) {
			ShoTemplateArgument *arg = (ShoTemplateArgument *) cat_iiterator_next(iter);
			ShoTemplateArgument *new_arg = sho_template_argument_deep_copy(arg);
			cat_array_wo_append(priv->arguments, (GObject *) new_arg);
			cat_unref_ptr(new_arg);
		}
		cat_unref_ptr(iter);
	}
	return result;
}

CatStringWo *sho_template_get_name(ShoTemplate *stemplate) {
	ShoTemplatePrivate *priv = sho_template_get_instance_private(stemplate);
	return priv->name;
}

void sho_template_add_argument(ShoTemplate *stemplate, ShoTemplateArgument *arg) {
	ShoTemplatePrivate *priv = sho_template_get_instance_private(stemplate);
	cat_array_wo_append(priv->arguments, (GObject *) arg);
}
