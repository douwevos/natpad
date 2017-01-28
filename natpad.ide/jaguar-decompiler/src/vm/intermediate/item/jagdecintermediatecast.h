/*
   File:    jagdecintermediatecast.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 20, 2012
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


#ifndef JAGDECINTERMEDIATECAST_H_
#define JAGDECINTERMEDIATECAST_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_CAST              (jag_dec_intermediate_cast_get_type())
#define JAG_DEC_INTERMEDIATE_CAST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_cast_get_type(), JagDecIntermediateCast))
#define JAG_DEC_INTERMEDIATE_CAST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_CAST, JagDecIntermediateCastClass))
#define JAG_DEC_IS_INTERMEDIATE_CAST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CAST))
#define JAG_DEC_IS_INTERMEDIATE_CAST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_CAST))
#define JAG_DEC_INTERMEDIATE_CAST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_CAST, JagDecIntermediateCastClass))
#define JAG_DEC_INTERMEDIATE_CAST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_CAST, JagDecIntermediateCastPrivate))

typedef struct _JagDecIntermediateCast               JagDecIntermediateCast;
typedef struct _JagDecIntermediateCastPrivate        JagDecIntermediateCastPrivate;
typedef struct _JagDecIntermediateCastClass          JagDecIntermediateCastClass;


struct _JagDecIntermediateCast {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateCastPrivate *priv;
};

struct _JagDecIntermediateCastClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_cast_get_type();

JagDecIntermediateCast *jag_dec_intermediate_cast_new(int mnemonic_index, JagAstDeclarationType *cast_decl_type);

G_END_DECLS


#endif /* JAGDECINTERMEDIATECAST_H_ */
