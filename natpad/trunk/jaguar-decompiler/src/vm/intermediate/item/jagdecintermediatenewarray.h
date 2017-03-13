/*
   File:    jagdecintermediatenewarray.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 14, 2012
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


#ifndef JAGDECINTERMEDIATENEWARRAY_H_
#define JAGDECINTERMEDIATENEWARRAY_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_NEW_ARRAY              (jag_dec_intermediate_new_array_get_type())
#define JAG_DEC_INTERMEDIATE_NEW_ARRAY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_new_array_get_type(), JagDecIntermediateNewArray))
#define JAG_DEC_INTERMEDIATE_NEW_ARRAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_NEW_ARRAY, JagDecIntermediateNewArrayClass))
#define JAG_DEC_IS_INTERMEDIATE_NEW_ARRAY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_NEW_ARRAY))
#define JAG_DEC_IS_INTERMEDIATE_NEW_ARRAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_NEW_ARRAY))
#define JAG_DEC_INTERMEDIATE_NEW_ARRAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_NEW_ARRAY, JagDecIntermediateNewArrayClass))
#define JAG_DEC_INTERMEDIATE_NEW_ARRAY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_NEW_ARRAY, JagDecIntermediateNewArrayPrivate))

typedef struct _JagDecIntermediateNewArray               JagDecIntermediateNewArray;
typedef struct _JagDecIntermediateNewArrayPrivate        JagDecIntermediateNewArrayPrivate;
typedef struct _JagDecIntermediateNewArrayClass          JagDecIntermediateNewArrayClass;


struct _JagDecIntermediateNewArray {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateNewArrayPrivate *priv;
};

struct _JagDecIntermediateNewArrayClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_new_array_get_type();

JagDecIntermediateNewArray *jag_dec_intermediate_new_array_new(int mnemonic_index, JagAstDeclarationType *array_declaration_type);

G_END_DECLS


#endif /* JAGDECINTERMEDIATENEWARRAY_H_ */