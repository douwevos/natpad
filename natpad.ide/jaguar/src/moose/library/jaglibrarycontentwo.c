/*
   File:    jaglibrarycontentwo.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
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

#include "jaglibrarycontentwo.h"
#include "jaglibraryapplyrequest.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLibraryContentWo"
#include <logging/catlog.h>

struct _WoInfo {
	JagLibraryContentWo *original;
};

typedef struct _WoInfo WoInfo;


struct _JagLibraryContentWoPrivate {
	CatWeakReference *jag_moose_service_ref;
	MooIdPath *module_id_path;
	MooIdPath *node_id_path;
	CatWeakReference *jar_entry_ref; // JgiJarEntry *
	CatTreeNode *tree_node;
	int last_jar_node_version;
	int version;
	WoInfo *wo_info;
};

static CatS jag_s_library_content_key = CAT_S_DEF("JagLibraryContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_link_listener_iface_init(JagILinkListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagLibraryContentWo, jag_library_content_wo, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagLibraryContentWo)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init)
		G_IMPLEMENT_INTERFACE(JAG_TYPE_ILINK_LISTENER, l_link_listener_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_library_content_wo_class_init(JagLibraryContentWoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_library_content_wo_init(JagLibraryContentWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagLibraryContentWo *instance = JAG_LIBRARY_CONTENT_WO(object);
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(instance);
	if (priv->jar_entry_ref) {
		JgiJarEntry *jar_entry = (JgiJarEntry *) cat_weak_reference_get(priv->jar_entry_ref);
		if (jar_entry) {
			jgi_jar_entry_remove_link_listener(jar_entry, (JagILinkListener *) instance);
			cat_unref_ptr(jar_entry);
		}
//		cat_unref_ptr(priv->jar_entry);
		cat_unref_ptr(priv->jar_entry_ref);
	}
	cat_unref_ptr(priv->tree_node);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
	}
	cat_unref_ptr(priv->jag_moose_service_ref);
	cat_unref_ptr(priv->module_id_path);
	cat_unref_ptr(priv->node_id_path);
	G_OBJECT_CLASS(jag_library_content_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_library_content_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagLibraryContentWo *jag_library_content_wo_new(JagMooseService *jag_moose_service, MooIdPath *module_id_path, JgiJarEntry *jar_entry) {
	JagLibraryContentWo *result = g_object_new(JAG_TYPE_LIBRARY_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(result);
	priv->jag_moose_service_ref = cat_weak_reference_new((GObject *) jag_moose_service);
	priv->module_id_path = cat_ref_ptr(module_id_path);
	priv->node_id_path = NULL;
	priv->jar_entry_ref = cat_weak_reference_new((GObject *) jar_entry);
	priv->tree_node = NULL;
	priv->last_jar_node_version = -1;
	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->original = NULL;

	jgi_jar_entry_add_link_listener(jar_entry, (JagILinkListener *) result);
	return result;
}

JagLibraryContentWo *jag_library_content_wo_ensure_editable(JagLibraryContentWo *library_content) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	if (priv->wo_info) {
		return cat_ref_ptr(library_content);
	}

	JagLibraryContentWo *result = g_object_new(JAG_TYPE_LIBRARY_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	JagLibraryContentWoPrivate *opriv = jag_library_content_wo_get_instance_private(result);
	opriv->jag_moose_service_ref = cat_ref_ptr(priv->jag_moose_service_ref);
	opriv->module_id_path = cat_ref_ptr(priv->module_id_path);
	opriv->node_id_path = cat_ref_ptr(priv->node_id_path);
	opriv->jar_entry_ref = cat_ref_ptr(priv->jar_entry_ref);
	opriv->tree_node = cat_ref_ptr(priv->tree_node);
	opriv->last_jar_node_version = priv->last_jar_node_version;
	opriv->wo_info = g_new0(WoInfo, 1);
	opriv->wo_info->original = cat_ref_ptr(library_content);
	return result;
}



JagLibraryContentWo *jag_library_content_wo_anchor(JagLibraryContentWo *library_content, int version) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	if (priv->wo_info==NULL) {
		return library_content;
	}

	cat_log_debug("anchoring:%p", library_content);

	if (priv->wo_info->original) {
		JagLibraryContentWoPrivate *opriv = jag_library_content_wo_get_instance_private(priv->wo_info->original);
		gboolean was_modified = priv->tree_node!=opriv->tree_node
				|| priv->jar_entry_ref!=opriv->jar_entry_ref
				|| priv->node_id_path!=opriv->node_id_path
				|| priv->last_jar_node_version!=opriv->last_jar_node_version;
//				|| priv->source_folders!=opriv->source_folders
//				|| priv->src_folder_paths!=priv->wo_info->orig_src_paths
//				|| priv->library_content_list!=priv->wo_info->orig_library_content_list
//				|| priv->libraries!=opriv->libraries
//				|| priv->jre_link!=opriv->jre_link
//				|| priv->resource_content_version!=opriv->resource_content_version;

		cat_log_debug("was_modified=%d", was_modified);

		if (!was_modified) {
			JagLibraryContentWo *result = priv->wo_info->original;
			priv->wo_info->original = NULL;
			cat_unref_ptr(library_content);
			return result;
		}
	}
	priv->version = version;

	JgiJarEntry *jar_entry = (JgiJarEntry *) cat_weak_reference_get(priv->jar_entry_ref);
	if (jar_entry) {
		jgi_jar_entry_add_link_listener(jar_entry, (JagILinkListener *) library_content);
		cat_unref_ptr(jar_entry);
	}
	cat_unref_ptr(priv->wo_info->original);
	cat_free_ptr(priv->wo_info);
	return library_content;
}


gboolean jag_library_content_wo_is_fixed(JagLibraryContentWo *library_content) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	return priv->wo_info==NULL;
}

int jag_library_content_wo_get_version(JagLibraryContentWo *library_content) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	return priv->version;
}


#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:"); \
			return rval; \
		} \

gboolean jag_library_content_wo_set_vip_node(JagLibraryContentWo *e_library_content, CatTreeNode *tree_node) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(e_library_content);
	if (priv->tree_node == tree_node) {
		return FALSE;
	}
	CHECK_IF_WRITABLE(FALSE);
	cat_ref_swap(priv->tree_node, tree_node);
	return TRUE;
}

CatTreeNode *jag_library_content_wo_get_vip_node(JagLibraryContentWo *library_content) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	return priv->tree_node;
}

void jag_library_content_wo_set_node_id_path(JagLibraryContentWo *library_content, MooIdPath *node_id_path) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->node_id_path, node_id_path);
}


CatStringWo *jag_library_content_wo_key() {
	return CAT_S(jag_s_library_content_key);
}

gboolean jag_library_content_wo_set_new_jar_node_version(JagLibraryContentWo *library_content, MooNodeWo *real_jar_node) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	CHECK_IF_WRITABLE(FALSE);
	if (priv->last_jar_node_version>=0) {
		if (real_jar_node) {
			if (moo_node_wo_get_version(real_jar_node)<=priv->last_jar_node_version) {
				return FALSE;
			}
		}
	}

	priv->last_jar_node_version = real_jar_node==NULL ? -1 : moo_node_wo_get_version(real_jar_node);
	return TRUE;
}

void jag_library_content_wo_apply_jar_request(JagLibraryContentWo *library_content) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(library_content);
	JagMooseService *jag_moose_service = (JagMooseService *) cat_weak_reference_get(priv->jag_moose_service_ref);
	if (jag_moose_service==NULL) {
		return;
	}

	JgiJarEntry *jar_entry = (JgiJarEntry *) cat_weak_reference_get(priv->jar_entry_ref);
	if (jar_entry==NULL) {
		return;
	}


	MooNodeWo *node_wo_jar = jgi_jar_entry_get_moo_node_ref(jar_entry);


	WorRequest *apply_request = (WorRequest *) jag_library_apply_request_new(jag_moose_service, priv->node_id_path, node_wo_jar);
	WorService *wor_service = jag_moose_service_get_worm_service(jag_moose_service);
//	cat_log_debug("wor_service=%o", wor_service)
	wor_service_post_request(wor_service, apply_request);
	cat_unref_ptr(apply_request);
	cat_unref_ptr(node_wo_jar);
	cat_unref_ptr(jag_moose_service);
	cat_unref_ptr(jar_entry);
}

/********************* start JagILinkListener implementation *********************/

