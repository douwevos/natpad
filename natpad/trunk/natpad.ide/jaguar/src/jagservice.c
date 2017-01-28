/*
   File:    jagservice.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 10, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "jagservice.h"
#include "moose/module/jagmodulepropertieshandler.h"
#include "moose/jagmooseservice.h"
#include "moose/srcfile/jagsrcfilecontentwo.h"
#include "properties/module/jagmodulepanelfactory.h"
#include "editor/jageditorfactory.h"
#include "editor/jagclasseditorfactory.h"
#include "indexer/jagindexer.h"
#include "indexer/context/jagindexerjremap.h"
#include "indexer/jar/jagindexerjarmap.h"
#include "preferences/ui/jagprefspanelfactory.h"
#include "preferences/jagprefsmapper.h"
#include <worm.h>
#include <caterpillar.h>
#include <dragonfly.h>
#include <cow.h>
#include <leafhopper.h>
#include <elk.h>
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagService"
#include <logging/catlog.h>

struct _JagServicePrivate {
	WorService *wor_service;
	ElkService *elk_service;

	JagIndexer *indexer;
	JagModulePropertiesHandler *module_properties_handler;

	JagModulePanelFactory *module_panel_factory;

	JagPrefsPanelFactory *prefs_panel_factory;
	JagPrefsMapper *prefs_mapper;


	JagIndexerJarMap *jar_map;
	JagIndexerJreMap *jre_map;

//	JagModuleMap *module_map;

	MooService *moo_service;
	JagMooseService *jag_moose_service;

	JagEditorConnector *connector;

};

static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface);
static void l_change_listener_iface_init(CowIChangeListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagService, jag_service, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(ELK_TYPE_IRESOURCE_HANDLER, l_resource_handler_iface_init);
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_change_listener_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_service_class_init(JagServiceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagServicePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

}

static void jag_service_init(JagService *instance) {
	JagServicePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_SERVICE, JagServicePrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagService *instance = JAG_SERVICE(object);
	JagServicePrivate *priv = instance->priv;


	cat_unref_ptr(priv->jag_moose_service);


	if (priv->indexer) {
//		if (priv->module_map) {
//			jag_module_map_remove_listener(priv->module_map, JAG_IMODULE_MAP_LISTENER(priv->indexer));
//		}
		cat_unref_ptr(priv->indexer);
	}

	if (priv->prefs_panel_factory) {
		if (priv->elk_service) {
			ElkPreferencesService *prefs_service = elk_service_get_preferences_service(priv->elk_service);
			if (prefs_service) {
				CowPanelModel *panel_model = elk_preferences_service_get_panel_model(prefs_service);
				if (panel_model) {
					cow_panel_model_remove_factory(panel_model, (CowIPanelFactory *) priv->prefs_panel_factory);
				}
			}
		}
		cat_unref_ptr(priv->prefs_panel_factory);
	}
	cat_unref_ptr(priv->prefs_mapper);

	cat_unref_ptr(priv->connector);
	cat_unref_ptr(priv->jre_map);
	cat_unref_ptr(priv->jar_map);
	cat_unref_ptr(priv->elk_service);
	cat_unref_ptr(priv->module_panel_factory);
	cat_unref_ptr(priv->module_properties_handler);

//	if (priv->module_map) {
//		g_object_run_dispose(G_OBJECT(priv->module_map));
//	}
//	cat_unref_ptr(priv->module_map);
//	cat_unref_ptr(priv->wor_service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}




static void l_setup_properties_container(JagService *jag_service) {
	JagServicePrivate *priv = JAG_SERVICE_GET_PRIVATE(jag_service);
	priv->module_properties_handler = jag_module_properties_handler_new(priv->jag_moose_service);

	MooCowPropertiesContainer *cow_properties_container = (MooCowPropertiesContainer *) moo_service_get_cow_properties_container(priv->moo_service);
	moo_cow_properties_container_add_handler(cow_properties_container, (MooIPropertiesHandler *) priv->module_properties_handler);


	priv->module_panel_factory = jag_module_panel_factory_new();

	CowPanelModel *panel_model = moo_service_get_properties_panel_model(priv->moo_service);
	cow_panel_model_add_factory(panel_model, (CowIPanelFactory *) priv->module_panel_factory);
}


static void l_attach_preferences(JagService *jag_service) {
	JagServicePrivate *priv = JAG_SERVICE_GET_PRIVATE(jag_service);
	ElkPreferencesService *prefs_service = elk_service_get_preferences_service(priv->elk_service);
	CowPanelModel *panel_model = elk_preferences_service_get_panel_model(prefs_service);

	VipService *vip_service = moo_service_get_viper_service(priv->moo_service);

	priv->prefs_panel_factory = jag_prefs_panel_factory_new(vip_service);
	cow_panel_model_add_factory(panel_model, (CowIPanelFactory *) priv->prefs_panel_factory);


	priv->prefs_mapper = jag_prefs_mapper_new();
	elk_preferences_services_add_mapper(prefs_service, (ElkIPreferencesMapper *) priv->prefs_mapper);


	ElkPreferencesContainer *container = elk_preferences_service_get_container(prefs_service);
	cow_container_add_change_listener((CowContainer *) container, COW_ICHANGE_LISTENER(jag_service));
}

JagService *jag_service_new(WorService *wor_service, ElkService *elk_service, MooService *moo_service) {
	JagService *result = g_object_new(JAG_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);
	JagServicePrivate *priv = result->priv;
	priv->wor_service = wor_service;
	priv->elk_service = cat_ref_ptr(elk_service);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->connector = jag_editor_connector_new();

	priv->jar_map = jag_indexer_jar_map_new(moo_service, moo_service_get_viper_service(moo_service));
	priv->jre_map = jag_indexer_jre_map_new(priv->jar_map, moo_service, moo_service_get_viper_service(moo_service), wor_service);
	priv->jag_moose_service = jag_moose_service_new(moo_service, priv->jre_map, priv->jar_map);

	priv->indexer = jag_indexer_new(wor_service, moo_service);
	l_setup_properties_container(result);
//	priv->module_map = jag_module_map_new(mon_exp_service, wor_service, priv->settings_clazz);
//	l_setup_configuration_container(result);
//	jag_module_map_add_listener(priv->module_map, JAG_IMODULE_MAP_LISTENER(priv->indexer));





	l_attach_preferences(result);

	return result;
}



//JagIJarMap *jag_service_get_jar_map(JagService *jag_service) {
//	JagServicePrivate *priv = JAG_SERVICE_GET_PRIVATE(jag_service);
//	return (JagIJarMap *) priv->module_map;
//}



static CatS s_txt_java = CAT_S_DEF(".java");
static CatS s_txt_dot_class = CAT_S_DEF(".class");

static void l_enlist_editor_factories(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node) {
	JagService *jag_service = JAG_SERVICE(self);
	JagServicePrivate *priv = JAG_SERVICE_GET_PRIVATE(jag_service);

	JagSrcfileContentWo *classfile_content = (JagSrcfileContentWo *) moo_node_wo_get_content((MooNodeWo *) node, jag_srcfile_content_wo_key());
	if (classfile_content) {
//		MooIdPath *id_path = jag_srcfile_content_wo_get_file_path(classfile_content);
//		cat_log_error("id_path=%o", id_path);
//		MooNodeWo *root_node = moo_service_get_root_node_ref(priv->moo_service);
//		MooNodeWo *node_to_open = moo_id_path_get_tail(id_path, root_node);
//		MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content((MooNodeWo *) node_to_open, moo_resource_content_wo_key());
//		VipIFile *file_to_open = (VipIFile *) moo_resource_content_wo_get_viper_node(resource_content);

		VipIFile *file_to_open = jag_srcfile_content_wo_get_file(classfile_content);


		ElkDocumentIO *document_io = priv->elk_service->document_io;
		cat_log_debug("priv->elk_service=%o", priv->elk_service);
//		JagEditorFactory *editor_factory = jag_editor_factory_new((ElkIService *) priv->elk_service, priv->moo_service, priv->connector_map, document_io, priv->indexer , file_to_open);
		LeaIPanelOwner *panel_owner = (LeaIPanelOwner *) priv->elk_service->panel_owner;
		JagEditorFactory *editor_factory = jag_editor_factory_new(panel_owner, priv->connector, document_io, file_to_open, priv->indexer);
		cat_array_wo_append(e_enlist_to, (GObject *) editor_factory);
		cat_unref(editor_factory);
//		cat_unref_ptr(root_node);
	} else {

		MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content((MooNodeWo *) node, moo_resource_content_wo_key());
		if (resource_content!=NULL) {
			VipNode *node = moo_resource_content_wo_get_viper_node(resource_content);
			VipIFile *file_to_open = node==NULL ? NULL : (VipIFile *) vip_node_get_content(node);
			if (file_to_open!=NULL) {
				cat_log_detail("nm:%p", file_to_open);
				CatStringWo *a_name = vip_iresource_get_name((VipIResource *) file_to_open);
				cat_log_detail("name:%p", a_name);
				cat_log_debug("priv->elk_service=%o", priv->elk_service);
				cat_log_debug("priv->elk_service->document_io=%o", priv->elk_service->document_io);

				ElkDocumentIO *document_io = priv->elk_service->document_io;
				LeaIPanelOwner *panel_owner = (LeaIPanelOwner *) priv->elk_service->panel_owner;
				if (cat_string_wo_endswith(a_name, CAT_S(s_txt_java))) {
					JagEditorFactory *editor_factory = jag_editor_factory_new(panel_owner, priv->connector, document_io, file_to_open, priv->indexer);

					cat_log_detail("editor_factory:%o, document_io=%p", editor_factory, document_io);
					cat_array_wo_append(e_enlist_to, (GObject *) editor_factory);
					cat_unref(editor_factory);
				} else if (cat_string_wo_endswith(a_name, CAT_S(s_txt_dot_class))) {
					JagClassEditorFactory *editor_factory = jag_class_editor_factory_new(panel_owner, priv->connector, document_io, file_to_open/*, priv->indexer*/);
					cat_array_wo_append(e_enlist_to, (GObject *) editor_factory);
					cat_unref(editor_factory);
				}
			}
		}
	}
}


