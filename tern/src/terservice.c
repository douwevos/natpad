/*
   File:    terservice.c
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 14, 2012
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

#include "terservice.h"
#include "editor/tereditorfactory.h"
#include "editor/tereditorconnectormap.h"
#include "grammar/tergrammarmap.h"
#include "grammar/tercolors.h"
#include "preferences/terprefsmapper.h"
#include "preferences/terprefsclazzwo.h"
#include "preferences/terpreferenceswo.h"
#include "preferences/ui/terprefspanelfactory.h"
#include <caterpillar.h>
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerService"
#include <logging/catlog.h>

struct _TerServicePrivate {
	WorService *wor_service;
	ElkService *elk_service;

	TerEditorConnectorMap *connector_map;
//	TerHighlighterMapContainer *map_container;
	TerGrammarMap *grammar_map;

	TerPrefsPanelFactory *prefs_panel_factory;
	TerPrefsMapper *prefs_mapper;

	TerPreferencesWo *a_ter_preferences;
	CatHashMapWo *a_edit_prefs_map;
};

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface);
static void l_cow_change_listener_iface_init(CowIChangeListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerService, ter_service, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(TerService)
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_HANDLER, l_resource_handler_iface_init);
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_cow_change_listener_iface_init);
})

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_service_class_init(TerServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_service_init(TerService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerService *instance = TER_SERVICE(object);
	TerServicePrivate *priv = ter_service_get_instance_private(instance);

	if (priv->grammar_map) {
		g_object_run_dispose(G_OBJECT(priv->grammar_map));
	}
	cat_unref_ptr(priv->grammar_map);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->elk_service);
	cat_unref_ptr(priv->prefs_panel_factory);
	cat_unref_ptr(priv->prefs_mapper);
	cat_unref_ptr(priv->a_ter_preferences);
	cat_unref_ptr(priv->a_edit_prefs_map);
	cat_unref_ptr(priv->connector_map);
	G_OBJECT_CLASS(ter_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static CatS l_s_txt_config = CAT_S_DEF("config");

static void l_attach_preferences(TerService *ter_service) {
	TerServicePrivate *priv = ter_service_get_instance_private(ter_service);
	ElkPreferencesService *prefs_service = elk_service_get_preferences_service(priv->elk_service);
	CowPanelModel *panel_model = elk_preferences_service_get_panel_model(prefs_service);

	VipPath *shared_path = elk_preferences_services_get_shared_config_map(prefs_service);
	VipPath *shared_config_path = vip_path_create_child(shared_path, CAT_S(l_s_txt_config));

	priv->prefs_panel_factory = ter_prefs_panel_factory_new(shared_config_path);
	cat_unref_ptr(shared_config_path);
	cow_panel_model_add_factory(panel_model, (CowIPanelFactory *) priv->prefs_panel_factory);


	priv->prefs_mapper = ter_prefs_mapper_new();
	elk_preferences_services_add_mapper(prefs_service, (ElkIPreferencesMapper *) priv->prefs_mapper);


	ElkPreferencesContainer *prefs_container = elk_preferences_service_get_container(prefs_service);
	cow_container_add_change_listener((CowContainer *) prefs_container, COW_ICHANGE_LISTENER(ter_service));
	cow_ichange_listener_config_changed(COW_ICHANGE_LISTENER(ter_service), (GObject *) elk_preferences_container_get(prefs_container));
}

TerService *ter_service_new(WorService *wor_service, ElkService *elk_service, VipService *vip_service) {
	TerService *result = g_object_new(TER_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	TerServicePrivate *priv = ter_service_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->elk_service = cat_ref_ptr(elk_service);
	priv->a_edit_prefs_map = NULL;
	priv->a_ter_preferences = NULL;
	ElkPreferencesService *prefs_service = elk_service_get_preferences_service(priv->elk_service);
	priv->grammar_map = ter_grammar_map_new(prefs_service, vip_service, priv->wor_service);
	priv->connector_map = ter_editor_connector_map_new(priv->grammar_map);
	l_attach_preferences(result);
	return result;
}


/********************* start ElkIResourceHandler implementation *********************/

