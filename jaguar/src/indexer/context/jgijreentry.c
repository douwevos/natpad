/*
   File:    jgijreentry.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 17, 2014
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

#include "jgijreentryprivate.h"
#include "jagmjrerefreshentryrequest.h"
#include "../../link/jagilinklistener.h"
#include "../../link/jagatomiclink.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JgiJreEntry"
#include <logging/catlog.h>

struct _JgiJreEntryPrivate {
	WorService *wor_service;
	VipISequence *moo_sequence;
	JagPrefsJreWo *a_jre;
	int jre_list_version;
	CatArrayWo *e_jar_entries;
	CatLock *lock;

	CatWeakReference *jre_link;
	JagLink *link_moo_node;
	int moo_node_version;
};

static void l_ilink_listener_iface_init(JagILinkListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JgiJreEntry, jgi_jre_entry, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JgiJreEntry)
		G_IMPLEMENT_INTERFACE(JAG_TYPE_ILINK_LISTENER, l_ilink_listener_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jgi_jre_entry_class_init(JgiJreEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jgi_jre_entry_init(JgiJreEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JgiJreEntry *instance = JGI_JRE_ENTRY(object);
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(instance);
	cat_unref_ptr(priv->jre_link);
	cat_unref_ptr(priv->a_jre);
	cat_unref_ptr(priv->e_jar_entries);
	cat_unref_ptr(priv->moo_sequence);
	cat_unref_ptr(priv->link_moo_node);
	cat_unref_ptr(priv->wor_service);
	cat_unref_ptr(priv->lock);
	G_OBJECT_CLASS(jgi_jre_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jgi_jre_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JgiJreEntry *jgi_jre_entry_new(WorService *wor_service, VipISequence *moo_sequence, JagPrefsJreWo *a_jre, JagLink *jre_link) {
	JgiJreEntry *result = g_object_new(JGI_TYPE_JRE_ENTRY, NULL);
	cat_ref_anounce(result);
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(result);
	priv->wor_service = cat_ref_ptr(wor_service);
	priv->moo_sequence = cat_ref_ptr(moo_sequence);
	priv->jre_link = cat_weak_reference_new((GObject *) jre_link);
	priv->a_jre = cat_ref_ptr(a_jre);
	priv->lock = cat_lock_new();
	priv->e_jar_entries = cat_array_wo_new();
	priv->jre_list_version = -1;
	priv->link_moo_node = (JagLink *) jag_atomic_link_new();
	priv->moo_node_version = 1;

	CatStringWo *a_jre_name = jag_prefs_jre_wo_get_name(a_jre);

	MooNodeWo *root_node = moo_node_wo_new(0, NULL, NULL, NULL, 0);
	MooNodeWo *e_compiler_node = moo_node_wo_new(vip_isequence_next(moo_sequence), cat_ref_ptr(a_jre_name), NULL, NULL, 10400);
	MooContentMapWo *e_content_map = moo_node_wo_get_content_map(e_compiler_node);
	CatStringWo *a_txt_jag_jre = cat_string_wo_new_with("JagJre");
	moo_content_map_wo_create_link(e_content_map, a_txt_jag_jre, a_txt_jag_jre);
	cat_unref_ptr(a_txt_jag_jre);
	moo_node_wo_append_child(root_node, e_compiler_node);
	cat_unref_ptr(e_compiler_node);
	root_node = moo_node_wo_anchor(root_node, priv->moo_node_version);
	jag_link_set_value(priv->link_moo_node, (GObject *) root_node);
	cat_unref_ptr(root_node);
	return result;
}

CatStringWo *jgi_jre_entry_get_name(JgiJreEntry *entry) {
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(entry);
	CatStringWo *a_jre_name = jag_prefs_jre_wo_get_name(priv->a_jre);
	return a_jre_name;
}

MooNodeWo *jgi_jre_entry_get_moo_node_ref(JgiJreEntry *entry) {
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(entry);
	return (MooNodeWo *) jag_link_get_value_ref(priv->link_moo_node);
}


void jgi_jre_entry_add_jar(JgiJreEntry *jre_entry, JgiJarEntry *jar_entry) {
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(jre_entry);
	cat_lock_lock(priv->lock);
	cat_array_wo_append(priv->e_jar_entries, (GObject *) jar_entry);
	cat_lock_unlock(priv->lock);
	jgi_jar_entry_add_link_listener(jar_entry, (JagILinkListener *) jre_entry);
}

gboolean jgi_jre_entry_set_jar_entry_list(JgiJreEntry *jre_entry, JagIndexerJarMap *jar_map, CatArrayWo *e_new_jar_entry_list, int jre_list_version) {
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(jre_entry);
	cat_log_debug("new_jar_entry_list=%o, jre_list_version=%d", e_new_jar_entry_list, jre_list_version);
	cat_lock_lock(priv->lock);
	if (priv->jre_list_version>= jre_list_version) {
		cat_lock_unlock(priv->lock);
		return FALSE;
	}
	gboolean dif_size = cat_array_wo_size(e_new_jar_entry_list)!=cat_array_wo_size(priv->e_jar_entries);
	gboolean did_change = dif_size;
	int idx;
	for(idx=cat_array_wo_size(e_new_jar_entry_list)-1; idx>=0; idx--) {
		JgiJarEntry *entry = (JgiJarEntry *) cat_array_wo_get(e_new_jar_entry_list, idx);
		int e_idx = cat_array_wo_find_index(priv->e_jar_entries, entry, idx);
		cat_log_debug("entry=%o, e_idx=%d", entry, e_idx);
		if (e_idx>=0) {
			if (e_idx!=idx) {
				did_change = TRUE;
			}
			jag_indexer_jar_map_detach(jar_map, entry);
		} else {
			cat_array_wo_append(priv->e_jar_entries, (GObject *) entry);
			jgi_jar_entry_add_link_listener(entry, (JagILinkListener *) jre_entry);
			did_change = TRUE;
		}
	}

	if (dif_size) {
		for(idx=cat_array_wo_size(priv->e_jar_entries)-1; idx>=0; idx--) {
			JgiJarEntry *entry = (JgiJarEntry *) cat_array_wo_get(priv->e_jar_entries, idx);
			int e_idx = cat_array_wo_find_index(e_new_jar_entry_list, entry, idx);
			if (e_idx<0) {
				cat_array_wo_remove(priv->e_jar_entries, idx, NULL);
				jgi_jar_entry_remove_link_listener(entry, (JagILinkListener *) jre_entry);
			}
		}
	}

	cat_lock_unlock(priv->lock);
	if (did_change) {
		// TODO: should this be here ?
		jgi_jre_entry_update_moose_node(jre_entry);
	}
	return did_change;
}

void jgi_jre_entry_update_moose_node(JgiJreEntry *jre_entry) {
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(jre_entry);
	MooNodeWo *init_root_node = (MooNodeWo *) jag_link_get_value_ref(priv->link_moo_node);
	MooNodeWo *e_root_node = moo_node_wo_ensure_editable(init_root_node, NULL);
	MooNodeWo *e_jre_node = moo_node_wo_get_editable_child_at(e_root_node, 0);
	gboolean did_change = FALSE;
	cat_lock_lock(priv->lock);
	MooIdNodeMatcher *id_matcher = moo_id_node_matcher_new(0);
	int idx = 0;
	for(idx=cat_array_wo_size(priv->e_jar_entries)-1; idx>=0; idx--) {
		JgiJarEntry *entry = (JgiJarEntry *) cat_array_wo_get(priv->e_jar_entries, idx);
		MooNodeWo *new_jar_root_node = jgi_jar_entry_get_moo_node_ref(entry);
		cat_log_debug("[%d] new_jar_node=%o", idx, new_jar_root_node);
		if (new_jar_root_node==NULL) {
			continue;
		}
		MooNodeWo *new_jar_node = moo_node_wo_child_at(new_jar_root_node, 0);
		if (new_jar_node==NULL) {
			cat_unref_ptr(new_jar_root_node);
			continue;
		}

		moo_id_node_matcher_set_id(id_matcher, moo_node_wo_get_unique_id(new_jar_node));
		int e_idx = moo_node_wo_find_index(e_jre_node, (MooINodeMatcher *) id_matcher, NULL, idx);
		cat_log_debug("[%d] e_idx=%d", idx, e_idx);
		if (e_idx>=0) {
			MooNodeListWo *e_children = moo_node_wo_get_editable_children(e_jre_node);
			moo_node_list_wo_set_at(e_children, new_jar_node, e_idx);
		} else {
			moo_node_wo_insert_child_at(e_jre_node, idx, new_jar_node);
		}
		cat_unref_ptr(new_jar_root_node);
	}
	cat_unref_ptr(id_matcher);
	MooNodeWo *a_root_node = moo_node_wo_anchor(e_root_node, priv->moo_node_version+1);
	if (a_root_node!=init_root_node) {
		priv->moo_node_version++;
		jag_link_set_value(priv->link_moo_node, (GObject *) a_root_node);
		did_change = TRUE;
	}
	cat_unref_ptr(a_root_node);
	cat_lock_unlock(priv->lock);
	if (did_change) {
		JagLink *jre_link = (JagLink *) cat_weak_reference_get(priv->jre_link);
		if (jre_link) {
			jag_link_notify(jre_link, (GObject *) jre_entry);
			cat_unref_ptr(jre_link);
		}
	}
	cat_unref_ptr(init_root_node);
}

void jgi_jre_entry_refresh_jar_entry(JgiJreEntry *jre_entry, JgiJarEntry *jar_entry, int last_jar_entry_index) {
//	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(jre_entry);
}

static void l_link_notify(JagILinkListener *self, JagLink *link, GObject *notify_data) {
	JgiJreEntry *instance = JGI_JRE_ENTRY(self);
	JgiJreEntryPrivate *priv = jgi_jre_entry_get_instance_private(instance);
	JgiJarEntry *jar_entry = (JgiJarEntry *) notify_data;
	cat_lock_lock(priv->lock);
	int jar_entry_index = cat_array_wo_find_index(priv->e_jar_entries, jar_entry, -1);
	cat_lock_unlock(priv->lock);
	cat_log_debug("notified: notify_data=%o, jar_entry_index=%d", notify_data, jar_entry_index);

	if (jar_entry_index>=0) {

		jgi_jre_entry_update_moose_node((JgiJreEntry *) self);

//		JagmJreRefreshEntryRequest *request = jagm_jre_refresh_entry_request_new((JgiJreEntry *) self, jar_entry, jar_entry_index);
//		wor_service_post_request(priv->wor_service, (WorRequest *) request);
//		cat_unref_ptr(request);
	}
}

static void l_ilink_listener_iface_init(JagILinkListenerInterface *iface) {
	iface->notify = l_link_notify;
}
