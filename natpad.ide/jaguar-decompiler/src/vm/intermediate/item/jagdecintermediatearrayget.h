/*
   File:    jagdecintermediatearrayget.h
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


#ifndef JAGDECINTERMEDIATEARRAYGET_H_
#define JAGDECINTERMEDIATEARRAYGET_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_ARRAY_GET              (jag_dec_intermediate_array_get_get_type())
#define JAG_DEC_INTERMEDIATE_ARRAY_GET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_array_get_get_type(), JagDecIntermediateArrayGet))
#define JAG_DEC_INTERMEDIATE_ARRAY_GET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_ARRAY_GET, JagDecIntermediateArrayGetClass))
#define JAG_DEC_IS_INTERMEDIATE_ARRAY_GET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_ARRAY_GET))
#define JAG_DEC_IS_INTERMEDIATE_ARRAY_GET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_ARRAY_GET))
#define JAG_DEC_INTERMEDIATE_ARRAY_GET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_ARRAY_GET, JagDecIntermediateArrayGetClass))
#define JAG_DEC_INTERMEDIATE_ARRAY_GET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_ARRAY_GET, JagDecIntermediateArrayGetPrivate))

typedef struct _JagDecIntermediateArrayGet               JagDecIntermediateArrayGet;
typedef struct _JagDecIntermediateArrayGetPrivate        JagDecIntermediateArrayGetPrivate;
typedef struct _JagDecIntermediateArrayGetClass          JagDecIntermediateArrayGetClass;


struct _JagDecIntermediateArrayGet {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateArrayGetPrivate *priv;
};

struct _JagDecIntermediateArrayGetClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_array_get_get_type();

JagDecIntermediateArrayGet *jag_dec_intermediate_array_get_new(int mnemonic_index, JagBytType load_type);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEARRAYGET_H_ */
