/*
   File:    mooactionproperties.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Mar 22, 2014
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

#include "mooactionproperties.h"
#include "../moonodelayout.h"
#include <cow.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionProperties"
#include <logging/catlog.h>

struct _MooActionPropertiesPrivate {
	MooService *moo_service;
	CatArrayWo *selection;
};


G_DEFINE_TYPE(MooActionProperties, moo_action_properties, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void moo_action_properties_class_init(MooActionPropertiesClass *clazz) {
	g_type_class_add_private(clazz, sizeof(MooActionPropertiesPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void moo_action_properties_init(MooActionProperties *instance) {
	MooActionPropertiesPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_ACTION_PROPERTIES, MooActionPropertiesPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionProperties *instance = MOO_ACTION_PROPERTIES(object);
	MooActionPropertiesPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->selection);
	G_OBJECT_CLASS(moo_action_properties_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_properties_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooActionProperties *moo_action_properties_new(MooService *moo_service) {
	MooActionProperties *result = g_object_new(MOO_TYPE_ACTION_PROPERTIES, NULL);
	cat_ref_anounce(result);
	MooActionPropertiesPrivate *priv = result->priv;
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->selection = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("moose.propeties"), cat_string_wo_new_with("P_roperties"), cat_string_wo_new_with("document-properties"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_data("Alt+Enter")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}

void moo_action_properties_set_selection(MooActionProperties *action_properties, CatArrayWo *selection) {
	MooActionPropertiesPrivate *priv = MOO_ACTION_PROPERTIES_GET_PRIVATE(action_properties);
	if (priv->selection == selection) {
		return;
	}
	cat_ref_swap(priv->selection, selection);
	gboolean pd_enabled = FALSE;
	if (selection!=NULL && cat_array_wo_size(selection)==1) {
		pd_enabled = TRUE;
	}
	lea_action_set_sensitive_self(LEA_ACTION(action_properties), pd_enabled);
}



static void l_action_run(LeaAction *self) {
	MooActionPropertiesPrivate *priv = MOO_ACTION_PROPERTIES_GET_PRIVATE(self);
	if (priv->selection!=NULL && cat_array_wo_size(priv->selection)==1) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_array_wo_get_first(priv->selection);
		MooNodeWo *node = moo_node_layout_get_node(node_layout);

		MooCowPropertiesContainer *container = moo_service_get_cow_properties_container(priv->moo_service);

		moo_cow_properties_container_set_node(container, node);

		CowPanelModel *panel_model = moo_service_get_properties_panel_model(priv->moo_service);
		CowDialog *cdialog = cow_dialog_new(panel_model, (CowContainer *) container, COW_DIALOG_OK_AND_CANCEL_ONLY);
		int response = cow_dialog_show(cdialog);
		gboolean keep_open = TRUE;
		while(keep_open) {
			cat_log_debug("response=%d", response);
			switch(response) {
				case GTK_RESPONSE_CLOSE :
				case GTK_RESPONSE_DELETE_EVENT :
				case GTK_RESPONSE_CANCEL : {
					keep_open = FALSE;

				} break;
				case GTK_RESPONSE_OK : {
					keep_open = FALSE;
					CowContainer *input_container = cow_dialog_get_edit_container(cdialog);
					cow_container_set_configuration((CowContainer *) container, cow_container_get_configuration(input_container));
					moo_cow_properties_container_commit(container);
				} break;
				case GTK_RESPONSE_APPLY : {
				}
			}
			if (keep_open) {
				response = gtk_dialog_run((GtkDialog *) cdialog);
			}
		}
		gtk_widget_destroy((GtkWidget *) cdialog);

	}
}

