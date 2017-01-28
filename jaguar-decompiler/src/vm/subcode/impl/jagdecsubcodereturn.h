/*
   File:    jagdecsubcodereturn.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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


#ifndef JAGDECSUBCODERETURN_H_
#define JAGDECSUBCODERETURN_H_

#include "../jagdecabstractsubcode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_RETURN              (jag_dec_sub_code_return_get_type())
#define JAG_DEC_SUB_CODE_RETURN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_return_get_type(), JagDecSubCodeReturn))
#define JAG_DEC_SUB_CODE_RETURN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_RETURN, JagDecSubCodeReturnClass))
#define JAG_DEC_IS_SUB_CODE_RETURN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_RETURN))
#define JAG_DEC_IS_SUB_CODE_RETURN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_RETURN))
#define JAG_DEC_SUB_CODE_RETURN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_RETURN, JagDecSubCodeReturnClass))
#define JAG_DEC_SUB_CODE_RETURN_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_RETURN, JagDecSubCodeReturnPrivate))

typedef struct _JagDecSubCodeReturn               JagDecSubCodeReturn;
typedef struct _JagDecSubCodeReturnPrivate        JagDecSubCodeReturnPrivate;
typedef struct _JagDecSubCodeReturnClass          JagDecSubCodeReturnClass;


struct _JagDecSubCodeReturn {
	JagDecAbstractSubCode parent;
	JagDecSubCodeReturnPrivate *priv;
};

struct _JagDecSubCodeReturnClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_return_get_type();

JagDecSubCodeReturn *jag_dec_sub_code_return_new(int mnemonic_index, JagAstDeclarationType *return_type);


G_END_DECLS


#endif /* JAGDECSUBCODERETURN_H_ */
