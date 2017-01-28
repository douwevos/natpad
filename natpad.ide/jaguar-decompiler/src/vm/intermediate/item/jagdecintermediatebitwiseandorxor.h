/*
   File:    jagdecintermediatebitwiseandorxor.h
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


#ifndef JAGDECINTERMEDIATEBITWISEANDORXOR_H_
#define JAGDECINTERMEDIATEBITWISEANDORXOR_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_BITWISE_AND_OR_XOR              (jag_dec_intermediate_bitwise_and_or_xor_get_type())
#define JAG_DEC_INTERMEDIATE_BITWISE_AND_OR_XOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_bitwise_and_or_xor_get_type(), JagDecIntermediateBitwiseAndOrXor))
#define JAG_DEC_INTERMEDIATE_BITWISE_AND_OR_XOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_BITWISE_AND_OR_XOR, JagDecIntermediateBitwiseAndOrXorClass))
#define JAG_DEC_IS_INTERMEDIATE_BITWISE_AND_OR_XOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_BITWISE_AND_OR_XOR))
#define JAG_DEC_IS_INTERMEDIATE_BITWISE_AND_OR_XOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_BITWISE_AND_OR_XOR))
#define JAG_DEC_INTERMEDIATE_BITWISE_AND_OR_XOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_BITWISE_AND_OR_XOR, JagDecIntermediateBitwiseAndOrXorClass))
#define JAG_DEC_INTERMEDIATE_BITWISE_AND_OR_XOR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_BITWISE_AND_OR_XOR, JagDecIntermediateBitwiseAndOrXorPrivate))

typedef struct _JagDecIntermediateBitwiseAndOrXor               JagDecIntermediateBitwiseAndOrXor;
typedef struct _JagDecIntermediateBitwiseAndOrXorPrivate        JagDecIntermediateBitwiseAndOrXorPrivate;
typedef struct _JagDecIntermediateBitwiseAndOrXorClass          JagDecIntermediateBitwiseAndOrXorClass;


struct _JagDecIntermediateBitwiseAndOrXor {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateBitwiseAndOrXorPrivate *priv;
};

struct _JagDecIntermediateBitwiseAndOrXorClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_bitwise_and_or_xor_get_type();

JagDecIntermediateBitwiseAndOrXor *jag_dec_intermediate_bitwise_and_or_xor_new(int mnemonic_index, JagBytMathOperator bitwise_operator);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEBITWISEANDORXOR_H_ */
