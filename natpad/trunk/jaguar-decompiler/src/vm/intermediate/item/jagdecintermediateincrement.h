/*
   File:    jagdecintermediateincrement.h
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


#ifndef JAGDECINTERMEDIATEINCREMENT_H_
#define JAGDECINTERMEDIATEINCREMENT_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_INCREMENT              (jag_dec_intermediate_increment_get_type())
#define JAG_DEC_INTERMEDIATE_INCREMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_increment_get_type(), JagDecIntermediateIncrement))
#define JAG_DEC_INTERMEDIATE_INCREMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_INCREMENT, JagDecIntermediateIncrementClass))
#define JAG_DEC_IS_INTERMEDIATE_INCREMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_INCREMENT))
#define JAG_DEC_IS_INTERMEDIATE_INCREMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_INCREMENT))
#define JAG_DEC_INTERMEDIATE_INCREMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_INCREMENT, JagDecIntermediateIncrementClass))
#define JAG_DEC_INTERMEDIATE_INCREMENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_INCREMENT, JagDecIntermediateIncrementPrivate))

typedef struct _JagDecIntermediateIncrement               JagDecIntermediateIncrement;
typedef struct _JagDecIntermediateIncrementPrivate        JagDecIntermediateIncrementPrivate;
typedef struct _JagDecIntermediateIncrementClass          JagDecIntermediateIncrementClass;


struct _JagDecIntermediateIncrement {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateIncrementPrivate *priv;
};

struct _JagDecIntermediateIncrementClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_increment_get_type();

JagDecIntermediateIncrement *jag_dec_intermediate_increment_new(int mnemonic_index, int frameIndex, int value);

int jag_dec_intermediate_increment_get_frame_index(JagDecIntermediateIncrement *inc);

int jag_dec_intermediate_increment_get_value(JagDecIntermediateIncrement *inc);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEINCREMENT_H_ */
