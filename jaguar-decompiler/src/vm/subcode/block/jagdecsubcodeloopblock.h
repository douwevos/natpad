/*
   File:    jagdecsubcodeloopblock.h
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


#ifndef JAGDECSUBCODELOOPBLOCK_H_
#define JAGDECSUBCODELOOPBLOCK_H_

#include "../jagdecconsumer.h"
#include "../jagdecisubcode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK              (jag_dec_sub_code_loop_block_get_type())
#define JAG_DEC_SUB_CODE_LOOP_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_sub_code_loop_block_get_type(), JagDecSubCodeLoopBlock))
#define JAG_DEC_SUB_CODE_LOOP_BLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK, JagDecSubCodeLoopBlockClass))
#define JAG_DEC_IS_SUB_CODE_LOOP_BLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK))
#define JAG_DEC_IS_SUB_CODE_LOOP_BLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK))
#define JAG_DEC_SUB_CODE_LOOP_BLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK, JagDecSubCodeLoopBlockClass))
#define JAG_DEC_SUB_CODE_LOOP_BLOCK_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_SUB_CODE_LOOP_BLOCK, JagDecSubCodeLoopBlockPrivate))

typedef struct _JagDecSubCodeLoopBlock               JagDecSubCodeLoopBlock;
typedef struct _JagDecSubCodeLoopBlockPrivate        JagDecSubCodeLoopBlockPrivate;
typedef struct _JagDecSubCodeLoopBlockClass          JagDecSubCodeLoopBlockClass;


struct _JagDecSubCodeLoopBlock {
	GObject parent;
	JagDecSubCodeLoopBlockPrivate *priv;
};

struct _JagDecSubCodeLoopBlockClass {
	GObjectClass parent_class;
};


GType jag_dec_sub_code_loop_block_get_type();

JagDecSubCodeLoopBlock *jag_dec_sub_code_loop_block_new(JagDecConsumer *condition_consumer, JagDecISubCode *child_block);

JagDecConsumer *jag_dec_sub_code_loop_block_get_consumer(JagDecSubCodeLoopBlock *sub_code_loop_block);


G_END_DECLS


#endif /* JAGDECSUBCODELOOPBLOCK_H_ */
