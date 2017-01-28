/*
   File:    jagastargumentlist.h
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


#ifndef JAGASTARGUMENTLIST_H_
#define JAGASTARGUMENTLIST_H_

#include "jagastargument.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_AST_TYPE_ARGUMENT_LIST              (jag_ast_argument_list_get_type())
#define JAG_AST_ARGUMENT_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_argument_list_get_type(), JagAstArgumentList))
#define JAG_AST_ARGUMENT_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_ARGUMENT_LIST, JagAstArgumentListClass))
#define JAG_AST_IS_ARGUMENT_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_ARGUMENT_LIST))
#define JAG_AST_IS_ARGUMENT_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_ARGUMENT_LIST))
#define JAG_AST_ARGUMENT_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_ARGUMENT_LIST, JagAstArgumentListClass))
#define JAG_AST_ARGUMENT_LIST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_ARGUMENT_LIST, JagAstArgumentListPrivate))

typedef struct _JagAstArgumentList               JagAstArgumentList;
typedef struct _JagAstArgumentListPrivate        JagAstArgumentListPrivate;
typedef struct _JagAstArgumentListClass          JagAstArgumentListClass;


struct _JagAstArgumentList {
	GObject parent;
	JagAstArgumentListPrivate *priv;
};

struct _JagAstArgumentListClass {
	GObjectClass parent_class;
};


GType jag_ast_argument_list_get_type();

JagAstArgumentList *jag_ast_argument_list_new();


JagAstArgument *jag_ast_argument_list_get_at(JagAstArgumentList *argument_list, int index);

void jag_ast_argument_list_add(JagAstArgumentList *argument_list, JagAstArgument *argument);

int jag_ast_argument_list_count(JagAstArgumentList *argument_list);

gboolean jag_ast_argument_list_equal(JagAstArgumentList *argument_list_a, JagAstArgumentList *argument_list_b);

CatStringWo *jag_ast_argument_list_as_text(JagAstArgumentList *argument_list, CatStringWo *e_dump_buffer);

G_END_DECLS


#endif /* JAGASTARGUMENTLIST_H_ */
