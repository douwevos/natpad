/*
   File:    jagdecsubcodeblock.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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


#ifndef JAGDECSUBCODEBLOCK_H_
#define JAGDECSUBCODEBLOCK_H_

#include "../jagdecisubcode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_BLOCK              (jag_dec_sub_code_block_get_type())
#define JAG_DEC_SUB_CODE_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_block_get_type(), JagDecSubCodeBlock))
#define JAG_DEC_SUB_CODE_BLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_BLOCK, JagDecSubCodeBlockClass))
#define JAG_DEC_IS_SUB_CODE_BLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_BLOCK))
#define JAG_DEC_IS_SUB_CODE_BLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_BLOCK))
#define JAG_DEC_SUB_CODE_BLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_BLOCK, JagDecSubCodeBlockClass))
#define JAG_DEC_SUB_CODE_BLOCK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_BLOCK, JagDecSubCodeBlockPrivate))

typedef struct _JagDecSubCodeBlock               JagDecSubCodeBlock;
typedef struct _JagDecSubCodeBlockPrivate        JagDecSubCodeBlockPrivate;
typedef struct _JagDecSubCodeBlockClass          JagDecSubCodeBlockClass;


struct _JagDecSubCodeBlock {
	GObject parent;
	JagDecSubCodeBlockPrivate *priv;
};

struct _JagDecSubCodeBlockClass {
	GObjectClass parent_class;
};


GType jag_dec_sub_code_block_get_type();

JagDecSubCodeBlock *jag_dec_sub_code_block_new();


void jag_dec_sub_code_block_append(JagDecSubCodeBlock *block, JagDecISubCode *model);

int jag_dec_sub_code_block_count(JagDecSubCodeBlock *block);

JagDecISubCode *jag_dec_sub_code_block_get(JagDecSubCodeBlock *block, int modelIndex);


G_END_DECLS

#endif /* JAGDECSUBCODEBLOCK_H_ */
