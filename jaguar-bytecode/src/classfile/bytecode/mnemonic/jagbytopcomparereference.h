/*
   File:    jagbytopcomparereference.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 3, 2012
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

#ifndef JAGBYTOPCOMPAREREFERENCE_H_
#define JAGBYTOPCOMPAREREFERENCE_H_

#include "../jagbytabstractmnemonic.h"
#include "../jagbytconditiontype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_COMPARE_REFERENCE              (jag_byt_op_compare_reference_get_type())
#define JAG_BYT_OP_COMPARE_REFERENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_compare_reference_get_type(), JagBytOpCompareReference))
#define JAG_BYT_OP_COMPARE_REFERENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_COMPARE_REFERENCE, JagBytOpCompareReferenceClass))
#define JAG_BYT_IS_OP_COMPARE_REFERENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_COMPARE_REFERENCE))
#define JAG_BYT_IS_OP_COMPARE_REFERENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_COMPARE_REFERENCE))
#define JAG_BYT_OP_COMPARE_REFERENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_COMPARE_REFERENCE, JagBytOpCompareReferenceClass))
#define JAG_BYT_OP_COMPARE_REFERENCE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_OP_COMPARE_REFERENCE, JagBytOpCompareReferencePrivate))

typedef struct _JagBytOpCompareReference               JagBytOpCompareReference;
typedef struct _JagBytOpCompareReferencePrivate        JagBytOpCompareReferencePrivate;
typedef struct _JagBytOpCompareReferenceClass          JagBytOpCompareReferenceClass;


struct _JagBytOpCompareReference {
	JagBytAbstractMnemonic parent;
	JagBytOpCompareReferencePrivate *priv;
};

struct _JagBytOpCompareReferenceClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_compare_reference_get_type();

JagBytOpCompareReference *jag_byt_op_compare_reference_new(JagBytOperation operation, int offset, int branch_offset, JagBytConditionType condition_type, gboolean with_zero);

JagBytConditionType jag_byt_op_compare_reference_get_condition_type(JagBytOpCompareReference *compare_reference);

gboolean jag_byt_op_compare_reference_is_with_zero(JagBytOpCompareReference *compare_reference);

G_END_DECLS

#endif /* JAGBYTOPCOMPAREREFERENCE_H_ */
