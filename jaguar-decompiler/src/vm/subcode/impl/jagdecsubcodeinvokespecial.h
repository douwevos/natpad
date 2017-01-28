/*
   File:    jagdecsubcodeinvokespecial.h
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


#ifndef JAGDECSUBCODEINVOKESPECIAL_H_
#define JAGDECSUBCODEINVOKESPECIAL_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdecconsumer.h"
#include "../jagdecivalueproducer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_INVOKE_SPECIAL              (jag_dec_sub_code_invoke_special_get_type())
#define JAG_DEC_SUB_CODE_INVOKE_SPECIAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_invoke_special_get_type(), JagDecSubCodeInvokeSpecial))
#define JAG_DEC_SUB_CODE_INVOKE_SPECIAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_INVOKE_SPECIAL, JagDecSubCodeInvokeSpecialClass))
#define JAG_DEC_IS_SUB_CODE_INVOKE_SPECIAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_INVOKE_SPECIAL))
#define JAG_DEC_IS_SUB_CODE_INVOKE_SPECIAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_INVOKE_SPECIAL))
#define JAG_DEC_SUB_CODE_INVOKE_SPECIAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_INVOKE_SPECIAL, JagDecSubCodeInvokeSpecialClass))
#define JAG_DEC_SUB_CODE_INVOKE_SPECIAL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_INVOKE_SPECIAL, JagDecSubCodeInvokeSpecialPrivate))

typedef struct _JagDecSubCodeInvokeSpecial               JagDecSubCodeInvokeSpecial;
typedef struct _JagDecSubCodeInvokeSpecialPrivate        JagDecSubCodeInvokeSpecialPrivate;
typedef struct _JagDecSubCodeInvokeSpecialClass          JagDecSubCodeInvokeSpecialClass;


struct _JagDecSubCodeInvokeSpecial {
	JagDecAbstractSubCode parent;
	JagDecSubCodeInvokeSpecialPrivate *priv;
};

struct _JagDecSubCodeInvokeSpecialClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_invoke_special_get_type();

JagDecSubCodeInvokeSpecial *jag_dec_sub_code_invoke_special_new(int mnemonic_index, JagAstDeclarationType *instance_decl_type, JagBytMethodHeader *method_header, int argument_count);

G_END_DECLS

#endif /* JAGDECSUBCODEINVOKESPECIAL_H_ */
