/*
   File:    jagdecsubcodefieldput.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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


#ifndef JAGDECSUBCODEFIELDPUT_H_
#define JAGDECSUBCODEFIELDPUT_H_

#include "../jagdecabstractsubcode.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"
#include "../jagdecconsumer.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_FIELD_PUT              (jag_dec_sub_code_field_put_get_type())
#define JAG_DEC_SUB_CODE_FIELD_PUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_field_put_get_type(), JagDecSubCodeFieldPut))
#define JAG_DEC_SUB_CODE_FIELD_PUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_FIELD_PUT, JagDecSubCodeFieldPutClass))
#define JAG_DEC_IS_SUB_CODE_FIELD_PUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_FIELD_PUT))
#define JAG_DEC_IS_SUB_CODE_FIELD_PUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_FIELD_PUT))
#define JAG_DEC_SUB_CODE_FIELD_PUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_FIELD_PUT, JagDecSubCodeFieldPutClass))
#define JAG_DEC_SUB_CODE_FIELD_PUT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_FIELD_PUT, JagDecSubCodeFieldPutPrivate))

typedef struct _JagDecSubCodeFieldPut               JagDecSubCodeFieldPut;
typedef struct _JagDecSubCodeFieldPutPrivate        JagDecSubCodeFieldPutPrivate;
typedef struct _JagDecSubCodeFieldPutClass          JagDecSubCodeFieldPutClass;


struct _JagDecSubCodeFieldPut {
	JagDecAbstractSubCode parent;
	JagDecSubCodeFieldPutPrivate *priv;
};

struct _JagDecSubCodeFieldPutClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_field_put_get_type();

JagDecSubCodeFieldPut *jag_dec_sub_code_field_put_new(int mnemonic_index, JagAstDeclarationType *field_decl_type, JagAstDeclarationType *field_source, JagAstIdentifier *field_name, gboolean is_non_static);

G_END_DECLS


#endif /* JAGDECSUBCODEFIELDPUT_H_ */
