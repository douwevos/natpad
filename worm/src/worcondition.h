/*
 * worcondition.h
 *
 *  Created on: Dec 28, 2011
 *      Author: superman
 */

#ifndef WORCONDITION_H_
#define WORCONDITION_H_

#include <caterpillar.h>
#include "woriconditionobserver.h"

G_BEGIN_DECLS

#define WOR_TYPE_CONDITION              (wor_condition_get_type())
#define WOR_CONDITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_condition_get_type(), WorCondition))
#define WOR_CONDITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_CONDITION, WorConditionClass))
#define WOR_IS_CONDITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_CONDITION))
#define WOR_IS_CONDITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_CONDITION))
#define WOR_CONDITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_CONDITION, WorConditionClass))

typedef struct _WorCondition               WorCondition;
typedef struct _WorConditionPrivate        WorConditionPrivate;
typedef struct _WorConditionClass          WorConditionClass;


struct _WorCondition {
	GObject parent;
};

struct _WorConditionClass {
	GObjectClass parent_class;
};


GType wor_condition_get_type();

WorCondition *wor_condition_new();


int wor_condition_register_observer(WorCondition *condition, WorIConditionObserver *observer);
gboolean wor_condition_unregister_observer(WorCondition *condition, WorIConditionObserver *observer);

void wor_condition_up(WorCondition *condition);

int wor_condition_get_select(WorCondition *condition);

int wor_condition_hash_code(WorCondition *condition);

gboolean wor_condition_equals(WorCondition *condition, WorCondition *other);

G_END_DECLS

#endif /* WORCONDITION_H_ */
