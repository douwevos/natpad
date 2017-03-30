/*
   File:    jagdecintermediatefieldput.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 19, 2012
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


#ifndef JAGDECINTERMEDIATEFIELDPUT_H_
#define JAGDECINTERMEDIATEFIELDPUT_H_

#include "../jagdecabstractintermediateitem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT              (jag_dec_intermediate_field_put_get_type())
#define JAG_DEC_INTERMEDIATE_FIELD_PUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_field_put_get_type(), JagDecIntermediateFieldPut))
#define JAG_DEC_INTERMEDIATE_FIELD_PUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT, JagDecIntermediateFieldPutClass))
#define JAG_DEC_IS_INTERMEDIATE_FIELD_PUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT))
#define JAG_DEC_IS_INTERMEDIATE_FIELD_PUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT))
#define JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT, JagDecIntermediateFieldPutClass))
#define JAG_DEC_INTERMEDIATE_FIELD_PUT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_FIELD_PUT, JagDecIntermediateFieldPutPrivate))

typedef struct _JagDecIntermediateFieldPut               JagDecIntermediateFieldPut;
typedef struct _JagDecIntermediateFieldPutPrivate        JagDecIntermediateFieldPutPrivate;
typedef struct _JagDecIntermediateFieldPutClass          JagDecIntermediateFieldPutClass;


struct _JagDecIntermediateFieldPut {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateFieldPutPrivate *priv;
};

struct _JagDecIntermediateFieldPutClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_field_put_get_type();


JagDecIntermediateFieldPut *jag_dec_intermediate_field_put_new(int mnemonic_index, JagAstDeclarationType *field_decl_type, JagAstDeclarationType *field_source, JagAstIdentifier *field_name, gboolean is_non_static);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEFIELDPUT_H_ */