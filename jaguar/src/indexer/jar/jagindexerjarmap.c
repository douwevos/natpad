/*
   File:    jagindexerjarmap.c
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

#include "jagindexerjarmap.h"
#include "jgijarrefreshviper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagIndexerJarMap"
#include <logging/catlog.h>

struct _JagIndexerJarMapPrivate {
	MooService *moo_service;
	VipService *vip_service;
	CatHashMapWo *e_map;			// <VipPath*, JgiJarEntry*>
	CatLock *lock;
};

static void l_viper_listener_iface_init(VipIListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagIndexerJarMap, jag_indexer_jar_map, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagIndexerJarMap)
		G_IMPLEMENT_INTERFACE(VIP_TYPE_ILISTENER, l_viper_listener_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_indexer_jar_map_class_init(JagIndexerJarMapClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_indexer_jar_map_init(JagIndexerJarMap *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagIndexerJarMap *instance = JAG_INDEXER_JAR_MAP(object);
	JagIndexerJarMapPrivate *priv = jag_indexer_jar_map_get_instance_private(instance);
	cat_unref_ptr(priv->e_map);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(jag_indexer_jar_map_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_indexer_jar_map_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagIndexerJarMap *jag_indexer_jar_map_new(MooService *moo_service, VipService *vip_service) {
	JagIndexerJarMap *result = g_object_new(JAG_TYPE_INDEXER_JAR_MAP, NULL);
	cat_ref_anounce(result);
	JagIndexerJarMapPrivate *priv = jag_indexer_jar_map_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->e_map = cat_hash_map_wo_new((GHashFunc) vip_path_hash, (GEqualFunc) vip_path_equal);
	priv->lock = cat_lock_new();
	vip_service_add_listener(priv->vip_service, (VipIListener *) result);
	return result;
}

JgiJarEntry *jag_indexer_jar_map_attach(JagIndexerJarMap *jar_map, VipNodePath *full_jar_node_path) {
	JagIndexerJarMapPrivate *priv = jag_indexer_jar_map_get_instance_private(jar_map);
	VipPath *full_path = vip_node_path_create_path(full_jar_node_path);
	cat_lock_lock(priv->lock);
	JgiJarEntry *result = (JgiJarEntry *) cat_hash_map_wo_get(priv->e_map, (GObject *) full_path);
	if (result == NULL) {
		result = jgi_jar_entry_new((VipISequence *) priv->moo_service, full_jar_node_path);
		cat_log_debug("full_path=%o", vip_path_to_string(full_path));
		VipNode *node = vip_node_path_get_tail(full_jar_node_path);
		vip_node_recursive_up(node);
		cat_hash_map_wo_put(priv->e_map, (GObject *) full_path, (GObject *) result);
		cat_unref(result);

		CatTreeNode *tree_node = vip_node_path_get_tail_node(full_jar_node_path);
		vip_service_refresh_node(priv->vip_service, (CatReadableTreeNode *) tree_node);
	}
	jgi_jar_entry_usage_up(result);
	cat_lock_unlock(priv->lock);
	cat_unref_ptr(full_path);
	return result;
}

void jag_indexer_jar_map_detach(JagIndexerJarMap *jar_map, JgiJarEntry *entry) {
	JagIndexerJarMapPrivate *priv = jag_indexer_jar_map_get_instance_private(jar_map);
	cat_log_debug("entry=%p", entry);
	VipNodePath *full_jar_node_path = jgi_jar_entry_get_vip_node_path(entry);
	VipPath *full_path = vip_node_path_create_path(full_jar_node_path);
	cat_lock_lock(priv->lock);
	JgiJarEntry *result = (JgiJarEntry *) cat_hash_map_wo_get(priv->e_map, (GObject *) full_path);
	if (result) {
		if (jgi_jar_entry_usage_down(result)) {
			GObject *remd = cat_hash_map_wo_remove(priv->e_map, (GObject *) full_path);
			cat_unref_ptr(remd);
		}
	}
	cat_lock_unlock(priv->lock);
	cat_unref_ptr(full_path);
}


static void l_viper_snapshot_set(VipIListener *listener, struct _VipSnapshot *new_snapshot) {
	JagIndexerJarMap *instance = JAG_INDEXER_JAR_MAP(listener);
	JagIndexerJarMapPrivate *priv = jag_indexer_jar_map_get_instance_private(instance);
	cat_lock_lock(priv->lock);
	CatArrayWo *e_entries = cat_hash_map_wo_enlist_values(priv->e_map, NULL);
	if (e_entries && cat_array_wo_size(e_entries)>0) {

		WorService *wor_service = moo_service_get_worm_service(priv->moo_service);
		CatIIterator *iter = cat_array_wo_iterator(e_entries);
		while(cat_iiterator_has_next(iter)) {
			JgiJarEntry *jar_entry = (JgiJarEntry *) cat_iiterator_next(iter);
			JgiJarRefreshViper *request = jgi_jar_refresh_viper_new(jar_entry, new_snapshot);
			wor_service_post_request(wor_service, (WorRequest *) request);
			cat_unref_ptr(request);
		}
		cat_unref_ptr(iter);
	}
	cat_lock_unlock(priv->lock);
	cat_unref_ptr(e_entries);
}


static void l_viper_listener_iface_init(VipIListenerInterface *iface) {
	iface->snapshotSet = l_viper_snapshot_set;
}
