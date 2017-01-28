/*
   File:    jagdecsubcodefinally.h
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


#ifndef JAGDECSUBCODEFINALLY_H_
#define JAGDECSUBCODEFINALLY_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdecisubcode.h"
#include "../jagdecdefaultvalueproducer.h"
#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_FINALLY              (jag_dec_sub_code_finally_get_type())
#define JAG_DEC_SUB_CODE_FINALLY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_finally_get_type(), JagDecSubCodeFinally))
#define JAG_DEC_SUB_CODE_FINALLY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_FINALLY, JagDecSubCodeFinallyClass))
#define JAG_DEC_IS_SUB_CODE_FINALLY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_FINALLY))
#define JAG_DEC_IS_SUB_CODE_FINALLY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_FINALLY))
#define JAG_DEC_SUB_CODE_FINALLY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_FINALLY, JagDecSubCodeFinallyClass))
#define JAG_DEC_SUB_CODE_FINALLY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_FINALLY, JagDecSubCodeFinallyPrivate))

typedef struct _JagDecSubCodeFinally               JagDecSubCodeFinally;
typedef struct _JagDecSubCodeFinallyPrivate        JagDecSubCodeFinallyPrivate;
typedef struct _JagDecSubCodeFinallyClass          JagDecSubCodeFinallyClass;


struct _JagDecSubCodeFinally {
	JagDecAbstractSubCode parent;
	JagDecSubCodeFinallyPrivate *priv;
};

struct _JagDecSubCodeFinallyClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_finally_get_type();

JagDecSubCodeFinally *jag_dec_sub_code_finally_new(int mnemonic_index, JagDecISubCode *block);

JagDecDefaultValueProducer *jag_dec_sub_code_finally_get_value_producer(JagDecSubCodeFinally *sub_code_finally);

JagAstFinallyBlock *jag_dec_sub_code_finally_get_finally_block(JagDecSubCodeFinally *sub_code_finally);

G_END_DECLS


#endif /* JAGDECSUBCODEFINALLY_H_ */
