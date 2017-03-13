/*
   File:    jagdecsubcodeinvokestatic.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Jan 1, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef JAGDECSUBCODEINVOKESTATIC_H_
#define JAGDECSUBCODEINVOKESTATIC_H_


#include "../jagdecabstractsubcode.h"
#include "../jagdecconsumer.h"
#include "../jagdecivalueproducer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_INVOKE_STATIC              (jag_dec_sub_code_invoke_static_get_type())
#define JAG_DEC_SUB_CODE_INVOKE_STATIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_invoke_static_get_type(), JagDecSubCodeInvokeStatic))
#define JAG_DEC_SUB_CODE_INVOKE_STATIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_INVOKE_STATIC, JagDecSubCodeInvokeStaticClass))
#define JAG_DEC_IS_SUB_CODE_INVOKE_STATIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_INVOKE_STATIC))
#define JAG_DEC_IS_SUB_CODE_INVOKE_STATIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_INVOKE_STATIC))
#define JAG_DEC_SUB_CODE_INVOKE_STATIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_INVOKE_STATIC, JagDecSubCodeInvokeStaticClass))
#define JAG_DEC_SUB_CODE_INVOKE_STATIC_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_INVOKE_STATIC, JagDecSubCodeInvokeStaticPrivate))

typedef struct _JagDecSubCodeInvokeStatic               JagDecSubCodeInvokeStatic;
typedef struct _JagDecSubCodeInvokeStaticPrivate        JagDecSubCodeInvokeStaticPrivate;
typedef struct _JagDecSubCodeInvokeStaticClass          JagDecSubCodeInvokeStaticClass;


struct _JagDecSubCodeInvokeStatic {
	JagDecAbstractSubCode parent;
	JagDecSubCodeInvokeStaticPrivate *priv;
};

struct _JagDecSubCodeInvokeStaticClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_invoke_static_get_type();

JagDecSubCodeInvokeStatic *jag_dec_sub_code_invoke_static_new(int mnemonic_index, JagAstDeclarationType *class_decl_type, JagBytMethodHeader *method_header, int argument_count);

G_END_DECLS

#endif /* JAGDECSUBCODEINVOKESTATIC_H_ */