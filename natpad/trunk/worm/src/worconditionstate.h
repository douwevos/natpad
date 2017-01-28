/*
 * worconditionstate.h
 *
 *  Created on: Dec 29, 2011
 *      Author: superman
 */

#ifndef WORCONDITIONSTATE_H_
#define WORCONDITIONSTATE_H_

#include <caterpillar.h>
#include "worcondition.h"
#include "woriconditionobserver.h"

G_BEGIN_DECLS

#define WOR_TYPE_CONDITION_STATE              (wor_condition_state_get_type())
#define WOR_CONDITION_STATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_condition_state_get_type(), WorConditionState))
#define WOR_CONDITION_STATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_CONDITION_STATE, WorConditionStateClass))
#define WOR_IS_CONDITION_STATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_CONDITION_STATE))
#define WOR_IS_CONDITION_STATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_CONDITION_STATE))
#define WOR_CONDITION_STATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_CONDITION_STATE, WorConditionStateClass))
#define WOR_CONDITION_STATE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), WOR_TYPE_CONDITION_STATE, WorConditionStatePrivate))

typedef struct _WorConditionState               WorConditionState;
typedef struct _WorConditionStatePrivate        WorConditionStatePrivate;
typedef struct _WorConditionStateClass          WorConditionStateClass;


struct _WorConditionState {
	GObject parent;
	WorConditionStatePrivate *priv;
};

struct _WorConditionStateClass {
	GObjectClass parent_class;
};


GType wor_condition_state_get_type();

WorConditionState *wor_condition_state_new(WorCondition *condition, WorIConditionObserver *observer);

void wor_condition_state_hold(WorConditionState *state);

gboolean wor_condition_state_peek(WorConditionState *state);


gboolean wor_condition_state_was_selected(WorConditionState *state);


G_END_DECLS



#endif /* WORCONDITIONSTATE_H_ */
