/*
 * worcondition.c
 *
 *  Created on: Dec 28, 2011
 *      Author: superman
 */


#include "worcondition.h"
#include "woriconditionobserver.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorCondition"
#include <logging/catlog.h>

struct _WorConditionPrivate {
	CatLock *list_lock;
	CatArrayWo *observer_list;
	volatile int select;
};

G_DEFINE_TYPE_WITH_PRIVATE(WorCondition, wor_condition, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void wor_condition_class_init(WorConditionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void wor_condition_init(WorCondition *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorCondition *instance = WOR_CONDITION(object);
	WorConditionPrivate *priv = wor_condition_get_instance_private(instance);
	cat_unref_ptr(priv->observer_list);
	cat_unref_ptr(priv->list_lock);
	G_OBJECT_CLASS(wor_condition_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(wor_condition_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

WorCondition *wor_condition_new() {
	WorCondition *result = g_object_new(WOR_TYPE_CONDITION, NULL);
	cat_ref_anounce(result);
	WorConditionPrivate *priv = wor_condition_get_instance_private(result);
	priv->list_lock = cat_lock_new();
	priv->observer_list = NULL;
	priv->select = 0;
	return result;
}


static void l_observer_died(gpointer data, GObject *object_killed) {
	WorCondition *condition = WOR_CONDITION(data);
	wor_condition_unregister_observer(condition, (WorIConditionObserver *) object_killed);
}


int wor_condition_register_observer(WorCondition *condition, WorIConditionObserver *observer) {
	WorConditionPrivate *priv = wor_condition_get_instance_private(condition);
	g_object_weak_ref((GObject *) observer, l_observer_died, condition);
	cat_lock_lock(priv->list_lock);
	CatArrayWo *new_list = priv->observer_list;
	if (new_list==NULL) {
		new_list = cat_array_wo_new();
	} else {
		new_list = cat_array_wo_clone(new_list, CAT_CLONE_DEPTH_MAIN);
	}
	cat_array_wo_append(new_list, (GObject *) observer);
	cat_ref_swap(priv->observer_list, new_list);
	cat_unref_ptr(observer);
	cat_unref_ptr(new_list);
	int result = priv->select;
	cat_lock_unlock(priv->list_lock);
	return result;
}

gboolean wor_condition_unregister_observer(WorCondition *condition, WorIConditionObserver *observer) {
	gboolean result = FALSE;
	WorConditionPrivate *priv = wor_condition_get_instance_private(condition);
	cat_lock_lock(priv->list_lock);
	if (priv->observer_list!=NULL) {
		int idx = cat_array_wo_find_index(priv->observer_list, observer, -1);
		if (idx>=0) {
			result = TRUE;
			cat_ref_ptr(observer);
			if (cat_array_wo_size(priv->observer_list)==1) {
				cat_unref_ptr(priv->observer_list);
			} else {

				CatArrayWo *new_list = cat_array_wo_clone(priv->observer_list, CAT_CLONE_DEPTH_MAIN);
				cat_array_wo_remove(new_list, idx, NULL);
				cat_ref_swap(priv->observer_list, new_list);
				cat_unref_ptr(new_list);
			}
			g_object_weak_unref((GObject *) observer, l_observer_died, condition);
		}
	}
	cat_lock_unlock(priv->list_lock);
	return result;
}

void wor_condition_up(WorCondition *condition) {
	WorConditionPrivate *priv = wor_condition_get_instance_private(condition);
	cat_lock_lock(priv->list_lock);
	priv->select++;
	CatArrayWo *local_list = cat_ref_ptr(priv->observer_list);
	cat_lock_unlock(priv->list_lock);


	if (local_list) {
		CatIIterator *iter = cat_array_wo_iterator(local_list);
		while(cat_iiterator_has_next(iter)) {
			WorIConditionObserver *observer = WOR_ICONDITION_OBSERVER(cat_iiterator_next(iter));
			wor_icondition_observer_condition_reporting(observer);
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(local_list);
	}
}

int wor_condition_get_select(WorCondition *condition) {
	WorConditionPrivate *priv = wor_condition_get_instance_private(condition);
	cat_lock_lock(priv->list_lock);
	int result = priv->select;
	cat_lock_unlock(priv->list_lock);
	return result;
}

int wor_condition_hash_code(WorCondition *condition) {
	return (int) ((long) condition);	/* a very simple hashcode :) */
}

gboolean wor_condition_equals(WorCondition *condition, WorCondition *other) {
	return condition == other ? TRUE : FALSE;
}
