/*
   File:    jagbytmethodheader.h
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

#ifndef JAGBYTMETHODHEADER_H_
#define JAGBYTMETHODHEADER_H_

#include "../ast/jagastargumentlist.h"
#include "jagbytexceptions.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_METHOD_HEADER              (jag_byt_method_header_get_type())
#define JAG_BYT_METHOD_HEADER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_method_header_get_type(), JagBytMethodHeader))
#define JAG_BYT_METHOD_HEADER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_METHOD_HEADER, JagBytMethodHeaderClass))
#define JAG_BYT_IS_METHOD_HEADER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_METHOD_HEADER))
#define JAG_BYT_IS_METHOD_HEADER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_METHOD_HEADER))
#define JAG_BYT_METHOD_HEADER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_METHOD_HEADER, JagBytMethodHeaderClass))
#define JAG_BYT_METHOD_HEADER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_METHOD_HEADER, JagBytMethodHeaderPrivate))

typedef struct _JagBytMethodHeader               JagBytMethodHeader;
typedef struct _JagBytMethodHeaderPrivate        JagBytMethodHeaderPrivate;
typedef struct _JagBytMethodHeaderClass          JagBytMethodHeaderClass;


struct _JagBytMethodHeader {
	GObject parent;
	JagBytMethodHeaderPrivate *priv;
};

struct _JagBytMethodHeaderClass {
	GObjectClass parent_class;
};


GType jag_byt_method_header_get_type();


JagBytMethodHeader *jag_byt_method_header_new_full(CatStringWo *a_method_name, gboolean is_constructor, JagAstDeclarationType *return_type, JagAstArgumentList *argument_list);

JagBytMethodHeader *jag_byt_method_header_new(JagBytName *class_name, CatStringWo *a_method_name, CatStringWo *a_description);

void jag_byt_method_header_set_exceptions(JagBytMethodHeader *method_header, JagBytExceptions *exceptions);

JagBytExceptions *jag_byt_method_header_get_exceptions(JagBytMethodHeader *method_header);

JagAstArgumentList *jag_byt_method_header_get_argument_list(JagBytMethodHeader *method_header);

JagAstDeclarationType *jag_byt_method_header_get_return_type(JagBytMethodHeader *method_header);

CatStringWo *jag_byt_method_header_get_method_name(JagBytMethodHeader *method_header);

gboolean jag_byt_method_header_is_constructor(JagBytMethodHeader *method_header);


gboolean jag_byt_method_header_equal(JagBytMethodHeader *method_header_a, JagBytMethodHeader *method_header_b);


CatStringWo *jag_byt_method_header_as_text(JagBytMethodHeader *method_header, CatStringWo *e_dump_buffer);

G_END_DECLS

#endif /* JAGBYTMETHODHEADER_H_ */
