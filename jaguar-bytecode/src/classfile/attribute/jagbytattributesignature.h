/*
   File:    jagbytattributesignature.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 7, 2012
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

#ifndef JAGBYTATTRIBUTESIGNATURE_H_
#define JAGBYTATTRIBUTESIGNATURE_H_

#include "jagbytattribute.h"
#include "../constant/jagbyticonstantprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_SIGNATURE              (jag_byt_attribute_signature_get_type())
#define JAG_BYT_ATTRIBUTE_SIGNATURE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_signature_get_type(), JagBytAttributeSignature))
#define JAG_BYT_ATTRIBUTE_SIGNATURE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_SIGNATURE, JagBytAttributeSignatureClass))
#define JAG_BYT_IS_ATTRIBUTE_SIGNATURE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_SIGNATURE))
#define JAG_BYT_IS_ATTRIBUTE_SIGNATURE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_SIGNATURE))
#define JAG_BYT_ATTRIBUTE_SIGNATURE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_SIGNATURE, JagBytAttributeSignatureClass))

typedef struct _JagBytAttributeSignature               JagBytAttributeSignature;
typedef struct _JagBytAttributeSignaturePrivate        JagBytAttributeSignaturePrivate;
typedef struct _JagBytAttributeSignatureClass          JagBytAttributeSignatureClass;


struct _JagBytAttributeSignature {
	JagBytAttribute parent;
};

struct _JagBytAttributeSignatureClass {
	JagBytAttributeClass parent_class;
};

CatS jag_txt_attribute_signature;


GType jag_byt_attribute_signature_get_type();

JagBytAttributeSignature *jag_byt_attribute_signature_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider);

CatStringWo *jag_byt_attribute_signature_as_text(JagBytAttributeSignature *signature);

G_END_DECLS

#endif /* JAGBYTATTRIBUTESIGNATURE_H_ */