static TerPrefsClazzWo *l_find_best_document_clazz(ElkIResourceHandler *self, CatStringWo *a_filename) {
	TerService *service = TER_SERVICE(self);
	TerServicePrivate *priv = ter_service_get_instance_private(service);

	ElkPreferencesService *prefs_service = elk_service_get_preferences_service(priv->elk_service);
	ElkPreferencesContainer *prefs_container = elk_preferences_service_get_container(prefs_service);
	ElkPreferencesWo *a_elk_prefs = elk_preferences_container_get(prefs_container);
	if (a_elk_prefs==NULL) {
		return NULL;
	}
	TerPreferencesWo *a_ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(a_elk_prefs, CAT_S(ter_s_textpad));
	if (a_ter_prefs==NULL) {
		return NULL;
	}

	TerPrefsClazzWo *result = NULL;
	CatHashMapWo *a_clazz_map = ter_preferences_wo_get_document_clazzes(a_ter_prefs);
	CatArrayWo *a_clazz_list = cat_hash_map_wo_enlist_values(a_clazz_map, NULL);
	CatIIterator *clazz_iter = cat_array_wo_iterator(a_clazz_list);
	cat_unref_ptr(a_clazz_list);
	cat_log_debug("a_filename=%o", a_filename);

	while(result==NULL && cat_iiterator_has_next(clazz_iter)) {
		TerPrefsClazzWo *a_ter_clazz = (TerPrefsClazzWo *) cat_iiterator_next(clazz_iter);
		CatArrayWo *a_pattern_list = ter_prefs_clazz_wo_get_pattern_list(a_ter_clazz);
		if (a_pattern_list) {
			CatIIterator *pat_iter = cat_array_wo_iterator(a_pattern_list);
			while(result==NULL && cat_iiterator_has_next(pat_iter)) {
				CatStringWo *pattern = (CatStringWo *) cat_iiterator_next(pat_iter);
				CatStringWo *cp = cat_string_wo_clone(pattern, CAT_CLONE_DEPTH_MAIN);
				CatRegexp *exp = cat_regexp_new(cp);
				cat_unref_ptr(cp);
				cat_log_debug("pattern=%o", pattern);
				if (cat_regexp_match(exp, a_filename)) {
					result = a_ter_clazz;
					cat_log_debug("result=%o", result);
				}
				cat_unref(exp);
			}
			cat_unref_ptr(pat_iter);
		}
	}
	cat_unref_ptr(clazz_iter);
	return result;
}

static void l_enlist_editor_factories(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node) {
	TerService *instance = TER_SERVICE(self);
	TerServicePrivate *priv = ter_service_get_instance_private(instance);
	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
	if (resource_content!=NULL) {
		VipNode *node = moo_resource_content_wo_get_viper_node(resource_content);
		VipIFile *file_to_open = node==NULL ? NULL : (VipIFile *) vip_node_get_content(node);
		if (file_to_open!=NULL) {
			CatStringWo *a_name = vip_iresource_get_name((VipIResource *) file_to_open);
			TerPrefsClazzWo *a_prefs_clazz = l_find_best_document_clazz(self, a_name);
			if (a_prefs_clazz) {
				CatStringWo *a_clazz_name = ter_prefs_clazz_wo_get_name(a_prefs_clazz);
				ElkDocumentIO *document_io = priv->elk_service->document_io;
				LeaIPanelOwner *panel_owner = (LeaIPanelOwner *) priv->elk_service->panel_owner;
				TerEditorFactory *factory = ter_editor_factory_new(panel_owner, document_io, priv->connector_map, priv->grammar_map, file_to_open, priv->a_edit_prefs_map, a_clazz_name);
				cat_log_debug("factory=%o", factory);
				cat_array_wo_append(e_enlist_to, (GObject *) factory);
				cat_unref_ptr(factory);
			}
		}
	}
}

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface) {
	iface->matchEditorFactories = l_enlist_editor_factories;
}

/********************* end ElkIResourceHandler implementation *********************/


/********************* start CowIChangeListener implementation *********************/


static void l_config_changed(CowIChangeListener *self, GObject *config) {
	ElkPreferencesWo *a_elk_prefs = (ElkPreferencesWo *) config;
	TerService *instance = TER_SERVICE(self);
	TerServicePrivate *priv = ter_service_get_instance_private(instance);

	TerPreferencesWo *a_ter_prefs = NULL;
	if (a_elk_prefs) {
		a_ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(a_elk_prefs, CAT_S(ter_s_textpad));
	}

	if (priv->a_ter_preferences == a_ter_prefs) {
		return;
	}

	CatHashMapWo *e_editor_prefs_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);

	if (a_ter_prefs!=NULL) {

		CatHashMapWo *a_ter_clazz_map = ter_preferences_wo_get_document_clazzes(a_ter_prefs);
		CatArrayWo *a_clazz_list = cat_hash_map_wo_enlist_values(a_ter_clazz_map, NULL);
		CatIIterator *clazz_iter = cat_array_wo_iterator(a_clazz_list);
		CatArrayWo *e_class_names = cat_array_wo_new();
		while(cat_iiterator_has_next(clazz_iter)) {
			TerPrefsClazzWo *a_ter_clazz = (TerPrefsClazzWo *) cat_iiterator_next(clazz_iter);
			cat_array_wo_append(e_class_names, (GObject *) ter_prefs_clazz_wo_get_name(a_ter_clazz));
		}
		ter_editor_connector_map_set_names(priv->connector_map, e_class_names);
		cat_unref_ptr(e_class_names);
		cat_unref_ptr(clazz_iter);
		cat_unref_ptr(a_clazz_list);
	}
	CatHashMapWo *a_editor_prefs_map = cat_hash_map_wo_anchor(e_editor_prefs_map, 0);
	cat_ref_swap(priv->a_edit_prefs_map, a_editor_prefs_map);
	cat_unref_ptr(a_editor_prefs_map);
}

static void l_cow_change_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}

/********************* end CowIChangeListener implementation *********************/
