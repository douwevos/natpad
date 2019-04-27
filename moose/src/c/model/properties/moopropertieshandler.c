/*
   File:    moopropertieshandler.c
   Project: moose
   Author:  Douwe Vos
   Date:    Mar 29, 2014
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

#include "moopropertieshandler.h"
#include "mooipropertieshandler.h"
#include "../module/moomodulecontentwo.h"
#include "../module/moomodulerefreshviperrequest.h"
#include "../moonodewo.h"
#include "../../mooservice.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooPropertiesHandler"
#include <logging/catlog.h>

struct _MooPropertiesHandlerPrivate {
	MooService *moo_service;
};

static void l_properties_handler_iface_init(MooIPropertiesHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooPropertiesHandler, moo_properties_handler, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(MooPropertiesHandler)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_IPROPERTIES_HANDLER, l_properties_handler_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_properties_handler_class_init(MooPropertiesHandlerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_properties_handler_init(MooPropertiesHandler *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooPropertiesHandler *instance = MOO_PROPERTIES_HANDLER(object);
	MooPropertiesHandlerPrivate *priv = moo_properties_handler_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(moo_properties_handler_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_properties_handler_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooPropertiesHandler *moo_properties_handler_new(struct _MooService *moo_service) {
	MooPropertiesHandler *result = g_object_new(MOO_TYPE_PROPERTIES_HANDLER, NULL);
	cat_ref_anounce(result);
	MooPropertiesHandlerPrivate *priv = moo_properties_handler_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	return result;
}


/********************* begin MooIPropertiesHandler implementation *********************/

static void l_properties_handler_commit(MooIPropertiesHandler *self, CatIIterator *iter, struct _MooNodeWo *from_node, struct _MooNodeWo *to_node) {
	cat_log_debug("before:from=%o", from_node);
	cat_log_debug("before:to=%o", to_node);

	MOO_IPROPERTIES_HANDLER_NEXT(iter, from_node, to_node);
	MooPropertiesHandler *instance = MOO_PROPERTIES_HANDLER(self);
	MooPropertiesHandlerPrivate *priv = moo_properties_handler_get_instance_private(instance);

	MooModuleContentWo *from_module_content = (MooModuleContentWo *) moo_node_wo_get_content((MooNodeWo *) from_node, moo_module_content_wo_key());
	cat_log_debug("post:from_module_content=%o", from_module_content);
	if (from_module_content) {
		MooModuleContentWo *to_module_content = (MooModuleContentWo *) moo_node_wo_get_content((MooNodeWo *) to_node, moo_module_content_wo_key());
		if (to_module_content) {
			cat_log_debug("post:to_module_content=%o", to_module_content);
			if (to_module_content!=from_module_content) {

				MooModuleContentWo *e_to_module_content = moo_module_content_wo_ensure_editable(to_module_content);
				if (to_module_content!=e_to_module_content) {
					moo_node_wo_set_content(to_node, (MooIContent *) e_to_module_content);
				}
				cat_unref(e_to_module_content);

				cat_log_debug("e_to_module_content=%p", e_to_module_content);

				moo_module_content_wo_transfer_settings(priv->moo_service, e_to_module_content, from_module_content);


				cat_log_debug("after-transfer:e_to_module_content=%o", e_to_module_content);
				cat_log_debug("after-transfer:to_node=%o", to_node);


				MooService *moo_service = priv->moo_service;
				WorService *wor_service = moo_service_get_worm_service(moo_service);

				MooModuleRefreshViperRequest *req = moo_module_refresh_viper_request_new(priv->moo_service, moo_node_wo_get_unique_id((MooNodeWo *) to_node), TRUE);
				wor_request_set_time_out((WorRequest *) req, cat_date_current_time()+500);
				wor_service_post_request(wor_service, (WorRequest *) req);
				cat_unref_ptr(req);

			}
		} else {
			cat_log_error("no module content in to-node: to_node=%o", to_node);
		}
	}
}

static void l_properties_handler_iface_init(MooIPropertiesHandlerInterface *iface) {
	iface->commit = l_properties_handler_commit;
}

/********************* end MooIPropertiesHandler implementation *********************/
