/*
   File:    jagbytimnemonic.c
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

#include "jagbytimnemonic.h"

G_DEFINE_INTERFACE(JagBytIMnemonic, jag_byt_imnemonic, G_TYPE_OBJECT);

static void jag_byt_imnemonic_default_init(JagBytIMnemonicInterface *klass) {
	klass->toString = NULL;
}


JagBytOperation jag_byt_imnemonic_get_operation(JagBytIMnemonic *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), -1);
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->getOperation(self);
}

int jag_byt_imnemonic_get_offset(JagBytIMnemonic *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), -1);
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->getOffset(self);
}


int jag_byt_imnemonic_get_length(JagBytIMnemonic *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), 0);
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->getLength(self);
}


int jag_byt_imnemonic_get_continues_offset(JagBytIMnemonic *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), -1);
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->getContinuesOffset(self);
}


int jag_byt_imnemonic_get_branch_offset(JagBytIMnemonic *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), -1);
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->getBranchOffset(self);
}

//String jag_byt_imnemonic_toString(JagBytIMnemonic *self, IMnemonicBlock imnemonicBlock) {
//	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), NULL);
//	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->get(self, index);
//}

short jag_byt_imnemonic_get_opp_code(JagBytIMnemonic *self) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), -1);
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->getOppCode(self);
}

CatStringWo *jag_byt_imnemonic_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	g_return_val_if_fail(JAG_BYT_IS_IMNEMONIC(self), NULL);
	JagBytIMnemonicInterface *iface = JAG_BYT_IMNEMONIC_GET_INTERFACE(self);
	if (iface->toString==NULL) {
		return NULL;
	}
	return JAG_BYT_IMNEMONIC_GET_INTERFACE(self)->toString(self, label_repository);

}


