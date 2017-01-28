/*
   File:    jagdecsubcodecast.h
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


#ifndef JAGDECSUBCODECAST_H_
#define JAGDECSUBCODECAST_H_


#include "../jagdecabstractsubcode.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_CAST              (jag_dec_sub_code_cast_get_type())
#define JAG_DEC_SUB_CODE_CAST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_cast_get_type(), JagDecSubCodeCast))
#define JAG_DEC_SUB_CODE_CAST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_CAST, JagDecSubCodeCastClass))
#define JAG_DEC_IS_SUB_CODE_CAST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_CAST))
#define JAG_DEC_IS_SUB_CODE_CAST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_CAST))
#define JAG_DEC_SUB_CODE_CAST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_CAST, JagDecSubCodeCastClass))
#define JAG_DEC_SUB_CODE_CAST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_CAST, JagDecSubCodeCastPrivate))

typedef struct _JagDecSubCodeCast               JagDecSubCodeCast;
typedef struct _JagDecSubCodeCastPrivate        JagDecSubCodeCastPrivate;
typedef struct _JagDecSubCodeCastClass          JagDecSubCodeCastClass;


struct _JagDecSubCodeCast {
	JagDecAbstractSubCode parent;
	JagDecSubCodeCastPrivate *priv;
};

struct _JagDecSubCodeCastClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_cast_get_type();

JagDecSubCodeCast *jag_dec_sub_code_cast_new(int mnemonic_index, JagAstDeclarationType *cast_decl_type);


G_END_DECLS

#endif /* JAGDECSUBCODECAST_H_ */
