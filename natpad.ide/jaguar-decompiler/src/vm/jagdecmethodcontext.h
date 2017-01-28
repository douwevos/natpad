/*
   File:    jagdecmethodcontext.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#ifndef JAGDECMETHODCONTEXT_H_
#define JAGDECMETHODCONTEXT_H_

#include "../type/jagdecimportrepository.h"
#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_METHOD_CONTEXT              (jag_dec_method_context_get_type())
#define JAG_DEC_METHOD_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_method_context_get_type(), JagDecMethodContext))
#define JAG_DEC_METHOD_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_METHOD_CONTEXT, JagDecMethodContextClass))
#define JAG_DEC_IS_METHOD_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_METHOD_CONTEXT))
#define JAG_DEC_IS_METHOD_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_METHOD_CONTEXT))
#define JAG_DEC_METHOD_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_METHOD_CONTEXT, JagDecMethodContextClass))
#define JAG_DEC_METHOD_CONTEXT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_METHOD_CONTEXT, JagDecMethodContextPrivate))

typedef struct _JagDecMethodContext               JagDecMethodContext;
typedef struct _JagDecMethodContextPrivate        JagDecMethodContextPrivate;
typedef struct _JagDecMethodContextClass          JagDecMethodContextClass;


struct _JagDecMethodContext {
	GObject parent;
	JagDecMethodContextPrivate *priv;
};

struct _JagDecMethodContextClass {
	GObjectClass parent_class;
};


GType jag_dec_method_context_get_type();

JagDecMethodContext *jag_dec_method_context_new(JagDecImportRepository *import_repository, JagBytClassfile *parsedClass, JagBytMethod *parsedMethod);

JagAstMethod *jag_dec_method_context_create_method(JagDecMethodContext *method_context);

G_END_DECLS


#endif /* JAGDECMETHODCONTEXT_H_ */
