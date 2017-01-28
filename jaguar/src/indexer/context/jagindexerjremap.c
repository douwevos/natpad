/*
   File:    jagindexerjremap.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 2, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "jagindexerjremapprivate.h"
#include "jagupdatejrelistrequest.h"
#include <worm.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexerJreMap"
#include <logging/catlog.h>

struct _JagIndexerJreMapPrivate {
	JagIndexerJarMap *jar_map;
	VipService *vip_service;
	MooService *moo_service;
	WorQueue *jre_queue;


	CatLock *map_lock;
	int jre_list_version;
	CatArrayWo *a_jre_list;
	CatHashMapWo *e_map;
};


G_DEFINE_TYPE(JagIndexerJreMap, jag_indexer_jre_map, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_indexer_jre_map_class_init(JagIndexerJreMapClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JagIndexerJreMapPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_indexer_jre_map_init(JagIndexerJreMap *instance) {
	JagIndexerJreMapPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_INDEXER_JRE_MAP, JagIndexerJreMapPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexerJreMap *instance = JAG_INDEXER_JRE_MAP(object);
	JagIndexerJreMapPrivate *priv = instance->priv;
	cat_unref_ptr(priv->jar_map);
	cat_unref_ptr(priv->a_jre_list);
	cat_unref_ptr(priv->jre_queue);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->e_map);
	cat_unref_ptr(priv->map_lock);
	G_OBJECT_CLASS(jag_indexer_jre_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_indexer_jre_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagIndexerJreMap *jag_indexer_jre_map_new(JagIndexerJarMap *jar_map, MooService *moo_service, VipService *vip_service, WorService *wor_service) {
	JagIndexerJreMap *result = g_object_new(JAG_TYPE_INDEXER_JRE_MAP, NULL);
	cat_ref_anounce(result);
	JagIndexerJreMapPrivate *priv = result->priv;
	priv->jar_map = cat_ref_ptr(jar_map);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->jre_queue = wor_queue_new(wor_service);
	priv->jre_list_version = 0;
	priv->a_jre_list = NULL;
	priv->map_lock = cat_lock_new();
	priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	return result;
}


JagJreLink *jag_indexer_jre_map_get_jre_link_ref(JagIndexerJreMap *jre_map, CatStringWo *a_jre_name) {
	JagIndexerJreMapPrivate *priv = JAG_INDEXER_JRE_MAP_GET_PRIVATE(jre_map);
	JagJreLink *result = NULL;
	cat_lock_lock(priv->map_lock);
	result = (JagJreLink *) cat_hash_map_wo_get(priv->e_map, (GObject *) a_jre_name);
	if (result == NULL) {
		result = jag_jre_link_new(a_jre_name);
		cat_hash_map_wo_put(priv->e_map, (GObject *) a_jre_name, (GObject *) result);
	} else {
		cat_ref_ptr(result);
	}
	cat_lock_unlock(priv->map_lock);
	return result;
}




void jag_indexer_jre_map_set_jre_list(JagIndexerJreMap *jre_map, CatArrayWo *a_jre_list) {
	JagIndexerJreMapPrivate *priv = JAG_INDEXER_JRE_MAP_GET_PRIVATE(jre_map);
	JagUpdateJreListRequest *request = jag_update_jre_list_request_new(jre_map, a_jre_list);
	wor_queue_post(priv->jre_queue, (WorRequest *) request);
	cat_unref_ptr(request);
}


void jag_index_jre_map_apply_new_jre_list(JagIndexerJreMap *jre_map, CatArrayWo *a_jre_list) {
	JagIndexerJreMapPrivate *priv = JAG_INDEXER_JRE_MAP_GET_PRIVATE(jre_map);
	if (cat_array_wo_equal(priv->a_jre_list, a_jre_list, NULL)) {
		return;
	}

	cat_lock_lock(priv->map_lock);
	cat_ref_swap(priv->a_jre_list, a_jre_list);
	priv->jre_list_version++;
	int jre_list_version = priv->jre_list_version;
	cat_lock_unlock(priv->map_lock);

	/* add new jre_list */
	if (a_jre_list) {
		CatIIterator *iter = cat_array_wo_iterator(a_jre_list);
		while(cat_iiterator_has_next(iter)) {
			JagPrefsJreWo *a_jre = (JagPrefsJreWo *) cat_iiterator_next(iter);
			CatStringWo *a_jre_name = jag_prefs_jre_wo_get_name(a_jre);

			cat_lock_lock(priv->map_lock);
			if (priv->jre_list_version>jre_list_version) {
				cat_lock_unlock(priv->map_lock);
				break;
			}
			JagJreLink *jre_link = (JagJreLink *) cat_hash_map_wo_get(priv->e_map, (GObject *) a_jre_name);
			if (jre_link == NULL) {
				jre_link = jag_jre_link_new(a_jre_name);
				cat_hash_map_wo_put(priv->e_map, (GObject *) a_jre_name, (GObject *) jre_link);
			} else {
				cat_ref_ptr(jre_link);
			}
			JgiJreEntry *jre_entry = jag_jre_link_get_entry_ref(jre_link);
			if (jre_entry == NULL) {
				jre_entry = jgi_jre_entry_new(moo_service_get_worm_service(priv->moo_service), (VipISequence *) priv->moo_service, a_jre, (JagLink *) jre_link);
				jag_jre_link_set_entry(jre_link, jre_entry);

			}
			cat_lock_unlock(priv->map_lock);


			CatStringWo *a_jre_path = jag_prefs_jre_wo_get_path(a_jre);
			CatStringWo *e_base_path = cat_string_wo_clone(a_jre_path, CAT_CLONE_DEPTH_MAIN);
			cat_string_wo_append_chars_len(e_base_path, "/", 1);


			CatArrayWo *e_new_jar_entry_list = cat_array_wo_new();

			CatArrayWo *a_jar_list = jag_prefs_jre_wo_get_library_paths(a_jre);
			CatIIterator *jar_iter = cat_array_wo_iterator(a_jar_list);
			while(cat_iiterator_has_next(jar_iter)) {
				CatStringWo *a_name = (CatStringWo *) cat_iiterator_next(jar_iter);
				CatStringWo *e_full_jar_path = cat_string_wo_clone(e_base_path, CAT_CLONE_DEPTH_MAIN);
				cat_string_wo_append(e_full_jar_path, a_name);
				VipPath *p = vip_path_new(e_full_jar_path);
				cat_log_debug("path=%o", p);
				VipCreatePathRequest *path_request = vip_service_create_path(priv->vip_service, p);
				cat_unref_ptr(p);
				VipNodePath *full_jar_vip_path = vip_create_path_request_wait_for_path(path_request);
				JgiJarEntry *jar_entry = jag_indexer_jar_map_attach(priv->jar_map, full_jar_vip_path);
				cat_array_wo_append(e_new_jar_entry_list, (GObject *) jar_entry);
				cat_unref_ptr(path_request);
			}
			cat_unref_ptr(jar_iter);
			cat_unref_ptr(e_base_path);


			jgi_jre_entry_set_jar_entry_list(jre_entry, priv->jar_map, e_new_jar_entry_list, jre_list_version);

			cat_unref_ptr(e_new_jar_entry_list);
			cat_unref_ptr(jre_entry);
			cat_unref_ptr(jre_link);

		}
		cat_unref_ptr(iter);
	}

}

