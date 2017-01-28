/*
   File:    jagdecsubcodearrayput.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 14, 2012
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


#ifndef JAGDECSUBCODEARRAYPUT_H_
#define JAGDECSUBCODEARRAYPUT_H_

#include "../jagdecabstractsubcode.h"
#include "../jagdecconsumer.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_ARRAY_PUT              (jag_dec_sub_code_array_put_get_type())
#define JAG_DEC_SUB_CODE_ARRAY_PUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_array_put_get_type(), JagDecSubCodeArrayPut))
#define JAG_DEC_SUB_CODE_ARRAY_PUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_ARRAY_PUT, JagDecSubCodeArrayPutClass))
#define JAG_DEC_IS_SUB_CODE_ARRAY_PUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_ARRAY_PUT))
#define JAG_DEC_IS_SUB_CODE_ARRAY_PUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_ARRAY_PUT))
#define JAG_DEC_SUB_CODE_ARRAY_PUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_ARRAY_PUT, JagDecSubCodeArrayPutClass))
#define JAG_DEC_SUB_CODE_ARRAY_PUT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_ARRAY_PUT, JagDecSubCodeArrayPutPrivate))

typedef struct _JagDecSubCodeArrayPut               JagDecSubCodeArrayPut;
typedef struct _JagDecSubCodeArrayPutPrivate        JagDecSubCodeArrayPutPrivate;
typedef struct _JagDecSubCodeArrayPutClass          JagDecSubCodeArrayPutClass;


struct _JagDecSubCodeArrayPut {
	JagDecAbstractSubCode parent;
	JagDecSubCodeArrayPutPrivate *priv;
};

struct _JagDecSubCodeArrayPutClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_array_put_get_type();

JagDecSubCodeArrayPut *jag_dec_sub_code_array_put_new(int mnemonic_index);


G_END_DECLS


#endif /* JAGDECSUBCODEARRAYPUT_H_ */
