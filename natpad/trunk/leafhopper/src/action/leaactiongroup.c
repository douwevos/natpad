/*
   File:    leaactiongroup.c
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

#include "leaactiongroup.h"
#include "leaactionprivate.h"


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaActionGroup"
#include <logging/catlog.h>

struct _LeaActionGroupPrivate {
	CatHashMapWo *e_map;
};

G_DEFINE_TYPE(LeaActionGroup, lea_action_group, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_dispatch_sensitivity_changed(LeaAction *self, gboolean sensitivity);
static void l_dispatch_visibility_changed(LeaAction *self, gboolean visibility);

static void lea_action_group_class_init(LeaActionGroupClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaActionGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->dispatchSensitivityChanged = l_dispatch_sensitivity_changed;
	action_class->dispatchVisibilityChanged = l_dispatch_visibility_changed;
}

static void lea_action_group_init(LeaActionGroup *node) {
}



static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(object);
	cat_unref_ptr(priv->e_map);
	G_OBJECT_CLASS(lea_action_group_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_action_group_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void lea_action_group_construct(LeaActionGroup *result, CatStringWo *a_group_name, CatStringWo *a_label) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(result);
	priv->e_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	lea_action_construct((LeaAction *) result, a_group_name, a_label, NULL);
}

LeaActionGroup *lea_action_group_new(CatStringWo *a_group_name, CatStringWo *a_label) {
	LeaActionGroup *result = g_object_new(LEA_TYPE_ACTION_GROUP, NULL);
	cat_ref_anounce(result);
	lea_action_group_construct(result, a_group_name, a_label);
	return result;
}


LeaAction *lea_action_group_get(LeaActionGroup *action_group, CatStringWo *action_name) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	LeaAction *result = NULL;
	cat_ref_sink_ptr(action_name);
	result = (LeaAction *) cat_hash_map_wo_get(priv->e_map, (GObject *) action_name);
	cat_unref(action_name);
	return result;
}

LeaActionGroup *lea_action_group_create_sub(LeaActionGroup *action_group, CatStringWo *a_sub_group_name, CatStringWo *a_sub_label) {
	LeaActionGroup *result = lea_action_group_new(a_sub_group_name, a_sub_label);
	lea_action_group_add(action_group, (LeaAction *) result);
	return result;
}


void lea_action_group_add(LeaActionGroup *action_group, LeaAction *action) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	cat_log_debug("adding action:%o to group:%o", action, action_group);
	cat_hash_map_wo_put(priv->e_map, (GObject *) lea_action_get_name(action), (GObject *) action);
	lea_action_set_sensitive_group(action, lea_action_is_sensitive((LeaAction *) action_group));
}

void lea_action_group_remove(LeaActionGroup *action_group, LeaAction *action) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	cat_log_debug("removing action:%o to group:%o", action, action_group);
	GObject *result = cat_hash_map_wo_remove(priv->e_map, (GObject *) lea_action_get_name(action));
	lea_action_set_sensitive_group(action, TRUE);
	cat_unref_ptr(result);
}

CatArrayWo *lea_action_group_enlist_keys(LeaActionGroup *action_group, CatArrayWo *e_use_array) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	return cat_hash_map_wo_enlist_keys(priv->e_map, e_use_array);
}

int lea_action_group_size(LeaActionGroup *action_group) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	return cat_hash_map_wo_size(priv->e_map);
}


CatArrayWo *lea_action_group_enlist_ordered(LeaActionGroup *action_group) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	CatArrayWo *e_result = cat_hash_map_wo_enlist_values(priv->e_map, NULL);
	if (e_result) {
		cat_array_wo_sort(e_result, (GCompareFunc) lea_action_compare);
	}
	return e_result;
}

void lea_action_group_enlist_recursive(LeaActionGroup *action_group, CatArrayWo *e_list, gboolean include_groups) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(action_group);
	if (cat_hash_map_wo_size(priv->e_map)>0) {
		CatArrayWo *e_children = cat_hash_map_wo_enlist_values(priv->e_map, NULL);
		CatIIterator *iter = cat_array_wo_iterator(e_children);
		while(cat_iiterator_has_next(iter)) {
			LeaAction *action = (LeaAction *) cat_iiterator_next(iter);
			if (LEA_IS_ACTION_GROUP(action)) {
				if (include_groups) {
					cat_array_wo_append(e_list, (GObject *) action);
				}
				lea_action_group_enlist_recursive((LeaActionGroup *) action, e_list, include_groups);
			} else {
				cat_array_wo_append(e_list, (GObject *) action);
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(e_children);
	}
}


static void l_dispatch_sensitivity_changed(LeaAction *self, gboolean sensitivity) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(self);

	CatArrayWo *e_children = cat_hash_map_wo_enlist_values(priv->e_map, NULL);
	CatIIterator *iter = cat_array_wo_iterator(e_children);
	while(cat_iiterator_has_next(iter)) {
		LeaAction *action = (LeaAction *) cat_iiterator_next(iter);
		lea_action_set_sensitive_group(action, sensitivity);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(e_children);

	LEA_ACTION_CLASS(lea_action_group_parent_class)->dispatchSensitivityChanged(self, sensitivity);
}


static void l_dispatch_visibility_changed(LeaAction *self, gboolean visibility) {
	LeaActionGroupPrivate *priv = LEA_ACTION_GROUP_GET_PRIVATE(self);

	CatArrayWo *e_children = cat_hash_map_wo_enlist_values(priv->e_map, NULL);
	CatIIterator *iter = cat_array_wo_iterator(e_children);
	while(cat_iiterator_has_next(iter)) {
		LeaAction *action = (LeaAction *) cat_iiterator_next(iter);
		lea_action_set_visible_group(action, visibility);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(e_children);

	LEA_ACTION_CLASS(lea_action_group_parent_class)->dispatchVisibilityChanged(self, visibility);
}
