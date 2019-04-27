/*
 * worconditionstate.c
 *
 *  Created on: Dec 29, 2011
 *      Author: superman
 */

#include "worconditionstate.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "WorConditionState"
#include <logging/catlog.h>

struct _WorConditionStatePrivate {
	WorCondition *condition;
	WorIConditionObserver *observer;
	volatile int last_select;
	volatile gboolean hold;
};

G_DEFINE_TYPE_WITH_PRIVATE(WorConditionState, wor_condition_state, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void wor_condition_state_class_init(WorConditionStateClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void wor_condition_state_init(WorConditionState *instance) {
}

static void l_observer_killed(gpointer data, GObject *object_killed);

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorConditionState *instance = WOR_CONDITION_STATE(object);
	WorConditionStatePrivate *priv = wor_condition_state_get_instance_private(instance);
	WorCondition *condition = priv->condition;
	WorIConditionObserver *observer = priv->observer;
	priv->condition = NULL;
	priv->observer = NULL;

	if (observer) {
		g_object_weak_unref((GObject *) observer, (GWeakNotify) l_observer_killed, instance);
	}
	if (condition && observer) {
		wor_condition_unregister_observer(condition, observer);
	}
	cat_unref_ptr(condition);
	G_OBJECT_CLASS(wor_condition_state_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(wor_condition_state_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_observer_killed(gpointer data, GObject *object_killed) {
	l_dispose((GObject *) data);
}


WorConditionState *wor_condition_state_new(WorCondition *condition, WorIConditionObserver *observer) {
	WorConditionState *result = g_object_new(WOR_TYPE_CONDITION_STATE, NULL);
	cat_ref_anounce(result);
	WorConditionStatePrivate *priv = wor_condition_state_get_instance_private(result);
	g_object_weak_ref((GObject *) observer, (GWeakNotify) l_observer_killed, result);
	priv->condition = cat_ref_ptr(condition);
	priv->observer = observer;
	priv->last_select = wor_condition_register_observer(condition, observer);
	priv->hold = FALSE;
	return result;
}


void wor_condition_state_hold(WorConditionState *state) {
	WorConditionStatePrivate *priv = wor_condition_state_get_instance_private(state);
	int cur_select = wor_condition_get_select(priv->condition);
	if (cur_select!=priv->last_select) {
		priv->hold = TRUE;
		priv->last_select = cur_select;
	} else {
		priv->hold = FALSE;
	}
}

gboolean wor_condition_state_peek(WorConditionState *state) {
	WorConditionStatePrivate *priv = wor_condition_state_get_instance_private(state);
	int cur_select = wor_condition_get_select(priv->condition);
	return  cur_select!=priv->last_select;
}

gboolean wor_condition_state_was_selected(WorConditionState *state) {
	WorConditionStatePrivate *priv = wor_condition_state_get_instance_private(state);
	return priv->hold;
}
