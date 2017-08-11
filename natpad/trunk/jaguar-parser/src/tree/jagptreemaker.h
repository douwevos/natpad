/*
   File:    jagptreemaker.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 2, 2017
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

#ifndef TREE_JAGPTREEMAKER_H_
#define TREE_JAGPTREEMAKER_H_

#include "../jagptoken.h"
#include "jagpjcannotation.h"
#include "jagpjcannotatedtype.h"
#include "jagpjcarraytypetree.h"
#include "jagpjcarrayaccess.h"
#include "jagpjcassign.h"
#include "jagpjcassignop.h"
#include "jagpjcassert.h"
#include "jagpjcblock.h"
#include "jagpjcbreak.h"
#include "jagpjcbinary.h"
#include "jagpjccase.h"
#include "jagpjccatch.h"
#include "jagpjcclassdecl.h"
#include "jagpjcconditional.h"
#include "jagpjccontinue.h"
#include "jagpjccompilationunit.h"
#include "jagpjcdowhileloop.h"
#include "jagpjcenhancedforloop.h"
#include "jagpjcerroneous.h"
#include "jagpjcexpression.h"
#include "jagpjcexpressionstatement.h"
#include "jagpjcfieldaccess.h"
#include "jagpjcforloop.h"
#include "jagpjcident.h"
#include "jagpjcif.h"
#include "jagpjcinstanceof.h"
#include "jagpjcimport.h"
#include "jagpjclabeledstatement.h"
#include "jagpjclambda.h"
#include "jagpjcliteral.h"
#include "jagpjcmodifiers.h"
#include "jagpjcmemberreference.h"
#include "jagpjcmethoddecl.h"
#include "jagpjcmethodinvocation.h"
#include "jagpjcnewarray.h"
#include "jagpjcnewclass.h"
#include "jagpjcparens.h"
#include "jagpjcpackagedecl.h"
#include "jagpjcprimitivetypetree.h"
#include "jagpjcreturn.h"
#include "jagpjcskip.h"
#include "jagpjcswitch.h"
#include "jagpjcsynchronized.h"
#include "jagpjcthrow.h"
#include "jagpjctry.h"
#include "jagpjctypeapply.h"
#include "jagpjctypecast.h"
#include "jagpjctypeintersection.h"
#include "jagpjctypeparameter.h"
#include "jagpjctypeunion.h"
#include "jagpjcunary.h"
#include "jagpjcvariabledecl.h"
#include "jagpjcwhileloop.h"
#include "jagpjcwildcard.h"
#include "../jagpcursor.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_TREE_MAKER              (jagp_tree_maker_get_type())
#define JAGP_TREE_MAKER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_tree_maker_get_type(), JagPTreeMaker))
#define JAGP_TREE_MAKER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_TREE_MAKER, JagPTreeMakerClass))
#define JAGP_IS_TREE_MAKER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_TREE_MAKER))
#define JAGP_IS_TREE_MAKER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_TREE_MAKER))
#define JAGP_TREE_MAKER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_TREE_MAKER, JagPTreeMakerClass))


typedef struct _JagPTreeMaker               JagPTreeMaker;
typedef struct _JagPTreeMakerPrivate        JagPTreeMakerPrivate;
typedef struct _JagPTreeMakerClass          JagPTreeMakerClass;


struct _JagPTreeMaker {
	GObject parent;
	JagPCursor *cursor;
};

struct _JagPTreeMakerClass {
	GObjectClass parent_class;
};


GType jagp_tree_maker_get_type();

JagPTreeMaker *jagp_tree_maker_new();


void jagp_tree_maker_at(JagPTreeMaker *tree_maker, JagPCursor *cursor);

JagPJCCompilationUnit *jagp_tree_maker_top_level(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCTree>*/ *defs);
JagPJCPackageDecl *jagp_tree_maker_package_decl(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCAnnotation>*/ *annotations, JagPJCExpression *pid);
JagPJCImport *jagp_tree_maker_import(JagPTreeMaker *tree_maker, JagPJCTree *qualid, gboolean import_static);
JagPJCClassDecl *jagp_tree_maker_class_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, CatArrayWo *typarams, JagPJCExpression *extending, CatArrayWo *implementing, CatArrayWo *defs);
JagPJCMethodDecl *jagp_tree_maker_method_def1(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, JagPJCExpression *restype,
						   CatArrayWo /*<JCTypeParameter>*/ *typarams,
						   CatArrayWo /*<JCVariableDecl>*/ *params,
						   CatArrayWo /*<JCExpression>*/ *thrown,
						   JagPJCBlock *body, JagPJCExpression *defaultValue);
