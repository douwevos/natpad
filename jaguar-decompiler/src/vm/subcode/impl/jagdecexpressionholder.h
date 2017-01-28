/*
   File:    jagdecexpressionholder.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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


#ifndef JAGDECEXPRESSIONHOLDER_H_
#define JAGDECEXPRESSIONHOLDER_H_

#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_EXPRESSION_HOLDER              (jag_dec_expression_holder_get_type())
#define JAG_DEC_EXPRESSION_HOLDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_expression_holder_get_type(), JagDecExpressionHolder))
#define JAG_DEC_EXPRESSION_HOLDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_EXPRESSION_HOLDER, JagDecExpressionHolderClass))
#define JAG_DEC_IS_EXPRESSION_HOLDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_EXPRESSION_HOLDER))
#define JAG_DEC_IS_EXPRESSION_HOLDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_EXPRESSION_HOLDER))
#define JAG_DEC_EXPRESSION_HOLDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_EXPRESSION_HOLDER, JagDecExpressionHolderClass))
#define JAG_DEC_EXPRESSION_HOLDER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_EXPRESSION_HOLDER, JagDecExpressionHolderPrivate))

typedef struct _JagDecExpressionHolder               JagDecExpressionHolder;
typedef struct _JagDecExpressionHolderPrivate        JagDecExpressionHolderPrivate;
typedef struct _JagDecExpressionHolderClass          JagDecExpressionHolderClass;


struct _JagDecExpressionHolder {
	GObject parent;
	JagDecExpressionHolderPrivate *priv;
};

struct _JagDecExpressionHolderClass {
	GObjectClass parent_class;
};


GType jag_dec_expression_holder_get_type();

JagDecExpressionHolder *jag_dec_expression_holder_new();


JagAstIExpression *jag_dec_expression_holder_get(JagDecExpressionHolder *holder);
void jag_dec_expression_holder_set(JagDecExpressionHolder *holder, JagAstIExpression *to_set);


G_END_DECLS


#endif /* JAGDECEXPRESSIONHOLDER_H_ */
