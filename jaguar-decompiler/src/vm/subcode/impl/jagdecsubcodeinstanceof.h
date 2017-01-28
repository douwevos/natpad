/*
   File:    jagdecsubcodeinstanceof.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 20, 2012
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


#ifndef JAGDECSUBCODEINSTANCEOF_H_
#define JAGDECSUBCODEINSTANCEOF_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdecconsumer.h"
#include "../jagdecivalueproducer.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_INSTANCEOF              (jag_dec_sub_code_instanceof_get_type())
#define JAG_DEC_SUB_CODE_INSTANCEOF(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_instanceof_get_type(), JagDecSubCodeInstanceof))
#define JAG_DEC_SUB_CODE_INSTANCEOF_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_INSTANCEOF, JagDecSubCodeInstanceofClass))
#define JAG_DEC_IS_SUB_CODE_INSTANCEOF(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_INSTANCEOF))
#define JAG_DEC_IS_SUB_CODE_INSTANCEOF_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_INSTANCEOF))
#define JAG_DEC_SUB_CODE_INSTANCEOF_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_INSTANCEOF, JagDecSubCodeInstanceofClass))
#define JAG_DEC_SUB_CODE_INSTANCEOF_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_INSTANCEOF, JagDecSubCodeInstanceofPrivate))

typedef struct _JagDecSubCodeInstanceof               JagDecSubCodeInstanceof;
typedef struct _JagDecSubCodeInstanceofPrivate        JagDecSubCodeInstanceofPrivate;
typedef struct _JagDecSubCodeInstanceofClass          JagDecSubCodeInstanceofClass;


struct _JagDecSubCodeInstanceof {
	JagDecAbstractSubCode parent;
	JagDecSubCodeInstanceofPrivate *priv;
};

struct _JagDecSubCodeInstanceofClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_instanceof_get_type();

JagDecSubCodeInstanceof *jag_dec_sub_code_instanceof_new(int mnemonic_index, JagAstDeclarationType *instanceof_decl_type);


G_END_DECLS

#endif /* JAGDECSUBCODEINSTANCEOF_H_ */
