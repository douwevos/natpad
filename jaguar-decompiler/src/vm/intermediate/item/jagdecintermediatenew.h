/*
   File:    jagdecintermediatenew.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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


#ifndef JAGDECINTERMEDIATENEW_H_
#define JAGDECINTERMEDIATENEW_H_

#include "../jagdecabstractintermediateitem.h"
#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_NEW              (jag_dec_intermediate_new_get_type())
#define JAG_DEC_INTERMEDIATE_NEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_new_get_type(), JagDecIntermediateNew))
#define JAG_DEC_INTERMEDIATE_NEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_NEW, JagDecIntermediateNewClass))
#define JAG_DEC_IS_INTERMEDIATE_NEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_NEW))
#define JAG_DEC_IS_INTERMEDIATE_NEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_NEW))
#define JAG_DEC_INTERMEDIATE_NEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_NEW, JagDecIntermediateNewClass))
#define JAG_DEC_INTERMEDIATE_NEW_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_NEW, JagDecIntermediateNewPrivate))

typedef struct _JagDecIntermediateNew               JagDecIntermediateNew;
typedef struct _JagDecIntermediateNewPrivate        JagDecIntermediateNewPrivate;
typedef struct _JagDecIntermediateNewClass          JagDecIntermediateNewClass;


struct _JagDecIntermediateNew {
	JagDecAbstractIntermediateItem parent;
	JagDecIntermediateNewPrivate *priv;
};

struct _JagDecIntermediateNewClass {
	JagDecAbstractIntermediateItemClass parent_class;
};


GType jag_dec_intermediate_new_get_type();

JagDecIntermediateNew *jag_dec_intermediate_new_new(int mnemonic_index, JagAstDeclarationType *declaration_type);

G_END_DECLS


#endif /* JAGDECINTERMEDIATENEW_H_ */