JagPJCMethodDecl *jagp_tree_maker_method_def2(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, JagPJCExpression *restype,
							CatArrayWo /*<JCTypeParameter>*/ *typarams,
							JagPJCVariableDecl *recvparam,
							CatArrayWo /*<JCVariableDecl>*/ *params,
							CatArrayWo /*<JCExpression>*/ *thrown,
							JagPJCBlock *body,JagPJCExpression *defaultValue);
JagPJCVariableDecl *jagp_tree_maker_var_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, JagPJCExpression *vartype, JagPJCExpression *init);
JagPJCVariableDecl *jagp_tree_maker_receiver_var_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPJCExpression *nameexpr, JagPJCExpression *vartype);
JagPJCVariableDecl *jagp_tree_maker_receiver_var_def_ext(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPJCExpression *nameexpr, JagPJCExpression *vartype, JagPJCExpression *init);
JagPJCSkip *jagp_tree_maker_skip(JagPTreeMaker *tree_maker);
JagPJCBlock *jagp_tree_maker_block(JagPTreeMaker *tree_maker, long long flags, CatArrayWo /*JagPJCStatement*/*stats);
JagPJCDoWhileLoop *jagp_tree_maker_do_loop(JagPTreeMaker *tree_maker, JagPJCStatement *body, JagPJCExpression *cond);
JagPJCWhileLoop *jagp_tree_maker_while_loop(JagPTreeMaker *tree_maker, JagPJCExpression *cond, JagPJCStatement *body);
JagPJCForLoop *jagp_tree_maker_for_loop(JagPTreeMaker *tree_maker, CatArrayWo /*<JCStatement>*/ *init,
                           JagPJCExpression *cond, CatArrayWo /*<JagPJCExpressionStatement>*/ *step, JagPJCStatement *body);
JagPJCEnhancedForLoop *jagp_tree_maker_foreach_loop(JagPTreeMaker *tree_maker, JagPJCVariableDecl *var, JagPJCExpression *expr, JagPJCStatement *body);
JagPJCLabeledStatement *jagp_tree_maker_labelled(JagPTreeMaker *tree_maker, JagPName *label, JagPJCStatement *body);
JagPJCSwitch *jagp_tree_maker_switch(JagPTreeMaker *tree_maker, JagPJCExpression *selector, CatArrayWo /*<JagPJCCase>*/ *cases);
JagPJCCase *jagp_tree_maker_case(JagPTreeMaker *tree_maker, JagPJCExpression *pat, CatArrayWo /*<JagPJCStatement>*/ *stats);
JagPJCSynchronized *jagp_tree_maker_synchronized(JagPTreeMaker *tree_maker, JagPJCExpression *lock, JagPJCBlock *body);








JagPJCTypeParameter *jagp_tree_maker_type_parameter(JagPTreeMaker *tree_maker, JagPName *name, CatArrayWo /*<JCExpression>*/ *bounds, CatArrayWo /*<JCAnnotation>*/ *annos);



JagPJCMethodInvocation *jagp_tree_maker_apply(JagPTreeMaker *tree_maker, CatArrayWo /*<JCExpression>*/ *typeargs, JagPJCExpression *fn, CatArrayWo /*<JCExpression>*/ *args);
JagPTypeBoundKind *jagp_tree_maker_type_bound_kind(JagPTreeMaker *tree_maker, JagPBoundKind kind);
JagPJCWildcard *jagp_tree_maker_wildcard(JagPTreeMaker *tree_maker, JagPTypeBoundKind *kind, JagPJCTree *type);
JagPJCErroneous *jagp_tree_maker_erroneous(JagPTreeMaker *tree_maker, CatArrayWo /*JagPJCTree*/ *errs);


JagPJCNewClass *jagp_tree_maker_new_class(JagPTreeMaker *tree_maker, JagPJCExpression *encl, CatArrayWo /*<JagPJCExpression>*/ *typeargs,
			JagPJCExpression *clazz, CatArrayWo /*<JCExpression>*/ *args, JagPJCClassDecl *def);
JagPJCNewArray *jagp_tree_maker_new_array(JagPTreeMaker *tree_maker, JagPJCExpression *elemtype, CatArrayWo /*<JCExpression>*/ *dims,
							 CatArrayWo /*<JCExpression>*/ *elems);

JagPJCClassDecl *jagp_tree_maker_anonymous_class_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name_empty, CatArrayWo *defs);
JagPJCExpressionStatement *jagp_tree_maker_exec(JagPTreeMaker *tree_maker, JagPJCExpression *expr);
JagPJCTry *jagp_tree_maker_try(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCTree>*/ *resources, JagPJCBlock *body,
		CatArrayWo /*<JCCatch>*/ *catchers, JagPJCBlock *finalizer);

