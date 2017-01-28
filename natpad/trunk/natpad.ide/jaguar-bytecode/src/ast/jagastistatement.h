/*
   File:    jagastistatement.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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


#ifndef JAGASTISTATEMENT_H_
#define JAGASTISTATEMENT_H_

#include "formater/jagastwriter.h"
#include <glib-object.h>

#define JAG_AST_TYPE_ISTATEMENT                 (jag_ast_istatement_get_type())
#define JAG_AST_ISTATEMENT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_AST_TYPE_ISTATEMENT, JagAstIStatement))
#define JAG_AST_IS_ISTATEMENT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_AST_TYPE_ISTATEMENT))
#define JAG_AST_ISTATEMENT_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_AST_TYPE_ISTATEMENT, JagAstIStatementInterface))


typedef struct _JagAstIStatement               JagAstIStatement; /* dummy object */
typedef struct _JagAstIStatementInterface      JagAstIStatementInterface;

struct _JagAstIStatementInterface {
	GTypeInterface parent_iface;

	void (*writeStatement)(JagAstIStatement *self, JagAstWriter *out);

	void (*setAtLeastLineNumber)(JagAstIStatement *self, int statementLineNr);

};

GType jag_ast_istatement_get_type(void);


void jag_ast_istatement_write_statement(JagAstIStatement *self, JagAstWriter *out);

void jag_ast_istatement_set_at_least_line_number(JagAstIStatement *self, int statementLineNr);


#endif /* JAGASTISTATEMENT_H_ */
