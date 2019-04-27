/*
   File:    jagbytexceptions.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#ifndef JAGBYTEXCEPTIONS_H_
#define JAGBYTEXCEPTIONS_H_

#include "jagbytname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_EXCEPTIONS              (jag_byt_exceptions_get_type())
#define JAG_BYT_EXCEPTIONS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_exceptions_get_type(), JagBytExceptions))
#define JAG_BYT_EXCEPTIONS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_EXCEPTIONS, JagBytExceptionsClass))
#define JAG_BYT_IS_EXCEPTIONS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_EXCEPTIONS))
#define JAG_BYT_IS_EXCEPTIONS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_EXCEPTIONS))
#define JAG_BYT_EXCEPTIONS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_EXCEPTIONS, JagBytExceptionsClass))

typedef struct _JagBytExceptions               JagBytExceptions;
typedef struct _JagBytExceptionsPrivate        JagBytExceptionsPrivate;
typedef struct _JagBytExceptionsClass          JagBytExceptionsClass;


struct _JagBytExceptions {
	GObject parent;
};

struct _JagBytExceptionsClass {
	GObjectClass parent_class;
};


GType jag_byt_exceptions_get_type();

JagBytExceptions *jag_byt_exceptions_new();

void jag_byt_exceptions_add(JagBytExceptions *exceptions, JagBytName *exception_type_name);


int jag_byt_exceptions_count(JagBytExceptions *exceptions);

JagBytName *jag_byt_exceptions_get(JagBytExceptions *exceptions, int index);

gboolean jag_byt_exceptions_equal(JagBytExceptions *exceptions_a, JagBytExceptions *exceptions_b);

G_END_DECLS

#endif /* JAGBYTEXCEPTIONS_H_ */
