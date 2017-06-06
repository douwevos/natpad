/*
   File:    jagptreeinfo.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#include "jagptreeinfo.h"
#include "jagpjcannotatedtype.h"
#include "jagpjcpackagedecl.h"
#include "jagpjcmethodinvocation.h"

#include "jagpjcconditional.h"
#include "jagpjcexpressionstatement.h"
#include "jagpjcarrayaccess.h"
#include "jagpjcfieldaccess.h"
#include "jagpjcarraytypetree.h"
#include "jagpjcnewclass.h"
#include "jagpjcvariabledecl.h"
#include "jagpjcerroneous.h"
#include "jagpjcassign.h"
#include "jagpjcmethoddecl.h"
#include "jagpjctypeapply.h"
#include "jagpjcunary.h"

JagPJCExpression *jagp_tree_info_type_in(JagPJCExpression *tree) {
	JagPTag tag = jagp_jctree_get_tag((JagPJCTree *) tree);
    switch (tag) {
		case JAGP_TAG_ANNOTATED_TYPE : {
			JagPJCAnnotatedType *ant = (JagPJCAnnotatedType *) tree;
			return ant->underlying_type;
		}
		case JAGP_TAG_IDENT: /* simple names */
		case JAGP_TAG_TYPEIDENT: /* primitive name */
		case JAGP_TAG_SELECT: /* qualified name */
		case JAGP_TAG_TYPEARRAY: /* array types */
		case JAGP_TAG_WILDCARD: /* wild cards */
		case JAGP_TAG_TYPEPARAMETER: /* type parameters */
		case JAGP_TAG_TYPEAPPLY: /* parameterized types */
		case JAGP_TAG_ERRONEOUS: /* error tree TODO: needed for BadCast JSR308 test case. Better way? */
			return tree;
//		default:
//			cat_log_fatal("Unexpected type tree: %O", tree);
    }
    return NULL;
}


/** Return true if the tree corresponds to an expression statement */
gboolean jagp_tree_info_is_expression_statement(JagPJCExpression *tree) {
	JagPTag tag = jagp_jctree_get_tag((JagPJCTree *) tree);
	switch(tag) {
		case JAGP_TAG_PREINC: case JAGP_TAG_PREDEC:
		case JAGP_TAG_POSTINC: case JAGP_TAG_POSTDEC:
		case JAGP_TAG_ASSIGN:
		case JAGP_TAG_BITOR_ASG: case JAGP_TAG_BITXOR_ASG: case JAGP_TAG_BITAND_ASG:
		case JAGP_TAG_SL_ASG: case JAGP_TAG_SR_ASG: case JAGP_TAG_USR_ASG:
		case JAGP_TAG_PLUS_ASG: case JAGP_TAG_MINUS_ASG:
		case JAGP_TAG_MUL_ASG: case JAGP_TAG_DIV_ASG: case JAGP_TAG_MOD_ASG:
		case JAGP_TAG_APPLY: case JAGP_TAG_NEWCLASS:
		case JAGP_TAG_ERRONEOUS:
			return TRUE;
		default:
			return FALSE;
	}
	return FALSE;
}

int jagp_tree_info_get_end_pos(JagPJCTree *tree) {
    if (tree == NULL) {
        return -1;
    }

    JagPTag tag = jagp_jctree_get_tag(tree);
    switch(tag) {
        case JAGP_TAG_PACKAGEDEF: {
            JagPJCPackageDecl *pd = (JagPJCPackageDecl *) tree;
            if (cat_array_wo_size(pd->annotations)==0) {
            	return pd->parent.pos;
            }
            JagPJCTree *ft = (JagPJCTree *) cat_array_wo_get_first(pd->annotations);
            return ft->pos;
        }
        case JAGP_TAG_APPLY:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCMethodInvocation*) tree)->meth);
        case JAGP_TAG_ASSIGN:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCAssign *) tree)->lhs);
        case JAGP_TAG_BITOR_ASG: case JAGP_TAG_BITXOR_ASG: case JAGP_TAG_BITAND_ASG:
        case JAGP_TAG_SL_ASG: case JAGP_TAG_SR_ASG: case JAGP_TAG_USR_ASG:
        case JAGP_TAG_PLUS_ASG: case JAGP_TAG_MINUS_ASG: case JAGP_TAG_MUL_ASG:
//        case JAGP_TAG_DIV_ASG: case JAGP_TAG_MOD_ASG:
//            return jagp_tree_info_get_end_pos(((JagPJCAssignOp *) tree).lhs);
        case JAGP_TAG_OR: case JAGP_TAG_AND: case JAGP_TAG_BITOR:
        case JAGP_TAG_BITXOR: case JAGP_TAG_BITAND: case JAGP_TAG_EQ:
        case JAGP_TAG_NE: case JAGP_TAG_LT: case JAGP_TAG_GT:
        case JAGP_TAG_LE: case JAGP_TAG_GE: case JAGP_TAG_SL:
        case JAGP_TAG_SR: case JAGP_TAG_USR: case JAGP_TAG_PLUS:
        case JAGP_TAG_MINUS: case JAGP_TAG_MUL: case JAGP_TAG_DIV:
//        case JAGP_TAG_MOD:
//            return jagp_tree_info_get_end_pos(((JagPJCBinary *) tree).lhs);
        case JAGP_TAG_CLASSDEF: {
            JagPJCClassDecl *node = (JagPJCClassDecl *)tree;
            if (node->modifiers->parent.pos != -1)
                return node->modifiers->parent.pos;
            break;
        }
