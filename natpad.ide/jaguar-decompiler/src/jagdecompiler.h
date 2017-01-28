/*
   File:    jagdecompiler.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 26, 2012
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


#ifndef JAGDECOMPILER_H_
#define JAGDECOMPILER_H_

#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_DECOMPILER              (jag_decompiler_get_type())
#define JAG_DECOMPILER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_decompiler_get_type(), JagDecompiler))
#define JAG_DECOMPILER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_DECOMPILER, JagDecompilerClass))
#define JAG_IS_DECOMPILER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_DECOMPILER))
#define JAG_IS_DECOMPILER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_DECOMPILER))
#define JAG_DECOMPILER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_DECOMPILER, JagDecompilerClass))
#define JAG_DECOMPILER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_DECOMPILER, JagDecompilerPrivate))

typedef struct _JagDecompiler               JagDecompiler;
typedef struct _JagDecompilerPrivate        JagDecompilerPrivate;
typedef struct _JagDecompilerClass          JagDecompilerClass;


struct _JagDecompiler {
	GObject parent;
	JagDecompilerPrivate *priv;
};

struct _JagDecompilerClass {
	GObjectClass parent_class;
};


GType jag_decompiler_get_type();

JagDecompiler *jag_decompiler_new(JagBytClassfile *classfile);

JagAstTp *jag_decompiler_create_type(JagDecompiler *decompiler);


G_END_DECLS

#endif /* JAGDECOMPILER_H_ */
