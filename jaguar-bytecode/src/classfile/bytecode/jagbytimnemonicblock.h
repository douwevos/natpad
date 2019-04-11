/*
   File:    jagbytimnemonicblock.h
   Project: jaguar-bytecode
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


#ifndef JAGBYTIMNEMONICBLOCK_H_
#define JAGBYTIMNEMONICBLOCK_H_

#include "jagbytimnemonic.h"
#include "../label/jagbytlabelrepository.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_IMNEMONIC_BLOCK                 (jag_byt_imnemonic_block_get_type())
#define JAG_BYT_IMNEMONIC_BLOCK(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_BYT_TYPE_IMNEMONIC_BLOCK, JagBytIMnemonicBlock))
#define JAG_BYT_IS_IMNEMONIC_BLOCK(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_BYT_TYPE_IMNEMONIC_BLOCK))
#define JAG_BYT_IMNEMONIC_BLOCK_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_BYT_TYPE_IMNEMONIC_BLOCK, JagBytIMnemonicBlockInterface))


typedef struct _JagBytIMnemonicBlock               JagBytIMnemonicBlock; /* dummy object */
typedef struct _JagBytIMnemonicBlockInterface      JagBytIMnemonicBlockInterface;

struct _JagBytIMnemonicBlockInterface {
	GTypeInterface parent_iface;

	JagBytIMnemonic *(*get)(JagBytIMnemonicBlock *self, int index);
	int (*count)(JagBytIMnemonicBlock *self);
	int (*findByBytecodeOffset)(JagBytIMnemonicBlock *self, int bytecodeOffset);
	JagBytLabelRepository *(*getLabelRepository)(JagBytIMnemonicBlock *self);

};

GType jag_byt_imnemonic_block_get_type(void);


JagBytIMnemonic *jag_byt_imnemonic_block_get(JagBytIMnemonicBlock *self, int index);
int jag_byt_imnemonic_block_count(JagBytIMnemonicBlock *self);
int jag_byt_imnemonic_block_find_by_bytecode_offset(JagBytIMnemonicBlock *self, int bytecodeOffset);
JagBytLabelRepository *jag_byt_imnemonic_block_get_label_repository(JagBytIMnemonicBlock *self);

G_END_DECLS

#endif /* JAGBYTIMNEMONICBLOCK_H_ */
