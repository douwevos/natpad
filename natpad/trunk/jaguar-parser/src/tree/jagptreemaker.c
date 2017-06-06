/*
   File:    jagptreemaker.c
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

#include "jagptreemaker.h"
#include "../code/jagpflags.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPTreeMaker"
#include <logging/catlog.h>

struct _JagPTreeMakerPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPTreeMaker, jagp_tree_maker, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPTreeMaker)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_tree_maker_class_init(JagPTreeMakerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_tree_maker_init(JagPTreeMaker *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagPTreeMaker *instance = JAGP_TREE_MAKER(object);
//	JagPTreeMakerPrivate *priv = jagp_tree_maker_get_instance_private(instance);
	G_OBJECT_CLASS(jagp_tree_maker_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_tree_maker_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPTreeMaker *jagp_tree_maker_new() {
	JagPTreeMaker *result = g_object_new(JAGP_TYPE_TREE_MAKER, NULL);
	cat_ref_anounce(result);
	JagPTreeMakerPrivate *priv = jagp_tree_maker_get_instance_private(result);
//	G_OBJECT_construct((GObject *) result);
	return result;
}

void jagp_tree_maker_at(JagPTreeMaker *tree_maker, int pos) {
	tree_maker->pos = pos;
}



JagPJCCompilationUnit *jagp_tree_maker_top_level(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCTree>*/ *defs) {
	JagPJCCompilationUnit *tree = jagp_jccompilation_unit_new(defs);
    tree->parent.pos = tree_maker->pos;
	return tree;
}

JagPJCPackageDecl *jagp_tree_maker_package_decl(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCAnnotation>*/ *annotations, JagPJCExpression *pid) {
	JagPJCPackageDecl *tree = jagp_jcpackage_decl_new(annotations, pid);
    tree->parent.pos = tree_maker->pos;
	return tree;
}

JagPJCImport *jagp_tree_maker_import(JagPTreeMaker *tree_maker, JagPJCTree *qualid, gboolean import_static) {
	JagPJCImport *tree = jagp_jcimport_new(qualid, import_static);
	tree->parent.pos = tree_maker->pos;
	return tree;
}

