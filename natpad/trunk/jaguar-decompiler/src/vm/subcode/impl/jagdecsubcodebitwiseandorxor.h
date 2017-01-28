/*
   File:    jagdecsubcodebitwiseandorxor.h
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


#ifndef JAGDECSUBCODEBITWISEANDORXOR_H_
#define JAGDECSUBCODEBITWISEANDORXOR_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdecconsumer.h"
#include "../jagdecivalueproducer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR              (jag_dec_sub_code_bitwise_and_or_xor_get_type())
#define JAG_DEC_SUB_CODE_BITWISE_AND_OR_XOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_bitwise_and_or_xor_get_type(), JagDecSubCodeBitwiseAndOrXor))
#define JAG_DEC_SUB_CODE_BITWISE_AND_OR_XOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR, JagDecSubCodeBitwiseAndOrXorClass))
#define JAG_DEC_IS_SUB_CODE_BITWISE_AND_OR_XOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR))
#define JAG_DEC_IS_SUB_CODE_BITWISE_AND_OR_XOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR))
#define JAG_DEC_SUB_CODE_BITWISE_AND_OR_XOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR, JagDecSubCodeBitwiseAndOrXorClass))
#define JAG_DEC_SUB_CODE_BITWISE_AND_OR_XOR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_BITWISE_AND_OR_XOR, JagDecSubCodeBitwiseAndOrXorPrivate))

typedef struct _JagDecSubCodeBitwiseAndOrXor               JagDecSubCodeBitwiseAndOrXor;
typedef struct _JagDecSubCodeBitwiseAndOrXorPrivate        JagDecSubCodeBitwiseAndOrXorPrivate;
typedef struct _JagDecSubCodeBitwiseAndOrXorClass          JagDecSubCodeBitwiseAndOrXorClass;


struct _JagDecSubCodeBitwiseAndOrXor {
	JagDecAbstractSubCode parent;
	JagDecSubCodeBitwiseAndOrXorPrivate *priv;
};

struct _JagDecSubCodeBitwiseAndOrXorClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_bitwise_and_or_xor_get_type();

JagDecSubCodeBitwiseAndOrXor *jag_dec_sub_code_bitwise_and_or_xor_new(int mnemonic_index, JagBytMathOperator bitwise_operator);

JagDecConsumer *jag_dec_sub_code_bitwise_and_or_xor_get_left_consumer(JagDecSubCodeBitwiseAndOrXor *sub_code_bitwise);

JagDecConsumer *jag_dec_sub_code_bitwise_and_or_xor_get_right_consumer(JagDecSubCodeBitwiseAndOrXor *sub_code_bitwise);

JagDecIValueProducer *jag_dec_sub_code_bitwise_and_or_xor_get_value_producer(JagDecSubCodeBitwiseAndOrXor *sub_code_bitwise);

G_END_DECLS


#endif /* JAGDECSUBCODEBITWISEANDORXOR_H_ */
