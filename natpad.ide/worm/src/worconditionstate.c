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

G_DEFINE_TYPE (WorConditionState, wor_condition_state, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void wor_condition_state_class_init(WorConditionStateClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(WorConditionStatePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void wor_condition_state_init(WorConditionState *instance) {
	WorConditionStatePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, WOR_TYPE_CONDITION_STATE, WorConditionStatePrivate);
	instance->priv = priv;
}
static void l_observer_killed(gpointer data, GObject *object_killed);

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	WorConditionState *instance = WOR_CONDITION_STATE(object);
	WorConditionStatePrivate *priv = instance->priv;
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
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_observer_killed(gpointer data, GObject *object_killed) {
//	WorConditionStatePrivate *priv = WOR_CONDITION_STATE_GET_PRIVATE(data);
	_dispose((GObject *) data);
}


WorConditionState *wor_condition_state_new(WorCondition *condition, WorIConditionObserver *observer) {
	WorConditionState *result = g_object_new(WOR_TYPE_CONDITION_STATE, NULL);
	cat_ref_anounce(result);
	WorConditionStatePrivate *priv = result->priv;
	g_object_weak_ref((GObject *) observer, (GWeakNotify) l_observer_killed, result);
	priv->condition = cat_ref_ptr(condition);
	priv->observer = observer;
	priv->last_select = wor_condition_register_observer(condition, observer);
	priv->hold = FALSE;
	return result;
}


void wor_condition_state_hold(WorConditionState *state) {
	WorConditionStatePrivate *priv = WOR_CONDITION_STATE_GET_PRIVATE(state);
	int cur_select = wor_condition_get_select(priv->condition);
	if (cur_select!=priv->last_select) {
		priv->hold = TRUE;
		priv->last_select = cur_select;
	} else {
		priv->hold = FALSE;
	}
}


gboolean wor_condition_state_peek(WorConditionState *state) {
	WorConditionStatePrivate *priv = WOR_CONDITION_STATE_GET_PRIVATE(state);
	int cur_select = wor_condition_get_select(priv->condition);
	return  cur_select!=priv->last_select;
}


gboolean wor_condition_state_was_selected(WorConditionState *state) {
	WorConditionStatePrivate *priv = WOR_CONDITION_STATE_GET_PRIVATE(state);
	return priv->hold;
}



