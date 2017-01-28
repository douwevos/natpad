/*
   File:    jagdecsubcodeincrement.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 18, 2012
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


#ifndef JAGDECSUBCODEINCREMENT_H_
#define JAGDECSUBCODEINCREMENT_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdecivalueproducer.h"
#include "jagdecmethodframevalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_INCREMENT              (jag_dec_sub_code_increment_get_type())
#define JAG_DEC_SUB_CODE_INCREMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_increment_get_type(), JagDecSubCodeIncrement))
#define JAG_DEC_SUB_CODE_INCREMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_INCREMENT, JagDecSubCodeIncrementClass))
#define JAG_DEC_IS_SUB_CODE_INCREMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_INCREMENT))
#define JAG_DEC_IS_SUB_CODE_INCREMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_INCREMENT))
#define JAG_DEC_SUB_CODE_INCREMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_INCREMENT, JagDecSubCodeIncrementClass))
#define JAG_DEC_SUB_CODE_INCREMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_INCREMENT, JagDecSubCodeIncrementPrivate))

typedef struct _JagDecSubCodeIncrement               JagDecSubCodeIncrement;
typedef struct _JagDecSubCodeIncrementPrivate        JagDecSubCodeIncrementPrivate;
typedef struct _JagDecSubCodeIncrementClass          JagDecSubCodeIncrementClass;


struct _JagDecSubCodeIncrement {
	JagDecAbstractSubCode parent;
	JagDecSubCodeIncrementPrivate *priv;
};

struct _JagDecSubCodeIncrementClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_increment_get_type();

JagDecSubCodeIncrement *jag_dec_sub_code_increment_new(int mnemonic_index, JagDecMethodFrameValue *method_frame_value, int increment_value);

JagDecIValueProducer *jag_dec_sub_code_increment_get_value_producer(JagDecSubCodeIncrement *sub_code_increment);

G_END_DECLS


#endif /* JAGDECSUBCODEINCREMENT_H_ */
