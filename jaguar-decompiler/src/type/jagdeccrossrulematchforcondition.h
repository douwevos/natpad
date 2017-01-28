/*
   File:    jagdeccrossrulematchforcondition.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef JAGDECCROSSRULEMATCHFORCONDITION_H_
#define JAGDECCROSSRULEMATCHFORCONDITION_H_


#include "jagdecvaluetypedefinitionlist.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION              (jag_dec_cross_rule_match_for_condition_get_type())
#define JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_cross_rule_match_for_condition_get_type(), JagDecCrossRuleMatchForCondition))
#define JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION, JagDecCrossRuleMatchForConditionClass))
#define JAG_DEC_IS_CROSS_RULE_MATCH_FOR_CONDITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION))
#define JAG_DEC_IS_CROSS_RULE_MATCH_FOR_CONDITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION))
#define JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION, JagDecCrossRuleMatchForConditionClass))
#define JAG_DEC_CROSS_RULE_MATCH_FOR_CONDITION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_CROSS_RULE_MATCH_FOR_CONDITION, JagDecCrossRuleMatchForConditionPrivate))

typedef struct _JagDecCrossRuleMatchForCondition               JagDecCrossRuleMatchForCondition;
typedef struct _JagDecCrossRuleMatchForConditionPrivate        JagDecCrossRuleMatchForConditionPrivate;
typedef struct _JagDecCrossRuleMatchForConditionClass          JagDecCrossRuleMatchForConditionClass;


struct _JagDecCrossRuleMatchForCondition {
	GObject parent;
	JagDecCrossRuleMatchForConditionPrivate *priv;
};

struct _JagDecCrossRuleMatchForConditionClass {
	GObjectClass parent_class;
};


GType jag_dec_cross_rule_match_for_condition_get_type();

JagDecCrossRuleMatchForCondition *jag_dec_cross_rule_match_for_condition_new(JagDecValueTypeDefinitionList *type_set_a, JagDecValueTypeDefinitionList *type_set_b);

G_END_DECLS


#endif /* JAGDECCROSSRULEMATCHFORCONDITION_H_ */
