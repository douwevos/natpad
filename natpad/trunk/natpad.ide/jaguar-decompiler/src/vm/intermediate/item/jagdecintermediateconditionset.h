/*
   File:    jagdecintermediateconditionset.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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


#ifndef JAGDECINTERMEDIATECONDITIONSET_H_
#define JAGDECINTERMEDIATECONDITIONSET_H_


#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET              (jag_dec_intermediate_condition_set_get_type())
#define JAG_DEC_INTERMEDIATE_CONDITION_SET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_condition_set_get_type(), JagDecIntermediateConditionSet))
#define JAG_DEC_INTERMEDIATE_CONDITION_SET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET, JagDecIntermediateConditionSetClass))
#define JAG_DEC_IS_INTERMEDIATE_CONDITION_SET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET))
#define JAG_DEC_IS_INTERMEDIATE_CONDITION_SET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET))
#define JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET, JagDecIntermediateConditionSetClass))
#define JAG_DEC_INTERMEDIATE_CONDITION_SET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CONDITION_SET, JagDecIntermediateConditionSetPrivate))

typedef struct _JagDecIntermediateConditionSet               JagDecIntermediateConditionSet;
typedef struct _JagDecIntermediateConditionSetPrivate        JagDecIntermediateConditionSetPrivate;
typedef struct _JagDecIntermediateConditionSetClass          JagDecIntermediateConditionSetClass;


struct _JagDecIntermediateConditionSet {
	GObject parent;
	JagDecIntermediateConditionSetPrivate *priv;
};

struct _JagDecIntermediateConditionSetClass {
	GObjectClass parent_class;
};


GType jag_dec_intermediate_condition_set_get_type();

JagDecIntermediateConditionSet *jag_dec_intermediate_condition_set_new();

G_END_DECLS


#endif /* JAGDECINTERMEDIATECONDITIONSET_H_ */
