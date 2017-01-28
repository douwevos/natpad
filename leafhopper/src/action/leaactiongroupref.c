/*
   File:    leaactiongroupref.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jan 2, 2014
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

#include "leaactiongroupref.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaActionGroupRef"
#include <logging/catlog.h>

struct _LeaActionGroupRefPrivate {
	CatArrayWo *e_referenced_groups;
};

static void l_attachable_iface_init(LeaIAttachableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaActionGroupRef, lea_action_group_ref, LEA_TYPE_ACTION_GROUP, {
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IATTACHABLE, l_attachable_iface_init);
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static gboolean l_calculate_actual_sensitivity(LeaAction *self);
static gboolean l_calculate_actual_visibility(LeaAction *self);

static void lea_action_group_ref_class_init(LeaActionGroupRefClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaActionGroupRefPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->calculateActualSensitivity = l_calculate_actual_sensitivity;
	action_class->calculateActualVisibility = l_calculate_actual_visibility;
}

static void lea_action_group_ref_init(LeaActionGroupRef *instance) {
	LeaActionGroupRefPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_ACTION_GROUP_REF, LeaActionGroupRefPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaActionGroupRef *instance = LEA_ACTION_GROUP_REF(object);
	LeaActionGroupRefPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_referenced_groups);
	G_OBJECT_CLASS(lea_action_group_ref_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_action_group_ref_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void lea_action_group_ref_add(LeaActionGroupRef *group_ref, LeaActionGroup *to_ref);


LeaActionGroupRef *lea_action_group_ref_new(LeaActionGroup *action_group) {
	LeaActionGroupRef *result = g_object_new(LEA_TYPE_ACTION_GROUP_REF, NULL);
	cat_ref_anounce(result);
	LeaActionGroupRefPrivate *priv = result->priv;
	priv->e_referenced_groups = cat_array_wo_new();

	if (action_group) {
		CatStringWo *a_name = lea_action_get_name((LeaAction *) action_group);
		CatStringWo *a_label = lea_action_get_label((LeaAction *) action_group);
		lea_action_group_construct((LeaActionGroup *) result, cat_ref_ptr(a_name), cat_ref_ptr(a_label));
		int order = lea_action_get_order((LeaAction *) action_group);
		lea_action_set_order((LeaAction *) result, order);

		lea_action_group_ref_add(result, action_group);
	} else {
		lea_action_group_construct((LeaActionGroup *) result, NULL, NULL);
	}
	return result;
}


void lea_action_group_ref_add(LeaActionGroupRef *group_ref, LeaActionGroup *to_ref) {
	LeaActionGroupRefPrivate *priv = LEA_ACTION_GROUP_REF_GET_PRIVATE(group_ref);
	cat_array_wo_append(priv->e_referenced_groups, (GObject *) to_ref);
	lea_action_attach((LeaAction *) group_ref, (LeaIAttachable *) group_ref);
}

void lea_action_group_ref_remove(LeaActionGroupRef *group_ref, LeaActionGroup *to_ref) {
	LeaActionGroupRefPrivate *priv = LEA_ACTION_GROUP_REF_GET_PRIVATE(group_ref);
	int idx = cat_array_wo_find_index(priv->e_referenced_groups, (GObject *) to_ref, -1);
	if (idx>=0) {
		cat_array_wo_remove(priv->e_referenced_groups, idx, NULL);
	}
	lea_action_detach((LeaAction *) group_ref, (LeaIAttachable *) group_ref);
}

gboolean lea_action_group_ref_is_empty(LeaActionGroupRef *group_ref) {
	LeaActionGroupRefPrivate *priv = LEA_ACTION_GROUP_REF_GET_PRIVATE(group_ref);
	return cat_array_wo_size(priv->e_referenced_groups)==0;
}

void lea_action_group_ref_merge(LeaActionGroupRef *action_group, LeaActionGroup *merge_group) {
	if (lea_action_group_size(merge_group)>0) {
		CatArrayWo *e_listed = lea_action_group_enlist_keys(merge_group, NULL);
		CatIIterator *iter = cat_array_wo_iterator(e_listed);
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *a_child_name = (CatStringWo *) cat_iiterator_next(iter);
			LeaAction *merge_child = lea_action_group_get(merge_group, a_child_name);
			LeaAction *child = lea_action_group_get((LeaActionGroup *) action_group, a_child_name);
			cat_log_debug("child_name=%o, child=%o, merge_child=%o", a_child_name, child, merge_child);
			if (LEA_IS_ACTION_GROUP(merge_child)) {
				/* action groups are referenced */
				if (child==NULL) {
					LeaActionGroupRef *new_group_ref = lea_action_group_ref_new((LeaActionGroup *) merge_child);
					lea_action_group_add((LeaActionGroup *) action_group, (LeaAction *) new_group_ref);
					lea_action_group_ref_merge(new_group_ref, (LeaActionGroup *) merge_child);
					cat_unref_ptr(new_group_ref);
				} else if (LEA_IS_ACTION_GROUP_REF(child)) {
					LeaActionGroupRef *old_group_ref = (LeaActionGroupRef *) child;
					lea_action_group_ref_add(old_group_ref, (LeaActionGroup *) merge_child);
					lea_action_group_ref_merge(old_group_ref, (LeaActionGroup *) merge_child);
				} else {
					cat_log_error("merge collision: main=%o, merge=%o", child, merge_child);
					continue;
				}
			} else {
				/* plane actions are simply added */
				if (child==NULL) {
					lea_action_group_add((LeaActionGroup *) action_group, merge_child);
				} else if (child != merge_child) {
					cat_log_error("merge collision: main=%o, merge=%o", child, merge_child);
				}
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(e_listed);
	}
}

