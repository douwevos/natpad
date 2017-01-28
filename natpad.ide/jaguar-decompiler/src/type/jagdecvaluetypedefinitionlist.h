/*
   File:    jagdecvaluetypedefinitionlist.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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


#ifndef JAGDECVALUETYPEDEFINITIONLIST_H_
#define JAGDECVALUETYPEDEFINITIONLIST_H_

#include "jagdecvaluetypedefinition.h"
#include "jagdecicrossrule.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST              (jag_dec_value_type_definition_list_get_type())
#define JAG_DEC_VALUE_TYPE_DEFINITION_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_value_type_definition_list_get_type(), JagDecValueTypeDefinitionList))
#define JAG_DEC_VALUE_TYPE_DEFINITION_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST, JagDecValueTypeDefinitionListClass))
#define JAG_DEC_IS_VALUE_TYPE_DEFINITION_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST))
#define JAG_DEC_IS_VALUE_TYPE_DEFINITION_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST))
#define JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST, JagDecValueTypeDefinitionListClass))
#define JAG_DEC_VALUE_TYPE_DEFINITION_LIST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST, JagDecValueTypeDefinitionListPrivate))

typedef struct _JagDecValueTypeDefinitionList               JagDecValueTypeDefinitionList;
typedef struct _JagDecValueTypeDefinitionListPrivate        JagDecValueTypeDefinitionListPrivate;
typedef struct _JagDecValueTypeDefinitionListClass          JagDecValueTypeDefinitionListClass;


struct _JagDecValueTypeDefinitionList {
	GObject parent;
	JagDecValueTypeDefinitionListPrivate *priv;
};

struct _JagDecValueTypeDefinitionListClass {
	GObjectClass parent_class;
};


GType jag_dec_value_type_definition_list_get_type();

void jag_dec_value_type_definition_list_construct(JagDecValueTypeDefinitionList *value_type_definition_list);

JagDecValueTypeDefinitionList *jag_dec_value_type_definition_list_new();

JagDecValueTypeDefinitionList *jag_dec_value_type_definition_list_new_hidden_exception();

gboolean jag_dec_value_type_definition_list_is_empty(JagDecValueTypeDefinitionList *list);

void jag_dec_value_type_definition_list_add_definition(JagDecValueTypeDefinitionList *list, JagDecValueTypeDefinition *definition);

void jag_dec_value_type_definition_list_add_basic(JagDecValueTypeDefinitionList *list, JagBytType primitive_type, JagDecValueTypeDefinitionKind kind);

CatIIterator *jag_dec_value_type_definition_list_iterator(JagDecValueTypeDefinitionList *list);


void jag_dec_value_type_definition_list_add_cross_rule(JagDecValueTypeDefinitionList *list, JagDecICrossRule *cross_rule);

gboolean jag_dec_value_type_definition_list_apply_cross_rules(JagDecValueTypeDefinitionList *list);

gboolean jag_dec_value_type_definition_list_all_cross_rules_applied(JagDecValueTypeDefinitionList *list);

gboolean jag_dec_value_type_definition_list_cross_rules_running(JagDecValueTypeDefinitionList *list);

int jag_dec_value_type_definition_list_hash(JagDecValueTypeDefinitionList *list);

JagAstDeclarationType *jag_dec_value_type_definition_list_get_best_type_ref(JagDecValueTypeDefinitionList *list);


gboolean jag_dec_value_type_definition_list_equal(JagDecValueTypeDefinitionList *list_a, JagDecValueTypeDefinitionList *list_b);

CatHashSet *jag_dec_value_type_definition_list_enlist_included(JagDecValueTypeDefinitionList *list);

void jag_dec_value_type_definition_list_exclude_set(JagDecValueTypeDefinitionList *list, CatHashSet *set_to_exclude, gboolean opposite);

void jag_dec_value_type_definition_list_copy_to(JagDecValueTypeDefinitionList *list, JagDecValueTypeDefinitionList *dest);


G_END_DECLS


#endif /* JAGDECVALUETYPEDEFINITIONLIST_H_ */