JagPJCClassDecl *jagp_tree_maker_class_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, CatArrayWo *typarams, JagPJCExpression *extending, CatArrayWo *implementing, CatArrayWo *defs) {
	JagPJCClassDecl *tree = jagp_jcclass_decl_new(mods, name, typarams, extending, implementing, defs);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCMethodDecl *jagp_tree_maker_method_def1(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, JagPJCExpression *restype,
						   CatArrayWo /*<JCTypeParameter>*/ *typarams,
						   CatArrayWo /*<JCVariableDecl>*/ *params,
						   CatArrayWo /*<JCExpression>*/ *thrown,
						   JagPJCBlock *body, JagPJCExpression *defaultValue) {
	JagPJCMethodDecl *tree = jagp_jcmethod_decl_new(mods, name, restype, typarams, NULL, params, thrown, body, defaultValue);
	tree->parent.pos = tree_maker->pos;
	return tree;
}


JagPJCMethodDecl *jagp_tree_maker_method_def2(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, JagPJCExpression *restype,
							CatArrayWo /*<JCTypeParameter>*/ *typarams,
							JagPJCVariableDecl *recvparam,
							CatArrayWo /*<JCVariableDecl>*/ *params,
							CatArrayWo /*<JCExpression>*/ *thrown,
							JagPJCBlock *body,JagPJCExpression *defaultValue) {
	JagPJCMethodDecl *tree = jagp_jcmethod_decl_new(mods, name, restype, typarams, recvparam, params, thrown, body, defaultValue);
	tree->parent.pos = tree_maker->pos;
	return tree;
}


JagPJCVariableDecl *jagp_tree_maker_var_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name, JagPJCExpression *vartype, JagPJCExpression *init) {
	JagPJCVariableDecl *tree = jagp_jcvariable_decl_new(mods, name, NULL, vartype, init);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCVariableDecl *jagp_tree_maker_receiver_var_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPJCExpression *nameexpr, JagPJCExpression *vartype) {
	JagPJCVariableDecl *tree = jagp_jcvariable_decl_new(mods, NULL, nameexpr, vartype, NULL);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCSkip *jagp_tree_maker_skip(JagPTreeMaker *tree_maker) {
	JagPJCSkip *tree = jagp_jcskip_new();
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCBlock *jagp_tree_maker_block(JagPTreeMaker *tree_maker, long long flags, CatArrayWo /*JagPJCStatement*/*stats) {
	JagPJCBlock *tree = jagp_jcblock_new(flags, stats);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCDoWhileLoop *jagp_tree_maker_do_loop(JagPTreeMaker *tree_maker, JagPJCStatement *body, JagPJCExpression *cond) {
	JagPJCDoWhileLoop *tree = jagp_jcdo_while_loop_new(body, cond);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCWhileLoop *jagp_tree_maker_while_loop(JagPTreeMaker *tree_maker, JagPJCExpression *cond, JagPJCStatement *body) {
	JagPJCWhileLoop *tree = jagp_jcwhile_loop_new(cond, body);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCForLoop *jagp_tree_maker_for_loop(JagPTreeMaker *tree_maker, CatArrayWo /*<JCStatement>*/ *init,
                           JagPJCExpression *cond, CatArrayWo /*<JagPJCExpressionStatement>*/ *step, JagPJCStatement *body) {
	JagPJCForLoop *tree = jagp_jcfor_loop_new(init, cond, step, body);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCEnhancedForLoop *jagp_tree_maker_foreach_loop(JagPTreeMaker *tree_maker, JagPJCVariableDecl *var, JagPJCExpression *expr, JagPJCStatement *body) {
	JagPJCEnhancedForLoop *tree = jagp_jcenhanced_for_loop_new(var, expr, body);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCLabeledStatement *jagp_tree_maker_labelled(JagPTreeMaker *tree_maker, JagPName *label, JagPJCStatement *body) {
	JagPJCLabeledStatement *tree = jagp_jclabeled_statement_new(label, body);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCSwitch *jagp_tree_maker_switch(JagPTreeMaker *tree_maker, JagPJCExpression *selector, CatArrayWo /*<JagPJCCase>*/ *cases) {
	JagPJCSwitch *tree = jagp_jcswitch_new(selector, cases);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCCase *jagp_tree_maker_case(JagPTreeMaker *tree_maker, JagPJCExpression *pat, CatArrayWo /*<JagPJCStatement>*/ *stats) {
	JagPJCCase *tree = jagp_jccase_new(pat, stats);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCSynchronized *jagp_tree_maker_synchronized(JagPTreeMaker *tree_maker, JagPJCExpression *lock, JagPJCBlock *body) {
	JagPJCSynchronized *tree = jagp_jcsynchronized_new(lock, body);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}




























JagPJCTypeParameter *jagp_tree_maker_type_parameter(JagPTreeMaker *tree_maker, JagPName *name, CatArrayWo /*<JCExpression>*/ *bounds, CatArrayWo /*<JCAnnotation>*/ *annos) {
	JagPJCTypeParameter *tree = jagp_jctype_parameter_new(name, bounds, annos);
	tree->parent.pos = tree_maker->pos;
	return tree;
}


JagPJCMethodInvocation *jagp_tree_maker_apply(JagPTreeMaker *tree_maker, CatArrayWo /*<JCExpression>*/ *typeargs, JagPJCExpression *fn, CatArrayWo /*<JCExpression>*/ *args) {
	JagPJCMethodInvocation *tree = jagp_jcmethod_invocation_new(typeargs, fn, args);
	tree->parent.parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPTypeBoundKind *jagp_tree_maker_type_bound_kind(JagPTreeMaker *tree_maker, JagPBoundKind kind) {
	JagPTypeBoundKind *tree = jagp_type_bound_kind_new(kind);
	tree->parent.pos = tree_maker->pos;
	return tree;
}

JagPJCWildcard *jagp_tree_maker_wildcard(JagPTreeMaker *tree_maker, JagPTypeBoundKind *kind, JagPJCTree *type) {
	JagPJCWildcard *tree = jagp_jcwildcard_new(kind, type);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCErroneous *jagp_tree_maker_erroneous(JagPTreeMaker *tree_maker, CatArrayWo /*JagPJCTree*/ *errs) {
	JagPJCErroneous *tree = jagp_jcerroneous_new(errs);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCNewClass *jagp_tree_maker_new_class(JagPTreeMaker *tree_maker, JagPJCExpression *encl, CatArrayWo /*<JagPJCExpression>*/ *typeargs,
			JagPJCExpression *clazz, CatArrayWo /*<JCExpression>*/ *args, JagPJCClassDecl *def) {
	JagPJCNewClass *tree = jagp_jcnew_class_new(encl, typeargs, clazz, args, def);
	tree->parent.parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCNewArray *jagp_tree_maker_new_array(JagPTreeMaker *tree_maker, JagPJCExpression *elemtype, CatArrayWo /*<JCExpression>*/ *dims,
							 CatArrayWo /*<JCExpression>*/ *elems) {
	JagPJCNewArray *tree = jagp_jcnew_array_new(elemtype, dims, elems);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}



JagPJCModifiers *jagp_tree_maker_modifiers(JagPTreeMaker *tree_maker, long long flags, CatArrayWo *annotations) {
    JagPJCModifiers *tree = jagp_jcmodifiers_new(flags, annotations);
    gboolean noFlags = (flags & (JAGP_FLAG_ModifierFlags | JAGP_FLAG_ANNOTATION)) == 0;
    tree->parent.pos = (noFlags && cat_array_wo_size(annotations)==0) ? -1 : tree_maker->pos;
    return tree;
}

JagPJCIdent *jagp_tree_maker_ident(JagPTreeMaker *tree_maker, JagPName *name) {
    JagPJCIdent *tree = jagp_jcident_new(name);
    tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCPrimitiveTypeTree *jagp_tree_maker_type_ident(JagPTreeMaker *tree_maker, JagPTypeTag typetag) {
	JagPJCPrimitiveTypeTree *tree = jagp_jcprimitive_type_tree_new(typetag);
    tree->parent.parent.pos = tree_maker->pos;
    return tree;
}

JagPJCLiteral *jagp_tree_maker_literal(JagPTreeMaker *tree_maker, JagPTypeTag typetag, GObject *value) {
	JagPJCLiteral *tree = jagp_jcliteral_new(typetag, value);
    tree->parent.parent.pos = tree_maker->pos;
    return tree;
}


JagPJCFieldAccess *jagp_tree_maker_select(JagPTreeMaker *tree_maker, JagPJCExpression *selected, JagPName *selector) {
	JagPJCFieldAccess *tree = jagp_jcfield_access_new(selected, selector, NULL);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCMemberReference *jagp_tree_maker_reference(JagPTreeMaker *tree_maker, JagPReferenceMode mode, JagPName *name,
            JagPJCExpression *expr, CatArrayWo /*<JCExpression>*/ *typeargs) {
	JagPJCMemberReference *tree = jagp_jcmember_reference_new(mode, name, expr, typeargs);
	tree->parent.parent.parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCAnnotation *jagp_tree_maker_annotation(JagPTreeMaker *tree_maker, JagPJCTree *annotation_type, CatArrayWo *args) {
	JagPJCAnnotation *tree = jagp_jcannotation_new(FALSE, annotation_type, args);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCAnnotation *jagp_tree_maker_type_annotation(JagPTreeMaker *tree_maker, JagPJCTree *annotation_type, CatArrayWo *args) {
	JagPJCAnnotation *tree = jagp_jcannotation_new(TRUE, annotation_type, args);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCAssign *jagp_tree_maker_assign(JagPTreeMaker *tree_maker, JagPJCExpression *lhs, JagPJCExpression *rhs) {
	JagPJCAssign *tree = jagp_jcassign_new(lhs, rhs);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCConditional *jagp_tree_maker_conditional(JagPTreeMaker *tree_maker, JagPJCExpression *cond,
		JagPJCExpression *thenpart, JagPJCExpression *elsepart) {
	JagPJCConditional *tree = jagp_jcconditional_new(cond, thenpart, elsepart);
	tree->parent.parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCUnary *jagp_tree_maker_unary(JagPTreeMaker *tree_maker, JagPTag opcode, JagPJCExpression *arg) {
	JagPJCUnary *tree = jagp_jcunary_new(opcode, arg);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCTypeCast *jagp_tree_maker_type_cast(JagPTreeMaker *tree_maker, JagPJCTree *clazz, JagPJCExpression *expr) {
	JagPJCTypeCast *tree = jagp_jctype_cast_new(clazz, expr);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCTypeApply *jagp_tree_maker_type_apply(JagPTreeMaker *tree_maker, JagPJCTree *clazz, CatArrayWo *arguments) {
	JagPJCTypeApply *tree = jagp_jctype_apply_new(clazz, arguments);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCTypeIntersection *jagp_tree_maker_type_intersection(JagPTreeMaker *tree_maker, CatArrayWo *components) {
	JagPJCTypeIntersection *tree = jagp_jctype_intersection_new(components);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCParens *jagp_tree_maker_parens(JagPTreeMaker *tree_maker, JagPJCExpression *expr) {
	JagPJCParens *tree = jagp_jcparens_new(expr);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCAnnotatedType *jagp_tree_maker_annotated_type(JagPTreeMaker *tree_maker, CatArrayWo *annotations, JagPJCExpression *underlying_type) {
	JagPJCTypeIntersection *tree = jagp_jcannotated_type_new(annotations, underlying_type);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCArrayTypeTree *jagp_tree_maker_type_array(JagPTreeMaker *tree_maker, JagPJCExpression *elemtype) {
	JagPJCArrayTypeTree *tree = jagp_jcarray_type_tree_new(elemtype);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCArrayAccess *jagp_tree_maker_indexed(JagPTreeMaker *tree_maker, JagPJCExpression *indexed, JagPJCExpression *index) {
	JagPJCArrayAccess *tree = jagp_jcarray_access_new(indexed, index);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCLambda *jagp_tree_maker_lambda(JagPTreeMaker *tree_maker, CatArrayWo /*JagPJCVariableDecl*/*params, JagPJCTree *body) {
	JagPJCLambda *tree = jagp_jclambda_new(params, body);
	tree->parent.parent.parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCIf *jagp_tree_maker_if(JagPTreeMaker *tree_maker, JagPJCExpression *cond, JagPJCStatement *thenpart, JagPJCStatement *elsepart) {
	JagPJCIf *tree = jagp_jcif_new(cond, thenpart, elsepart);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}



JagPJCClassDecl *jagp_tree_maker_anonymous_class_def(JagPTreeMaker *tree_maker, JagPJCModifiers *mods, JagPName *name_empty, CatArrayWo *defs) {
	JagPJCClassDecl *tree = jagp_jcclass_decl_new(mods, name_empty, cat_array_wo_new(), NULL, cat_array_wo_new(), defs);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCExpressionStatement *jagp_tree_maker_exec(JagPTreeMaker *tree_maker, JagPJCExpression *expr) {
	JagPJCExpressionStatement *tree = jagp_jcexpression_statement_new(expr);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}


JagPJCTry *jagp_tree_maker_try(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCTree>*/ *resources, JagPJCBlock *body,
		CatArrayWo /*<JCCatch>*/ *catchers, JagPJCBlock *finalizer) {
	JagPJCTry *tree = jagp_jctry_new(resources, body, catchers, finalizer);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCReturn *jagp_tree_maker_return(JagPTreeMaker *tree_maker, JagPJCExpression *expr) {
	JagPJCReturn *tree = jagp_jcreturn_new(expr);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCThrow *jagp_tree_maker_throw(JagPTreeMaker *tree_maker, JagPJCExpression *expr) {
	JagPJCThrow *tree = jagp_jcthrow_new(expr);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCBreak *jagp_tree_maker_break(JagPTreeMaker *tree_maker, JagPName *label) {
	JagPJCBreak *tree = jagp_jcbreak_new(label);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCContinue *jagp_tree_maker_continue(JagPTreeMaker *tree_maker, JagPName *label) {
	JagPJCContinue *tree = jagp_jccontinue_new(label);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCAssert *jagp_tree_maker_assert(JagPTreeMaker *tree_maker, JagPJCExpression *cond, JagPJCExpression *detail) {
	JagPJCAssert *tree = jagp_jcassert_new(cond, detail);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCTypeUnion *jagp_tree_maker_type_union(JagPTreeMaker *tree_maker, CatArrayWo /*<JagPJCExpression>*/ *alternatives) {
	JagPJCTypeUnion *tree = jagp_jctype_union_new(alternatives);
	tree->parent.parent.pos = tree_maker->pos;
	return tree;
}

JagPJCCatch *jagp_tree_maker_catch(JagPTreeMaker *tree_maker, JagPJCVariableDecl *param, JagPJCBlock *body) {
	JagPJCCatch *tree = jagp_jccatch_new(param, body);
	tree->parent.pos = tree_maker->pos;
	return tree;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPTreeMaker *instance = JAGP_TREE_MAKER(self);
	JagPTreeMakerPrivate *priv = jagp_tree_maker_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
