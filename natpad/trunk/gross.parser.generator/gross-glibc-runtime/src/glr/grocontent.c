
/*
   File:    grocontent.c
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
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

#include "grocontent.h"

struct _GroContentPrivate {
	GroContent *linked;
	GObject *value;
	long long begin_row;
	int begin_column;
	long long end_row;
	int end_column;
};


G_DEFINE_TYPE_WITH_CODE(GroContent, gro_content, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroContent)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_content_class_init(GroContentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_content_init(GroContent *instance) {
}

static void l_dispose(GObject *object) {
	gro_log("dispose:%p", object);
//	GroContent *instance = GRO_CONTENT(object);
//	GroContentPrivate *priv = gro_content_get_instance_private(instance);
	G_OBJECT_CLASS(gro_content_parent_class)->dispose(object);
	gro_log("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	gro_log("finalize:%p", object);
	G_OBJECT_CLASS(gro_content_parent_class)->finalize(object);
	gro_log("finalized:%p", object);
}


GroContent *gro_content_new(GroContent *linked, GObject *value, long long begin_row, int begin_column, long long end_row, int end_column) {
	GroContent *result = g_object_new(GRO_TYPE_CONTENT, NULL);
	GroContentPrivate *priv = gro_content_get_instance_private(result);
	priv->linked = gro_ref_ptr(linked);
	priv->value = gro_ref_ptr(value);
	priv->begin_row = begin_row;
	priv->begin_column = begin_column;
	priv->end_row = end_row;
	priv->end_column = end_column;
	return result;
}


GroContent *gro_content_get_linked(GroContent *content) {
	if (content==NULL) {
		return NULL;
	}
	GroContentPrivate *priv = gro_content_get_instance_private(content);
	return priv->linked;
}

GroContent *gro_content_get_linked_at(GroContent *content, int skip) {
	while(content!=NULL && skip>0) {
		GroContentPrivate *priv = gro_content_get_instance_private(content);
		content = priv->linked;
		skip--;

	}
	return content;
}

void gro_content_get_left(GroContent *content, long long *left_row, int *left_column) {
	GroContentPrivate *priv = gro_content_get_instance_private(content);
	*left_row = priv->begin_row;
	*left_column = priv->begin_column;
}

void gro_content_get_right(GroContent *content, long long *right_row, int *right_column) {
	GroContentPrivate *priv = gro_content_get_instance_private(content);
	*right_row = priv->end_row;
	*right_column = priv->end_column;
}

GObject *gro_content_get_value(GroContent *content) {
	GroContentPrivate *priv = gro_content_get_instance_private(content);
	return priv->value;
}