void lea_action_group_ref_unmerge(LeaActionGroupRef *action_group, LeaActionGroup *merge_group) {
	cat_log_debug("unmerge: merge_group=%o", merge_group);
	if (lea_action_group_size(merge_group)>0) {
		CatArrayWo *e_listed = lea_action_group_enlist_keys(merge_group, NULL);
		CatIIterator *iter = cat_array_wo_iterator(e_listed);
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *a_child_name = (CatStringWo *) cat_iiterator_next(iter);
			LeaAction *merge_child = lea_action_group_get(merge_group, a_child_name);
			LeaAction *child = lea_action_group_get((LeaActionGroup *) action_group, a_child_name);

			if (LEA_IS_ACTION_GROUP(merge_child)) {
				/* action groups need to be dereferenced */
				if (LEA_IS_ACTION_GROUP_REF(child)) {
					LeaActionGroupRef *ref = (LeaActionGroupRef *) child;
					lea_action_group_ref_unmerge(ref, (LeaActionGroup *) merge_child);
					lea_action_group_ref_remove(ref, (LeaActionGroup *) merge_child);
					if (lea_action_group_ref_is_empty(ref) || lea_action_group_size((LeaActionGroup *) ref)==0) {
						lea_action_group_remove((LeaActionGroup *) action_group, child);
					}
				} else {
					cat_log_error("Child is not a reference container: %o", child);
				}
			} else {
				/* plane actions are simply removed */
				if (child==merge_child) {
					lea_action_group_remove((LeaActionGroup *) action_group, merge_child);
				} else {
					cat_log_error("non existing child: %o", merge_child);
				}
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(e_listed);
	}
}




static gboolean l_calculate_actual_sensitivity(LeaAction *self) {
	LeaActionGroupRefPrivate *priv = LEA_ACTION_GROUP_REF_GET_PRIVATE(self);
	gboolean is_sensitive = LEA_ACTION_CLASS(lea_action_group_ref_parent_class)->calculateActualSensitivity(self);
	if (is_sensitive) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_referenced_groups);
		while(cat_iiterator_has_next(iter)) {
			LeaAction *child = (LeaAction *) cat_iiterator_next(iter);
			if (!lea_action_is_sensitive(child)) {
				is_sensitive = FALSE;
				break;
			}
		}
		cat_unref_ptr(iter);
	}
	return is_sensitive;
}

static gboolean l_calculate_actual_visibility(LeaAction *self) {
	LeaActionGroupRefPrivate *priv = LEA_ACTION_GROUP_REF_GET_PRIVATE(self);
	gboolean is_visible = LEA_ACTION_CLASS(lea_action_group_ref_parent_class)->calculateActualVisibility(self);
	if (is_visible) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_referenced_groups);
		while(cat_iiterator_has_next(iter)) {
			LeaAction *child = (LeaAction *) cat_iiterator_next(iter);
			if (!lea_action_is_visible(child)) {
				is_visible = FALSE;
				break;
			}
		}
		cat_unref_ptr(iter);
	}
	return is_visible;
}


/********************* begin LeaIAttachable implementation *********************/


static void l_attachable_sensitivity_set(LeaIAttachable *self, gboolean sensitivity) {
	lea_action_update_sensitivity(LEA_ACTION(self));
}

static void l_attachable_visibility_set(LeaIAttachable *self, gboolean sensitivity) {
	lea_action_update_visibility(LEA_ACTION(self));
}


static void l_attachable_iface_init(LeaIAttachableInterface *iface) {
	iface->sensitivitySet = l_attachable_sensitivity_set;
	iface->visibilitySet = l_attachable_visibility_set;
}

/********************* end LeaIAttachable implementation *********************/
