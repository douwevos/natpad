/*
   File:    shoreference.c
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



#include "shoreference.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoReference"
#include <logging/catlog.h>

struct _ShoReferencePrivate {
	CatStringWo *ref_id;
	ShoIValue *value;
};

G_DEFINE_TYPE (ShoReference, sho_reference, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void sho_reference_class_init(ShoReferenceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ShoReferencePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void sho_reference_init(ShoReference *instance) {
	ShoReferencePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, SHO_TYPE_REFERENCE, ShoReferencePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ShoReference *instance = SHO_REFERENCE(object);
	ShoReferencePrivate *priv = instance->priv;
	cat_unref_ptr(priv->ref_id);
	cat_unref_ptr(priv->value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ShoReference *sho_reference_new(CatStringWo *ref_id) {
	ShoReference *result = g_object_new(SHO_TYPE_REFERENCE, NULL);
	cat_ref_anounce(result);
	ShoReferencePrivate *priv = result->priv;
	priv->ref_id = cat_ref_ptr(ref_id);
	priv->value = NULL;
	return result;
}


ShoReference *sho_reference_deep_copy(ShoReference *source) {
	ShoReference *result = g_object_new(SHO_TYPE_REFERENCE, NULL);
	cat_ref_anounce(result);
	ShoReferencePrivate *priv = result->priv;
	ShoReferencePrivate *spriv = SHO_REFERENCE_GET_PRIVATE(source);

	priv->ref_id = cat_ref_ptr(spriv->ref_id);
	priv->value = spriv->value==NULL ? NULL : sho_ivalue_deep_copy(spriv->value);
	return result;
}


CatStringWo *sho_reference_get_id(ShoReference *reference) {
	ShoReferencePrivate *priv = SHO_REFERENCE_GET_PRIVATE(reference);
	return priv->ref_id;
}


void sho_reference_set_value(ShoReference *reference, ShoIValue *value) {
	ShoReferencePrivate *priv = SHO_REFERENCE_GET_PRIVATE(reference);
	cat_ref_swap(priv->value, value);
}

ShoIValue *sho_reference_get_value(ShoReference *reference) {
	ShoReferencePrivate *priv = SHO_REFERENCE_GET_PRIVATE(reference);
	return priv->value;
}
