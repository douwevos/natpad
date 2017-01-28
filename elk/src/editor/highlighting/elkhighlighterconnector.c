/*
   File:    elkhighlighterconnector.c
   Project: elk
   Author:  Douwe Vos
   Date:    Feb 9, 2012
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

#include "elkhighlighterconnector.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkHighlighterConnector"
#include <logging/catlog.h>

struct _ElkHighlighterConnectorPrivate {
	CatStringWo *a_markup_hook_key;
	int usage_count;
};

G_DEFINE_TYPE (ElkHighlighterConnector, elk_highlighter_connector, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void elk_highlighter_connector_class_init(ElkHighlighterConnectorClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkHighlighterConnectorPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void elk_highlighter_connector_init(ElkHighlighterConnector *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkHighlighterConnector *instance = ELK_HIGHLIGHTER_CONNECTOR(object);
	ElkHighlighterConnectorPrivate *priv = ELK_HIGHLIGHTER_CONNECTOR_GET_PRIVATE(instance);
	cat_unref_ptr(priv->a_markup_hook_key);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void elk_highlighter_connector_construct(ElkHighlighterConnector *connector, CatStringWo *a_markup_hook_key) {
	ElkHighlighterConnectorPrivate *priv = ELK_HIGHLIGHTER_CONNECTOR_GET_PRIVATE(connector);
	priv->a_markup_hook_key = cat_ref_ptr(a_markup_hook_key);
	priv->usage_count = 1;
}


CatStringWo *elk_highlighter_connector_get_markup_hook_key(ElkHighlighterConnector *connector) {
	ElkHighlighterConnectorPrivate *priv = ELK_HIGHLIGHTER_CONNECTOR_GET_PRIVATE(connector);
	return priv->a_markup_hook_key;
}


void elk_highlighter_connector_increase_usage(ElkHighlighterConnector *connector) {
	ElkHighlighterConnectorPrivate *priv = ELK_HIGHLIGHTER_CONNECTOR_GET_PRIVATE(connector);
	priv->usage_count++;
}

gboolean elk_highlighter_connector_decrease_usage(ElkHighlighterConnector *connector) {
	ElkHighlighterConnectorPrivate *priv = ELK_HIGHLIGHTER_CONNECTOR_GET_PRIVATE(connector);
	priv->usage_count--;
	return priv->usage_count==0;
}
