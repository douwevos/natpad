/*
   File:    jagbytimnemonicblock.c
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

#include "jagbytimnemonicblock.h"

G_DEFINE_INTERFACE(JagBytIMnemonicBlock, jag_byt_imnemonic_block, G_TYPE_OBJECT);

static void jag_byt_imnemonic_block_default_init(JagBytIMnemonicBlockInterface *klass) {
}


JagBytIMnemonic *jag_byt_imnemonic_block_get(JagBytIMnemonicBlock *self, int index) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC_BLOCK(self), NULL);
	return JAG_BYT_IMNEMONIC_BLOCK_GET_INTERFACE(self)->get(self, index);
}

int jag_byt_imnemonic_block_count(JagBytIMnemonicBlock *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC_BLOCK(self), 0);
	return JAG_BYT_IMNEMONIC_BLOCK_GET_INTERFACE(self)->count(self);
}

int jag_byt_imnemonic_block_find_by_bytecode_offset(JagBytIMnemonicBlock *self, int bytecodeOffset) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC_BLOCK(self), -1);
	return JAG_BYT_IMNEMONIC_BLOCK_GET_INTERFACE(self)->findByBytecodeOffset(self, bytecodeOffset);
}

JagBytLabelRepository *jag_byt_imnemonic_block_get_label_repository(JagBytIMnemonicBlock *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC_BLOCK(self), NULL);
	return JAG_BYT_IMNEMONIC_BLOCK_GET_INTERFACE(self)->getLabelRepository(self);
}

