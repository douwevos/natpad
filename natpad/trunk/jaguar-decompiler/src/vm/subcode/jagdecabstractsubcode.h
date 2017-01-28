/*
   File:    jagdecabstractsubcode.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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


#ifndef JAGDECABSTRACTSUBCODE_H_
#define JAGDECABSTRACTSUBCODE_H_

#include <caterpillar.h>
#include "jagdecivalueproducer.h"
#include "jagdeciconsumer.h"

G_BEGIN_DECLS

#define JAG_DEC_TYPE_ABSTRACT_SUB_CODE              (jag_dec_abstract_sub_code_get_type())
#define JAG_DEC_ABSTRACT_SUB_CODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_abstract_sub_code_get_type(), JagDecAbstractSubCode))
#define JAG_DEC_ABSTRACT_SUB_CODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_ABSTRACT_SUB_CODE, JagDecAbstractSubCodeClass))
#define JAG_DEC_IS_ABSTRACT_SUB_CODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_ABSTRACT_SUB_CODE))
#define JAG_DEC_IS_ABSTRACT_SUB_CODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_ABSTRACT_SUB_CODE))
#define JAG_DEC_ABSTRACT_SUB_CODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_ABSTRACT_SUB_CODE, JagDecAbstractSubCodeClass))
#define JAG_DEC_ABSTRACT_SUB_CODE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_ABSTRACT_SUB_CODE, JagDecAbstractSubCodePrivate))

typedef struct _JagDecAbstractSubCode               JagDecAbstractSubCode;
typedef struct _JagDecAbstractSubCodePrivate        JagDecAbstractSubCodePrivate;
typedef struct _JagDecAbstractSubCodeClass          JagDecAbstractSubCodeClass;


struct _JagDecAbstractSubCode {
	GObject parent;
	JagDecAbstractSubCodePrivate *priv;
};

struct _JagDecAbstractSubCodeClass {
	GObjectClass parent_class;
};


GType jag_dec_abstract_sub_code_get_type();

void jag_dec_abstract_sub_code_construct(JagDecAbstractSubCode *sub_code, int index);

int jag_dec_abstract_sub_code_get_mnemonic_index(JagDecAbstractSubCode *sub_code);

void jag_dec_abstract_sub_code_set_value_producer(JagDecAbstractSubCode *sub_code, JagDecIValueProducer *value_producer);

void jag_dec_abstract_sub_code_add_consumer(JagDecAbstractSubCode *sub_code, JagDecIConsumer *consumer);


G_END_DECLS


#endif /* JAGDECABSTRACTSUBCODE_H_ */
