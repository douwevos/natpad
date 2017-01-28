/*
   File:    jagdecsubcodecatch.h
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


#ifndef JAGDECSUBCODECATCH_H_
#define JAGDECSUBCODECATCH_H_

#include "../jagdecabstractsubcode.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_CATCH              (jag_dec_sub_code_catch_get_type())
#define JAG_DEC_SUB_CODE_CATCH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_catch_get_type(), JagDecSubCodeCatch))
#define JAG_DEC_SUB_CODE_CATCH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_CATCH, JagDecSubCodeCatchClass))
#define JAG_DEC_IS_SUB_CODE_CATCH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_CATCH))
#define JAG_DEC_IS_SUB_CODE_CATCH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_CATCH))
#define JAG_DEC_SUB_CODE_CATCH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_CATCH, JagDecSubCodeCatchClass))
#define JAG_DEC_SUB_CODE_CATCH_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_CATCH, JagDecSubCodeCatchPrivate))

typedef struct _JagDecSubCodeCatch               JagDecSubCodeCatch;
typedef struct _JagDecSubCodeCatchPrivate        JagDecSubCodeCatchPrivate;
typedef struct _JagDecSubCodeCatchClass          JagDecSubCodeCatchClass;


struct _JagDecSubCodeCatch {
	JagDecAbstractSubCode parent;
	JagDecSubCodeCatchPrivate *priv;
};

struct _JagDecSubCodeCatchClass {
	JagDecAbstractSubCodeClass parent_class;
};


GType jag_dec_sub_code_catch_get_type();

JagDecSubCodeCatch *jag_dec_sub_code_catch_new();

JagAstCatchBlock *jag_dec_sub_code_catch_get_catch_block(JagDecSubCodeCatch *sub_code_catch);

G_END_DECLS


#endif /* JAGDECSUBCODECATCH_H_ */