//        case JAGP_TAG_CONDEXPR:
//            return jagp_tree_info_get_end_pos(((JagPJCConditional *) tree)->cond);
        case JAGP_TAG_EXEC:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCExpressionStatement *) tree)->expr);
        case JAGP_TAG_INDEXED:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCArrayAccess *) tree)->indexed);
        case JAGP_TAG_METHODDEF: {
            JagPJCMethodDecl *node = (JagPJCMethodDecl *) tree;
            if (node->mods->parent.pos != -1)
                return node->mods->parent.pos;
            if (cat_array_wo_size(node->typarams)>0) // List.nil() used for no typarams
                return jagp_tree_info_get_end_pos((JagPJCTree *) cat_array_wo_get_first(node->typarams));
            return node->restype == NULL ? node->parent.pos : jagp_tree_info_get_end_pos((JagPJCTree *) node->restype);
        }
        case JAGP_TAG_SELECT:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCFieldAccess *) tree)->selected);
        case JAGP_TAG_TYPEAPPLY:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCTypeApply *) tree)->clazz);
        case JAGP_TAG_TYPEARRAY:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCArrayTypeTree *) tree)->elemtype);
//        case JAGP_TAG_TYPETEST:
//            return jagp_tree_info_get_end_pos(((JagPJCInstanceOf *) tree).expr);
        case JAGP_TAG_POSTINC:
        case JAGP_TAG_POSTDEC:
            return jagp_tree_info_get_end_pos((JagPJCTree *) ((JagPJCUnary *) tree)->arg);
        case JAGP_TAG_ANNOTATED_TYPE: {
            JagPJCAnnotatedType *node = (JagPJCAnnotatedType *) tree;
            if (cat_array_wo_size(node->annotations)>0) {
                if (jagp_jctree_has_tag((JagPJCTree *) node->underlying_type, JAGP_TAG_TYPEARRAY) ||
                		jagp_jctree_has_tag((JagPJCTree *) node->underlying_type, JAGP_TAG_SELECT)) {
                    return jagp_tree_info_get_end_pos((JagPJCTree *) node->underlying_type);
                } else {
                    return jagp_tree_info_get_end_pos((JagPJCTree *) cat_array_wo_get_first(node->annotations));
                }
            } else {
                return jagp_tree_info_get_end_pos((JagPJCTree *) node->underlying_type);
            }
        }
        case JAGP_TAG_NEWCLASS: {
            JagPJCNewClass *node = (JagPJCNewClass *) tree;
            if (node->encl != NULL)
                return jagp_tree_info_get_end_pos((JagPJCTree *) node->encl);
            break;
        }
        case JAGP_TAG_VARDEF: {
            JagPJCVariableDecl *node = (JagPJCVariableDecl *) tree;
            if (node->mods->parent.pos != -1) {
                return node->mods->parent.pos;
            } else if (node->vartype == NULL) {
                //if there's no type (partially typed lambda parameter)
                //simply return node position
                return node->parent.parent.pos;
            } else {
                return jagp_tree_info_get_end_pos((JagPJCTree *) node->vartype);
            }
        }
        case JAGP_TAG_ERRONEOUS: {
            JagPJCErroneous *node = (JagPJCErroneous *) tree;
            if (node->errs != NULL && cat_array_wo_size(node->errs)>0)
                return jagp_tree_info_get_end_pos((JagPJCTree *) cat_array_wo_get_first(node->errs));
        }
    }
    return tree->pos;
}


/** Map operators to their precedence levels.
 */
int jagp_tree_info_op_prec(JagPTag op) {
	switch(op) {
		case JAGP_TAG_POS:
		case JAGP_TAG_NEG:
		case JAGP_TAG_NOT:
		case JAGP_TAG_COMPL:
		case JAGP_TAG_PREINC:
		case JAGP_TAG_PREDEC: return jagp_tree_info_prefixPrec;
		case JAGP_TAG_POSTINC:
		case JAGP_TAG_POSTDEC:
		case JAGP_TAG_NULLCHK: return jagp_tree_info_postfixPrec;
		case JAGP_TAG_ASSIGN: return jagp_tree_info_assignPrec;
		case JAGP_TAG_BITOR_ASG:
		case JAGP_TAG_BITXOR_ASG:
		case JAGP_TAG_BITAND_ASG:
		case JAGP_TAG_SL_ASG:
		case JAGP_TAG_SR_ASG:
		case JAGP_TAG_USR_ASG:
		case JAGP_TAG_PLUS_ASG:
		case JAGP_TAG_MINUS_ASG:
		case JAGP_TAG_MUL_ASG:
		case JAGP_TAG_DIV_ASG:
		case JAGP_TAG_MOD_ASG: return jagp_tree_info_assignopPrec;
		case JAGP_TAG_OR: return jagp_tree_info_orPrec;
		case JAGP_TAG_AND: return jagp_tree_info_andPrec;
		case JAGP_TAG_EQ:
		case JAGP_TAG_NE: return jagp_tree_info_eqPrec;
		case JAGP_TAG_LT:
		case JAGP_TAG_GT:
		case JAGP_TAG_LE:
		case JAGP_TAG_GE: return jagp_tree_info_ordPrec;
		case JAGP_TAG_BITOR: return jagp_tree_info_bitorPrec;
		case JAGP_TAG_BITXOR: return jagp_tree_info_bitxorPrec;
		case JAGP_TAG_BITAND: return jagp_tree_info_bitandPrec;
		case JAGP_TAG_SL:
		case JAGP_TAG_SR:
		case JAGP_TAG_USR: return jagp_tree_info_shiftPrec;
		case JAGP_TAG_PLUS:
		case JAGP_TAG_MINUS: return jagp_tree_info_addPrec;
		case JAGP_TAG_MUL:
		case JAGP_TAG_DIV:
		case JAGP_TAG_MOD: return jagp_tree_info_mulPrec;
		case JAGP_TAG_TYPETEST: return jagp_tree_info_ordPrec;
	}
	return -1;
}
