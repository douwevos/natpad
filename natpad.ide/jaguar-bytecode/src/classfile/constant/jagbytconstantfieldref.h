/*
   File:    jagbytconstantfieldref.h
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


#ifndef JAGBYTCONSTANTFIELDREF_H_
#define JAGBYTCONSTANTFIELDREF_H_

#define JAG_BYT_CONSTANT_FIELDREF_ID  9

#include "../../ast/jagastdeclarationtype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_FIELDREF              (jag_byt_constant_fieldref_get_type())
#define JAG_BYT_CONSTANT_FIELDREF(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_fieldref_get_type(), JagBytConstantFieldref))
#define JAG_BYT_CONSTANT_FIELDREF_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_FIELDREF, JagBytConstantFieldrefClass))
#define JAG_BYT_IS_CONSTANT_FIELDREF(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_FIELDREF))
#define JAG_BYT_IS_CONSTANT_FIELDREF_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_FIELDREF))
#define JAG_BYT_CONSTANT_FIELDREF_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_FIELDREF, JagBytConstantFieldrefClass))
#define JAG_BYT_CONSTANT_FIELDREF_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_CONSTANT_FIELDREF, JagBytConstantFieldrefPrivate))

typedef struct _JagBytConstantFieldref               JagBytConstantFieldref;
typedef struct _JagBytConstantFieldrefPrivate        JagBytConstantFieldrefPrivate;
typedef struct _JagBytConstantFieldrefClass          JagBytConstantFieldrefClass;


struct _JagBytConstantFieldref {
	GObject parent;
	JagBytConstantFieldrefPrivate *priv;
};

struct _JagBytConstantFieldrefClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_fieldref_get_type();

JagBytConstantFieldref *jag_byt_constant_fieldref_new(int class_index, int name_and_type_index);

JagAstDeclarationType *jag_byt_constant_fieldref_get_declaration_type(JagBytConstantFieldref *constant_field_ref);

JagAstDeclarationType *jag_byt_constant_fieldref_get_field_source_declaration_type(JagBytConstantFieldref *constant_field_ref);

CatStringWo *jag_byt_constant_fieldref_get_field_name(JagBytConstantFieldref *constant_field_ref);

G_END_DECLS

#endif /* JAGBYTCONSTANTFIELDREF_H_ */
