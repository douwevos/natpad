/*
   File:    jagdecintermediatefieldget.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 13, 2012
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


#ifndef JAGDECINTERMEDIATEFIELDGET_H_
#define JAGDECINTERMEDIATEFIELDGET_H_

#include "../jagdecabstractintermediateitem.h"
#include "../../../type/jagdecvaluetypedefinitionlist.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET              (jag_dec_intermediate_field_get_get_type())
#define JAG_DEC_INTERMEDIATE_FIELD_GET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_field_get_get_type(), JagDecIntermediateFieldGet))
#define JAG_DEC_INTERMEDIATE_FIELD_GET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET, JagDecIntermediateFieldGetClass))
#define JAG_DEC_IS_INTERMEDIATE_FIELD_GET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET))
#define JAG_DEC_IS_INTERMEDIATE_FIELD_GET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET))
#define JAG_DEC_INTERMEDIATE_FIELD_GET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET, JagDecIntermediateFieldGetClass))
#define JAG_DEC_INTERMEDIATE_FIELD_GET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_FIELD_GET, JagDecIntermediateFieldGetPrivate))

typedef struct _JagDecIntermediateFieldGet               JagDecIntermediateFieldGet;
typedef struct _JagDecIntermediateFieldGetPrivate        JagDecIntermediateFieldGetPrivate;
typedef struct _JagDecIntermediateFieldGetClass          JagDecIntermediateFieldGetClass;


struct _JagDecIntermediateFieldGet {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateFieldGetPrivate *priv;
};

struct _JagDecIntermediateFieldGetClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_field_get_get_type();

JagDecIntermediateFieldGet *jag_dec_intermediate_field_get_new(int mnemonic_index, JagAstDeclarationType *field_type, JagAstDeclarationType *field_source, JagAstIdentifier *field_name, gboolean is_non_static);

G_END_DECLS


#endif /* JAGDECINTERMEDIATEFIELDGET_H_ */
