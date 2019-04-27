/*
   File:    jagmodulepropertieshandler.c
   Project: jaguar
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

#include "jagmodulepropertieshandler.h"
#include "jagmodulecontentwo.h"
#include "../jagrefreshmodulerequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagModulePropertiesHandler"
#include <logging/catlog.h>

struct _JagModulePropertiesHandlerPrivate {
	JagMooseService *jag_moose_service;
};

static void l_properties_handler_iface_init(MooIPropertiesHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagModulePropertiesHandler, jag_module_properties_handler, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagModulePropertiesHandler)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_IPROPERTIES_HANDLER, l_properties_handler_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_module_properties_handler_class_init(JagModulePropertiesHandlerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_module_properties_handler_init(JagModulePropertiesHandler *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagModulePropertiesHandler *instance = JAG_MODULE_PROPERTIES_HANDLER(object);
	JagModulePropertiesHandlerPrivate *priv = jag_module_properties_handler_get_instance_private(instance);
	cat_unref_ptr(priv->jag_moose_service);
	G_OBJECT_CLASS(jag_module_properties_handler_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_module_properties_handler_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagModulePropertiesHandler *jag_module_properties_handler_new(JagMooseService *jag_moose_service) {
	JagModulePropertiesHandler *result = g_object_new(JAG_TYPE_MODULE_PROPERTIES_HANDLER, NULL);
	cat_ref_anounce(result);
	JagModulePropertiesHandlerPrivate *priv = jag_module_properties_handler_get_instance_private(result);
	priv->jag_moose_service = cat_ref_ptr(jag_moose_service);
	return result;
}


/********************* begin MooIPropertiesHandler implementation *********************/

static void l_properties_handler_commit(MooIPropertiesHandler *self, CatIIterator *iter, struct _MooNodeWo *input_node, struct _MooNodeWo *real_node) {
	JagModulePropertiesHandler *instance = JAG_MODULE_PROPERTIES_HANDLER(self);
	JagModulePropertiesHandlerPrivate *priv = jag_module_properties_handler_get_instance_private(instance);

	JagModuleContentWo *inp_jag_module_content = (JagModuleContentWo *) moo_node_wo_get_content(input_node, jag_module_content_wo_key());
	MooModuleContentWo *inp_moo_module_content = (MooModuleContentWo *) moo_node_wo_get_content(input_node, moo_module_content_wo_key());

	JagModuleContentWo *to_jag_module_content = (JagModuleContentWo *) moo_node_wo_get_content(real_node, jag_module_content_wo_key());
	if (jag_module_content_wo_is_fixed(to_jag_module_content)) {
		to_jag_module_content = jag_module_content_wo_ensure_editable(to_jag_module_content);
		moo_node_wo_set_content(real_node, (MooIContent *) to_jag_module_content);
		cat_unref(to_jag_module_content);
		jag_module_content_wo_set_module_settings(to_jag_module_content, jag_module_content_wo_get_module_settings(inp_jag_module_content));
	}


	cat_log_debug("1.from_jag_module_content=%o", inp_jag_module_content);
	cat_log_debug("2.from_moo_module_content=%o", inp_moo_module_content);
	cat_log_debug("3.to_jag_module_content=%o", to_jag_module_content);

	MooModuleContentWo *e_inp_moo_module_content = moo_module_content_wo_ensure_editable(inp_moo_module_content);
	if (e_inp_moo_module_content!=inp_moo_module_content) {
		moo_node_wo_set_content(input_node, (MooIContent *) e_inp_moo_module_content);
	}
	cat_unref(e_inp_moo_module_content);
	cat_log_debug("writable moo_module_content=%o", e_inp_moo_module_content);
	ShoModel *model = moo_module_content_wo_get_writable_model(e_inp_moo_module_content);

	jag_module_content_write_settings_2_model(inp_jag_module_content, model);
	cat_log_debug("writable moo_module_content=%o", e_inp_moo_module_content);


	JagModuleSettingsWo *jag_module_settings = jag_module_content_wo_get_module_settings(inp_jag_module_content);
	cat_log_debug("e_moo_module_content=%o", e_inp_moo_module_content);
	jag_module_content_wo_set_module_settings(to_jag_module_content, jag_module_settings);

	MOO_IPROPERTIES_HANDLER_NEXT(iter, input_node, real_node);


	WorRequest *request = (WorRequest *) jag_refresh_module_request_new(priv->jag_moose_service, moo_node_wo_get_unique_id(real_node), -1);
	WorService *wor_service = jag_moose_service_get_worm_service(priv->jag_moose_service);
	wor_request_set_time_out(request, cat_date_current_time()+50);
	wor_service_post_request(wor_service, request);
	cat_unref_ptr(request);

}

static void l_properties_handler_iface_init(MooIPropertiesHandlerInterface *iface) {
	iface->commit = l_properties_handler_commit;
}

/********************* end MooIPropertiesHandler implementation *********************/