JagPJCReturn *jagp_tree_maker_return(JagPTreeMaker *tree_maker, JagPJCExpression *expr);
JagPJCThrow *jagp_tree_maker_throw(JagPTreeMaker *tree_maker, JagPJCExpression *expr);
JagPJCBreak *jagp_tree_maker_break(JagPTreeMaker *tree_maker, JagPName *label);
JagPJCContinue *jagp_tree_maker_continue(JagPTreeMaker *tree_maker, JagPName *label);
JagPJCAssert *jagp_tree_maker_assert(JagPTreeMaker *tree_maker, JagPJCExpression *cond, JagPJCExpression *detail);
JagPJCTypeUnion *jagp_tree_maker_type_union(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCExpression>*/ *alternatives);
JagPJCCatch *jagp_tree_maker_catch(JagPTreeMaker *tree_maker, JagPJCVariableDecl *param, JagPJCBlock *body);


JagPJCModifiers *jagp_tree_maker_modifiers(JagPTreeMaker *tree_maker, long long flags, CatArrayWo *annotations);
JagPJCIdent *jagp_tree_maker_ident(JagPTreeMaker *tree_maker, JagPName *name);
JagPJCFieldAccess *jagp_tree_maker_select(JagPTreeMaker *tree_maker, JagPJCExpression *selected, JagPToken *selector);
JagPJCMemberReference *jagp_tree_maker_reference(JagPTreeMaker *tree_maker, JagPReferenceMode mode, JagPName *name,
            JagPJCExpression *expr, CatArrayWo /*<JCExpression>*/ *typeargs);

JagPJCAnnotation *jagp_tree_maker_annotation(JagPTreeMaker *tree_maker, JagPJCTree *annotation_type, CatArrayWo *args);
JagPJCAnnotation *jagp_tree_maker_type_annotation(JagPTreeMaker *tree_maker, JagPJCTree *annotation_type, CatArrayWo *args);

JagPJCAssign *jagp_tree_maker_assign(JagPTreeMaker *tree_maker, JagPJCExpression *lhs, JagPJCExpression *rhs);
JagPJCAssignOp *jagp_tree_maker_assignop(JagPTreeMaker *tree_maker, JagPTag opcode, JagPJCExpression *lhs, JagPJCExpression *rhs);

JagPJCConditional *jagp_tree_maker_conditional(JagPTreeMaker *tree_maker, JagPJCExpression *cond,
		JagPJCExpression *thenpart, JagPJCExpression *elsepart);

JagPJCUnary *jagp_tree_maker_unary(JagPTreeMaker *tree_maker, JagPTag opcode, JagPJCExpression *arg);
JagPJCBinary *jagp_tree_maker_binary(JagPTreeMaker *tree_maker, JagPTag opcode, JagPJCExpression *lhs, JagPJCExpression *rhs);

JagPJCTypeCast *jagp_tree_maker_type_cast(JagPTreeMaker *tree_maker, JagPJCTree *clazz, JagPJCExpression *expre);
JagPJCInstanceOf *jagp_tree_maker_type_test(JagPTreeMaker *tree_maker, JagPJCExpression *expr, JagPJCTree *clazz);
JagPJCTypeApply *jagp_tree_maker_type_apply(JagPTreeMaker *tree_maker, JagPJCTree *clazz, CatArrayWo *arguments);
JagPJCPrimitiveTypeTree *jagp_tree_maker_type_ident(JagPTreeMaker *tree_maker, JagPTypeTag typetag);

JagPJCLiteral *jagp_tree_maker_literal(JagPTreeMaker *tree_maker, JagPTypeTag typetag, GObject *value);


JagPJCTypeIntersection *jagp_tree_maker_type_intersection(JagPTreeMaker *tree_maker, CatArrayWo *components);

JagPJCParens *jagp_tree_maker_parens(JagPTreeMaker *tree_maker, JagPJCExpression *expr);

JagPJCAnnotatedType *jagp_tree_maker_annotated_type(JagPTreeMaker *tree_maker, CatArrayWo *annotations, JagPJCExpression *underlying_type);

JagPJCArrayTypeTree *jagp_tree_maker_type_array(JagPTreeMaker *tree_maker, JagPJCExpression *elemtype);

JagPJCArrayAccess *jagp_tree_maker_indexed(JagPTreeMaker *tree_maker, JagPJCExpression *indexed, JagPJCExpression *index);

JagPJCLambda *jagp_tree_maker_lambda(JagPTreeMaker *tree_maker, CatArrayWo /*JagPJCVariableDecl*/*params, JagPJCTree *body);


JagPJCIf *jagp_tree_maker_if(JagPTreeMaker *tree_maker, JagPJCExpression *cond, JagPJCStatement *thenpart, JagPJCStatement *elsepart);

G_END_DECLS

#endif /* TREE_JAGPTREEMAKER_H_ */
