/*
   File:    jagdecintermediategoto.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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


#ifndef JAGDECINTERMEDIATEGOTO_H_
#define JAGDECINTERMEDIATEGOTO_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_GOTO              (jag_dec_intermediate_goto_get_type())
#define JAG_DEC_INTERMEDIATE_GOTO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_goto_get_type(), JagDecIntermediateGoto))
#define JAG_DEC_INTERMEDIATE_GOTO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_GOTO, JagDecIntermediateGotoClass))
#define JAG_DEC_IS_INTERMEDIATE_GOTO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_GOTO))
#define JAG_DEC_IS_INTERMEDIATE_GOTO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_GOTO))
#define JAG_DEC_INTERMEDIATE_GOTO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_GOTO, JagDecIntermediateGotoClass))
#define JAG_DEC_INTERMEDIATE_GOTO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_GOTO, JagDecIntermediateGotoPrivate))

typedef struct _JagDecIntermediateGoto               JagDecIntermediateGoto;
typedef struct _JagDecIntermediateGotoPrivate        JagDecIntermediateGotoPrivate;
typedef struct _JagDecIntermediateGotoClass          JagDecIntermediateGotoClass;


struct _JagDecIntermediateGoto {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateGotoPrivate *priv;
};

struct _JagDecIntermediateGotoClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_goto_get_type();

JagDecIntermediateGoto *jag_dec_intermediate_goto_new(int mnemonic_index, int exitIndex);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEGOTO_H_ */
