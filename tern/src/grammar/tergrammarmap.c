/*
   File:    tergrammarmap.c
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 16, 2012
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

#include "tergrammarmap.h"
#include "scanner/tergrammarreader.h"
#include "terdocumentclazzgrammar.h"
#include "tersyntaxresource.h"
#include "../preferences/terpreferenceswo.h"
#include "../preferences/terprefsmapper.h"
#include <caterpillar.h>
#include <cow.h>
#include <elk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerGrammarMap"
#include <logging/catlog.h>

CatS s_txt_dot_syn = CAT_S_DEF(".syn");

struct _TerGrammarMapPrivate {
	ElkPreferencesService *preference_service;
	VipService *vip_service;
	WorService *wor_service;
	CatLock *lock;
	CatHashMapWo *e_clazz_path_map;		/* Map<CatStringWo *clazz_name, TerDocumentGlazzGrammar *> */
	CatHashMapWo *e_syntax_resource_map;	/* Map<Long *, TerSyntaxResource *> */
	TerPreferencesWo *a_ter_prefs;
};


static void l_cow_change_listener_iface_init(CowIChangeListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerGrammarMap, ter_grammar_map, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(TerGrammarMap)
		G_IMPLEMENT_INTERFACE(COW_TYPE_ICHANGE_LISTENER, l_cow_change_listener_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_grammar_map_class_init(TerGrammarMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_grammar_map_init(TerGrammarMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerGrammarMap *instance = TER_GRAMMAR_MAP(object);
	TerGrammarMapPrivate *priv = ter_grammar_map_get_instance_private(instance);
	if (priv->preference_service) {
		CowContainer *container = (CowContainer *) elk_preferences_service_get_container(priv->preference_service);
		if (container) {
			cow_container_remove_change_listener(container, COW_ICHANGE_LISTENER(instance));
		}
	}
	cat_unref_ptr(priv->a_ter_prefs);
	cat_unref_ptr(priv->e_clazz_path_map);
	cat_unref_ptr(priv->e_syntax_resource_map);
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->preference_service);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->wor_service);
	G_OBJECT_CLASS(ter_grammar_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_grammar_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerGrammarMap *ter_grammar_map_new(ElkPreferencesService *preference_service, VipService *vip_service, WorService *wor_service) {
	TerGrammarMap *result = g_object_new(TER_TYPE_GRAMMAR_MAP, NULL);
	cat_ref_anounce(result);
	TerGrammarMapPrivate *priv = ter_grammar_map_get_instance_private(result);
	priv->preference_service = cat_ref_ptr(preference_service);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->lock = cat_lock_new();
	priv->e_clazz_path_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	priv->e_syntax_resource_map = cat_hash_map_wo_new((GHashFunc) cat_long_hash, (GEqualFunc) cat_long_equal);
	priv->a_ter_prefs = NULL;
	CowContainer *container = (CowContainer *) elk_preferences_service_get_container(priv->preference_service);
	if (container) {
		cow_container_add_change_listener(container, COW_ICHANGE_LISTENER(result));
	}

	cow_ichange_listener_config_changed(COW_ICHANGE_LISTENER(result), cow_container_get_configuration(container));
	return result;
}

TerSyntax *ter_grammar_map_get_syntax(TerGrammarMap *grammar_map, CatStringWo *clazz_name) {
	TerSyntax *result = NULL;
	TerGrammarMapPrivate *priv = ter_grammar_map_get_instance_private(grammar_map);
	cat_lock_lock(priv->lock);

	TerDocumentClazzGrammar *document_clazz_grammar = (TerDocumentClazzGrammar *) cat_hash_map_wo_get(priv->e_clazz_path_map, clazz_name);
	if (document_clazz_grammar) {
		CatLong *viper_id = ter_document_clazz_grammar_get_viper_id(document_clazz_grammar);

		TerSyntaxResource *syntax_resource = (TerSyntaxResource *) cat_hash_map_wo_get(priv->e_syntax_resource_map, viper_id);
		cat_log_debug("get syntax_resource:%o", syntax_resource);
		if (syntax_resource) {
			result = ter_syntax_resource_get_or_load_syntax(syntax_resource);
		}

	}
	cat_ref_ptr(result);
	cat_lock_unlock(priv->lock);
	return result;
}

static CatS l_s_txt_config = CAT_S_DEF("config");

static void l_config_changed(CowIChangeListener *listener, GObject *config) {
	ElkPreferencesWo *a_elk_prefs = ELK_PREFERENCES_WO(config);

	TerPreferencesWo *a_ter_prefs = NULL;
	if (a_elk_prefs) {
		a_ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(a_elk_prefs, CAT_S(ter_s_textpad));
	}

	TerGrammarMap *grammar_map = TER_GRAMMAR_MAP(listener);
	TerGrammarMapPrivate *priv = ter_grammar_map_get_instance_private(grammar_map);

	if (priv->a_ter_prefs==a_ter_prefs) {
		return;
	}

	VipPath *shared_path = elk_preferences_services_get_shared_config_map(priv->preference_service);
	VipPath *shared_config_path = vip_path_create_child(shared_path, CAT_S(l_s_txt_config));

	CatArrayWo *e_changed_clazz_name_list = cat_array_wo_new();

	CatHashMapWo *a_ter_class_map = ter_preferences_wo_get_document_clazzes(a_ter_prefs);
	CatArrayWo *a_class_list = cat_hash_map_wo_enlist_values(a_ter_class_map, NULL);

	CatIIterator *class_iter = cat_array_wo_iterator(a_class_list);
	while(cat_iiterator_has_next(class_iter)) {
		TerPrefsClazzWo *a_ter_clazz = (TerPrefsClazzWo *) cat_iiterator_next(class_iter);
		cat_log_trace("a_ter_clazz=%p", a_ter_clazz);

		/* extract grammar-path for document-clazz */
		CatStringWo *a_grammar = ter_prefs_clazz_wo_get_grammar_file(a_ter_clazz);
		VipPath *grammar_path = NULL;
		if (a_grammar) {
			if (cat_string_wo_index_of(a_grammar, '/')<0) {
				CatStringWo *e_sub_grammar = cat_string_wo_clone(a_grammar, CAT_CLONE_DEPTH_FULL);
				if (!cat_string_wo_endswith(e_sub_grammar, CAT_S(s_txt_dot_syn))) {
					cat_string_wo_append(e_sub_grammar, CAT_S(s_txt_dot_syn));
				}
				CatStringWo *p = cat_string_wo_new_with(cat_string_wo_getchars(e_sub_grammar));
				grammar_path = vip_path_create_child(shared_config_path, p);
				cat_unref_ptr(p);
				cat_unref_ptr(e_sub_grammar);
			} else {
				grammar_path = vip_path_new(cat_ref_ptr(a_grammar));
			}

			cat_log_debug("loading grammar:%o", grammar_path);

			VipCreatePathRequest *create_request = vip_service_create_path(priv->vip_service, grammar_path);
			VipNodePath *node_path = vip_create_path_request_wait_for_path(create_request);
			CatTreeNode *node = vip_node_path_get_tail_node(node_path);
			VipNode *vip_node = (VipNode *) cat_tree_node_get_content(node);
			long long int id = vip_node_get_id(vip_node);
			CatLong *lid = cat_long_new(id);
			cat_log_debug("node_path:%o, node=%o", node_path, node);

			/* add the document-clazz to the clazz_path_map with the extracted grammar-path */
			CatStringWo *a_clazz_name = ter_prefs_clazz_wo_get_name(a_ter_clazz);
			cat_lock_lock(priv->lock);
			TerDocumentClazzGrammar *document_clazz_grammar = (TerDocumentClazzGrammar *) cat_hash_map_wo_get(priv->e_clazz_path_map, a_clazz_name);
			if (document_clazz_grammar==NULL) {

				document_clazz_grammar = ter_document_clazz_grammar_new(a_ter_clazz, lid);
				cat_array_wo_append(e_changed_clazz_name_list, (GObject *) a_clazz_name);
				cat_hash_map_wo_put(priv->e_clazz_path_map, (GObject *) a_clazz_name, (GObject *) document_clazz_grammar);
				cat_unref(document_clazz_grammar);
			} else {
				if (ter_document_clazz_update_viper_id(document_clazz_grammar, lid)) {
					cat_array_wo_append(e_changed_clazz_name_list, (GObject *) a_clazz_name);
				}
			}

			/* ensure the grammar-path is part of the syntax_resource_map */
			TerSyntaxResource *syntax_resource = (TerSyntaxResource *) cat_hash_map_wo_get(priv->e_syntax_resource_map, lid);
			if (syntax_resource==NULL) {
				syntax_resource = ter_syntax_resource_new(node);
				cat_hash_map_wo_put(priv->e_syntax_resource_map, (GObject *) lid, (GObject *) syntax_resource);
				cat_unref(syntax_resource);
			}

			cat_log_debug("syntax_resource:%o", syntax_resource);


			cat_unref_ptr(lid);
			cat_unref_ptr(create_request);
			cat_lock_unlock(priv->lock);
			cat_unref_ptr(grammar_path);
		}
	}
	cat_unref_ptr(class_iter);
	cat_unref_ptr(a_class_list);
	cat_unref_ptr(shared_config_path);

//	if (priv->highlighter_map) {
//		elk_highlighter_map_rescan_all(priv->highlighter_map, l_rescan_if_clazz_matches, e_changed_clazz_name_list);
//	}

	// TODO run through all highlightermaps that have changed
	cat_unref_ptr(e_changed_clazz_name_list);
}

static void l_cow_change_listener_iface_init(CowIChangeListenerInterface *iface) {
	iface->configChanged = l_config_changed;
}
