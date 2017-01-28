/*
   File:    jagbytattributeexceptions.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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


#ifndef JAGBYTATTRIBUTEEXCEPTIONS_H_
#define JAGBYTATTRIBUTEEXCEPTIONS_H_

#include "jagbytattribute.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS              (jag_byt_attribute_exceptions_get_type())
#define JAG_BYT_ATTRIBUTE_EXCEPTIONS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_exceptions_get_type(), JagBytAttributeExceptions))
#define JAG_BYT_ATTRIBUTE_EXCEPTIONS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS, JagBytAttributeExceptionsClass))
#define JAG_BYT_IS_ATTRIBUTE_EXCEPTIONS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS))
#define JAG_BYT_IS_ATTRIBUTE_EXCEPTIONS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS))
#define JAG_BYT_ATTRIBUTE_EXCEPTIONS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS, JagBytAttributeExceptionsClass))
#define JAG_BYT_ATTRIBUTE_EXCEPTIONS_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_ATTRIBUTE_EXCEPTIONS, JagBytAttributeExceptionsPrivate))

typedef struct _JagBytAttributeExceptions               JagBytAttributeExceptions;
typedef struct _JagBytAttributeExceptionsPrivate        JagBytAttributeExceptionsPrivate;
typedef struct _JagBytAttributeExceptionsClass          JagBytAttributeExceptionsClass;


struct _JagBytAttributeExceptions {
	JagBytAttribute parent;
	JagBytAttributeExceptionsPrivate *priv;
};

struct _JagBytAttributeExceptionsClass {
	JagBytAttributeClass parent_class;
};

CatS jag_txt_attribute_exceptions;

GType jag_byt_attribute_exceptions_get_type();

JagBytAttributeExceptions *jag_byt_attribute_exceptions_new(CatStringWo *e_attribute_data);

int jag_byt_attribute_exceptions_count(JagBytAttributeExceptions *attribute_exceptions);

int jag_byt_attribute_exceptions_get(JagBytAttributeExceptions *attribute_exceptions, int index);


G_END_DECLS


#endif /* JAGBYTATTRIBUTEEXCEPTIONS_H_ */