static void l_resource_handler_iface_init(ElkIResourceHandlerInterface *iface) {
//	iface->matchScoreFile = l_match_score_file;
//	iface->createFileEditor = l_create_file_editor;
	iface->enlistEditorFactories = l_enlist_editor_factories;
}



/********************* start CowIChangeListener implementation *********************/


static void l_config_changed(CowIChangeListener *self, GObject *new_config) {
	JagService *jag_service = JAG_SERVICE(self);
	JagServicePrivate *priv = JAG_SERVICE_GET_PRIVATE(jag_service);
	ElkPreferencesWo *a_elk_prefs = (ElkPreferencesWo *) new_config;
	JagPreferencesWo *a_jag_prefs = NULL;
	if (a_elk_prefs) {
		a_jag_prefs = (JagPreferencesWo *) elk_preferences_wo_get_sub(a_elk_prefs, CAT_S(jag_s_java));
	}

	jag_module_panel_factory_set_preferences(priv->module_panel_factory, a_jag_prefs);
	jag_indexer_set_preferences(priv->indexer, a_jag_prefs);
//	jag_module_map_set_jag_configuration(priv->module_map, jag_configuration);
	CatArrayWo *a_jre_list = a_jag_prefs==NULL ? NULL : jag_preferences_wo_get_jre_list(a_jag_prefs);
	jag_indexer_jre_map_set_jre_list(priv->jre_map, a_jre_list);


}


static void l_change_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}

/********************* end CowIChangeListener implementation *********************/
