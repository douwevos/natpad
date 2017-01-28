/*
   File:    jagdecsubcodeifthenelse.h
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


#ifndef JAGDECSUBCODEIFTHENELSE_H_
#define JAGDECSUBCODEIFTHENELSE_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdeciconsumer.h"
#include "../jagdecisubcode.h"
#include "../jagdecivalueproducer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE              (jag_dec_sub_code_if_then_else_get_type())
#define JAG_DEC_SUB_CODE_IF_THEN_ELSE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_if_then_else_get_type(), JagDecSubCodeIfThenElse))
#define JAG_DEC_SUB_CODE_IF_THEN_ELSE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE, JagDecSubCodeIfThenElseClass))
#define JAG_DEC_IS_SUB_CODE_IF_THEN_ELSE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE))
#define JAG_DEC_IS_SUB_CODE_IF_THEN_ELSE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE))
#define JAG_DEC_SUB_CODE_IF_THEN_ELSE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE, JagDecSubCodeIfThenElseClass))
#define JAG_DEC_SUB_CODE_IF_THEN_ELSE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_IF_THEN_ELSE, JagDecSubCodeIfThenElsePrivate))

typedef struct _JagDecSubCodeIfThenElse               JagDecSubCodeIfThenElse;
typedef struct _JagDecSubCodeIfThenElsePrivate        JagDecSubCodeIfThenElsePrivate;
typedef struct _JagDecSubCodeIfThenElseClass          JagDecSubCodeIfThenElseClass;


struct _JagDecSubCodeIfThenElse {
	JagDecAbstractSubCode parent;
	JagDecSubCodeIfThenElsePrivate *priv;
};

struct _JagDecSubCodeIfThenElseClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_if_then_else_get_type();

JagDecSubCodeIfThenElse *jag_dec_sub_code_if_then_else_new(JagDecISubCode *then_sub_code, JagDecISubCode *else_sub_code);


G_END_DECLS


#endif /* JAGDECSUBCODEIFTHENELSE_H_ */
