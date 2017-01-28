/*
   File:    jagbytconstantnameandtype.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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


#ifndef JAGBYTCONSTANTNAMEANDTP_H_
#define JAGBYTCONSTANTNAMEANDTP_H_

#define JAG_BYT_CONSTANT_NAME_AND_TP_ID  12

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_NAME_AND_TP              (jag_byt_constant_name_and_tp_get_type())
#define JAG_BYT_CONSTANT_NAME_AND_TP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_name_and_tp_get_type(), JagBytConstantNameAndTp))
#define JAG_BYT_CONSTANT_NAME_AND_TP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_NAME_AND_TP, JagBytConstantNameAndTpClass))
#define JAG_BYT_IS_CONSTANT_NAME_AND_TP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_NAME_AND_TP))
#define JAG_BYT_IS_CONSTANT_NAME_AND_TP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_NAME_AND_TP))
#define JAG_BYT_CONSTANT_NAME_AND_TP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_NAME_AND_TP, JagBytConstantNameAndTpClass))
#define JAG_BYT_CONSTANT_NAME_AND_TP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_CONSTANT_NAME_AND_TP, JagBytConstantNameAndTpPrivate))

typedef struct _JagBytConstantNameAndTp               JagBytConstantNameAndTp;
typedef struct _JagBytConstantNameAndTpPrivate        JagBytConstantNameAndTpPrivate;
typedef struct _JagBytConstantNameAndTpClass          JagBytConstantNameAndTpClass;


struct _JagBytConstantNameAndTp {
	GObject parent;
	JagBytConstantNameAndTpPrivate *priv;
};

struct _JagBytConstantNameAndTpClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_name_and_tp_get_type();

JagBytConstantNameAndTp *jag_byt_constant_name_and_tp_new(int name_index, int descriptor_index);


CatStringWo *jag_byt_constant_name_and_tp_get_name(JagBytConstantNameAndTp *name_and_type);
CatStringWo *jag_byt_constant_name_and_tp_get_descriptor(JagBytConstantNameAndTp *name_and_type);


G_END_DECLS

#endif /* JAGBYTCONSTANTNAMEANDTP_H_ */
