/*
   File:    leaaction.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jul 31, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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

#include "leaaction.h"
#include "../key/leaikeybinding.h"
#include "../layout/leapanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaAction"
#include <logging/catlog.h>


struct _LeaActionPrivate {
	gboolean disposed;
	CatStringWo *a_name;
	CatStringWo *a_label;
	CatStringWo *a_stock_id;
	CatWeakList *attach_list;
	LeaKeySequence *key_sequence;
	LeaKeySequence *default_key_sequence;
	LeaKeyContext *key_context;
	gboolean is_sensitive_self;
	gboolean is_sensitive_group;
	gboolean is_sensitive;
	gboolean is_visible_self;
	gboolean is_visible_group;
	gboolean is_visible;
	gboolean toggable;
	gboolean is_toggled;
	int order;
};

static void l_key_binding_iface_init(LeaIKeyBindingInterface *_iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaAction, lea_action, G_TYPE_OBJECT, {
			G_IMPLEMENT_INTERFACE(LEA_TYPE_IKEY_BINDING, l_key_binding_iface_init);
			G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_dummy_run(LeaAction *action) {
	cat_log_error("method LeaAction.action_run for class %s not implemented !", g_type_name_from_instance((GTypeInstance *) action));
}
static void l_dispatch_sensitivity_changed(LeaAction *self, gboolean sensitivity);
static void l_dispatch_visibility_changed(LeaAction *self, gboolean sensitivity);
static gboolean l_alculate_actual_sensitivity(LeaAction *self);
static gboolean l_alculate_actual_visibility(LeaAction *self);


static void lea_action_class_init(LeaActionClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaActionPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	clazz->refresh = NULL;
	clazz->action_run = l_dummy_run;
	clazz->calculateActualSensitivity = l_alculate_actual_sensitivity;
	clazz->dispatchSensitivityChanged = l_dispatch_sensitivity_changed;
	clazz->calculateActualVisibility = l_alculate_actual_visibility;
	clazz->dispatchVisibilityChanged = l_dispatch_visibility_changed;
}

static void lea_action_init(LeaAction *node) {
}

static void l_dispose(GObject *object) {
	LeaAction *action = LEA_ACTION(object);
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	if (!priv->disposed) {
		priv->disposed = TRUE;
		cat_unref_ptr(priv->default_key_sequence);
		cat_unref_ptr(priv->key_sequence);
		cat_unref_ptr(priv->attach_list);
		cat_unref_ptr(priv->a_stock_id);
		cat_unref_ptr(priv->a_label);
		cat_unref_ptr(priv->a_name);
		cat_unref_ptr(priv->key_context);
	}
}

void lea_action_construct(LeaAction *action, CatStringWo *a_name, CatStringWo *a_label, CatStringWo *a_stock_id) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(LEA_ACTION(action));

	priv->disposed = FALSE;
	priv->a_name = cat_string_wo_anchor(a_name, 0);
	priv->a_label = cat_string_wo_anchor(a_label, 0);
	priv->a_stock_id = cat_string_wo_anchor(a_stock_id, 0);
	priv->attach_list = cat_weak_list_new();
	priv->is_sensitive = TRUE;
	priv->is_sensitive_self = TRUE;
	priv->is_sensitive_group = TRUE;
	priv->is_visible = TRUE;
	priv->is_visible_self = TRUE;
	priv->is_visible_group = TRUE;
	priv->key_sequence = NULL;
	priv->key_context = NULL;
	priv->default_key_sequence = NULL;
	priv->order = 0;
}


LeaKeyContext *lea_action_get_key_context(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->key_context;
}

void lea_action_set_key_context(LeaAction *action, LeaKeyContext *new_key_context) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	cat_ref_swap(priv->key_context, new_key_context);
}

void lea_action_set_order(LeaAction *action, int order) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	priv->order = order;
}

int lea_action_get_order(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->order;
}


CatStringWo *lea_action_get_name(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->a_name;
}

CatStringWo *lea_action_get_label(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->a_label;
}

void lea_action_attach(LeaAction *action, LeaIAttachable *attachable) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	cat_weak_list_append(priv->attach_list, (GObject *) attachable);
	lea_iattachable_sensitivity_set(attachable, priv->is_sensitive);
	lea_iattachable_toggability_set(attachable, priv->toggable);
	lea_iattachable_toggled(attachable, priv->is_toggled);
}

void lea_action_detach(LeaAction *action, LeaIAttachable *attachable) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	cat_weak_list_remove(priv->attach_list, (GObject *) attachable);
}


CatStringWo *lea_action_get_stock_id(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->a_stock_id;
}

static gboolean l_alculate_actual_sensitivity(LeaAction *self) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	gboolean is_sensitive = priv->is_sensitive_group && priv->is_sensitive_self;
	return is_sensitive;
}

gboolean lea_action_update_sensitivity(LeaAction *action) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	gboolean is_sensitive = LEA_ACTION_GET_CLASS(action)->calculateActualSensitivity(action);
	if (priv->is_sensitive == is_sensitive) {
		return FALSE;
	}
	priv->is_sensitive = is_sensitive;
	LEA_ACTION_GET_CLASS(action)->dispatchSensitivityChanged(action, is_sensitive);
	return TRUE;
}

static void l_dispatch_sensitivity_changed(LeaAction *self, gboolean sensitivity) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	CatIIterator *iterator = cat_weak_list_iterator(priv->attach_list);
	while(cat_iiterator_has_next(iterator)) {
		LeaIAttachable *attachable = (LeaIAttachable *) cat_iiterator_next(iterator);
		lea_iattachable_sensitivity_set(attachable, sensitivity);
	}
	cat_unref(iterator);
}

gboolean lea_action_is_sensitive(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->is_sensitive;
}


void lea_action_set_sensitive_self(LeaAction *action, gboolean flag) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	if (priv->is_sensitive_self==flag) {
		return;
	}
	priv->is_sensitive_self = flag;
	lea_action_update_sensitivity(action);
}

gboolean lea_action_is_sensitive_self(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->is_sensitive_self;
}

void lea_action_set_sensitive_group(LeaAction *action, gboolean flag) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	if (priv->is_sensitive_group==flag) {
		return;
	}
	priv->is_sensitive_group = flag;
	lea_action_update_sensitivity(action);
}

gboolean lea_action_is_sensitive_group(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->is_sensitive_self;
}



static gboolean l_alculate_actual_visibility(LeaAction *self) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	gboolean is_visible = priv->is_visible_group && priv->is_visible_self;
	return is_visible;
}

gboolean lea_action_update_visibility(LeaAction *action) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	gboolean is_visible = LEA_ACTION_GET_CLASS(action)->calculateActualVisibility(action);
	if (priv->is_visible == is_visible) {
		return FALSE;
	}
	priv->is_visible = is_visible;
	LEA_ACTION_GET_CLASS(action)->dispatchVisibilityChanged(action, is_visible);
	return TRUE;
}

static void l_dispatch_visibility_changed(LeaAction *self, gboolean visibility) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	CatIIterator *iterator = cat_weak_list_iterator(priv->attach_list);
	while(cat_iiterator_has_next(iterator)) {
		LeaIAttachable *attachable = (LeaIAttachable *) cat_iiterator_next(iterator);
		lea_iattachable_visibility_set(attachable, visibility);
	}
	cat_unref(iterator);
}

gboolean lea_action_is_visible(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->is_visible;
}


gboolean lea_action_set_visible_self(LeaAction *action, gboolean flag) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	if (priv->is_visible_self==flag) {
		return FALSE;
	}
	priv->is_visible_self = flag;
	return lea_action_update_visibility(action);
}

gboolean lea_action_is_visible_self(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->is_visible_self;
}

void lea_action_set_visible_group(LeaAction *action, gboolean flag) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	if (priv->is_visible_group==flag) {
		return;
	}
	priv->is_visible_group = flag;
	lea_action_update_visibility(action);
}

gboolean lea_action_is_visible_group(LeaAction *action) {
	return LEA_ACTION_GET_PRIVATE(action)->is_visible_self;
}

void lea_action_set_toggable(LeaAction *action, gboolean toggable) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	priv->toggable = toggable;
	CatIIterator *iterator = cat_weak_list_iterator(priv->attach_list);
	while(cat_iiterator_has_next(iterator)) {
		LeaIAttachable *attachable = (LeaIAttachable *) cat_iiterator_next(iterator);
		lea_iattachable_toggability_set(attachable, toggable);
	}
	cat_unref(iterator);
}

void lea_action_set_toggled(LeaAction *action, gboolean toggled) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	if (priv->is_toggled == toggled) {
		return;
	}
	priv->is_toggled = toggled;
	CatIIterator *iterator = cat_weak_list_iterator(priv->attach_list);
	while(cat_iiterator_has_next(iterator)) {
		LeaIAttachable *attachable = (LeaIAttachable *) cat_iiterator_next(iterator);
		lea_iattachable_toggled(attachable, toggled);
	}
	cat_unref(iterator);
}

gboolean lea_action_is_toggled(LeaAction *action) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	return priv->is_toggled;
}


void lea_action_set_default_key_sequence(LeaAction *action, LeaKeySequence *key_sequence) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(action);
	cat_ref_sink_swap(priv->default_key_sequence, key_sequence);
	if (priv->key_sequence==NULL) {
		priv->key_sequence = cat_ref_ptr(key_sequence);
	}
}


int lea_action_compare(LeaAction *action_a, LeaAction *action_b) {
	if (action_a==action_b) {
		return 0;
	} else if (action_a==NULL) {
		return -1;
	} else if (action_b==NULL) {
		return 1;
	}
	LeaActionPrivate *priv_a = LEA_ACTION_GET_PRIVATE(action_a);
	LeaActionPrivate *priv_b = LEA_ACTION_GET_PRIVATE(action_b);
	int order_a = priv_a->order;
	int order_b = priv_b->order;
	if (order_a < order_b) {
		return -1;
	} else if (order_a > order_b) {
		return 1;
	}
	return cat_string_wo_compare(priv_a->a_name, priv_b->a_name);
}


/********************* begin LeaIKeyBinding implementation *********************/

