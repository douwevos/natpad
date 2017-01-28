/*
   File:    jagdecsubcodetrycatchfinal.h
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


#ifndef JAGDECSUBCODETRYCATCHFINAL_H_
#define JAGDECSUBCODETRYCATCHFINAL_H_

#include "../block/jagdecsubcodeblock.h"
#include "jagdecsubcodefinally.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL              (jag_dec_sub_code_try_catch_final_get_type())
#define JAG_DEC_SUB_CODE_TRY_CATCH_FINAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_try_catch_final_get_type(), JagDecSubCodeTryCatchFinal))
#define JAG_DEC_SUB_CODE_TRY_CATCH_FINAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL, JagDecSubCodeTryCatchFinalClass))
#define JAG_DEC_IS_SUB_CODE_TRY_CATCH_FINAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL))
#define JAG_DEC_IS_SUB_CODE_TRY_CATCH_FINAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL))
#define JAG_DEC_SUB_CODE_TRY_CATCH_FINAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL, JagDecSubCodeTryCatchFinalClass))
#define JAG_DEC_SUB_CODE_TRY_CATCH_FINAL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_TRY_CATCH_FINAL, JagDecSubCodeTryCatchFinalPrivate))

typedef struct _JagDecSubCodeTryCatchFinal               JagDecSubCodeTryCatchFinal;
typedef struct _JagDecSubCodeTryCatchFinalPrivate        JagDecSubCodeTryCatchFinalPrivate;
typedef struct _JagDecSubCodeTryCatchFinalClass          JagDecSubCodeTryCatchFinalClass;


struct _JagDecSubCodeTryCatchFinal {
	GObject parent;
	JagDecSubCodeTryCatchFinalPrivate *priv;
};

struct _JagDecSubCodeTryCatchFinalClass {
	GObjectClass parent_class;
};


GType jag_dec_sub_code_try_catch_final_get_type();

JagDecSubCodeTryCatchFinal *jag_dec_sub_code_try_catch_final_new(JagDecSubCodeBlock *sub_code_try, JagDecSubCodeFinally *sub_code_finally, CatArrayWo *e_catch_sub_codes);

G_END_DECLS

#endif /* JAGDECSUBCODETRYCATCHFINAL_H_ */
