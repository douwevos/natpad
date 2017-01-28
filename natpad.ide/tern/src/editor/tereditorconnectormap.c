/*
   File:    tereditorconnectormap.c
   Project: tern
   Author:  Douwe Vos
   Date:    Dec 28, 2016
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

#include "tereditorconnectormap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerEditorConnectorMap"
#include <logging/catlog.h>

struct _TerEditorConnectorMapPrivate {
	TerGrammarMap *grammar_map;
	CatHashMapWo *map;				/* <SlotKey, DraEditorConnector> */
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerEditorConnectorMap, ter_editor_connector_map, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TerEditorConnectorMap)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_editor_connector_map_class_init(TerEditorConnectorMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_editor_connector_map_init(TerEditorConnectorMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerEditorConnectorMap *instance = TER_EDITOR_CONNECTOR_MAP(object);
	TerEditorConnectorMapPrivate *priv = ter_editor_connector_map_get_instance_private(instance);
	cat_unref_ptr(priv->grammar_map);
	cat_unref_ptr(priv->map);
	G_OBJECT_CLASS(ter_editor_connector_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_editor_connector_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerEditorConnectorMap *ter_editor_connector_map_new(TerGrammarMap *grammar_map) {
	TerEditorConnectorMap *result = g_object_new(TER_TYPE_EDITOR_CONNECTOR_MAP, NULL);
	cat_ref_anounce(result);
	TerEditorConnectorMapPrivate *priv = ter_editor_connector_map_get_instance_private(result);
	priv->grammar_map = cat_ref_ptr(grammar_map);
	priv->map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	return result;
}



void ter_editor_connector_map_set_names(TerEditorConnectorMap *connector_map, CatArrayWo *class_names) {
	TerEditorConnectorMapPrivate *priv = ter_editor_connector_map_get_instance_private(connector_map);
	CatIIterator *iter = cat_array_wo_iterator(class_names);
	CatHashMapWo *new_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	while(cat_iiterator_has_next(iter)) {
		CatStringWo *a_class_name = (CatStringWo *) cat_iiterator_next(iter);
		cat_log_debug("a_class_name=%o", a_class_name);
		TerEditorConnector *connector = (TerEditorConnector *) cat_hash_map_wo_get(priv->map, a_class_name);
		if (connector==NULL) {
			connector = ter_editor_connector_new(a_class_name, priv->grammar_map);
		} else {
			cat_ref_ptr(connector);
		}
		cat_hash_map_wo_put(new_map, (GObject *) a_class_name, (GObject *) connector);
		cat_unref_ptr(connector);
	}
	cat_unref_ptr(iter);
	cat_log_debug("new_map=%o", new_map);
	cat_ref_swap(priv->map, new_map);
	cat_unref_ptr(new_map);
}

TerEditorConnector *ter_editor_connector_map_get_connector(TerEditorConnectorMap *connector_map, CatStringWo *clazz_name) {
	TerEditorConnectorMapPrivate *priv = ter_editor_connector_map_get_instance_private(connector_map);
	cat_log_debug("clazz_name=%o", clazz_name);
	TerEditorConnector *hmap = (TerEditorConnector *) cat_hash_map_wo_get(priv->map, clazz_name);
	return hmap;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TerEditorConnectorMap *instance = TER_EDITOR_CONNECTOR_MAP(self);
	TerEditorConnectorMapPrivate *priv = ter_editor_connector_map_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
