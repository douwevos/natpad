/*
   File:    jagbytimnemonic.h
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


#ifndef JAGBYTIMNEMONIC_H_
#define JAGBYTIMNEMONIC_H_

#include "jagbytoperation.h"
#include <glib-object.h>

#define JAG_BYT_TYPE_IMNEMONIC                 (jag_byt_imnemonic_get_type())
#define JAG_BYT_IMNEMONIC(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_BYT_TYPE_IMNEMONIC, JagBytIMnemonic))
#define JAG_BYT_IS_IMNEMONIC(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_BYT_TYPE_IMNEMONIC))
#define JAG_BYT_IMNEMONIC_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_BYT_TYPE_IMNEMONIC, JagBytIMnemonicInterface))


typedef struct _JagBytIMnemonic               JagBytIMnemonic; /* dummy object */
typedef struct _JagBytIMnemonicInterface      JagBytIMnemonicInterface;

struct _JagBytIMnemonicInterface {
	GTypeInterface parent_iface;

	JagBytOperation (*getOperation)(JagBytIMnemonic *self);
	int (*getOffset)(JagBytIMnemonic *self);
	int (*getLength)(JagBytIMnemonic *self);
	int (*getContinuesOffset)(JagBytIMnemonic *self);
	int (*getBranchOffset)(JagBytIMnemonic *self);
	//String (*toString)(JagBytIMnemonic *self, IMnemonicBlock imnemonicBlock);
	short (*getOppCode)(JagBytIMnemonic *self);

};

GType jag_byt_imnemonic_get_type(void);

JagBytOperation jag_byt_imnemonic_get_operation(JagBytIMnemonic *self);
int jag_byt_imnemonic_get_offset(JagBytIMnemonic *self);
int jag_byt_imnemonic_get_length(JagBytIMnemonic *self);
int jag_byt_imnemonic_get_continues_offset(JagBytIMnemonic *self);
int jag_byt_imnemonic_get_branch_offset(JagBytIMnemonic *self);
//String jag_byt_imnemonic_toString(JagBytIMnemonic *self, IMnemonicBlock imnemonicBlock);
short jag_byt_imnemonic_get_opp_code(JagBytIMnemonic *self);




#endif /* JAGBYTIMNEMONIC_H_ */