static void l_link_notify(JagILinkListener *self, struct _JagLink *link, GObject *notify_data) {
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private((JagLibraryContentWo *) self);
	cat_log_debug("self=%p", self)
	cat_log_debug("self=%o", self)
//	priv->module_id_path;

	if (priv->node_id_path) {

		JagMooseService *jag_moose_service = (JagMooseService *) cat_weak_reference_get(priv->jag_moose_service_ref);
		if (jag_moose_service==NULL) {
			return;
		}
		MooNodeWo *node_wo_jar = (MooNodeWo *) jag_link_get_value_ref(link);

		WorRequest *apply_request = (WorRequest *) jag_library_apply_request_new(jag_moose_service, priv->node_id_path, node_wo_jar);
		WorService *wor_service = jag_moose_service_get_worm_service(jag_moose_service);
//	cat_log_debug("wor_service=%o", wor_service)
		wor_service_post_request(wor_service, apply_request);
		cat_unref_ptr(apply_request);
		cat_unref_ptr(node_wo_jar);
		cat_unref_ptr(jag_moose_service);

	}
}

static void l_link_listener_iface_init(JagILinkListenerInterface *iface) {
	iface->notify = l_link_notify;
}

/********************* end JagILinkListener implementation *********************/


/********************* start MooIContent implementation *********************/

static CatStringWo *l_content_get_key(MooIContent *self) {
	return jag_library_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) jag_library_content_wo_anchor(JAG_LIBRARY_CONTENT_WO(self), version);
}

static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagLibraryContentWo *instance = JAG_LIBRARY_CONTENT_WO(self);
	JagLibraryContentWoPrivate *priv = jag_library_content_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:tree_node=%o, jar_entry=%o]", iname, self, priv->tree_node, priv->jar_entry_ref);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

