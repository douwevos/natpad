/*
   File:    moocowpropertiescontainer.c
   Project: moose
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#include "moocowpropertiescontainer.h"
#include "../../mooservice.h"
#include "../access/mooidpath.h"
#include "../moonodewo.h"
#include "../../transaction/mootransaction.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooCowPropertiesContainer"
#include <logging/catlog.h>

struct _MooCowPropertiesContainerPrivate {
	MooService *moo_service;
	CatWeakList *properties_handlers;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooCowPropertiesContainer, moo_cow_properties_container, COW_TYPE_CONTAINER,
		G_ADD_PRIVATE(MooCowPropertiesContainer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);


static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
gboolean l_cow_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b);
CowContainer *l_cow_create_editor(CowContainer *container);

static void moo_cow_properties_container_class_init(MooCowPropertiesContainerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	CowContainerClass *cow_class = COW_CONTAINER_CLASS(clazz);
	cow_class->createEditor = l_cow_create_editor;
	cow_class->isSameConfig = l_cow_is_same_config;
}

static void moo_cow_properties_container_init(MooCowPropertiesContainer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooCowPropertiesContainer *instance = MOO_COW_PROPERTIES_CONTAINER(object);
	MooCowPropertiesContainerPrivate *priv = moo_cow_properties_container_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->properties_handlers);
	G_OBJECT_CLASS(moo_cow_properties_container_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_cow_properties_container_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooCowPropertiesContainer *moo_cow_properties_container_new(struct _MooService *moo_service) {
	MooCowPropertiesContainer *result = g_object_new(MOO_TYPE_COW_PROPERTIES_CONTAINER, NULL);
	cat_ref_anounce(result);
	MooCowPropertiesContainerPrivate *priv = moo_cow_properties_container_get_instance_private(result);
	cow_container_construct((CowContainer *) result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->properties_handlers = cat_weak_list_new();
	return result;
}



void moo_cow_properties_container_set_node(MooCowPropertiesContainer *container, MooNodeWo *node) {
	cow_container_set_configuration((CowContainer *) container, (GObject *) node);
}

MooNodeWo *moo_cow_properties_container_get_node(MooCowPropertiesContainer *container) {
	return MOO_NODE_WO(cow_container_get_configuration((CowContainer *) container));
}

void moo_cow_properties_container_add_handler(MooCowPropertiesContainer *container, MooIPropertiesHandler *handler) {
	MooCowPropertiesContainerPrivate *priv = moo_cow_properties_container_get_instance_private(container);
	cat_weak_list_append(priv->properties_handlers, (GObject *) handler);
}



void moo_cow_properties_container_commit(MooCowPropertiesContainer *container) {
	MooCowPropertiesContainerPrivate *priv = moo_cow_properties_container_get_instance_private(container);
	MooNodeWo *moo_root_node = moo_service_get_root_node_ref(priv->moo_service);
	MooNodeWo *input_node = moo_cow_properties_container_get_node(container);
	MooNodeWo *e_input_node = moo_node_wo_ensure_editable(input_node, NULL);
	MooIdPath *path =  moo_node_wo_find_path(moo_root_node, moo_node_wo_get_unique_id(e_input_node));
	cat_log_debug("confirmed:e_input_node=%o, path=%o", e_input_node, path);
	if (path) {
		MooTransaction *tx = moo_service_create_transaction((GObject *) container, priv->moo_service);
		int retry_count=5;
		while(retry_count-->0) {
			moo_transaction_begin(tx);
			MooNodeWo *tx_root_node = moo_transaction_get_tx_root_node(tx);
			MooNodeWo *e_root_node = moo_node_wo_ensure_editable(tx_root_node, NULL);
			MooNodeWo *e_real_node = moo_id_path_create_editable_node(path, e_root_node);
			cat_log_debug("e_input_node=%o, real_editable_node=%o", e_input_node, e_real_node);
			if (e_real_node) {
				CatIIterator *iter = cat_weak_list_iterator(priv->properties_handlers);
				if(cat_iiterator_has_next(iter)) {
					MooIPropertiesHandler *properties_handler = (MooIPropertiesHandler *) cat_iiterator_next(iter);
					moo_iproperties_handler_commit(properties_handler, iter, e_input_node, e_real_node);
				}
				cat_unref_ptr(iter);

				cat_log_debug("real_editable_node=%o", e_real_node);
				if (moo_transaction_commit(tx, e_root_node)) {
					cat_log_debug("commited root-node=%o", moo_transaction_get_commited_root_node(tx));
					cat_log_debug("commited root-commited_real_node=%o", moo_id_path_get_tail(path, moo_transaction_get_commited_root_node(tx)));
					break;
				}
			} else {
				cat_unref_ptr(e_root_node);
				break;
			}
		}
	} else {
		cat_log_error("node not found:%o", input_node);
	}
	cat_unref_ptr(moo_root_node);
}




gboolean l_cow_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b) {
	return FALSE;
//	return moo_node_wo_equal((MooNodeWo *) config_a, (MooNodeWo *) config_b);
}

CowContainer *l_cow_create_editor(CowContainer *container) {
	MooCowPropertiesContainerPrivate *spriv = moo_cow_properties_container_get_instance_private(MOO_COW_PROPERTIES_CONTAINER(container));
	MooCowPropertiesContainer *result = moo_cow_properties_container_new(spriv->moo_service);
	cow_container_copy_from((CowContainer *) result, container);

	MooNodeWo *node = moo_cow_properties_container_get_node((MooCowPropertiesContainer *) container);
	cat_log_detail("this=%p, node=%o", container, node);
	if (!moo_node_wo_is_fixed(node)) {
		cat_ref_ptr(node);
		MooNodeWo *n = moo_node_wo_anchor(node,1);
		cat_log_detail("this=%p, n=%o", container, n);
		moo_cow_properties_container_set_node((MooCowPropertiesContainer *) container, n);
		node = n;
//		cat_unref_ptr(n);
	}

	MooNodeWo *e_node = moo_node_wo_ensure_editable(node, NULL);
	cat_log_detail("this=%p, e_node=%o", container, e_node);
	moo_cow_properties_container_set_node((MooCowPropertiesContainer *) result, e_node);
	cat_unref_ptr(e_node);
	return (CowContainer *) result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






