/*
   File:    jagdeccrossrulearrayget.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 14, 2012
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


#ifndef JAGDECCROSSRULEARRAYGET_H_
#define JAGDECCROSSRULEARRAYGET_H_

#include "jagdecvaluetypedefinitionlist.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET              (jag_dec_cross_rule_array_get_get_type())
#define JAG_DEC_CROSS_RULE_ARRAY_GET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_cross_rule_array_get_get_type(), JagDecCrossRuleArrayGet))
#define JAG_DEC_CROSS_RULE_ARRAY_GET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET, JagDecCrossRuleArrayGetClass))
#define JAG_DEC_IS_CROSS_RULE_ARRAY_GET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET))
#define JAG_DEC_IS_CROSS_RULE_ARRAY_GET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET))
#define JAG_DEC_CROSS_RULE_ARRAY_GET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET, JagDecCrossRuleArrayGetClass))
#define JAG_DEC_CROSS_RULE_ARRAY_GET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_CROSS_RULE_ARRAY_GET, JagDecCrossRuleArrayGetPrivate))

typedef struct _JagDecCrossRuleArrayGet               JagDecCrossRuleArrayGet;
typedef struct _JagDecCrossRuleArrayGetPrivate        JagDecCrossRuleArrayGetPrivate;
typedef struct _JagDecCrossRuleArrayGetClass          JagDecCrossRuleArrayGetClass;


struct _JagDecCrossRuleArrayGet {
	GObject parent;
	JagDecCrossRuleArrayGetPrivate *priv;
};

struct _JagDecCrossRuleArrayGetClass {
	GObjectClass parent_class;
};


GType jag_dec_cross_rule_array_get_get_type();

JagDecCrossRuleArrayGet *jag_dec_cross_rule_array_get_new(JagDecValueTypeDefinitionList *array_type_set, JagDecValueTypeDefinitionList *result_type_set);

G_END_DECLS


#endif /* JAGDECCROSSRULEARRAYGET_H_ */
