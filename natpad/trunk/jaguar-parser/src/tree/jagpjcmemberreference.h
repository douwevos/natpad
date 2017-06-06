/*
   File:    jagpjcmemberreference.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 7, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef TREE_JAGPJCMEMBERREFERENCE_H_
#define TREE_JAGPJCMEMBERREFERENCE_H_

#include "../code/jagpname.h"
#include "jagpjcfunctionalexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS


typedef enum _JagPReferenceMode JagPReferenceMode;

/**
 * There are two kinds of member references: (i) method references and
 * (ii) constructor references
 */
enum _JagPReferenceMode {
	/** enum constant for method references. */
	JAGP_REFERENCE_MODE_INVOKE,
	/** enum constant for constructor references. */
	JAGP_REFERENCE_MODE_NEW
};


#define JAGP_TYPE_JCMEMBER_REFERENCE              (jagp_jcmember_reference_get_type())
#define JAGP_JCMEMBER_REFERENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcmember_reference_get_type(), JagPJCMemberReference))
#define JAGP_JCMEMBER_REFERENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCMEMBER_REFERENCE, JagPJCMemberReferenceClass))
#define JAGP_IS_JCMEMBER_REFERENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCMEMBER_REFERENCE))
#define JAGP_IS_JCMEMBER_REFERENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCMEMBER_REFERENCE))
#define JAGP_JCMEMBER_REFERENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCMEMBER_REFERENCE, JagPJCMemberReferenceClass))


typedef struct _JagPJCMemberReference               JagPJCMemberReference;
typedef struct _JagPJCMemberReferenceClass          JagPJCMemberReferenceClass;


struct _JagPJCMemberReference {
	JagPJCFunctionalExpression parent;
	JagPReferenceMode mode;
	JagPName *name;
	JagPJCExpression *expr;
	CatArrayWo /*<JCExpression>*/ *typeargs;
};

struct _JagPJCMemberReferenceClass {
	JagPJCFunctionalExpressionClass parent_class;
};


GType jagp_jcmember_reference_get_type();

JagPJCMemberReference *jagp_jcmember_reference_new(JagPReferenceMode mode, JagPName *name, JagPJCExpression *expr, CatArrayWo /*<JCExpression>*/ *typeargs);

G_END_DECLS

#endif /* TREE_JAGPJCMEMBERREFERENCE_H_ */
