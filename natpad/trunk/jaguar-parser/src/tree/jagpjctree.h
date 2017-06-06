/*
   File:    jagpjctree.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef TREE_JAGPJCTREE_H_
#define TREE_JAGPJCTREE_H_

#include <caterpillar.h>

G_BEGIN_DECLS


typedef enum _JagPTag JagPTag;

enum _JagPTag {
    /** For methods that return an invalid tag if a given condition is not met
     */
    JAGP_TAG_NO_TAG,

    /** Toplevel nodes, of type TopLevel, representing entire source files.
    */
	JAGP_TAG_TOPLEVEL,

    /** Package level definitions.
     */
	JAGP_TAG_PACKAGEDEF,

    /** Import clauses, of type Import.
     */
	JAGP_TAG_IMPORT,

    /** Class definitions, of type ClassDef.
     */
	JAGP_TAG_CLASSDEF,

    /** Method definitions, of type MethodDef.
     */
	JAGP_TAG_METHODDEF,

    /** Variable definitions, of type VarDef.
     */
	JAGP_TAG_VARDEF,

    /** The no-op statement ";", of type Skip
     */
	JAGP_TAG_SKIP,

    /** Blocks, of type Block.
     */
	JAGP_TAG_BLOCK,

    /** Do-while loops, of type DoLoop.
     */
	JAGP_TAG_DOLOOP,

    /** While-loops, of type WhileLoop.
     */
	JAGP_TAG_WHILELOOP,

    /** For-loops, of type ForLoop.
     */
	JAGP_TAG_FORLOOP,

    /** Foreach-loops, of type ForeachLoop.
     */
	JAGP_TAG_FOREACHLOOP,

    /** Labelled statements, of type Labelled.
     */
	JAGP_TAG_LABELLED,

    /** Switch statements, of type Switch.
     */
	JAGP_TAG_SWITCH,

    /** Case parts in switch statements, of type Case.
     */
	JAGP_TAG_CASE,

    /** Synchronized statements, of type Synchonized.
     */
	JAGP_TAG_SYNCHRONIZED,

    /** Try statements, of type Try.
     */
	JAGP_TAG_TRY,

    /** Catch clauses in try statements, of type Catch.
     */
	JAGP_TAG_CATCH,

    /** Conditional expressions, of type Conditional.
     */
	JAGP_TAG_CONDEXPR,

    /** Conditional statements, of type If.
     */
	JAGP_TAG_IF,

    /** Expression statements, of type Exec.
     */
	JAGP_TAG_EXEC,

    /** Break statements, of type Break.
     */
	JAGP_TAG_BREAK,

    /** Continue statements, of type Continue.
     */
	JAGP_TAG_CONTINUE,

    /** Return statements, of type Return.
     */
	JAGP_TAG_RETURN,

    /** Throw statements, of type Throw.
     */
	JAGP_TAG_THROW,

    /** Assert statements, of type Assert.
     */
	JAGP_TAG_ASSERT,

    /** Method invocation expressions, of type Apply.
     */
	JAGP_TAG_APPLY,

    /** Class instance creation expressions, of type NewClass.
     */
	JAGP_TAG_NEWCLASS,

    /** Array creation expressions, of type NewArray.
     */
	JAGP_TAG_NEWARRAY,

    /** Lambda expression, of type Lambda.
     */
	JAGP_TAG_LAMBDA,

    /** Parenthesized subexpressions, of type Parens.
     */
	JAGP_TAG_PARENS,

    /** Assignment expressions, of type Assign.
     */
	JAGP_TAG_ASSIGN,

    /** Type cast expressions, of type TypeCast.
     */
	JAGP_TAG_TYPECAST,

    /** Type test expressions, of type TypeTest.
     */
	JAGP_TAG_TYPETEST,

    /** Indexed array expressions, of type Indexed.
     */
	JAGP_TAG_INDEXED,

    /** Selections, of type Select.
     */
	JAGP_TAG_SELECT,

    /** Member references, of type Reference.
     */
	JAGP_TAG_REFERENCE,

    /** Simple identifiers, of type Ident.
     */
	JAGP_TAG_IDENT,

    /** Literals, of type Literal.
     */
	JAGP_TAG_LITERAL,

    /** Basic type identifiers, of type TypeIdent.
     */
	JAGP_TAG_TYPEIDENT,

    /** Array types, of type TypeArray.
     */
	JAGP_TAG_TYPEARRAY,

    /** Parameterized types, of type TypeApply.
     */
	JAGP_TAG_TYPEAPPLY,

    /** Union types, of type TypeUnion.
     */
	JAGP_TAG_TYPEUNION,

    /** Intersection types, of type TypeIntersection.
     */
	JAGP_TAG_TYPEINTERSECTION,

    /** Formal type parameters, of type TypeParameter.
     */
	JAGP_TAG_TYPEPARAMETER,

    /** Type argument.
     */
	JAGP_TAG_WILDCARD,

    /** Bound kind: extends, super, exact, or unbound
     */
	JAGP_TAG_TYPEBOUNDKIND,

    /** metadata: Annotation.
     */
	JAGP_TAG_ANNOTATION,

    /** metadata: Type annotation.
     */
	JAGP_TAG_TYPE_ANNOTATION,

    /** metadata: Modifiers
     */
	JAGP_TAG_MODIFIERS,

    /** An annotated type tree.
     */
	JAGP_TAG_ANNOTATED_TYPE,

    /** Error trees, of type Erroneous.
     */
	JAGP_TAG_ERRONEOUS,

    /** Unary operators, of type Unary.
     */
	JAGP_TAG_POS,                             // +
	JAGP_TAG_NEG,                             // -
	JAGP_TAG_NOT,                             // !
	JAGP_TAG_COMPL,                           // ~
	JAGP_TAG_PREINC,                          // ++ _
	JAGP_TAG_PREDEC,                          // -- _
	JAGP_TAG_POSTINC,                         // _ ++
	JAGP_TAG_POSTDEC,                         // _ --

    /** unary operator for null reference checks, only used internally.
     */
	JAGP_TAG_NULLCHK,

    /** Binary operators, of type Binary.
     */
	JAGP_TAG_OR,                              // ||
	JAGP_TAG_AND,                             // &&
	JAGP_TAG_BITOR,                           // |
	JAGP_TAG_BITXOR,                          // ^
	JAGP_TAG_BITAND,                          // &
	JAGP_TAG_EQ,                              // ==
	JAGP_TAG_NE,                              // !=
	JAGP_TAG_LT,                              // <
	JAGP_TAG_GT,                              // >
	JAGP_TAG_LE,                              // <=
	JAGP_TAG_GE,                              // >=
	JAGP_TAG_SL,                              // <<
	JAGP_TAG_SR,                              // >>
	JAGP_TAG_USR,                             // >>>
	JAGP_TAG_PLUS,                            // +
	JAGP_TAG_MINUS,                           // -
	JAGP_TAG_MUL,                             // *
	JAGP_TAG_DIV,                             // /
	JAGP_TAG_MOD,                             // %

    /** Assignment operators, of type Assignop.
     */
	JAGP_TAG_BITOR_ASG,                // |=
	JAGP_TAG_BITXOR_ASG,              // ^=
	JAGP_TAG_BITAND_ASG,              // &=

	JAGP_TAG_SL_ASG,                      // <<=
	JAGP_TAG_SR_ASG,                      // >>=
	JAGP_TAG_USR_ASG,                    // >>>=
	JAGP_TAG_PLUS_ASG,                  // +=
	JAGP_TAG_MINUS_ASG,                // -=
	JAGP_TAG_MUL_ASG,                    // *=
	JAGP_TAG_DIV_ASG,                    // /=
	JAGP_TAG_MOD_ASG,                    // %=

	JAGP_TAG_MODULEDEF,
	JAGP_TAG_EXPORTS,
	JAGP_TAG_PROVIDES,
	JAGP_TAG_REQUIRES,
	JAGP_TAG_USES,

    /** A synthetic let expression, of type LetExpr.
     */
	JAGP_TAG_LETEXPR                         // ala scheme

};

#define JAGP_TYPE_JCTREE              (jagp_jctree_get_type())
#define JAGP_JCTREE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jctree_get_type(), JagPJCTree))
#define JAGP_JCTREE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCTREE, JagPJCTreeClass))
#define JAGP_IS_JCTREE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCTREE))
#define JAGP_IS_JCTREE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCTREE))
#define JAGP_JCTREE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCTREE, JagPJCTreeClass))


typedef struct _JagPJCTree               JagPJCTree;
typedef struct _JagPJCTreePrivate        JagPJCTreePrivate;
typedef struct _JagPJCTreeClass          JagPJCTreeClass;


struct _JagPJCTree {
	GObject parent;
	int pos;
};

struct _JagPJCTreeClass {
	GObjectClass parent_class;
	JagPTag (*getTag)(JagPJCTree *tree);
};


GType jagp_jctree_get_type();

JagPJCTree *jagp_jctree_new();

JagPTag jagp_jctree_get_tag(JagPJCTree *tree);
gboolean jagp_jctree_has_tag(JagPJCTree *tree, JagPTag tag);

int jagp_jctree_get_start_position(JagPJCTree *tree);

G_END_DECLS

#endif /* TREE_JAGPJCTREE_H_ */
