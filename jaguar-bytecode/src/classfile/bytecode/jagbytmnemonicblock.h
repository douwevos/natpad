/*
   File:    jagbytmnemonicblock.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#ifndef JAGBYTMNEMONICBLOCK_H_
#define JAGBYTMNEMONICBLOCK_H_

#include "jagbytimnemonic.h"
#include "../label/jagbytlabelrepository.h"
#include "../label/jagbytlabel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_MNEMONIC_BLOCK              (jag_byt_mnemonic_block_get_type())
#define JAG_BYT_MNEMONIC_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_mnemonic_block_get_type(), JagBytMnemonicBlock))
#define JAG_BYT_MNEMONIC_BLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_MNEMONIC_BLOCK, JagBytMnemonicBlockClass))
#define JAG_BYT_IS_MNEMONIC_BLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_MNEMONIC_BLOCK))
#define JAG_BYT_IS_MNEMONIC_BLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_MNEMONIC_BLOCK))
#define JAG_BYT_MNEMONIC_BLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_MNEMONIC_BLOCK, JagBytMnemonicBlockClass))

typedef struct _JagBytMnemonicBlock               JagBytMnemonicBlock;
typedef struct _JagBytMnemonicBlockPrivate        JagBytMnemonicBlockPrivate;
typedef struct _JagBytMnemonicBlockClass          JagBytMnemonicBlockClass;


struct _JagBytMnemonicBlock {
	GObject parent;
};

struct _JagBytMnemonicBlockClass {
	GObjectClass parent_class;
};


GType jag_byt_mnemonic_block_get_type();

JagBytMnemonicBlock *jag_byt_mnemonic_block_new(CatStringWo *e_bytecode);

JagBytLabelRepository *jag_byt_mnemonic_block_get_label_repository(JagBytMnemonicBlock *mnemonic_block);

int jag_byt_mnemonic_block_find_by_bytecode_offset(JagBytMnemonicBlock *mnemonic_block, int bytecode_offset);

int jag_byt_mnemonic_block_count(JagBytMnemonicBlock *mnemonic_block);
JagBytIMnemonic *jag_byt_mnemonic_block_get(JagBytMnemonicBlock *mnemonic_block, int index);

JagBytLabel *jag_byt_mnemonic_block_find_or_create_label_at(JagBytMnemonicBlock *mnemonic_block, int mnemonic_index);

G_END_DECLS

#endif /* JAGBYTMNEMONICBLOCK_H_ */
