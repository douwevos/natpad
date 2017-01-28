/*
   File:    elkpreferencescontainer.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 23, 2014
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

#include "elkpreferencescontainer.h"
#include <cowcontainerpriv.h>
#include <shoveler.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkPreferencesContainer"
#include <logging/catlog.h>

struct _ElkPreferencesContainerPrivate {
	ShoModel *persited_model;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkPreferencesContainer, elk_preferences_container, COW_TYPE_CONTAINER,
		G_ADD_PRIVATE(ElkPreferencesContainer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b);
static CowContainer *l_create_editor(CowContainer *container);
static void l_set_configuration(CowContainer *container, gpointer config);


static void elk_preferences_container_class_init(ElkPreferencesContainerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	CowContainerClass *cow_class = COW_CONTAINER_CLASS(clazz);
	cow_class->createEditor = l_create_editor;
	cow_class->isSameConfig = l_is_same_config;
	cow_class->setConfiguration = l_set_configuration;
}

static void elk_preferences_container_init(ElkPreferencesContainer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkPreferencesContainer *instance = ELK_PREFERENCES_CONTAINER(object);
	ElkPreferencesContainerPrivate *priv = elk_preferences_container_get_instance_private(instance);
	cat_unref_ptr(priv->persited_model);
	G_OBJECT_CLASS(elk_preferences_container_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_preferences_container_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkPreferencesContainer *elk_preferences_container_new() {
	ElkPreferencesContainer *result = g_object_new(ELK_TYPE_PREFERENCES_CONTAINER, NULL);
	cat_ref_anounce(result);
	ElkPreferencesContainerPrivate *priv = elk_preferences_container_get_instance_private(result);
	priv->persited_model = sho_model_new();
	cow_container_construct((CowContainer *) result);
	ElkPreferencesWo *pref = elk_preferences_wo_new();
	pref = elk_preferences_wo_anchor(pref, 0);
	elk_preferences_container_set(result, pref);
	cat_unref_ptr(pref);
	return result;
}

gboolean elk_preferences_container_set(ElkPreferencesContainer *container, ElkPreferencesWo *new_prefs) {
	if (!ELK_PREFERENCES_WO(new_prefs)) {
		cat_log_error("not of type ElkPreferencesWo");
		return FALSE;
	}
	cow_container_set_configuration((CowContainer *) container, (GObject *) new_prefs);
	return TRUE;
}

ElkPreferencesWo *elk_preferences_container_get(ElkPreferencesContainer *container) {
	return (ElkPreferencesWo *) cow_container_get_configuration((CowContainer *) container);
}

ElkPreferencesWo *elk_preferences_container_get_fixed(ElkPreferencesContainer *container) {
	ElkPreferencesWo *prefs = (ElkPreferencesWo *) cow_container_get_configuration((CowContainer *) container);
	if (prefs==NULL) {
		return prefs;
	}
	if (elk_preferences_wo_is_anchored(prefs)) {
		return prefs;
	}
	cat_ref_ptr(prefs);
	prefs = elk_preferences_wo_anchor(prefs, 0);
	cow_container_set_configuration((CowContainer *) container, prefs);
	cat_unref(prefs);
	return prefs;
}

ElkPreferencesWo *elk_preferences_container_get_editable(ElkPreferencesContainer *container) {
	ElkPreferencesWo *prefs = (ElkPreferencesWo *) cow_container_get_configuration((CowContainer *) container);
	if (prefs==NULL) {
		return NULL;
	}
	if (!elk_preferences_wo_is_anchored(prefs)) {
		return prefs;
	}
	prefs = elk_preferences_wo_create_editable(prefs);
	cow_container_set_configuration((CowContainer *) container, prefs);
	cat_unref(prefs);
	return prefs;
}


ShoModel *elk_preferences_container_get_model(ElkPreferencesContainer *container) {
	ElkPreferencesContainerPrivate *priv = elk_preferences_container_get_instance_private(container);
	return priv->persited_model;
}

void elk_preferences_container_set_model(ElkPreferencesContainer *container, ShoModel *model) {
	ElkPreferencesContainerPrivate *priv = elk_preferences_container_get_instance_private(container);
	cat_ref_swap(priv->persited_model, model);

}



static gboolean l_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b) {
	return elk_preferences_wo_equal((ElkPreferencesWo *) config_a, (ElkPreferencesWo *) config_b);
}

static void l_set_configuration(CowContainer *container, gpointer config) {
	ElkPreferencesWo *current = (ElkPreferencesWo *) cow_container_get_configuration(container);
	ElkPreferencesWo *replacement = ELK_PREFERENCES_WO(config);
	gboolean needs_notify = !l_is_same_config(container, (GObject *) current, (GObject *) replacement);
	cat_log_debug("container=%p, needs_notify=%d, current=%p, config=%o", container, needs_notify, current, config);

	cow_container_set_configuration_direct(container, config);
	if (needs_notify) {
		cow_container_dispatch_change_event(container);
	}
}


static CowContainer *l_create_editor(CowContainer *container) {
	ElkPreferencesContainer *result = elk_preferences_container_new();
	ElkPreferencesWo *a_prefs = (ElkPreferencesWo *) cow_container_get_configuration(container);
	ElkPreferencesWo *e_prefs = NULL;
	if (elk_preferences_wo_is_anchored(a_prefs)) {
		e_prefs = elk_preferences_wo_create_editable(a_prefs);
	} else {
		e_prefs = cat_ref_ptr(a_prefs);
	}
	cat_log_debug("a_prefs=%p, e_prefs=%o", a_prefs, e_prefs);
	elk_preferences_container_set(result, e_prefs);
	cat_unref_ptr(e_prefs);
	return (CowContainer *) result;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkPreferencesContainer *instance = ELK_PREFERENCES_CONTAINER(self);
	ElkPreferencesContainerPrivate *priv = elk_preferences_container_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	GObject *current = cow_container_get_configuration((CowContainer *) self);
	cat_string_wo_format(append_to, "%s[%p persisted-model=%p, current=%p]", iname, self, priv->persited_model, current);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