static void l_key_binding_set_key_sequence(LeaIKeyBinding *self, LeaKeySequence *key_sequence) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	cat_ref_sink_swap(priv->key_sequence, key_sequence);
}

static LeaKeySequence *l_key_binding_get_key_sequence(LeaIKeyBinding *self) {
	return LEA_ACTION_GET_PRIVATE(self)->key_sequence;
}

static LeaKeySequence *l_key_binding_get_default_key_sequence(LeaIKeyBinding *self) {
	return LEA_ACTION_GET_PRIVATE(self)->default_key_sequence;
}

static void l_key_binding_activate(LeaIKeyBinding *self) {
	LeaAction *action = LEA_ACTION(self);
	LeaActionClass *action_class = LEA_ACTION_GET_CLASS(action);
	action_class->action_run(action);
}

static LeaKeyContext *l_key_binding_get_key_context(LeaIKeyBinding *self) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	return priv->key_context;
}

static void l_key_binding_iface_init(LeaIKeyBindingInterface *iface) {
	iface->setKeySequence = l_key_binding_set_key_sequence;
	iface->getKeySequence = l_key_binding_get_key_sequence;
	iface->getDefaultKeySequence = l_key_binding_get_default_key_sequence;
	iface->activate = l_key_binding_activate;
	iface->getKeyContext = l_key_binding_get_key_context;
}

/********************* begin LeaIKeyBinding implementation *********************/


/********************* end CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaActionPrivate *priv = LEA_ACTION_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: name=%o, label=%o, sensitive(m/g/a)=%d/%d/%d, visible(m/g/a)=%d/%d/%d]", iname, self, priv->a_name, priv->a_label, priv->is_sensitive_self, priv->is_sensitive_group, priv->is_sensitive, priv->is_visible_self, priv->is_visible_group, priv->is_visible);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
