/*
   File:    jagpparser.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Mar 10, 2017
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

#include "jagpparser.h"
#include "jagptoken.h"
#include "tree/jagpjcmodifiers.h"
#include "tree/jagpjcblock.h"
#include "tree/jagpjcmemberreference.h"
#include "tree/jagptreemaker.h"
#include "code/jagpflags.h"
#include "code/jagpname.h"
#include "code/jagpcomment.h"
#include "tree/jagptreeinfo.h"
#include "../src-generated/jagpparserconfig.h"
#include "../src-generated/ast/jagpastactions.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "JagPParser"
#include <logging/catlog.h>

/** The number of precedence levels of infix operators.
 */
#define infixPrecedenceLevels 10


/** When terms are parsed, the mode determines which is expected:
 *     mode = EXPR        : an expression
 *     mode = TYPE        : a type
 *     mode = NOPARAMS    : no parameters allowed for type
 *     mode = TYPEARG     : type argument
 */
#define JAGP_MODE_EXPR      0x1
#define JAGP_MODE_TYPE      0x2
#define JAGP_MODE_NOPARAMS  0x4
#define JAGP_MODE_TYPEARG   0x8
#define JAGP_MODE_DIAMOND   0x10


struct _JagPParserPrivate {
	JagPILexer *lexer;
	GroRunITokenFactory *token_factory;
	CatArrayWo *messages;
	JagPCompilationUnit *compilation_unit;
	CatArrayWo *token_cash;


	JagPToken *token;
	CatArrayWo *token_list;
	int token_index;
	JagPTreeMaker *tree_maker;
	int mode;
	int last_mode;


	/** Switch: Should diamond operator be recognized? */
	gboolean allowDiamond;
	
	/** Switch: Should multicatch clause be accepted? */
	gboolean allowMulticatch;
	
	/** Switch: should we recognize try-with-resources? */
	gboolean allowTWR;
	
	/** Switch: should we allow (effectively) final variables as resources in try-with-resources? */
	gboolean allowEffectivelyFinalVariablesInTWR;
	
	/** Switch: should we fold strings? */
	gboolean allowStringFolding;
	
	/** Switch: should we recognize lambda expressions? */
	gboolean allowLambda;
	
	/** Switch: should we allow method/constructor references? */
	gboolean allowMethodReferences;
	
	/** Switch: should we recognize modules? */
	gboolean allowModules;
	
	/** Switch: should we allow default methods in interfaces? */
	gboolean allowDefaultMethods;
	
	/** Switch: should we allow static methods in interfaces? */
	gboolean allowStaticInterfaceMethods;
	
	/** Switch: should we allow private (instance) methods in interfaces? */
	gboolean allowPrivateInterfaceMethods;
	
	/** Switch: should we allow intersection types in cast? */
	gboolean allowIntersectionTypesInCast;
	
	/** Switch: should we keep docComments? */
	gboolean keepDocComments;
	
	/** Switch: should we keep line table? */
	gboolean keepLineMap;
	
	/** Switch: should we recognize type annotations? */
	gboolean allowTypeAnnotations;
	
	/** Switch: should we allow annotations after the method type parameters? */
	gboolean allowAnnotationsAfterTypeParams;
	
	/** Switch: should we allow '_' as an identifier? */
	gboolean allowUnderscoreIdentifier;
	
	/** Switch: is "this" allowed as an identifier?
	* This is needed to parse receiver types.
	*/
	gboolean allowThisIdent;
	
	
	/** The type of the method receiver, as specified by a first "this" parameter. */
	JagPJCVariableDecl *receiverParam;
	
	/** Type annotations that have already been read but have not yet been used. **/
	CatArrayWo /*<JCAnnotation>*/ *typeAnnotationsPushedBack;
	
	/**
	* If the parser notices extra annotations, then it either immediately
	* issues an error (if this variable is false) or places the extra
	* annotations in variable typeAnnotationsPushedBack (if this variable
	* is true).
	*/
	gboolean permitTypeAnnotationsPushBack;


	JagPName *names_error;
	JagPName *names_asterisk;
	JagPName *names_hyphen;
	JagPName *names_this;
	JagPName *names_class;
	JagPName *names_super;
	JagPName *names_empty;
	JagPName *names_init;
	JagPName *names_module;

	JagPName *names_requires;
	JagPName *names_exports;
	JagPName *names_to;
	JagPName *names_provides;
	JagPName *names_with;
	JagPName *names_uses;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPParser, jagp_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(JagPParser)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jagp_parser_class_init(JagPParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jagp_parser_init(JagPParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPParser *instance = JAGP_PARSER(object);
	JagPParserPrivate *priv = jagp_parser_get_instance_private(instance);
	cat_unref_ptr(priv->compilation_unit);
	cat_unref_ptr(priv->token_cash);
	cat_unref_ptr(priv->token_list);
	cat_unref_ptr(priv->lexer);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->tree_maker);
	cat_unref_ptr(priv->messages);
	cat_unref_ptr(priv->typeAnnotationsPushedBack);

	cat_unref_ptr(priv->names_error);
	cat_unref_ptr(priv->names_asterisk);
	cat_unref_ptr(priv->names_hyphen);
	cat_unref_ptr(priv->names_this);
	cat_unref_ptr(priv->names_class);
	cat_unref_ptr(priv->names_super);
	cat_unref_ptr(priv->names_empty);
	cat_unref_ptr(priv->names_init);
	cat_unref_ptr(priv->names_module);
	cat_unref_ptr(priv->names_requires);
	cat_unref_ptr(priv->names_exports);
	cat_unref_ptr(priv->names_to);
	cat_unref_ptr(priv->names_provides);
	cat_unref_ptr(priv->names_with);
	cat_unref_ptr(priv->names_uses);

	G_OBJECT_CLASS(jagp_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jagp_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gint l_message_compare(gconstpointer a, gconstpointer b) {
	GroRunLocation *loc_a = grorun_message_get_location((GroRunMessage *) a);
	GroRunLocation *loc_b = grorun_message_get_location((GroRunMessage *) b);
	int col_a, col_b;
	long row_a, row_b;
	grorun_location_get_begin(loc_a, &col_a, &row_a);
	grorun_location_get_begin(loc_b, &col_b, &row_b);

	if (row_a<row_b) {
		return -1;
	} else if (row_a>row_b) {
		return 1;
	} else if (col_a<col_b) {
		return -1;
	} else if (col_a>col_b) {
		return 1;
	}
	return 0;
}


static void l_next_token(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	jagp_ilexer_next_token(priv->lexer);
	JagPToken *token = jagp_ilexer_token(priv->lexer);
	priv->token = token;
	cat_array_wo_append(priv->token_list, (GObject *) token);
	cat_log_debug("token=%O", priv->token);
}


static gboolean l_peek_token_lah(JagPParser *parser, int lah, JagPTokenKind kind) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPToken *token = jagp_ilexer_token_lah(priv->lexer, lah+1);
	return token->kind==kind;
}


static gboolean l_peek_token(JagPParser *parser, JagPTokenKind kind) {
	return l_peek_token_lah(parser, 0, kind);
}


static void *to(JagPParser *parser, void *p) {
	return p;
}

static void *toP(JagPParser *parser, void *p) {
	return p;
}


JagPParser *jagp_parser_new(JagPILexer *lexer, GroRunITokenFactory *token_factory) {
	JagPParser *result = g_object_new(JAGP_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	JagPParserPrivate *priv = jagp_parser_get_instance_private(result);
	priv->lexer = cat_ref_ptr(lexer);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->token_list = cat_array_wo_new();
	priv->token_index = -1;
	priv->messages = cat_array_wo_new();

	priv->tree_maker = jagp_tree_maker_new();
	priv->mode = 0;
	priv->last_mode = 0;
	priv->receiverParam = NULL;

	priv->allowThisIdent = FALSE;
	priv->typeAnnotationsPushedBack = cat_array_wo_new();
	priv->permitTypeAnnotationsPushBack = FALSE;


	priv->names_error = jagp_name_new();
	priv->names_asterisk = jagp_name_new();
	priv->names_this = jagp_name_new();
	priv->names_class = jagp_name_new();
	priv->names_super = jagp_name_new();
	priv->names_empty = jagp_name_new();
	priv->names_init = jagp_name_new();
	priv->names_module = jagp_name_new();

	priv->names_requires = jagp_name_new();
	priv->names_exports = jagp_name_new();
	priv->names_to = jagp_name_new();
	priv->names_provides = jagp_name_new();
	priv->names_with = jagp_name_new();
	priv->names_uses = jagp_name_new();

	return result;
}

static JagPJCCompilationUnit *l_parse_compilation_unit(JagPParser *parser);


/*GroAstSpec * */void jagp_parser_run(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//
////	GroAstSpec *result = NULL;
//
//	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);
//
//	JagPAstActions *actions = jagp_ast_actions_new();
//
//
//
//	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, GRORUN_IACTION_HANDLER(actions));
//	grorun_parser_do_parse(context, scanner);
//
//	GroRunFork *fork = grorun_context_accepted_fork_first(context);
//	if (fork==NULL) {
//		if (grorun_context_error_fork_count(context)>0) {
//			fork = grorun_context_error_fork_at(context, 0);
//		}
//	}
//	cat_log_debug("#############################################################++++ fork=%O", fork);
//	if (fork) {
//
//
//		GroRunStackEntry *entry = grorun_fork_get_entry(fork, 1);
//		if (entry) {
//			GroRunIToken *main_lah = grorun_stack_entry_get_lookahead(entry);
//			cat_log_debug("*********** %O", main_lah);
//
//			GObject *val = grorun_itoken_get_value(main_lah);
//			cat_log_debug("*********** %O", val);
////			if (GROAST_IS_SPEC(val)) {
////				result = (GroAstSpec *) val;
////				l_analyze_spec(parser, result);
////				priv->ast_specification = result;
////				cat_ref_ptr(result);
////
////			}
//
//		}
//
//		GroRunMessage *messages = grorun_fork_get_messages(fork);
//		cat_log_debug("messages=%O", messages);
//		if (messages) {
//			while(messages) {
//				cat_array_wo_append(priv->messages, (GObject *) messages);
//				messages = grorun_message_get_parent_message(messages);
//			}
//		}
//
//
//		if (cat_array_wo_size(priv->messages)>0) {
//			cat_array_wo_sort(priv->messages, (GCompareFunc) l_message_compare);
//		}
//	}


	l_next_token(parser);

	priv->compilation_unit = l_parse_compilation_unit(parser);

	priv->token_cash = cat_array_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->token_list);
	while(cat_iiterator_has_next(iter)) {
		JagPToken *token = (JagPToken *) cat_iiterator_next(iter);
		if (token->source!=NULL) {
			cat_array_wo_append(priv->token_cash, (GObject *) token->source);
		}
	}
	cat_unref_ptr(iter);
}



CatArrayWo *jagp_parser_get_token_cash(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return priv->token_cash;
}

CatArrayWo *jagp_parser_get_messages(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return priv->messages;
}

gboolean lax_identifier_accepts(JagPTokenKind kind) {
	return (kind == JAGP_KIND_IDENTIFIER) || (kind == JAGP_KIND_UNDERSCORE) || (kind == JAGP_KIND_ASSERT) || (kind == JAGP_KIND_ENUM);
}

static void l_store_end(JagPJCTree *tree, int endpos) {
//	endPosTable.storeEnd(tree, endpos);
}


static void l_set_error_end_pos(JagPParser *parser, int err_pos) {
//	endPosTable.setErrorEndPos(errPos);
}


static void l_error(JagPParser *parser, int err_pos) {
}


static JagPJCErroneous *l_syntax_error(JagPParser *parser, int pos, CatArrayWo /*<JCTree>*/ *errs, const char *key/*, TokenKind... args */) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_set_error_end_pos(parser, pos);
	if (errs==NULL) {
		errs = cat_array_wo_new();
	}
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//        reportSyntaxError(err, key, (Object[])args);
	if (errs != NULL) {
		JagPJCTree *last = (JagPJCTree *) cat_array_wo_get_last(errs);
		if (last != NULL) {
			l_store_end(last, pos);
		}
	}
	return toP(parser, err);
}


/** If next input token matches given token, skip it, otherwise report
 *  an error.
 */
static void l_accept(JagPParser *parser, JagPTokenKind tk) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (priv->token->kind == tk) {
		l_next_token(parser);
	} else {
		l_set_error_end_pos(parser, priv->token->pos);
//		reportSyntaxError(jagp_ilexer_prev_token(priv->lexer)->endPos, "expected", tk);
	}
}

/** Report an illegal start of expression/type error at given position.
 */
static JagPJCExpression *l_illegal(JagPParser *parser, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_set_error_end_pos(parser, pos);
	if ((priv->mode & JAGP_MODE_EXPR) != 0) {
		return (JagPJCExpression *) l_syntax_error(parser, pos, NULL, "illegal.start.of.expr");
	} else {
		return (JagPJCExpression *) l_syntax_error(parser, pos, NULL, "illegal.start.of.type");
	}
}


/** Diagnose a modifier flag from the set, if any. */
static void l_check_no_mods(JagPParser *parser, long long mods) {
	if (mods != 0) {
//		long lowestMod = mods & -mods;
//		error(priv->token->pos, "mod.not.allowed.here",
//				Flags.asFlagSet(lowestMod));
	}
}

static void l_attach(JagPParser *parser, JagPJCTree *tree, JagPComment *dc) {
//	if (keepDocComments && dc != NULL) {
////          System.out.println("doc comment = ");System.out.println(dc);//DEBUG
//		docComments.putComment(tree, dc);
//	}
}



/** Get the start position for a tree node.  The start position is
 * defined to be the position of the first character of the first
 * token of the node's source text.
 * @param tree  The tree node
 */
static int l_get_start_pos(JagPJCTree *tree) {
	return 0;
//	return TreeInfo.getStartPos(tree);
}

/**
 * Get the end position for a tree node.  The end position is
 * defined to be the position of the last character of the last
 * token of the node's source text.  Returns Position.NOPOS if end
 * positions are not generated or the position is otherwise not
 * found.
 * @param tree  The tree node
 */
static int l_get_end_pos(JagPJCTree *tree) {
	return 0;
//	return endPosTable.getEndPos(tree);
}











typedef JagPJCTree *(*JagPBasicErrorRecoveryAction)(JagPParser *parser);



static JagPName *l_ident(JagPParser *parser, gboolean advance_on_errors);
static JagPJCExpression *l_qualident(JagPParser *parser, gboolean allow_annos);
static JagPJCExpression *l_literal(JagPParser *parser, JagPName *prefix);
static JagPJCExpression *l_literal_at(JagPParser *parser, JagPName *prefix, int pos);
static JagPJCExpression *l_parse_expression(JagPParser *parser);
static JagPJCExpression *l_parse_type(JagPParser *parser);
static JagPJCExpression *l_parse_type_with_annotations(JagPParser *parser, CatArrayWo * /* <JCAnnotation> */ annotations);
static JagPJCExpression *l_unannotated_type(JagPParser *parser);
static JagPJCExpression *l_term(JagPParser *parser, int newmode);
static JagPJCExpression *l_term_raw(JagPParser *parser);
static JagPJCExpression *l_term_rest(JagPParser *parser, JagPJCExpression *t);
static JagPJCExpression *l_term1(JagPParser *parser);
static JagPJCExpression *l_term1_rest(JagPParser *parser, JagPJCExpression *t);
static JagPJCExpression *l_term2(JagPParser *parser);
static JagPJCExpression *l_term2_rest(JagPParser *parser, JagPJCExpression *t, int minprec);
static JagPJCExpression *l_term3(JagPParser *parser);
static JagPJCExpression *l_term3_rest(JagPParser *parser, JagPJCExpression *t, CatArrayWo */*<JAgPJCExpression>*/ type_args);
static gboolean l_is_unbound_member_ref(JagPParser *parser);
static JagPParensResult l_analyze_parens(JagPParser *parser);
static JagPJCExpression *l_lambda_expression_or_statement(JagPParser *parser, gboolean hasParens, gboolean explicitParams, int pos);
static JagPJCExpression *l_lambda_expression_or_statement_rest(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos);
static JagPJCExpression *l_lambda_statement(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos, int pos2);
static JagPJCExpression *l_lambda_expression(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos);
static JagPJCExpression *l_super_suffix(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t);
static JagPJCPrimitiveTypeTree *l_basic_type(JagPParser *parser);
static JagPJCExpression *l_arguments_opt(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t);
static CatArrayWo */*<JagPJCExpression>*/ l_arguments(JagPParser *parser);
static JagPJCMethodInvocation *l_arguments_apply(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t);
static JagPJCExpression *l_type_arguments_apply_opt(JagPParser *parser, JagPJCExpression *t);
static CatArrayWo */*<JagPJCExpression>*/ l_type_arguments_opt(JagPParser *parser, int useMode);
static CatArrayWo */*<JagPJCExpression>*/ l_type_arguments(JagPParser *parser, gboolean diamondAllowed);
static JagPJCExpression *l_type_argument(JagPParser *parser);
static JagPJCTypeApply *l_type_arguments_apply(JagPParser *parser, JagPJCExpression *t, gboolean diamondAllowed);
static JagPJCExpression *l_brackets_opt(JagPParser *parser, JagPJCExpression *t, CatArrayWo */*<JagPJCAnnotation>*/ annotations);
static JagPJCExpression *l_brackets_opt_simple(JagPParser *parser, JagPJCExpression *t);
static JagPJCExpression *l_brackets_opt_cont(JagPParser *parser, JagPJCExpression *t, int pos, CatArrayWo */*<JagPJCAnnotation>*/ annotations);
static JagPJCExpression *l_brackets_suffix(JagPParser *parser, JagPJCExpression *t);
static JagPJCExpression *l_member_reference_suffix(JagPParser *parser, JagPJCExpression *t);
static JagPJCExpression *l_member_reference_suffix_ext(JagPParser *parser, int pos1, JagPJCExpression *t);
static JagPJCExpression *l_creator(JagPParser *parser, int newpos, CatArrayWo */*<JagPJCExpression>*/ typeArgs);
static JagPJCExpression *l_inner_creator(JagPParser *parser, int newpos, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *encl);
static JagPJCExpression *l_array_creator_rest(JagPParser *parser, int newpos, JagPJCExpression *elemtype);
static JagPJCNewClass *l_class_creator_rest(JagPParser *parser, int newpos, JagPJCExpression *encl, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t);
static JagPJCExpression *l_array_initializer(JagPParser *parser, int newpos, JagPJCExpression *t);
static JagPJCExpression *l_variable_initializer(JagPParser *parser);
static JagPJCExpression *l_par_expression(JagPParser *parser);
static JagPJCBlock *l_block_ext(JagPParser *parser, int pos, long flags);
static JagPJCBlock *l_block(JagPParser *parser);
static CatArrayWo /*<JCStatement>*/ *l_block_statements(JagPParser *parser);
static JagPJCStatement *l_parse_statement_as_block(JagPParser *parser);
static CatArrayWo */*<JCStatement>*/ l_block_statement(JagPParser *parser);
static JagPJCStatement *l_parse_simple_statement(JagPParser *parser);
static JagPJCStatement *l_do_recover(JagPParser *parser, int startPos, JagPBasicErrorRecoveryAction action, CatStringWo *key);
static JagPJCCatch *l_catch_clause(JagPParser *parser);
static CatArrayWo /*<JagPJCExpression>*/ *l_catch_types(JagPParser *parser);
static CatArrayWo /*<JCCase>*/ *l_switch_block_statement_groups(JagPParser *parser);
static JagPJCCase *l_switch_block_statement_group(JagPParser *parser);
static CatArrayWo /*<? super JCExpressionStatement>>*/ *l_more_statement_expressions(JagPParser *parser, int pos, JagPJCExpression *first, CatArrayWo  *stats);
static CatArrayWo */*<JCStatement>*/ l_for_init(JagPParser *parser);
static CatArrayWo /*<JCExpressionStatement>*/ *l_for_update(JagPParser *parser);
static CatArrayWo */*<JagPJCAnnotation>*/ l_annotations_opt(JagPParser *parser, JagPTag kind);
static CatArrayWo */*<JagPJCAnnotation>*/ l_type_annotations_opt(JagPParser *parser);
static JagPJCModifiers *l_modifiers_opt(JagPParser *parser, JagPJCModifiers *partial);
static JagPJCAnnotation *l_annotation(JagPParser *parser, int pos, JagPTag kind);
static CatArrayWo */*<JagPJCExpression>*/ l_annotation_field_values_opt(JagPParser *parser);
static CatArrayWo */*<JagPJCExpression>*/ l_annotation_field_values(JagPParser *parser);
static JagPJCExpression *l_annotation_field_value(JagPParser *parser);
static JagPJCExpression *l_annotation_value(JagPParser *parser);
static CatArrayWo /*<T extends ListBuffer<? super JCVariableDecl>>*/ *l_variable_declarators(JagPParser *parser, JagPJCModifiers *mods,
		JagPJCExpression *type, CatArrayWo * vdefs);
static CatArrayWo /*<T extends ListBuffer<? super JCVariableDecl>>*/ *l_variable_declarators_rest(JagPParser *parser, int pos,
							JagPJCModifiers *mods, JagPJCExpression *type,JagPName *name, gboolean reqInit,
							JagPComment *dc, CatArrayWo * vdefs);
static JagPJCVariableDecl *l_variable_declarator(JagPParser *parser, JagPJCModifiers *mods, JagPJCExpression *type, gboolean reqInit, JagPComment *dc);
static JagPJCVariableDecl *l_variable_declarator_rest(JagPParser *parser, int pos, JagPJCModifiers *mods, JagPJCExpression *type, JagPName *name,
							  gboolean reqInit, JagPComment *dc);
static JagPJCVariableDecl *l_variable_declarator_id(JagPParser *parser, JagPJCModifiers *mods, JagPJCExpression *type, gboolean lambdaParameter);
static CatArrayWo */*<JCTree>*/ l_resources(JagPParser *parser);
static JagPJCTree *l_resource(JagPParser *parser);
//static JagPJCModuleDecl *moduleDecl(JagPParser *parser, JagPComment *dc);
static CatArrayWo /*<JagPJCDirective>*/ *l_module_directive_list(JagPParser *parser);
static JagPJCTree *l_import_declaration(JagPParser *parser);
static JagPJCTree *l_type_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *docComment);
static JagPJCStatement *l_class_or_interface_or_enum_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc);
static JagPJCClassDecl *l_class_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc);
static JagPJCClassDecl *l_interface_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc);
static JagPJCClassDecl *l_enum_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc);
static CatArrayWo */*<JCTree>*/ l_enum_body(JagPParser *parser, JagPName *enumName);
static JagPJCTree *l_enumerator_declaration(JagPParser *parser, JagPName *enumName);
static CatArrayWo */*<JagPJCExpression>*/ l_type_list(JagPParser *parser);
static CatArrayWo */*<JCTree>*/ l_class_or_interface_body(JagPParser *parser, JagPName *className, gboolean isInterface);
static CatArrayWo */*<JCTree>*/ l_class_or_interface_body_declaration(JagPParser *parser, JagPName *className, gboolean isInterface);
static JagPJCTree *l_method_declarator_rest(JagPParser *parser, int pos, JagPJCModifiers *mods, JagPJCExpression *type, JagPName *name,
			CatArrayWo /*<JagPJCTypeParameter>*/ * typarams, gboolean isInterface, gboolean isVoid, JagPComment *dc);
static CatArrayWo */*<JagPJCExpression>*/ l_qualident_list(JagPParser *parser);
static CatArrayWo /*<JagPJCTypeParameter>*/ *l_type_parameters_opt(JagPParser *parser);
static JagPJCTypeParameter *l_type_parameter(JagPParser *parser);
static CatArrayWo */*JagPJCVariableDecl>*/ l_formal_parameters(JagPParser *parser, gboolean lambdaParameters);
static CatArrayWo */*JagPJCVariableDecl>*/ l_implicit_parameters(JagPParser *parser, gboolean hasParens);
static JagPJCModifiers *l_opt_final(JagPParser *parser, long flags);
static JagPJCExpression *l_insert_annotations_to_most_inner(JagPParser *parser, JagPJCExpression *type, CatArrayWo */*<JagPJCAnnotation>*/ annos,
				gboolean createNewLevel);
static JagPJCVariableDecl *l_formal_parameter(JagPParser *parser, gboolean lambdaParameter);
static JagPJCVariableDecl *l_implicit_parameter(JagPParser *parser);
static JagPJCExpression *l_check_expr_stat(JagPParser *parser, JagPJCExpression *t);
static int l_prec(JagPTokenKind token);
static int l_earlier(int pos1, int pos2);
static JagPTag l_optag(JagPTokenKind token);

static JagPTag l_unoptag(JagPTokenKind token);
static JagPTypeTag l_typetag(JagPTokenKind token);
static void l_check_diamond(JagPParser *parser);
static void l_check_multicatch(JagPParser *parser);
static void l_check_try_with_resources(JagPParser *parser);
static void l_check_variable_in_try_with_resources(JagPParser *parser, int startPos);
static void l_check_lambda(JagPParser *parser);
static void l_check_method_references(JagPParser *parser);
static void l_check_default_methods(JagPParser *parser);
static void l_check_intersection_types_in_cast(JagPParser *parser);
static void l_check_static_interface_methods(JagPParser *parser);
static void l_check_type_annotations(JagPParser *parser);
static void l_check_private_interface_methods(JagPParser *parser);
static void l_check_annotations_after_type_params(JagPParser *parser, int pos);

static int l_list_head_pos(CatArrayWo *tree_list) {
	JagPJCTree *ft = (JagPJCTree *) cat_array_wo_get_first(tree_list);
	return ft->pos;
}

/* M1 */
static JagPName *l_ident(JagPParser *parser, gboolean advance_on_errors) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_ident:token=%d", priv->token->kind);
	if (priv->token->kind == JAGP_KIND_IDENTIFIER) {
		JagPName *name = (JagPName *) priv->token->value;
		l_next_token(parser);
		return cat_ref_ptr(name);
//	} else if (priv->token->kind == JAGP_KIND_ASSERT) {
//		error(priv->token->pos, "assert.as.identifier");
//		l_next_token(parser);
//		return names.error;
//	} else if (priv->token->kind == ENUM) {
//		error(priv->token->pos, "enum.as.identifier");
//		l_next_token(parser);
//		return names.error;
//	} else if (priv->token->kind == THIS) {
//		if (allowThisIdent) {
//			// Make sure we're using a supported source version.
//			l_check_type_annotations(parser);
//			Name name = jagp_token_name(priv->token);
//			l_next_token(parser);
//			return name;
//		} else {
//			error(priv->token->pos, "this.as.identifier");
//			l_next_token(parser);
//			return names.error;
//		}
//	} else if (priv->token->kind == UNDERSCORE) {
//		if (allowUnderscoreIdentifier) {
//			warning(priv->token->pos, "underscore.as.identifier");
//		} else {
//			error(priv->token->pos, "underscore.as.identifier");
//		}
//		Name name = jagp_token_name(priv->token);
//		l_next_token(parser);
//		return name;
	} else {
		l_accept(parser, JAGP_KIND_IDENTIFIER);
		if (advance_on_errors) {
			l_next_token(parser);
		}
		return cat_ref_ptr(priv->names_error);
	}
}

/**
 * Qualident = Ident { DOT [Annotations] Ident }
 */
/* M1 */
static JagPJCExpression *l_qualident(JagPParser *parser, gboolean allow_annos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPName *ident_name = l_ident(parser, FALSE);
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPJCExpression *ident = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, ident_name);
	cat_unref_ptr(ident_name);
	while (priv->token->kind == JAGP_KIND_DOT) {
		int pos = priv->token->pos;
		l_next_token(parser);
		CatArrayWo *tyannos = NULL;
		if (allow_annos) {
			tyannos = l_type_annotations_opt(parser);
		}
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPName *sub_name = l_ident(parser, FALSE);
		JagPJCExpression *next_t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, ident, sub_name);
		cat_unref_ptr(sub_name);
		cat_unref_ptr(ident);
		ident = next_t;
		if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
			jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
			JagPJCExpression *thetype = ident;
			ident = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, thetype);
			cat_unref_ptr(thetype);
			ident = toP(parser, ident);
		}
		cat_unref_ptr(tyannos);
	}
	return ident;
}

/* M1 */
static JagPJCExpression *l_literal(JagPParser *parser, JagPName *prefix) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_literal:token=%d", priv->token->kind);
	return l_literal_at(parser, prefix, priv->token->pos);
}

//where
//boolean isZero(String s) {
//	char[] cs = s.toCharArray();
//	int base = ((cs.length > 1 && Character.toLowerCase(cs[1]) == 'x') ? 16 : 10);
//	int i = ((base==16) ? 2 : 0);
//	while (i < cs.length && (cs[i] == '0' || cs[i] == '.')) i++;
//	return !(i < cs.length && (Character.digit(cs[i], base) > 0));
//}
//
//String strval(Name prefix) {
//	String s = token.stringVal();
//	return prefix.isEmpty() ? s : prefix + s;
//}

/**
 * Literal =
 *     INTLITERAL
 *   | LONGLITERAL
 *   | FLOATLITERAL
 *   | DOUBLELITERAL
 *   | CHARLITERAL
 *   | STRINGLITERAL
 *   | TRUE
 *   | FALSE
 *   | NULL
 */
/* M1 */
static JagPJCExpression *l_literal_at(JagPParser *parser, JagPName *prefix, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *t = NULL;
//	JagPJCExpression *t = errorTree;
	switch (priv->token->kind) {
		case JAGP_KIND_INTLITERAL:
//		try {
			jagp_tree_maker_at(priv->tree_maker, pos);
			t = (JagPJCExpression *) jagp_tree_maker_literal(priv->tree_maker, JAGP_TP_TAG_INT, NULL);
//			t = F.at(pos).Literal(
//				TypeTag.INT,
//				Convert.string2int(strval(prefix), token.radix()));
//		} catch (NumberFormatException ex) {
//			error(priv->token->pos, "int.number.too.large", strval(prefix));
//		}
		break;
//	case LONGLITERAL:
//		try {
//			t = F.at(pos).Literal(
//				TypeTag.LONG,
//				new Long(Convert.string2long(strval(prefix), token.radix())));
//		} catch (NumberFormatException ex) {
//			error(priv->token->pos, "int.number.too.large", strval(prefix));
//		}
//		break;
//	case FLOATLITERAL: {
//		String proper = token.radix() == 16 ?
//				("0x"+ token.stringVal()) :
//				token.stringVal();
//		Float n;
//		try {
//			n = Float.valueOf(proper);
//		} catch (NumberFormatException ex) {
//			// error already reported in scanner
//			n = Float.NaN;
//		}
//		if (n.floatValue() == 0.0f && !isZero(proper))
//			error(priv->token->pos, "fp.number.too.small");
//		else if (n.floatValue() == Float.POSITIVE_INFINITY)
//			error(priv->token->pos, "fp.number.too.large");
//		else
//			t = F.at(pos).Literal(TypeTag.FLOAT, n);
//		break;
//	}
//	case DOUBLELITERAL: {
//		String proper = token.radix() == 16 ?
//				("0x"+ token.stringVal()) :
//				token.stringVal();
//		Double n;
//		try {
//			n = Double.valueOf(proper);
//		} catch (NumberFormatException ex) {
//			// error already reported in scanner
//			n = Double.NaN;
//		}
//		if (n.doubleValue() == 0.0d && !isZero(proper))
//			error(priv->token->pos, "fp.number.too.small");
//		else if (n.doubleValue() == Double.POSITIVE_INFINITY)
//			error(priv->token->pos, "fp.number.too.large");
//		else
//			t = F.at(pos).Literal(TypeTag.DOUBLE, n);
//		break;
//	}
//	case CHARLITERAL:
//		t = F.at(pos).Literal(
//			TypeTag.CHAR,
//			token.stringVal().charAt(0) + 0);
//		break;
//	case STRINGLITERAL:
//		t = F.at(pos).Literal(
//			TypeTag.CLASS,
//			token.stringVal());
//		break;
//	case TRUE: case FALSE:
//		t = F.at(pos).Literal(
//			TypeTag.BOOLEAN,
//			(priv->token->kind == TRUE ? 1 : 0));
//		break;
//	case NULL:
//		t = F.at(pos).Literal(
//			TypeTag.BOT,
//			NULL);
//		break;
		default:
			// Assert.error();
			break;
	}
//	if (t == errorTree)
//		t = F.at(pos).Erroneous();
//	l_store_end(t, token.endPos);
	l_next_token(parser);
	return t;
}


/** terms can be either expressions or types.
 */
/* M1 */
static JagPJCExpression *l_parse_expression(JagPParser *parser) {
	return l_term(parser, JAGP_MODE_EXPR);
}


/**
 * parses (optional) type annotations followed by a type. If the
 * annotations are present before the type and are not consumed during array
 * parsing, this method returns a {@link JCAnnotatedType} consisting of
 * these annotations and the underlying type. Otherwise, it returns the
 * underlying type.
 *
 * <p>
 *
 * Note that this method sets {@code mode} to {@code TYPE} first, before
 * parsing annotations.
 */
/* M1 */
static JagPJCExpression *l_parse_type(JagPParser *parser) {
	CatArrayWo * /*<JCAnnotation>*/ annotations = l_type_annotations_opt(parser);
	JagPJCExpression *result = l_parse_type_with_annotations(parser, annotations);
	cat_unref_ptr(annotations);
	return result;
}

/* M1 */
static JagPJCExpression *l_parse_type_with_annotations(JagPParser *parser, CatArrayWo * /* <JCAnnotation> */ annotations) {
	JagPJCExpression *result = l_unannotated_type(parser);

	if (cat_array_wo_size(annotations)>0) {
		JagPJCExpression *theinner = result;
		result = l_insert_annotations_to_most_inner(parser, theinner, annotations, FALSE);
		cat_unref_ptr(theinner);
	}

	return result;
}

/* M1 */
static JagPJCExpression *l_unannotated_type(JagPParser *parser) {
	return l_term(parser, JAGP_MODE_TYPE);
}

/* M1 */
static JagPJCExpression *l_term(JagPParser *parser, int newmode) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_term:token=%d", priv->token->kind);
	int prevmode = priv->mode;
	priv->mode = newmode;
	JagPJCExpression *result = l_term_raw(parser);
	priv->last_mode = priv->mode;
	priv->mode = prevmode;
	cat_log_debug("l_term:done:result=%O", result);
	cat_log_indent_level--;
	return result;
}

/**
 *  {@literal
 *  Expression = Expression1 [ExpressionRest]
 *  ExpressionRest = [AssignmentOperator Expression1]
 *  AssignmentOperator = "=" | "+=" | "-=" | "*=" | "/=" |
 *                       "&=" | "|=" | "^=" |
 *                       "%=" | "<<=" | ">>=" | ">>>="
 *  Type = Type1
 *  TypeNoParams = TypeNoParams1
 *  StatementExpression = Expression
 *  ConstantExpression = Expression
 *  }
 */
/* M1 */
static JagPJCExpression *l_term_raw(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_term_raw:token=%d", priv->token->kind);
	JagPJCExpression *term1rest = l_term1(parser);
	JagPTokenKind tk = priv->token->kind;
	JagPJCExpression *result = NULL;
	if ((priv->mode & JAGP_MODE_EXPR) != 0 &&
		((tk == JAGP_KIND_EQ) ||
		(tk == JAGP_KIND_PLUSEQ) ||
		(tk == JAGP_KIND_SUBEQ) ||
		(tk == JAGP_KIND_STAREQ) ||
		(tk == JAGP_KIND_SLASHEQ) ||
		(tk == JAGP_KIND_AMPEQ) ||
		(tk == JAGP_KIND_BAREQ) ||
		(tk == JAGP_KIND_CARETEQ) ||
		(tk == JAGP_KIND_PERCENTEQ) ||
		(tk == JAGP_KIND_LTLTEQ) ||
		(tk == JAGP_KIND_GTGTEQ) ||
		(tk == JAGP_KIND_GTGTGTEQ))) {
		result = l_term_rest(parser, term1rest);
		cat_unref_ptr(term1rest);
	} else {
		result = term1rest;
	}
	cat_log_debug("l_term_raw:done:result=%O", result);
	cat_log_indent_level--;
	return result;
}


static JagPJCExpression *l_term_rest(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	switch (priv->token->kind) {
		case JAGP_KIND_EQ: {
			int pos = priv->token->pos;
			l_next_token(parser);
			priv->mode = JAGP_MODE_EXPR;
			JagPJCExpression *t1 = l_term_raw(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			return (JagPJCExpression *) jagp_tree_maker_assign(priv->tree_maker, t, t1);
		}
		case JAGP_KIND_PLUSEQ:
		case JAGP_KIND_SUBEQ:
		case JAGP_KIND_STAREQ:
		case JAGP_KIND_SLASHEQ:
		case JAGP_KIND_PERCENTEQ:
		case JAGP_KIND_AMPEQ:
		case JAGP_KIND_BAREQ:
		case JAGP_KIND_CARETEQ:
		case JAGP_KIND_LTLTEQ:
		case JAGP_KIND_GTGTEQ:
		case JAGP_KIND_GTGTGTEQ: {
			int pos = priv->token->pos;
			JagPTokenKind tk = priv->token->kind;
			l_next_token(parser);
			priv->mode = JAGP_MODE_EXPR;
			JagPJCExpression *t1 = l_term_raw(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			return (JagPJCExpression *) jagp_tree_maker_assignop(priv->tree_maker, l_optag(tk), (JagPJCTree *) t,  (JagPJCTree *) t1);
		}
		default:
			return t;
    }
}

/** Expression1   = Expression2 [Expression1Rest]
 *  Type1         = Type2
 *  TypeNoParams1 = TypeNoParams2
 */
/* M1 */
static JagPJCExpression *l_term1(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_term1:token=%d", priv->token->kind);
	JagPJCExpression *term2res = l_term2(parser);
	JagPJCExpression *result = NULL;
	if ((priv->mode & JAGP_MODE_EXPR) != 0 && priv->token->kind == JAGP_KIND_QUES) {
		priv->mode = JAGP_MODE_EXPR;
		result = l_term1_rest(parser, term2res);
		cat_unref_ptr(term2res);
	} else {
		result = term2res;
	}
	cat_log_debug("l_term1:done:result=%O", result);
	cat_log_indent_level--;
	return result;
}

/** Expression1Rest = ["?" Expression ":" Expression1]
 */
static JagPJCExpression *l_term1_rest(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term1_rest:token=%d", priv->token->kind);
	if (priv->token->kind == JAGP_KIND_QUES) {
		int pos = priv->token->pos;
		l_next_token(parser);
		JagPJCExpression *t1 = l_term_raw(parser);
		l_accept(parser, JAGP_KIND_COLON);
		JagPJCExpression *t2 = l_term1(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		return (JagPJCExpression *) jagp_tree_maker_conditional(priv->tree_maker, t, t1, t2);
	} else {
		return t;
	}
}

/** Expression2   = Expression3 [Expression2Rest]
 *  Type2         = Type3
 *  TypeNoParams2 = TypeNoParams3
 */
/* M1 */
static JagPJCExpression *l_term2(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_term2:token=%d", priv->token->kind);
	JagPJCExpression *term3res = l_term3(parser);
	cat_log_debug("l_term2:term3res=%O", term3res);
	JagPJCExpression *result = NULL;
	if ((priv->mode & JAGP_MODE_EXPR) != 0 && l_prec(priv->token->kind) >= jagp_tree_info_orPrec) {
		priv->mode = JAGP_MODE_EXPR;
		result = l_term2_rest(parser, term3res, jagp_tree_info_orPrec);
		cat_unref_ptr(term3res);
	} else {
		result = term3res;
	}
	cat_log_debug("l_term2:done:result=%O", result);
	cat_log_indent_level--;
	return result;
}

static JagPJCExpression *l_make_op(JagPParser *parser, int pos, JagPTokenKind topOp,
							JagPJCExpression *od1, JagPJCExpression *od2);
static JagPJCExpression *l_fold_strings(JagPParser *parser, JagPJCExpression *tree);
static JagPJCExpression **l_new_od_stack(JagPParser *parser);
static JagPToken **l_new_op_stack(JagPParser *parser);

/*  Expression2Rest = {infixop Expression3}
 *                  | Expression3 instanceof Type
 *  infixop         = "||"
 *                  | "&&"
 *                  | "|"
 *                  | "^"
 *                  | "&"
 *                  | "==" | "!="
 *                  | "<" | ">" | "<=" | ">="
 *                  | "<<" | ">>" | ">>>"
 *                  | "+" | "-"
 *                  | "*" | "/" | "%"
 */
static JagPJCExpression *l_term2_rest(JagPParser *parser, JagPJCExpression *t, int minprec) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_term2_rest:token=%d, t=%O", priv->token->kind, t);
	JagPJCExpression **odStack = l_new_od_stack(parser);
	JagPToken **opStack = l_new_op_stack(parser);

	/* optimization, was odStack = new Tree[...]; opStack = new Tree[...]; */
	int top = 0;
	odStack[0] = cat_ref_ptr(t);
	JagPToken *topOp = NULL;
	while (l_prec(priv->token->kind) >= minprec) {
		cat_log_indent_level++;
		cat_log_debug("l_term2_rest:token=%d, minprec=%d", priv->token->kind, minprec);
		cat_ref_swap(opStack[top], topOp);
		top++;
		topOp = priv->token;
		l_next_token(parser);
		odStack[top] = (topOp->kind == JAGP_KIND_INSTANCEOF) ? l_parse_type(parser) : l_term3(parser);
		while (top > 0 && l_prec(topOp->kind) >= l_prec(priv->token->kind)) {
			JagPJCExpression *nw_op =  l_make_op(parser, topOp->pos, topOp->kind, odStack[top-1],
									odStack[top]);
			cat_unref_ptr(odStack[top]);
			cat_unref_ptr(odStack[top-1]);
			odStack[top-1] = nw_op;
			top--;
			topOp = opStack[top];
		}
		cat_log_indent_level--;
	}
	cat_log_debug("l_term2_rest:token=%d, top=%d", priv->token->kind, top);
//	Assert.check(top == 0);
	t = odStack[0];

	if (jagp_jctree_has_tag((JagPJCTree *) t, JAGP_TAG_PLUS)) {
		t = l_fold_strings(parser, t);
	}

//	odStackSupply.add(odStack);
//	opStackSupply.add(opStack);
	cat_log_debug("l_term2_rest:done=%O", t);
	cat_log_indent_level--;
	return t;
}
//where
	/** Construct a binary or type test node.
	 */
	static JagPJCExpression *l_make_op(JagPParser *parser, int pos, JagPTokenKind topOp,
								JagPJCExpression *od1, JagPJCExpression *od2) {
		JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		if (topOp == JAGP_KIND_INSTANCEOF) {
//			return jagp_tree_maker_type_test(priv->tree_maker, od1, od2);
		} else {
			return jagp_tree_maker_binary(priv->tree_maker, l_optag(topOp), od1, od2);
		}
		return NULL;
	}
	/** If tree is a concatenation of string literals, replace it
	 *  by a single literal representing the concatenated string.
	 */
	static JagPJCExpression *l_fold_strings(JagPParser *parser, JagPJCExpression *tree) {
//		if (!allowStringFolding)
			return tree;
//		CatArrayWo /*<JagPJCExpression>*/ *opStack = cat_array_wo_new();
//		ListBuffer<JCLiteral> litBuf = cat_array_wo_new();
//		boolean needsFolding = FALSE;
//		JCExpression curr = tree;
//		while (true) {
//			if (curr.hasTag(JCTree.Tag.PLUS)) {
//				JCBinary op = (JCBinary)curr;
//				needsFolding |= foldIfNeeded(op.rhs, litBuf, opStack, FALSE);
//				curr = op.lhs;
//			} else {
//				needsFolding |= foldIfNeeded(curr, litBuf, opStack, true);
//				break; //last one!
//			}
//		}
//		if (needsFolding) {
//			CatArrayWo */*<JagPJCExpression>*/ ops = opStack.toList();
//			JCExpression res = ops.head;
//			for (JCExpression op : ops.tail) {
//				res = F.at(op.getStartPosition()).Binary(optag(TokenKind.PLUS), res, op);
//				storeEnd(res, getEndPos(op));
//			}
//			return res;
//		} else {
//			return tree;
//		}
	}

//	static gboolean l_fold_if_needed(JCExpression tree, ListBuffer<JCLiteral> litBuf,
//											CatArrayWo /*<JagPJCExpression>*/ *opStack, boolean last) {
//		JCLiteral str = stringLiteral(tree);
//		if (str != NULL) {
//			litBuf.prepend(str);
//			return last && merge(litBuf, opStack);
//		} else {
//			boolean res = merge(litBuf, opStack);
//			litBuf.clear();
//			opStack.prepend(tree);
//			return res;
//		}
//	}

//	boolean merge(ListBuffer<JCLiteral> litBuf, CatArrayWo /*<JagPJCExpression>*/ *opStack) {
//		if (litBuf.isEmpty()) {
//			return FALSE;
//		} else if (litBuf.size() == 1) {
//			opStack.prepend(litBuf.first());
//			return FALSE;
//		} else {
//			JCExpression t = F.at(litBuf.first().getStartPosition()).Literal(TypeTag.CLASS,
//					litBuf.stream().map(lit -> (String)lit.getValue()).collect(Collectors.joining()));
//			storeEnd(t, litBuf.last().getEndPosition(endPosTable));
//			opStack.prepend(t);
//			return true;
//		}
//	}

//	private JCLiteral stringLiteral(JCTree tree) {
//		if (tree.hasTag(LITERAL)) {
//			JCLiteral lit = (JCLiteral)tree;
//			if (lit.typetag == TypeTag.CLASS) {
//				return lit;
//			}
//		}
//		return NULL;
//	}


	/** optimization: To save allocating a new operand/operator stack
	 *  for every binary operation, we use supplys.
	 */
//	ArrayList<JCExpression[]> odStackSupply = new ArrayList<>();
//	ArrayList<Token[]> opStackSupply = new ArrayList<>();

	static JagPJCExpression **l_new_od_stack(JagPParser *parser) {
//		if (odStackSupply.isEmpty())
//			return new JCExpression[infixPrecedenceLevels + 1];
//		return odStackSupply.remove(odStackSupply.size() - 1);
		return (JagPJCExpression **) g_malloc0(sizeof(JagPJCExpression *) * (infixPrecedenceLevels + 1));
	}

	static JagPToken **l_new_op_stack(JagPParser *parser) {
//		if (opStackSupply.isEmpty())
//			return new Token[infixPrecedenceLevels + 1];
//		return opStackSupply.remove(opStackSupply.size() - 1);
		return (JagPToken **) g_malloc0(sizeof(JagPJCExpression *) * (infixPrecedenceLevels + 1));
	}



static JagPJCExpression *l_term3_ques(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	if ((priv->mode & JAGP_MODE_TYPE) != 0 && (priv->mode & (JAGP_MODE_TYPEARG|JAGP_MODE_NOPARAMS)) == JAGP_MODE_TYPEARG) {
		priv->mode = JAGP_MODE_TYPE;
		result = l_type_argument(parser);
	} else {
		result = l_illegal(parser, priv->token->pos);
	}
	return result;
}

/* M1 */
static JagPJCExpression *l_term3_prefix(JagPParser *parser, CatArrayWo /*JagPJCExpression*/ *typeArgs, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	if (typeArgs == NULL && (priv->mode & JAGP_MODE_EXPR) != 0) {
		JagPTokenKind tk = priv->token->kind;
		l_next_token(parser);
		priv->mode = JAGP_MODE_EXPR;
		if (tk == JAGP_KIND_SUB && jagp_numeric_token_radix(priv->token) == 10
				&& (priv->token->kind == JAGP_KIND_INTLITERAL || priv->token->kind == JAGP_KIND_LONGLITERAL)) {
			priv->mode = JAGP_MODE_EXPR;
			JagPJCExpression *literal = l_literal_at(parser, priv->names_hyphen, pos);
			result = l_term3_rest(parser, literal, typeArgs);
			cat_unref_ptr(literal);
		} else {
			JagPJCExpression *term3res = l_term3(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			result = (JagPJCExpression *) jagp_tree_maker_unary(priv->tree_maker, l_unoptag(tk), term3res);
			cat_unref_ptr(term3res);
		}
	} else {
		result = l_illegal(parser, priv->token->pos);
	}
	return result;
}

static JagPJCExpression *l_term3_cast_lambda_or_parens(JagPParser *parser, CatArrayWo /*JagPJCExpression*/ *typeArgs, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	if (typeArgs == NULL && (priv->mode & JAGP_MODE_EXPR) != 0) {
		JagPParensResult pres = l_analyze_parens(parser);
		cat_log_debug("l_term3:pres=%d", pres);
		
		switch (pres) {
			case JAGP_PAR_CAST : {
					l_accept(parser, JAGP_KIND_LPAREN);
					priv->mode = JAGP_MODE_TYPE;
					int pos1 = pos;
					CatArrayWo *targets = cat_array_wo_new();
					cat_log_debug("l_term3: recursion start");
					JagPJCExpression *term3res = l_term3(parser);
					cat_log_debug("l_term3:term3res=%O", term3res);
					cat_array_wo_append(targets, (GObject *) term3res);
					while (priv->token->kind == JAGP_KIND_AMP) {
						l_check_intersection_types_in_cast(parser);
						l_accept(parser, JAGP_KIND_AMP);
						JagPJCExpression *sub = l_term3(parser);
						cat_array_wo_append(targets, (GObject *) sub);
						cat_unref_ptr(sub);
					}
					if (cat_array_wo_size(targets) > 1) {
						jagp_tree_maker_at(priv->tree_maker, pos1);
						cat_unref(term3res);
						term3res = (JagPJCExpression *) jagp_tree_maker_type_intersection(priv->tree_maker, targets);
					}
					l_accept(parser, JAGP_KIND_RPAREN);
					priv->mode = JAGP_MODE_EXPR;
					JagPJCExpression *t1 = l_term3(parser);
					cat_log_debug("l_term3:t1=%O", t1);
					jagp_tree_maker_at(priv->tree_maker, pos);
					result = (JagPJCExpression *) jagp_tree_maker_type_cast(priv->tree_maker, (JagPJCTree *) term3res, t1);
					cat_unref_ptr(term3res);
				}
				break;

			case JAGP_PAR_IMPLICIT_LAMBDA:
			case JAGP_PAR_EXPLICIT_LAMBDA:
				result = l_lambda_expression_or_statement(parser, TRUE, pres == JAGP_PAR_EXPLICIT_LAMBDA, pos);
				result = l_term3_rest(parser, result, typeArgs);
				break;

			default: /* PARENS */
				l_accept(parser, JAGP_KIND_LPAREN);
				cat_log_debug("l_term3:default: token=%d", priv->token->kind);
				priv->mode = JAGP_MODE_EXPR;
				JagPJCExpression *l3 = l_term3(parser);
				cat_log_debug("l_term3:l3=%O", l3);
				JagPJCExpression *l2 = l_term2_rest(parser, l3, jagp_tree_info_orPrec);
				cat_log_debug("l_term3:l2=%O", l2);
				JagPJCExpression *l1 = l_term1_rest(parser, l2);
				JagPJCExpression *parexpr = l_term_rest(parser, l1);
				l_accept(parser, JAGP_KIND_RPAREN);
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCParens *parens = jagp_tree_maker_parens(priv->tree_maker, parexpr);
				cat_unref_ptr(parexpr);
				result = l_term3_rest(parser, parens, typeArgs);
				break;
		}
	} else {
		result = l_illegal(parser, priv->token->pos);
	}
	return result;
}

static JagPJCExpression *l_term3_annotation(JagPParser *parser, CatArrayWo /*JagPJCExpression*/ *typeArgs, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	/* Only annotated cast types and method references are valid */
	CatArrayWo *type_annos = l_type_annotations_opt(parser);
	if (cat_array_wo_size(type_annos)==0) {
		/* else there would be no '@' */
		cat_log_fatal("Expected type annotations, but found none!");
//				throw new AssertionError("Expected type annotations, but found none!");
	}

	JagPJCExpression *expr = l_term3(parser);

	if ((priv->mode & JAGP_MODE_TYPE) == 0) {
		/* Type annotations on class literals no longer legal */
		switch (jagp_jctree_get_tag((JagPJCTree *) expr)) {
			case JAGP_TAG_REFERENCE: {
					JagPJCMemberReference *mref = (JagPJCMemberReference *) expr;
					jagp_tree_maker_at(priv->tree_maker, pos);
					JagPJCAnnotatedType *nat = jagp_tree_maker_annotated_type(priv->tree_maker, type_annos, mref->expr);
					cat_unref_ptr(mref->expr)
					mref->expr = (JagPJCExpression *) nat;
					result = l_term3_rest(parser, (JagPJCExpression *) mref, typeArgs);
					expr = NULL;
				}
				break;

			case JAGP_TAG_SELECT: {
					JagPJCFieldAccess *sel = (JagPJCFieldAccess *) expr;

					if (sel->name != priv->names_class) {
						result = l_illegal(parser, priv->token->pos);
					} else {
//						log.error(priv->token->pos, "no.annotations.on.dot.class");
						cat_log_debug("l_term3:done:result=%O", expr);
						result = l_term3_rest(parser, expr, typeArgs);
						expr = NULL;
					}
				}
				break;

			default:
				result = l_illegal(parser, l_list_head_pos(type_annos));
		}

	} else {
		/* Type annotations targeting a cast */
		JagPJCExpression *innerres = (JagPJCExpression *) l_insert_annotations_to_most_inner(parser, expr, type_annos, FALSE);
		result = l_term3_rest(parser, innerres, typeArgs);
		cat_unref_ptr(innerres);
	}	
	cat_unref_ptr(expr);
	return result;
}

/* M1 */
static JagPJCExpression *l_term3_other_done(JagPParser *parser, JagPJCExpression *c_answer, CatArrayWo /*JagPJCExpression*/ *typeArgs) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (typeArgs != NULL) {
		l_illegal(parser, priv->token->pos);
	}
	JagPJCExpression *result = l_type_arguments_apply_opt(parser, c_answer);
	cat_unref_ptr(c_answer);
	return result;
}


static JagPJCExpression *l_term3_other(JagPParser *parser, CatArrayWo /*JagPJCExpression*/ *typeArgs, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	cat_log_indent_level++;
	cat_log_debug("l_term3_other:priv->mode=%d, priv->token=%d", priv->mode, priv->token->kind);
	if (typeArgs != NULL) {
		result = l_illegal(parser, priv->token->pos);
		cat_log_indent_level--;
		return result;
	}

	if ((priv->mode & JAGP_MODE_EXPR) != 0 && l_peek_token(parser, JAGP_KIND_ARROW)) {
		result = l_lambda_expression_or_statement(parser, FALSE, FALSE, pos);
	} else {
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		JagPName *id_name = l_ident(parser, FALSE);
		result = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, id_name);
		cat_unref_ptr(id_name);
		cat_log_debug("l_term3:result=%O, token=%d", result, priv->token->kind);
		gboolean keep_looping = TRUE;
		while(keep_looping) {
			pos = priv->token->pos;
			CatArrayWo *annos = l_type_annotations_opt(parser);
			cat_log_debug("l_term3:pos=%d, token=%d, annos=%O", pos, priv->token->kind, annos);

			/* need to report an error later if LBRACKET is for array
			   index access rather than array creation level */
			if (cat_array_wo_size(annos)!=0 && priv->token->kind != JAGP_KIND_LBRACKET && priv->token->kind != JAGP_KIND_ELLIPSIS) {
				cat_log_indent_level--;
				int annoshpos = l_list_head_pos(annos);
				cat_unref_ptr(annos);
				cat_unref_ptr(result);
				return l_illegal(parser, annoshpos);
			}

			switch (priv->token->kind) {
				case JAGP_KIND_LBRACKET:
					l_next_token(parser);
					if (priv->token->kind == JAGP_KIND_RBRACKET) {
						l_next_token(parser);
						JagPJCExpression *bracksop = l_brackets_opt(parser, result, cat_array_wo_new());
						jagp_tree_maker_at(priv->tree_maker, pos);
						JagPJCArrayTypeTree *tatt = jagp_tree_maker_type_array(priv->tree_maker, bracksop);
						cat_unref_ptr(bracksop);
						result = (JagPJCExpression *) tatt;
						if (cat_array_wo_size(annos)>0) {
							JagPJCAnnotatedType *tat = jagp_tree_maker_annotated_type(priv->tree_maker, annos, result);
							cat_unref_ptr(result);
							result = (JagPJCExpression *) tat;
						}
						JagPJCExpression *brackssuf = l_brackets_suffix(parser, result);
						cat_unref_ptr(result);
						result = brackssuf;
					} else {
						if ((priv->mode & JAGP_MODE_EXPR) != 0) {
							priv->mode = JAGP_MODE_EXPR;
							JagPJCExpression *t1 = l_term_raw(parser);
							if (cat_array_wo_size(annos)>0) {
								cat_unref_ptr(result);
								result = l_illegal(parser, l_list_head_pos(annos));
							}
							jagp_tree_maker_at(priv->tree_maker, pos);
							JagPJCArrayAccess *tidx = jagp_tree_maker_indexed(priv->tree_maker, result, t1);
							cat_unref_ptr(t1);
							cat_unref_ptr(result);
							result = (JagPJCExpression *) tidx;
						}
						l_accept(parser, JAGP_KIND_RBRACKET);
					}
					result = l_term3_other_done(parser, result, typeArgs);
					keep_looping = FALSE;
					break;

				case JAGP_KIND_LPAREN :
					if ((priv->mode & JAGP_MODE_EXPR) != 0) {
						priv->mode = JAGP_MODE_EXPR;
						JagPJCExpression *applyto = result;
						result = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, applyto);
						cat_unref_ptr(applyto);
						if (cat_array_wo_size(annos)>0) {
							cat_unref_ptr(result);
							result = l_illegal(parser, l_list_head_pos(annos));
						}
						cat_unref_ptr(typeArgs);
					}
					result = l_term3_other_done(parser, result, typeArgs);
					keep_looping = FALSE;
					break;

				case JAGP_KIND_DOT :
					l_next_token(parser);
					int oldmode = priv->mode;
					priv->mode &= ~JAGP_MODE_NOPARAMS;
					cat_unref_ptr(typeArgs);
					typeArgs = l_type_arguments_opt(parser, JAGP_MODE_EXPR);
					priv->mode = oldmode;
					gboolean was_handled = FALSE;
					if ((priv->mode & JAGP_MODE_EXPR) != 0) {
						switch (priv->token->kind) {
							case JAGP_KIND_CLASS : {
									if (typeArgs != NULL) {
										cat_unref_ptr(typeArgs);
										cat_log_indent_level--;
										return l_illegal(parser, priv->token->pos);
									}
									priv->mode = JAGP_MODE_EXPR;
									jagp_tree_maker_at(priv->tree_maker, pos);
									JagPJCExpression *selector = result;
									result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selector, priv->names_class);
									cat_unref_ptr(selector);
									l_next_token(parser);
									result = l_term3_other_done(parser, result, typeArgs);
									was_handled = TRUE;
								}
								break;

							case JAGP_KIND_THIS : {
									if (typeArgs != NULL) {
										cat_unref_ptr(typeArgs);
										cat_log_indent_level--;
										return l_illegal(parser, priv->token->pos);
									}
									priv->mode = JAGP_MODE_EXPR;
									jagp_tree_maker_at(priv->tree_maker, pos);
									JagPJCExpression *selector = result;
									result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selector, priv->names_this);
									cat_unref_ptr(selector);
									l_next_token(parser);
									result = l_term3_other_done(parser, result, NULL);
									was_handled = TRUE;
								}
								break;

							case JAGP_KIND_SUPER : {
									priv->mode = JAGP_MODE_EXPR;
									jagp_tree_maker_at(priv->tree_maker, pos);
									JagPJCExpression *selector = result;
									result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selector, priv->names_super);
									cat_unref_ptr(selector);
									JagPJCExpression *supexpr = result;
									result = l_super_suffix(parser, typeArgs, supexpr);
									cat_unref_ptr(supexpr);
									cat_unref_ptr(typeArgs);
									result = l_term3_other_done(parser, result, NULL);
									was_handled = TRUE;
								}
								break;

							case JAGP_KIND_NEW : {
									if (typeArgs != NULL) {
										cat_unref_ptr(typeArgs);
										cat_log_indent_level--;
										return l_illegal(parser, priv->token->pos);
									}
									priv->mode = JAGP_MODE_EXPR;
									int pos1 = priv->token->pos;
									l_next_token(parser);
									if (priv->token->kind == JAGP_KIND_LT) {
										typeArgs = l_type_arguments(parser, FALSE);
									}
									JagPJCExpression *tp = result;
									result = l_inner_creator(parser, pos1, typeArgs, tp);
									cat_unref_ptr(tp);
									cat_unref_ptr(typeArgs);
									result = l_term3_other_done(parser, result, NULL);
									was_handled = TRUE;
								}
								break;

							default :
								break;
						}
					}

					if (!was_handled) {
						CatArrayWo *tyannos = NULL;
						if ((priv->mode & JAGP_MODE_TYPE) != 0 && priv->token->kind == JAGP_KIND_MONKEYS_AT) {
							tyannos = l_type_annotations_opt(parser);
						}
						/* typeArgs saved for next loop iteration. */
						jagp_tree_maker_at(priv->tree_maker, pos);
						JagPName *sel_fld_name = l_ident(parser, FALSE);
						JagPJCFieldAccess *nfa = jagp_tree_maker_select(priv->tree_maker, result, sel_fld_name);
						cat_unref_ptr(sel_fld_name);
						cat_unref_ptr(result);
						result = (JagPJCExpression *) nfa;
						if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
							jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
							JagPJCAnnotatedType *nat = jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, result);
							cat_unref_ptr(result);
							result = (JagPJCExpression *) nat;
						}
					} else {
						keep_looping = FALSE;
					}
					break;

				case JAGP_KIND_ELLIPSIS:
					if (priv->permitTypeAnnotationsPushBack) {
						priv->typeAnnotationsPushedBack = annos;
					} else if (cat_array_wo_size(annos)>0) {
						/* Don't return here -- error recovery attempt */
						l_illegal(parser, l_list_head_pos(annos));
					}
					result = l_term3_other_done(parser, result, typeArgs);
					keep_looping = FALSE;
					break;

				case JAGP_KIND_LT:
					if ((priv->mode & JAGP_MODE_TYPE) == 0 && l_is_unbound_member_ref(parser)) {
						/* this is an unbound method reference whose qualifier
						 is a generic type i.e. A<S>::m */
						int pos1 = priv->token->pos;
						l_accept(parser, JAGP_KIND_LT);
						CatArrayWo * /*JCExpression*/ args = cat_array_wo_new();
						JagPJCExpression *sarg = l_type_argument(parser);
						cat_array_wo_append(args, (GObject *) sarg);
						cat_unref_ptr(sarg);
						while (priv->token->kind == JAGP_KIND_COMMA) {
							l_next_token(parser);
							JagPJCExpression *sarg = l_type_argument(parser);
							cat_array_wo_append(args, (GObject *) sarg);
							cat_unref_ptr(sarg);
						}
						l_accept(parser, JAGP_KIND_GT);
						jagp_tree_maker_at(priv->tree_maker, pos1);
						JagPJCTypeApply *nta = jagp_tree_maker_type_apply(priv->tree_maker, (JagPJCTree *) result, args);
						cat_unref_ptr(result);
						result = (JagPJCExpression *) nta;
						while (priv->token->kind == JAGP_KIND_DOT) {
							l_next_token(parser);
							priv->mode = JAGP_MODE_TYPE;
							jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
							JagPName *snm = l_ident(parser, FALSE);
							JagPJCExpression *selector = result;
							result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selector, snm);
							cat_unref_ptr(selector);
							cat_unref_ptr(snm);
							JagPJCExpression *argsexpr = result;
							result = l_type_arguments_apply_opt(parser, argsexpr);
							cat_unref_ptr(argsexpr);
						}
						JagPJCExpression *unbrexpr = result;
						result = l_brackets_opt(parser, unbrexpr, cat_array_wo_new());
						cat_unref_ptr(unbrexpr);
						if (priv->token->kind != JAGP_KIND_COLCOL) {
							/* method reference expected here */
							cat_unref_ptr(result);
							result = l_illegal(parser, priv->token->pos);
						}
						priv->mode = JAGP_MODE_EXPR;
					} else {
						result = l_term3_other_done(parser, result, typeArgs);
						keep_looping = FALSE;
					}
					break;

				default:
					result = l_term3_other_done(parser, result, typeArgs);
					keep_looping = FALSE;
					break;
			}
			cat_unref_ptr(annos);
		}
	}

	JagPJCExpression *result2 = l_term3_rest(parser, result, typeArgs);
	cat_unref_ptr(typeArgs);
	cat_unref_ptr(result);
	cat_log_debug("l_term3_other:result=%O", result2);
	cat_log_indent_level--;
	return result2;
}


/**
 *  Expression3    = PrefixOp Expression3
 *                 | "(" Expr | TypeNoParams ")" Expression3
 *                 | Primary {Selector} {PostfixOp}
 *
 *  {@literal
 *  Primary        = "(" Expression ")"
 *                 | Literal
 *                 | [TypeArguments] THIS [Arguments]
 *                 | [TypeArguments] SUPER SuperSuffix
 *                 | NEW [TypeArguments] Creator
 *                 | "(" Arguments ")" "->" ( Expression | Block )
 *                 | Ident "->" ( Expression | Block )
 *                 | [Annotations] Ident { "." [Annotations] Ident }
 *                 | Expression3 MemberReferenceSuffix
 *                   [ [Annotations] "[" ( "]" BracketsOpt "." CLASS | Expression "]" )
 *                   | Arguments
 *                   | "." ( CLASS | THIS | [TypeArguments] SUPER Arguments | NEW [TypeArguments] InnerCreator )
 *                   ]
 *                 | BasicType BracketsOpt "." CLASS
 *  }
 *
 *  PrefixOp       = "++" | "--" | "!" | "~" | "+" | "-"
 *  PostfixOp      = "++" | "--"
 *  Type3          = Ident { "." Ident } [TypeArguments] {TypeSelector} BracketsOpt
 *                 | BasicType
 *  TypeNoParams3  = Ident { "." Ident } BracketsOpt
 *  Selector       = "." [TypeArguments] Ident [Arguments]
 *                 | "." THIS
 *                 | "." [TypeArguments] SUPER SuperSuffix
 *                 | "." NEW [TypeArguments] InnerCreator
 *                 | "[" Expression "]"
 *  TypeSelector   = "." Ident [TypeArguments]
 *  SuperSuffix    = Arguments | "." Ident [Arguments]
 */
/* M1 */
static JagPJCExpression *l_term3(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_term3:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	gboolean call_rest = TRUE;
	JagPJCExpression *t;
	JagPJCExpression *result = NULL;
	CatArrayWo * /*JagPJCExpression*/ typeArgs = l_type_arguments_opt(parser, JAGP_MODE_EXPR);
	cat_log_debug("l_term3:token=%d", priv->token->kind);
	switch (priv->token->kind) {
		case JAGP_KIND_QUES:
			result = l_term3_ques(parser);
			call_rest = FALSE;
			break;

		case JAGP_KIND_PLUSPLUS :
		case JAGP_KIND_SUBSUB :
		case JAGP_KIND_BANG :
		case JAGP_KIND_TILDE :
		case JAGP_KIND_PLUS :
		case JAGP_KIND_SUB :
			result = l_term3_prefix(parser, typeArgs, pos);
			call_rest = FALSE;
			break;

		case JAGP_KIND_LPAREN : 
			result = l_term3_cast_lambda_or_parens(parser, typeArgs, pos);
			call_rest = FALSE;
			break;

		case JAGP_KIND_THIS:
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCExpression *ident = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, priv->names_this);
				l_next_token(parser);
				if (typeArgs == NULL) {
					result = l_arguments_opt(parser, NULL, ident);
				} else {
					result = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, ident);
				}
				cat_unref_ptr(typeArgs);
				cat_unref_ptr(ident);
			} else {
				result = l_illegal(parser, priv->token->pos);
				call_rest = FALSE;
			}
			break;
		case JAGP_KIND_SUPER:
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCExpression *ident = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, priv->names_super);
				result = l_super_suffix(parser, typeArgs, ident);
				cat_unref_ptr(typeArgs);
				cat_unref_ptr(ident);
			} else {
				result = l_illegal(parser, priv->token->pos);
				call_rest = FALSE;
			}
			break;

		case JAGP_KIND_INTLITERAL: case JAGP_KIND_LONGLITERAL: case JAGP_KIND_FLOATLITERAL: case JAGP_KIND_DOUBLELITERAL:
		case JAGP_KIND_CHARLITERAL: case JAGP_KIND_STRINGLITERAL:
		case JAGP_KIND_TRUE: case JAGP_KIND_FALSE: case JAGP_KIND_NULL:
			if (typeArgs == NULL && (priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				result = l_literal(parser, priv->names_empty);
			} else {
				result = l_illegal(parser, priv->token->pos);
				call_rest = FALSE;
			}
			break;

		case JAGP_KIND_NEW:
			if (typeArgs != NULL) {
				result = l_illegal(parser, priv->token->pos);
				call_rest = FALSE;
			} else if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				l_next_token(parser);
				if (priv->token->kind == JAGP_KIND_LT) {
					typeArgs = l_type_arguments(parser, FALSE);
				}
				result = l_creator(parser, pos, typeArgs);
				cat_unref_ptr(typeArgs);
			} else {
				result = l_illegal(parser, priv->token->pos);
				call_rest = FALSE;
			}
			cat_log_debug("result=%O", result);
			break;

		case JAGP_KIND_MONKEYS_AT: {
			result = l_term3_annotation(parser, typeArgs, pos);
			call_rest = FALSE;
		}
		break;

		case JAGP_KIND_UNDERSCORE :
		case JAGP_KIND_IDENTIFIER :
		case JAGP_KIND_ASSERT : 
		case JAGP_KIND_ENUM :
			result = l_term3_other(parser, typeArgs, pos);
			call_rest = FALSE;
			break;

		case JAGP_KIND_BYTE :
		case JAGP_KIND_SHORT :
		case JAGP_KIND_CHAR :
		case JAGP_KIND_INT :
		case JAGP_KIND_LONG :
		case JAGP_KIND_FLOAT :
		case JAGP_KIND_DOUBLE :
		case JAGP_KIND_BOOLEAN :
			if (typeArgs != NULL) {
				l_illegal(parser, priv->token->pos);
			}
			JagPJCExpression *basictype = (JagPJCExpression *) l_basic_type(parser);
			CatArrayWo *nillist = cat_array_wo_new();
			JagPJCExpression *bracksop = l_brackets_opt(parser, basictype, nillist);
			cat_unref_ptr(nillist);
			cat_unref_ptr(basictype);
			result = l_brackets_suffix(parser, bracksop);
			cat_unref_ptr(bracksop);
			break;

		case JAGP_KIND_VOID :
			if (typeArgs != NULL) {
				l_illegal(parser, priv->token->pos);
			}
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				l_next_token(parser);
				if (priv->token->kind == JAGP_KIND_DOT) {
					jagp_tree_maker_at(priv->tree_maker, pos);
					JagPJCPrimitiveTypeTree *ti = jagp_tree_maker_type_ident(priv->tree_maker, JAGP_TT_VOID);
					result = l_brackets_suffix(parser, (JagPJCExpression *) ti);
					cat_unref_ptr(ti);
				} else {
					call_rest = FALSE;
					result = l_illegal(parser, pos);
				}
			} else {
				/* Support the corner case of myMethodHandle.<void>invoke() by passing
				 * a void type (like other primitive types) to the next phase.
				 * The error will be reported in Attr.attribTypes or Attr.visitApply. */
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCExpression *result = (JagPJCExpression *) jagp_tree_maker_type_ident(priv->tree_maker, JAGP_TT_VOID);
				l_next_token(parser);
				call_rest = FALSE;
				//return l_illegal(parser, priv->token->pos);
			}
			break;
		default:
			cat_log_error("l_term3:returning illegal");
			cat_log_indent_level--;
			call_rest = FALSE;
			result = l_illegal(parser, priv->token->pos);
	}
	cat_log_debug("l_term3:result=%O, call_rest=%d", result, call_rest);
	if (call_rest) {
		JagPJCExpression *next_result = l_term3_rest(parser, result, typeArgs);
		cat_unref_ptr(result);
		result = next_result;
	}
	cat_unref_ptr(typeArgs);
	cat_log_debug("l_term3:done:result=%O", result);
	cat_log_indent_level--;
	return result;
}

/* M1 */
static JagPJCExpression *l_term3_rest(JagPParser *parser, JagPJCExpression *t, CatArrayWo */*<JAgPJCExpression>*/ type_args) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term3_rest:token=%d", priv->token->kind);
	if (type_args != NULL) {
		l_illegal(parser, priv->token->pos);
	}

	JagPJCExpression *result = cat_ref_ptr(t);
	while(TRUE) {
		int pos1 = priv->token->pos;
		CatArrayWo * /*<JagPJCAnnotation>*/ annos = l_type_annotations_opt(parser);
		
		if (priv->token->kind == JAGP_KIND_LBRACKET) {
			l_next_token(parser);
			if ((priv->mode & JAGP_MODE_TYPE) != 0) {
				int oldmode = priv->mode;
				priv->mode = JAGP_MODE_TYPE;
				if (priv->token->kind == JAGP_KIND_RBRACKET) {
					l_next_token(parser);
					JagPJCExpression *bracksop = l_brackets_opt(parser, result, cat_array_wo_new());
					cat_unref_ptr(result);
					jagp_tree_maker_at(priv->tree_maker, pos1);
					result = (JagPJCExpression *) jagp_tree_maker_type_array(priv->tree_maker, bracksop);
					result = toP(parser, result);
					cat_unref_ptr(bracksop);
					if (priv->token->kind == JAGP_KIND_COLCOL) {
						priv->mode = JAGP_MODE_EXPR;
						cat_unref_ptr(annos);
						continue;
					}
					if (cat_array_wo_size(annos)>0) {
						jagp_tree_maker_at(priv->tree_maker, pos1);
						JagPJCExpression *nxt = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annos, result);
						cat_unref_ptr(result);
						result = nxt;
						result = toP(parser, result);
					}
					cat_unref_ptr(annos);
					return result;
				}
				priv->mode = oldmode;
			}
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				JagPJCExpression *indexed = result;
				JagPJCExpression *index = (JagPJCExpression *) l_term_raw(parser);
				jagp_tree_maker_at(priv->tree_maker, pos1);
				result = (JagPJCExpression *) jagp_tree_maker_indexed(priv->tree_maker, indexed, index);
				result = to(parser, result);
				cat_unref_ptr(index);
				cat_unref_ptr(indexed);
			}
			l_accept(parser, JAGP_KIND_RBRACKET);
		} else if (priv->token->kind == JAGP_KIND_DOT) {
			l_next_token(parser);
			type_args = l_type_arguments_opt(parser, JAGP_MODE_EXPR);
			if (priv->token->kind == JAGP_KIND_SUPER && (priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				jagp_tree_maker_at(priv->tree_maker, pos1);
				JagPJCExpression *selected = result;
				result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, priv->names_super);
				result = to(parser, result);
				cat_unref_ptr(selected);
				l_next_token(parser);
				JagPJCExpression *aplexpr = result;
				result = (JagPJCExpression *) l_arguments_apply(parser, type_args, aplexpr);
				cat_unref_ptr(aplexpr);
			} else if (priv->token->kind == JAGP_KIND_NEW && (priv->mode & JAGP_MODE_EXPR) != 0) {
				if (type_args != NULL) {
					cat_unref_ptr(annos);
					cat_unref_ptr(type_args);
					return l_illegal(parser, priv->token->pos);
				}
				priv->mode = JAGP_MODE_EXPR;
				int pos2 = priv->token->pos;
				l_next_token(parser);
				if (priv->token->kind == JAGP_KIND_LT) {
					type_args = l_type_arguments(parser, FALSE);
				}
				JagPJCExpression *createtp = result;
				result = l_inner_creator(parser, pos2, type_args, createtp);
				cat_unref_ptr(createtp);
			} else {
				CatArrayWo * /*<JAgPJCAnnotation>*/ tyannos = NULL;
				if ((priv->mode & JAGP_MODE_TYPE) != 0 && priv->token->kind == JAGP_KIND_MONKEYS_AT) {
					/* is the mode check needed? */
					tyannos = l_type_annotations_opt(parser);
				}
				
				jagp_tree_maker_at(priv->tree_maker, pos1);
				JagPJCExpression *selected = result;
				JagPName *selector = l_ident(parser, TRUE);
				result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, selector);
				result = toP(parser, result);
				cat_unref_ptr(selected);
				cat_unref_ptr(selector);
				if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
					jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
					JagPJCExpression *underlying_type = result;
					result = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, underlying_type);
					cat_unref_ptr(underlying_type);
					result = toP(parser, result);
				}
				JagPJCExpression *applyopt = l_type_arguments_apply_opt(parser, result);
				cat_unref_ptr(result);
				result = l_arguments_opt(parser, type_args, applyopt);
				cat_unref_ptr(applyopt);
				cat_unref_ptr(tyannos);
			}
			cat_unref_ptr(type_args);
		} else if ((priv->mode & JAGP_MODE_EXPR) != 0 && priv->token->kind == JAGP_KIND_COLCOL) {
			priv->mode = JAGP_MODE_EXPR;
			if (type_args != NULL) {
				cat_unref_ptr(annos);
				cat_unref_ptr(type_args);
				return l_illegal(parser, priv->token->pos);
			}
			l_accept(parser, JAGP_KIND_COLCOL);
			JagPJCExpression *refsuf = result;
			result = l_member_reference_suffix_ext(parser, pos1, refsuf);
			cat_unref_ptr(refsuf);
		} else {
			if (cat_array_wo_size(annos)>0) {
				if (priv->permitTypeAnnotationsPushBack) {
					priv->typeAnnotationsPushedBack = annos;
				} else {
					cat_unref_ptr(annos);
					return l_illegal(parser, l_list_head_pos(annos));
				}
			}
			cat_unref_ptr(annos);
			break;
		}
		cat_unref_ptr(annos);
	}

	cat_log_debug("t=%O", result);
	while ((priv->token->kind == JAGP_KIND_PLUSPLUS || priv->token->kind == JAGP_KIND_SUBSUB) && (priv->mode & JAGP_MODE_EXPR) != 0) {
		priv->mode = JAGP_MODE_EXPR;
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		JagPJCExpression *arg = result;
		result = (JagPJCExpression *) jagp_tree_maker_unary(priv->tree_maker, priv->token->kind == JAGP_KIND_PLUSPLUS ? JAGP_TAG_POSTINC : JAGP_TAG_POSTDEC, arg);
		cat_unref_ptr(arg);
		result = to(parser, result);
		l_next_token(parser);
	}
	cat_log_debug("l_term3_rest:done=%O", result);
	return toP(parser, result);
}

/**
 * If we see an identifier followed by a '&lt;' it could be an unbound
 * method reference or a binary expression. To disambiguate, look for a
 * matching '&gt;' and see if the subsequent terminal is either '.' or '::'.
 */
/* M1 */
static gboolean l_is_unbound_member_ref(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = 0;
	int depth = 0;
	for(pos=0 ; ; pos++) {
		JagPToken *token = jagp_ilexer_token_lah(priv->lexer, pos);
		switch(token->kind) {
			case JAGP_KIND_IDENTIFIER : case JAGP_KIND_UNDERSCORE : case JAGP_KIND_QUES :
			case JAGP_KIND_EXTENDS : case JAGP_KIND_SUPER : case JAGP_KIND_DOT :
			case JAGP_KIND_RBRACKET : case JAGP_KIND_LBRACKET : case JAGP_KIND_COMMA :
			case JAGP_KIND_BYTE : case JAGP_KIND_SHORT : case JAGP_KIND_INT :
			case JAGP_KIND_LONG : case JAGP_KIND_FLOAT : case JAGP_KIND_DOUBLE :
			case JAGP_KIND_BOOLEAN : case JAGP_KIND_CHAR : case JAGP_KIND_MONKEYS_AT :
				break;

			case JAGP_KIND_LPAREN : {
					/* skip annotation values */
					int nesting = 0;
					gboolean keep_looping = TRUE;
					for(; keep_looping ; pos++) {
						JagPToken *tk2 = jagp_ilexer_token_lah(priv->lexer, pos);
						switch (tk2->kind) {
							case JAGP_KIND_EOF :
								return FALSE;
							case JAGP_KIND_LPAREN :
								nesting++;
								break;
							case JAGP_KIND_RPAREN :
								nesting--;
								if (nesting == 0) {
									keep_looping = FALSE;
								}
								break;
							default :
								break;
						}
					}
				}
				break;

			case JAGP_KIND_LT :
				depth++;
				break;
			case JAGP_KIND_GTGTGT :
				depth--;
			case JAGP_KIND_GTGT :
				depth--;
			case JAGP_KIND_GT : {
					depth--;
					if (depth == 0) {
						JagPToken *nexttk = jagp_ilexer_token_lah(priv->lexer, pos+1);
						JagPTokenKind nextKind = nexttk->kind;
						return (nextKind == JAGP_KIND_DOT) ||
							(nextKind == JAGP_KIND_LBRACKET) ||
							(nextKind == JAGP_KIND_COLCOL);
					}
				}
				break;
			default:
				return FALSE;
		}
	}
	return FALSE;
}



/**
 * If we see an identifier followed by a '&lt;' it could be an unbound
 * method reference or a binary expression. To disambiguate, look for a
 * matching '&gt;' and see if the subsequent terminal is either '.' or '::'.
 */
static JagPParensResult l_analyze_parens(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int depth = 0;
	gboolean type = FALSE;
	int lookahead;
	for(lookahead=0 ; ; lookahead++) {
		JagPToken *lah_token = jagp_ilexer_token_lah(priv->lexer, lookahead);
		cat_log_debug("l_analyze_parens:lah[%d]=%d", lookahead, lah_token->kind);
		cat_log_debug("peek_token is RPAR: %d", l_peek_token_lah(parser, lookahead, JAGP_KIND_RPAREN));
		switch(lah_token->kind) {
			case JAGP_KIND_COMMA :
				type = TRUE;
			case JAGP_KIND_EXTENDS : case JAGP_KIND_SUPER : case JAGP_KIND_DOT : case JAGP_KIND_AMP :
				/* skip */
				break;

			case JAGP_KIND_QUES :
				if (l_peek_token_lah(parser, lookahead, JAGP_KIND_EXTENDS) || l_peek_token_lah(parser, lookahead, JAGP_KIND_SUPER)) {
					/* wildcards */
					type = TRUE;
				}
				break;

			case JAGP_KIND_BYTE :
			case JAGP_KIND_SHORT :
			case JAGP_KIND_INT :
			case JAGP_KIND_LONG :
			case JAGP_KIND_FLOAT :
			case JAGP_KIND_DOUBLE :
			case JAGP_KIND_BOOLEAN :
			case JAGP_KIND_CHAR :
			case JAGP_KIND_VOID : {
					JagPToken *lah_token2 = jagp_ilexer_token_lah(priv->lexer, lookahead+1);
	
					if (lah_token2->kind == JAGP_KIND_RPAREN) {
						/* Type, ')' -> cast */
						return JAGP_PAR_CAST;
					} else if (lax_identifier_accepts(lah_token2->kind)) {
						/* Type, Identifier/'_'/'assert'/'enum' -> explicit lambda */
						return JAGP_PAR_EXPLICIT_LAMBDA;
					}
				}
				break;

			case JAGP_KIND_LPAREN :
				if (lookahead != 0) {
					/* '(' in a non-starting position -> parens */
					return JAGP_PAR_PARENS;
				} else if (l_peek_token_lah(parser, lookahead, JAGP_KIND_RPAREN)) {
					/* '(', ')' -> explicit lambda */
					return JAGP_PAR_EXPLICIT_LAMBDA;
				}
				break;
			case JAGP_KIND_RPAREN : {
					/* if we have seen something that looks like a type,
					 * then it's a cast expression */
					if (type) {
						return JAGP_PAR_CAST;
					}
					/* otherwise, disambiguate cast vs. parenthesized expression
					 * based on subsequent token. */
					JagPToken *lah_token_p1 = jagp_ilexer_token_lah(priv->lexer, lookahead+1);
					switch (lah_token_p1->kind) {
						/*case JAGP_KIND_PLUSPLUS: case JAGP_KIND_SUBSUB: */
						case JAGP_KIND_BANG : case JAGP_KIND_TILDE :
						case JAGP_KIND_LPAREN : case JAGP_KIND_THIS : case JAGP_KIND_SUPER :
						case JAGP_KIND_INTLITERAL : case JAGP_KIND_LONGLITERAL : case JAGP_KIND_FLOATLITERAL :
						case JAGP_KIND_DOUBLELITERAL : case JAGP_KIND_CHARLITERAL : case JAGP_KIND_STRINGLITERAL :
						case JAGP_KIND_TRUE : case JAGP_KIND_FALSE : case JAGP_KIND_NULL :
						case JAGP_KIND_NEW : case JAGP_KIND_IDENTIFIER : case JAGP_KIND_ASSERT : case JAGP_KIND_ENUM : case JAGP_KIND_UNDERSCORE :
						case JAGP_KIND_BYTE : case JAGP_KIND_SHORT : case JAGP_KIND_CHAR : case JAGP_KIND_INT :
						case JAGP_KIND_LONG : case JAGP_KIND_FLOAT : case JAGP_KIND_DOUBLE : case JAGP_KIND_BOOLEAN : case JAGP_KIND_VOID :
							return JAGP_PAR_CAST;
						default:
							return JAGP_PAR_PARENS;
					}
				}
				break;

			case JAGP_KIND_UNDERSCORE :
			case JAGP_KIND_ASSERT :
			case JAGP_KIND_ENUM :
			case JAGP_KIND_IDENTIFIER : {
				JagPToken *lah_token1 = jagp_ilexer_token_lah(priv->lexer, lookahead+2);
				cat_log_debug("lah_token1=%d", lah_token1->kind);
				if (lax_identifier_accepts(lah_token1->kind)) {
					/* Identifier, Identifier/'_'/'assert'/'enum' -> explicit lambda */
					return JAGP_PAR_EXPLICIT_LAMBDA;
				} else if (lah_token1->kind==JAGP_KIND_ARROW) {
					/* Identifier, ')' '->' -> implicit lambda */
					return JAGP_PAR_IMPLICIT_LAMBDA;
				}
				type = FALSE;
				break;
			}

			case JAGP_KIND_FINAL :
			case JAGP_KIND_ELLIPSIS :
				/* those can only appear in explicit lambdas */
	                return JAGP_PAR_EXPLICIT_LAMBDA;

			case JAGP_KIND_MONKEYS_AT :
				type = TRUE;
				lookahead += 1; /* skip '@' */
//                while (peekToken(lookahead, DOT)) {
//                    lookahead += 2;
//                }
//                if (peekToken(lookahead, JAGP_KIND_LPAREN)) {
//                    lookahead++;
//                    //skip annotation values
//                    int nesting = 0;
//                    for (; ; lookahead++) {
//                        TokenKind tk2 = S.token(lookahead).kind;
//                        switch (tk2) {
//                            case EOF:
//                                return JAGP_PAR_PARENS;
//                            case JAGP_KIND_LPAREN:
//                                nesting++;
//                                break;
//                            case JAGP_KIND_RPAREN:
//                                nesting--;
//                                if (nesting == 0) {
//                                    continue outer;
//                                }
//                            break;
//                        }
//                    }
//                }
				break;
			case JAGP_KIND_LBRACKET:
//                if (peekToken(lookahead, RBRACKET, LAX_IDENTIFIER)) {
//                    // '[', ']', Identifier/'_'/'assert'/'enum' -> explicit lambda
//                    return JAGP_PAR_EXPLICIT_LAMBDA;
//                } else if (peekToken(lookahead, RBRACKET, JAGP_KIND_RPAREN) ||
//                        peekToken(lookahead, RBRACKET, AMP)) {
//                    // '[', ']', ')' -> cast
//                    // '[', ']', '&' -> cast (intersection type)
//                    return JAGP_PAR_CAST;
//                } else if (peekToken(lookahead, RBRACKET)) {
//                    //consume the ']' and skip
//                    type = true;
//                    lookahead++;
//                    break;
//                } else {
				return JAGP_PAR_PARENS;
//                }
			case JAGP_KIND_LT :
				depth++; 
				break;
			case JAGP_KIND_GTGTGT :
				depth--;
			case JAGP_KIND_GTGT :
				depth--;
			case JAGP_KIND_GT:
				depth--;
				if (depth == 0) {
//                    if (peekToken(lookahead, JAGP_KIND_RPAREN) ||
//                            peekToken(lookahead, AMP)) {
//                        // '>', ')' -> cast
//                        // '>', '&' -> cast
//                        return JAGP_PAR_CAST;
//                    } else if (peekToken(lookahead, LAX_IDENTIFIER, JAGP_KIND_COMMA) ||
//                            peekToken(lookahead, LAX_IDENTIFIER, JAGP_KIND_RPAREN, ARROW) ||
//                            peekToken(lookahead, ELLIPSIS)) {
//                        // '>', Identifier/'_'/'assert'/'enum', ',' -> explicit lambda
//                        // '>', Identifier/'_'/'assert'/'enum', ')', '->' -> explicit lambda
//                        // '>', '...' -> explicit lambda
//                        return JAGP_PAR_EXPLICIT_LAMBDA;
//                    }
//                    //it looks a type, but could still be (i) a cast to generic type,
//                    //(ii) an unbound method reference or (iii) an explicit lambda
//                    type = true;
//                    break;
				} else if (depth < 0) {
					/*unbalanced '<', '>' - not a generic type */
					return JAGP_PAR_PARENS;
				}
				break;
			default : 
				/* this includes EOF */
				return JAGP_PAR_PARENS;
		}
	}
	return JAGP_PAR_PARENS;
}


/* M1 */
static JagPJCExpression *l_lambda_expression_or_statement(JagPParser *parser, gboolean hasParens, gboolean explicitParams, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_lambda_expression_or_statement:token=%d", priv->token->kind);
	CatArrayWo */*JagPJCVariableDecl>*/ params = explicitParams ?
			l_formal_parameters(parser, TRUE) :
			l_implicit_parameters(parser, hasParens);

	JagPJCExpression *result = l_lambda_expression_or_statement_rest(parser, params, pos);
	cat_log_debug("l_lambda_expression_or_statement:done:result=%O", result);
	cat_log_indent_level--;
	cat_unref_ptr(params);
	return result;
}

/* M1 */
static JagPJCExpression *l_lambda_expression_or_statement_rest(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_check_lambda(parser);
	l_accept(parser, JAGP_KIND_ARROW);

	return priv->token->kind == JAGP_KIND_LBRACE ?
		l_lambda_statement(parser, args, pos, pos) :
		l_lambda_expression(parser, args, pos);
}

/* M1 */
static JagPJCExpression *l_lambda_statement(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos, int pos2) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCBlock *block = l_block_ext(parser, pos2, 0);
	jagp_tree_maker_at(priv->tree_maker, pos2);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_lambda(priv->tree_maker, args, (JagPJCTree *) block);
	cat_unref_ptr(block);
	t = toP(parser, t);
	return t;
}

/* M1 */
static JagPJCExpression *l_lambda_expression(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCTree *expr = (JagPJCTree *) l_parse_expression(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_lambda(priv->tree_maker, args, expr);
	cat_unref_ptr(expr);
	t = toP(parser, t);
	return t;
}

/** SuperSuffix = Arguments | "." [TypeArguments] Ident [Arguments]
 */
/* M1 */
static JagPJCExpression *l_super_suffix(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_next_token(parser);
	JagPJCExpression *result = NULL;
	if (priv->token->kind == JAGP_KIND_LPAREN || typeArgs != NULL) {
		result = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, t);
	} else if (priv->token->kind == JAGP_KIND_COLCOL) {
		if (typeArgs != NULL) {
			return l_illegal(parser, priv->token->pos);
		}
		result = l_member_reference_suffix(parser, t);
	} else {
		int pos = priv->token->pos;
		l_accept(parser, JAGP_KIND_DOT);
		typeArgs = (priv->token->kind == JAGP_KIND_LT) ? l_type_arguments(parser, FALSE) : NULL;
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *selected = t;
		JagPName *selector = l_ident(parser, FALSE);
		JagPJCExpression *select = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, selector);
		select = toP(parser, select);
		cat_unref_ptr(selector);
		result = l_arguments_opt(parser, typeArgs, select);
		cat_unref_ptr(select);
		cat_unref_ptr(typeArgs);
	}
	return result;
}

/** BasicType = BYTE | SHORT | CHAR | INT | LONG | FLOAT | DOUBLE | BOOLEAN
 */
/* M1 */
static JagPJCPrimitiveTypeTree *l_basic_type(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPJCPrimitiveTypeTree *result = jagp_tree_maker_type_ident(priv->tree_maker, l_typetag(priv->token->kind));
	result = to(parser, result);
	l_next_token(parser);
	return result;
}

/** ArgumentsOpt = [ Arguments ]
 */
/* M1 */
static JagPJCExpression *l_arguments_opt(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	if (((priv->mode & JAGP_MODE_EXPR) != 0) && (priv->token->kind == JAGP_KIND_LPAREN || typeArgs != NULL)) {
		priv->mode = JAGP_MODE_EXPR;
		result = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, t);
	} else {
		result = cat_ref_ptr(t);
	}
	return result;
}

/** Arguments = "(" [Expression { COMMA Expression }] ")"
 */
/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_arguments(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JCExpression>*/ args = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_LPAREN) {
		l_next_token(parser);
		if (priv->token->kind != JAGP_KIND_RPAREN) {
			JagPJCExpression *pe = l_parse_expression(parser);
			cat_array_wo_append(args, (GObject *) pe);
			cat_unref_ptr(pe);
			while (priv->token->kind == JAGP_KIND_COMMA) {
				l_next_token(parser);
				JagPJCExpression *pe = l_parse_expression(parser);
				cat_array_wo_append(args, (GObject *) pe);
				cat_unref_ptr(pe);
			}
		}
		l_accept(parser, JAGP_KIND_RPAREN);
//	} else {
//		syntaxError(priv->token->pos, "expected", JAGP_KIND_LPAREN);
	}
	return args;
}

/* M1 */
static JagPJCMethodInvocation *l_arguments_apply(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *fn) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	CatArrayWo */*<JagPJCExpression>*/ args = l_arguments(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCMethodInvocation *result = jagp_tree_maker_apply(priv->tree_maker, typeArgs, fn, args);
	cat_unref_ptr(args);
	result = toP(parser, result);
	return result;
}

/**  TypeArgumentsOpt = [ TypeArguments ]
 */
/* M1 */
static JagPJCExpression *l_type_arguments_apply_opt(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	if (priv->token->kind == JAGP_KIND_LT &&
			(priv->mode & JAGP_MODE_TYPE) != 0 &&
			(priv->mode & JAGP_MODE_NOPARAMS) == 0) {
		priv->mode = JAGP_MODE_TYPE;
		result = (JagPJCExpression *) l_type_arguments_apply(parser, t, FALSE);
	} else {
		result = cat_ref_ptr(t);
	}
	return result;
}

/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_type_arguments_opt(JagPParser *parser, int useMode) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo *result = NULL;
	cat_log_debug("l_type_arguments_opt:token=%d", priv->token->kind);
	if (priv->token->kind == JAGP_KIND_LT) {
		if ((priv->mode & useMode) == 0 ||
			(priv->mode & JAGP_MODE_NOPARAMS) != 0) {
			l_illegal(parser, priv->token->pos);
		}
		priv->mode = useMode;
		result = l_type_arguments(parser, FALSE);
	}
	return result;
}

/**
 *  {@literal
 *  TypeArguments  = "<" TypeArgument {"," TypeArgument} ">"
 *  }
 */
/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_type_arguments(JagPParser *parser, gboolean diamondAllowed) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_type_arguments:token=%d", priv->token->kind);
	CatArrayWo /*<JagPJCExpression>*/ *args = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_LT) {
		l_next_token(parser);
		if (priv->token->kind == JAGP_KIND_GT && diamondAllowed) {
			l_check_diamond(parser);
			priv->mode |= JAGP_MODE_DIAMOND;
			l_next_token(parser);
		} else {
			JagPJCExpression *sb = ((priv->mode & JAGP_MODE_EXPR) == 0) ? l_type_argument(parser) : l_parse_type(parser);
			cat_array_wo_append(args, (GObject *) sb);
			cat_unref_ptr(sb);
			while (priv->token->kind == JAGP_KIND_COMMA) {
				l_next_token(parser);
				JagPJCExpression *sb = ((priv->mode & JAGP_MODE_EXPR) == 0) ? l_type_argument(parser) : l_parse_type(parser);
				cat_array_wo_append(args, (GObject *) sb);
				cat_unref_ptr(sb);
			}
			switch (priv->token->kind) {
	
				case JAGP_KIND_GTGTGTEQ: case JAGP_KIND_GTGTEQ: case JAGP_KIND_GTEQ:
				case JAGP_KIND_GTGTGT: case JAGP_KIND_GTGT:
					priv->token = jagp_ilexer_split(priv->lexer);
					break;
				case JAGP_KIND_GT:
					l_next_token(parser);
					break;
				default:
	//				GObject *er = syntaxError(priv->token->pos, "expected", JAGP_KIND_GT);
	//				cat_array_wo_append(args, er);
	//				cat_unref_ptr(er);
					break;
			}
		}
	}
	return args;
}

/**
 *  {@literal
 *  TypeArgument = Type
 *               | [Annotations] "?"
 *               | [Annotations] "?" EXTENDS Type {"&" Type}
 *               | [Annotations] "?" SUPER Type
 *  }
 */
/* M1 */
static JagPJCExpression *l_type_argument(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCAnnotation>*/ annotations = l_type_annotations_opt(parser);
	if (priv->token->kind != JAGP_KIND_QUES) {
		JagPJCExpression *result = l_parse_type_with_annotations(parser, annotations);
		cat_unref_ptr(annotations);
		return result;
	}
	int pos = priv->token->pos;
	l_next_token(parser);
	JagPJCExpression *result;
	if (priv->token->kind == JAGP_KIND_EXTENDS) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_EXTENDS);
		t = to(parser, t);
		l_next_token(parser);
		JagPJCExpression *bound = l_parse_type(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker, t, (JagPJCTree *) bound);
		cat_unref_ptr(t);
		cat_unref_ptr(bound);
	} else if (priv->token->kind == JAGP_KIND_SUPER) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_SUPER);
		t = to(parser, t);
		l_next_token(parser);
		JagPJCExpression *bound = l_parse_type(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker,t, (JagPJCTree *) bound);
		cat_unref_ptr(t);
		cat_unref_ptr(bound);
	} else if (lax_identifier_accepts(priv->token->kind)) {
		/* error recovery */
		jagp_tree_maker_at(priv->tree_maker, -1);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_UNBOUND);

		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *wc = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker,t,NULL);
		wc = toP(parser, wc);
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		JagPName *id_name = l_ident(parser, FALSE);
		JagPJCIdent *id = jagp_tree_maker_ident(priv->tree_maker, id_name);
		cat_unref_ptr(id_name);
		id = toP(parser, id);
		jagp_tree_maker_at(priv->tree_maker, pos);
		CatArrayWo *errs = cat_array_wo_new();
		cat_array_wo_append(errs, (GObject *) wc);
		cat_array_wo_append(errs, (GObject *) id);
		cat_unref_ptr(wc);
		cat_unref_ptr(t);
		cat_unref_ptr(id);
		JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
		cat_unref_ptr(errs);
//		reportSyntaxError(err, "expected3", JAGP_KIND_GT, JAGP_KIND_EXTENDS, JAGP_KIND_SUPER);
		result = (JagPJCExpression *) err;
	} else {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_UNBOUND);
		t= toP(parser, t);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker,t,NULL);
		result = toP(parser, result);
		cat_unref_ptr(t);
	}
	if (cat_array_wo_size(annotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(annotations));
		JagPJCExpression *underlying_type = result;
		result = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annotations, underlying_type);
		cat_unref_ptr(underlying_type);
		result = toP(parser, result);
	}
	cat_unref_ptr(annotations);
	return result;
}

/* M1 */
static JagPJCTypeApply *l_type_arguments_apply(JagPParser *parser, JagPJCExpression *t, gboolean diamondAllowed) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	CatArrayWo */*<JagPJCExpression>*/ args = l_type_arguments(parser, diamondAllowed);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTypeApply *result = jagp_tree_maker_type_apply(priv->tree_maker, (JagPJCTree *) t, args);
	cat_unref_ptr(args);
	result= toP(parser, result);
	return result;
}


/**
 * BracketsOpt = { [Annotations] "[" "]" }*
 *
 * <p>
 *
 * <code>annotations</code> is the list of annotations targeting
 * the expression <code>t</code>.
 */
/* M1 */
static JagPJCExpression *l_brackets_opt(JagPParser *parser, JagPJCExpression *t, CatArrayWo */*<JagPJCAnnotation>*/ annotations) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCAnnotation>*/ nextLevelAnnotations = l_type_annotations_opt(parser);

	JagPJCExpression *underlying_type = NULL;
	if (priv->token->kind == JAGP_KIND_LBRACKET) {
		int pos = priv->token->pos;
		l_next_token(parser);
		underlying_type = l_brackets_opt_cont(parser, t, pos, nextLevelAnnotations);
	} else if (cat_array_wo_size(nextLevelAnnotations)>0) {
		if (priv->permitTypeAnnotationsPushBack) {
			priv->typeAnnotationsPushedBack = nextLevelAnnotations;
		} else {
			cat_unref_ptr(nextLevelAnnotations);
			return l_illegal(parser, l_list_head_pos(nextLevelAnnotations));
		}
		underlying_type = cat_ref_ptr(t);
	}

	if (cat_array_wo_size(annotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annotations, underlying_type);
		t = toP(parser, t);
		cat_unref_ptr(underlying_type);
	} else {
		t = underlying_type;
	}
	cat_unref_ptr(nextLevelAnnotations);
	return t;
}

/** BracketsOpt = [ "[" "]" { [Annotations] "[" "]"} ]
 */
/* M1 */
static JagPJCExpression *l_brackets_opt_simple(JagPParser *parser, JagPJCExpression *t) {
	CatArrayWo *nil_list = cat_array_wo_new();
	JagPJCExpression *result = l_brackets_opt(parser, t, nil_list);
	cat_unref_ptr(nil_list);
	return result;
}

/* M1 */
static JagPJCExpression *l_brackets_opt_cont(JagPParser *parser, JagPJCExpression *t, int pos, CatArrayWo */*<JagPJCAnnotation>*/ annotations) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_RBRACKET);
	JagPJCExpression *elemtype = l_brackets_opt_simple(parser, t);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *typearray = (JagPJCExpression *) jagp_tree_maker_type_array(priv->tree_maker, elemtype);
	typearray = toP(parser, typearray);
	cat_unref_ptr(elemtype)
	JagPJCExpression *result = NULL;
	if (cat_array_wo_size(annotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *underlying_type = typearray;
		result = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annotations, underlying_type);
		cat_unref_ptr(underlying_type)
		result = toP(parser, result);
	} else {
		result = typearray;
	}
	return result;
}

/** BracketsSuffixExpr = "." CLASS
 *  BracketsSuffixType =
 */
static JagPJCExpression *l_brackets_suffix(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	if ((priv->mode & JAGP_MODE_EXPR) != 0 && priv->token->kind == JAGP_KIND_DOT) {
		priv->mode = JAGP_MODE_EXPR;
		int pos = priv->token->pos;
		l_next_token(parser);
		l_accept(parser, JAGP_KIND_CLASS);
//		if (priv->token->pos == endPosTable.errorEndPos) {
//			// error recovery
//			JagPName *name;
//			if (lax_identifier_accepts(priv->token->kind)) {
//				name = jagp_token_name(priv->token);
//				l_next_token(parser);
//			} else {
//				name = priv->names_error;
//			}
//			CatArrayWo *errs = cat_array_wo_new();
//			jagp_tree_maker_at(priv->tree_maker, pos);
//			JagPJCFieldAccess *fldacc = jagp_tree_maker_select(priv->tree_maker, t, name);
//			fldacc = toP(parser, fldacc);
//			cat_array_wo_append(errs, fldacc);
//			jagp_tree_maker_at(priv->tree_maker, pos);
//			t = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//			cat_unref_ptr(errs);
//		} else {
//			JagPTag tag = jagp_jctree_get_tag((JagPJCTree *) t);

			// Type annotations are illegal on class literals. Annotated non array class literals
			// are complained about directly in term3(), Here check for type annotations on dimensions
			// taking care to handle some interior dimension(s) being annotated.
//			if ((tag == JAGP_TAG_TYPEARRAY && TreeInfo.containsTypeAnnotation(t)) || tag == JAGP_TAG_ANNOTATED_TYPE) {
//				syntaxError("no.annotations.on.dot.class");
//			}
			jagp_tree_maker_at(priv->tree_maker, pos);
			result = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, t, priv->names_class);
			result = toP(parser, result);
//		}
	} else if ((priv->mode & JAGP_MODE_TYPE) != 0) {
		if (priv->token->kind != JAGP_KIND_COLCOL) {
			priv->mode = JAGP_MODE_TYPE;
		}
		result = cat_ref_ptr(t);
//	} else if (priv->token->kind != JAGP_KIND_COLCOL) {
//		syntaxError(priv->token->pos, "dot.class.expected");
	}
	return result;
}

/**
 * MemberReferenceSuffix = "::" [TypeArguments] Ident
 *                       | "::" [TypeArguments] "new"
 */
static JagPJCExpression *l_member_reference_suffix(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos1 = priv->token->pos;
	l_accept(parser, JAGP_KIND_COLCOL);
	return l_member_reference_suffix_ext(parser, pos1, t);
}

/* M1 */
static JagPJCExpression *l_member_reference_suffix_ext(JagPParser *parser, int pos1, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_check_method_references(parser);
	priv->mode = JAGP_MODE_EXPR;
	CatArrayWo */*<JagPJCExpression>*/ typeArgs = NULL;
	if (priv->token->kind == JAGP_KIND_LT) {
		typeArgs = l_type_arguments(parser, FALSE);
	}
	JagPName *refName;
	JagPReferenceMode refMode;
	if (priv->token->kind == JAGP_KIND_NEW) {
		refMode = JAGP_REFERENCE_MODE_NEW;
		refName = cat_ref_ptr(priv->names_init);
		l_next_token(parser);
	} else {
		refMode = JAGP_REFERENCE_MODE_INVOKE;
		refName = l_ident(parser, FALSE);
	}
	jagp_tree_maker_at(priv->tree_maker, jagp_jctree_get_start_position((JagPJCTree *) t));
	JagPJCExpression *expr = t;
	t = (JagPJCExpression *) jagp_tree_maker_reference(priv->tree_maker, refMode, refName, expr, typeArgs);
	cat_unref_ptr(expr);
	cat_unref_ptr(refName);
	cat_unref_ptr(typeArgs);
	t = toP(parser, t);
	return t;
}

/** Creator = [Annotations] Qualident [TypeArguments] ( ArrayCreatorRest | ClassCreatorRest )
 */
/* M1 */
static JagPJCExpression *l_creator(JagPParser *parser, int newpos, CatArrayWo */*<JagPJCExpression>*/ typeArgs) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_creator:token=%d", priv->token->kind);
	CatArrayWo */*<JagPJCAnnotation>*/ newAnnotations = l_type_annotations_opt(parser);

	switch (priv->token->kind) {
		case JAGP_KIND_BYTE :
		case JAGP_KIND_SHORT :
		case JAGP_KIND_CHAR :
		case JAGP_KIND_INT :
		case JAGP_KIND_LONG :
		case JAGP_KIND_FLOAT :
		case JAGP_KIND_DOUBLE :
		case JAGP_KIND_BOOLEAN : {
			if (typeArgs == NULL) {
				JagPJCExpression *result = NULL;
				if (cat_array_wo_size(newAnnotations)==0) {
					JagPJCExpression *basictype = (JagPJCExpression *) l_basic_type(parser);
					result = l_array_creator_rest(parser, newpos, basictype);
					cat_unref_ptr(basictype);
				} else {
					jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(newAnnotations));
					JagPJCAnnotatedType *annotype = jagp_tree_maker_annotated_type(priv->tree_maker, newAnnotations, (JagPJCExpression *) l_basic_type(parser));
					annotype = toP(parser, annotype);
					result = l_array_creator_rest(parser, newpos, (JagPJCExpression *) annotype);
					cat_unref_ptr(annotype);
				}
				cat_unref_ptr(newAnnotations);
				cat_log_debug("l_creator:result=%O", result);
				cat_log_indent_level--;
				return result;
			}
			break;
		}
		default : {
		}
	}
	JagPJCExpression *creatorid = l_qualident(parser, TRUE);

	int oldmode = priv->mode;
	priv->mode = JAGP_MODE_TYPE;
	gboolean diamondFound = FALSE;
	int lastTypeargsPos = -1;
	if (priv->token->kind == JAGP_KIND_LT) {
		lastTypeargsPos = priv->token->pos;
		JagPJCExpression *argexpr = creatorid;
		creatorid = (JagPJCExpression *) l_type_arguments_apply(parser, argexpr, TRUE);
		cat_unref_ptr(argexpr);
		diamondFound = (priv->mode & JAGP_MODE_DIAMOND) != 0;
	}
	while (priv->token->kind == JAGP_KIND_DOT) {
		if (diamondFound) {
			/* cannot select after a diamond */
			l_illegal(parser, priv->token->pos);
		}
		int pos = priv->token->pos;
		l_next_token(parser);
		CatArrayWo */*<JagPJCAnnotation>*/ tyannos = l_type_annotations_opt(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *selected = creatorid;
		JagPName *selector = l_ident(parser, FALSE);
		creatorid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, selector);
		cat_unref_ptr(selected);
		cat_unref_ptr(selector);
		creatorid = toP(parser, creatorid);

		if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
			jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
			JagPJCExpression *underlying_type = creatorid;
			creatorid = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, underlying_type);
			cat_unref_ptr(underlying_type);
			creatorid = toP(parser, creatorid);
		}
		cat_unref_ptr(tyannos);

		if (priv->token->kind == JAGP_KIND_LT) {
			lastTypeargsPos = priv->token->pos;
			JagPJCExpression *argexpr = creatorid;
			creatorid = (JagPJCExpression *) l_type_arguments_apply(parser, argexpr, TRUE);
			cat_unref_ptr(argexpr);
			diamondFound = (priv->mode & JAGP_MODE_DIAMOND) != 0;
		}
	}
	JagPJCErroneous *result = NULL;
	priv->mode = oldmode;
	if (priv->token->kind == JAGP_KIND_LBRACKET || priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		/* handle type annotations for non primitive arrays */
		if (cat_array_wo_size(newAnnotations)>0) {
			JagPJCExpression *most_inner = creatorid;
			creatorid = l_insert_annotations_to_most_inner(parser, most_inner, newAnnotations, FALSE);
			cat_unref_ptr(most_inner);
		}

		JagPJCExpression *arrcrres = l_array_creator_rest(parser, newpos, creatorid);
		if (diamondFound) {
//			reportSyntaxError(lastTypeargsPos, "cannot.create.array.with.diamond");
			jagp_tree_maker_at(priv->tree_maker, newpos);
			CatArrayWo *errs = cat_array_wo_new();
			cat_array_wo_append(errs, (GObject *) arrcrres);
			cat_unref_ptr(arrcrres);
			JagPJCErroneous *res = jagp_tree_maker_erroneous(priv->tree_maker, errs);
			cat_unref_ptr(errs);
			result = toP(parser, res);
		} else if (typeArgs != NULL) {
			int pos = newpos;
			if (cat_array_wo_size(typeArgs)>0 && l_list_head_pos(typeArgs) != -1) {
				/* note: this should always happen but we should
				   not rely on this as the parser is continuously
				   modified to improve error recovery. */
				pos = l_list_head_pos(typeArgs);
			}
			l_set_error_end_pos(parser, jagp_ilexer_prev_token(priv->lexer)->endPos);
			jagp_tree_maker_at(priv->tree_maker, pos);
			CatArrayWo *errs = cat_array_wo_clone(typeArgs, CAT_CLONE_DEPTH_FULL);
			cat_array_wo_insert(errs, (GObject *) arrcrres, 0);
			cat_unref_ptr(arrcrres);
			JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//			reportSyntaxError(err, "cannot.create.array.with.type.arguments");
			cat_unref_ptr(errs);
			result = toP(parser, err);
		} else {
			result = arrcrres;
		}
	} else if (priv->token->kind == JAGP_KIND_LPAREN) {
		JagPJCNewClass *newClass = l_class_creator_rest(parser, newpos, NULL, typeArgs, creatorid);
		if (newClass->def != NULL) {
			if (cat_array_wo_size(newAnnotations)>0) {
				/* Add type and declaration annotations to the new class;
				   com.sun.tools.javac.code.TypeAnnotations.TypeAnnotationPositions.visitNewClass(JCNewClass)
				   will later remove all type annotations and only leave the
				   declaration annotations. */
				newClass->def->modifiers->parent.pos = l_earlier(newClass->def->modifiers->parent.pos, l_list_head_pos(newAnnotations));
				cat_unref_ptr(newClass->def->modifiers->annotations);
				newClass->def->modifiers->annotations = cat_ref_ptr(newAnnotations);
			}
		} else {
			/* handle type annotations for instantiations */
			if (cat_array_wo_size(newAnnotations)>0) {
				JagPJCErroneous *most_inner = creatorid;
				creatorid = l_insert_annotations_to_most_inner(parser, most_inner, newAnnotations, FALSE);
				cat_unref_ptr(most_inner);
				cat_ref_swap(newClass->clazz, creatorid);
			}
		}
		result = (JagPJCExpression *) newClass;
	} else {
		l_set_error_end_pos(parser, priv->token->pos);
//		reportSyntaxError(priv->token->pos, "expected2", JAGP_KIND_LPAREN, JAGP_KIND_LBRACKET);
		jagp_tree_maker_at(priv->tree_maker, newpos);
		CatArrayWo *nillist = cat_array_wo_new();
		JagPJCExpression *nwclsexpr = (JagPJCExpression *) jagp_tree_maker_new_class(priv->tree_maker, NULL, typeArgs, creatorid, nillist, NULL);
		cat_unref_ptr(nillist);
		nwclsexpr = toP(parser, nwclsexpr);
		CatArrayWo *errs = cat_array_wo_new();
		cat_array_wo_append(errs, (GObject *) nwclsexpr);
		cat_unref_ptr(nwclsexpr);
		jagp_tree_maker_at(priv->tree_maker, newpos);
		JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
		cat_unref_ptr(errs);
		result = toP(parser, err);
	}
	cat_unref_ptr(creatorid);
	cat_unref_ptr(newAnnotations);
	cat_log_debug("l_creator:result=%O", result);
	cat_log_indent_level--;
	return result;
}

/** InnerCreator = [Annotations] Ident [TypeArguments] ClassCreatorRest
 */
static JagPJCExpression *l_inner_creator(JagPParser *parser, int newpos, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *encl) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCAnnotation>*/ newAnnotations = l_type_annotations_opt(parser);

	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPName *name = l_ident(parser, FALSE);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, name);
	cat_unref_ptr(name);
	t = toP(parser, t);

	if (cat_array_wo_size(newAnnotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(newAnnotations));
		JagPJCExpression *underlying_type = t;
		t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, newAnnotations, underlying_type);
		cat_unref_ptr(underlying_type);
		t = toP(parser, t);
	}

	if (priv->token->kind == JAGP_KIND_LT) {
		int oldmode = priv->mode;
		t = (JagPJCExpression *) l_type_arguments_apply(parser, t, TRUE);
		priv->mode = oldmode;
	}
	return (JagPJCExpression *) l_class_creator_rest(parser, newpos, encl, typeArgs, t);
}

/** ArrayCreatorRest = [Annotations] "[" ( "]" BracketsOpt ArrayInitializer
 *                         | Expression "]" {[Annotations]  "[" Expression "]"} BracketsOpt )
 */
static JagPJCExpression *l_array_creator_rest(JagPParser *parser, int newpos, JagPJCExpression *elemtype) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *result = NULL;
	CatArrayWo */*<JagPJCAnnotation>*/ annos = l_type_annotations_opt(parser);

	l_accept(parser, JAGP_KIND_LBRACKET);
	if (priv->token->kind == JAGP_KIND_RBRACKET) {
		l_accept(parser, JAGP_KIND_RBRACKET);
		elemtype = l_brackets_opt(parser, elemtype, annos);
		if (priv->token->kind == JAGP_KIND_LBRACE) {
			JagPJCNewArray *na = (JagPJCNewArray *) l_array_initializer(parser, newpos, elemtype);
			if (cat_array_wo_size(annos)>0) {
				// when an array initializer is present then
				// the parsed annotations should target the
				// new array tree
				// bracketsOpt inserts the annotation in
				// elemtype, and it needs to be corrected
				//
				JagPJCAnnotatedType *annotated = (JagPJCAnnotatedType *) elemtype;
//				assert annotated.annotations == annos;
				cat_ref_swap(na->annotations, annotated->annotations);
				cat_ref_swap(na->elemtype, annotated->underlying_type);
			}
			result = (JagPJCExpression *) na;
		} else {
			jagp_tree_maker_at(priv->tree_maker, newpos);
			JagPJCExpression *newarray = (JagPJCExpression *) jagp_tree_maker_new_array(priv->tree_maker, elemtype, cat_array_wo_new(), NULL);
			newarray = toP(parser, newarray);
			CatArrayWo *lst = cat_array_wo_new();
			cat_array_wo_append(lst, (GObject *) newarray);
			cat_unref_ptr(newarray);
//			result = syntaxError(priv->token->pos, lst, "array.dimension.missing");
			cat_unref_ptr(lst);
		}
		cat_unref_ptr(elemtype);
	} else {
		CatArrayWo /*<JagPJCExpression>*/ *dims = cat_array_wo_new();

		/* maintain array dimension type annotations */
		CatArrayWo */*<List<JCAnnotation>>*/ dimAnnotations = cat_array_wo_new();
		cat_array_wo_append(dimAnnotations, (GObject *) annos);
		cat_array_wo_append(dims, (GObject *) l_parse_expression(parser));
		l_accept(parser, JAGP_KIND_RBRACKET);
		while (priv->token->kind == JAGP_KIND_LBRACKET
				|| priv->token->kind == JAGP_KIND_MONKEYS_AT) {
			CatArrayWo */*<JagPJCAnnotation>*/ maybeDimAnnos = l_type_annotations_opt(parser);
			int pos = priv->token->pos;
			l_next_token(parser);
			if (priv->token->kind == JAGP_KIND_RBRACKET) {
				elemtype = l_brackets_opt_cont(parser, elemtype, pos, maybeDimAnnos);
			} else {
				if (priv->token->kind == JAGP_KIND_RBRACKET) { /* no dimension */
					elemtype = l_brackets_opt_cont(parser, elemtype, pos, maybeDimAnnos);
				} else {
					cat_array_wo_append(dimAnnotations, (GObject *) maybeDimAnnos);
					GObject *ddexpr = (GObject *) l_parse_expression(parser);
					cat_array_wo_append(dims, ddexpr);
					cat_unref_ptr(ddexpr);
					l_accept(parser, JAGP_KIND_RBRACKET);
				}
			}
		}

		jagp_tree_maker_at(priv->tree_maker, newpos);
		JagPJCNewArray *na = jagp_tree_maker_new_array(priv->tree_maker, elemtype, dims, NULL);
		na = toP(parser, na);
		cat_unref_ptr(na->dimAnnotations);
		na->dimAnnotations = dimAnnotations;
		result = (JagPJCExpression *) na;
	}
	cat_unref_ptr(annos);
	return result;
}

/** ClassCreatorRest = Arguments [ClassBody]
 */
/* M1 */
static JagPJCNewClass *l_class_creator_rest(JagPParser *parser, int newpos, JagPJCExpression *encl, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCExpression>*/ args = l_arguments(parser);
	JagPJCClassDecl *body = NULL;
	if (priv->token->kind == JAGP_KIND_LBRACE) {
		int pos = priv->token->pos;
		CatArrayWo */*<JCTree>*/ defs = l_class_or_interface_body(parser, priv->names_empty, FALSE);
		jagp_tree_maker_at(priv->tree_maker, -1);
		JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, 0, NULL);
		jagp_tree_maker_at(priv->tree_maker, pos);
		body = jagp_tree_maker_anonymous_class_def(priv->tree_maker, mods, priv->names_empty, defs);
		cat_unref_ptr(defs);
		cat_unref_ptr(mods);
		body = toP(parser, body);
	}
	jagp_tree_maker_at(priv->tree_maker, newpos);
	JagPJCNewClass *ncls = jagp_tree_maker_new_class(priv->tree_maker, encl, typeArgs, t, args, body);
	cat_unref_ptr(body);
	cat_unref_ptr(args);
	ncls = toP(parser, ncls);
	return ncls;
}

/** ArrayInitializer = "{" [VariableInitializer {"," VariableInitializer}] [","] "}"
 */
/* M1 */
static JagPJCExpression *l_array_initializer(JagPParser *parser, int newpos, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_LBRACE);
	CatArrayWo *elems = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);
	} else if (priv->token->kind != JAGP_KIND_RBRACE) {
		GObject *varinit = (GObject *) l_variable_initializer(parser);
		cat_array_wo_append(elems, varinit);
		cat_unref_ptr(varinit);
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			if (priv->token->kind == JAGP_KIND_RBRACE) {
				break;
			}
			GObject *varinit = (GObject *) l_variable_initializer(parser);
			cat_array_wo_append(elems, varinit);
			cat_unref_ptr(varinit);
		}
	}
	l_accept(parser, JAGP_KIND_RBRACE);
	jagp_tree_maker_at(priv->tree_maker, newpos);
	CatArrayWo *nillist = cat_array_wo_new();
	JagPJCNewArray *res = jagp_tree_maker_new_array(priv->tree_maker, t, nillist, elems);
	cat_unref_ptr(elems);
	cat_unref_ptr(nillist);
	res = toP(parser, res);
	return (JagPJCExpression *) res;
}

/** VariableInitializer = ArrayInitializer | Expression
 */
/* M1 */
static JagPJCExpression *l_variable_initializer(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return priv->token->kind == JAGP_KIND_LBRACE ? l_array_initializer(parser, priv->token->pos, NULL) : l_parse_expression(parser);
}

/** ParExpression = "(" Expression ")"
 */
/* M1 */
static JagPJCExpression *l_par_expression(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_LPAREN);
	JagPJCExpression *expr = l_parse_expression(parser);
	l_accept(parser, JAGP_KIND_RPAREN);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_parens(priv->tree_maker, expr);
	cat_unref_ptr(expr);
	t = toP(parser, t);
	return t;
}

/** Block = "{" BlockStatements "}"
 */
/* M1 */
static JagPJCBlock *l_block_ext(JagPParser *parser, int pos, long flags) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_LBRACE);
	CatArrayWo */*<JCStatement>*/ stats = l_block_statements(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCBlock *result = jagp_tree_maker_block(priv->tree_maker, flags, stats);
	cat_unref_ptr(stats);
	while (priv->token->kind == JAGP_KIND_CASE || priv->token->kind == JAGP_KIND_DEFAULT) {
//		syntaxError("orphaned", priv->token->kind);
		CatArrayWo *orpahned = l_switch_block_statement_groups(parser);
		cat_unref_ptr(orpahned);
	}
	/* the Block node has a field "endpos" for first char of last token, which is
	 * usually but not necessarily the last char of the last token. */
	result->endpos = priv->token->pos;
	l_accept(parser, JAGP_KIND_RBRACE);
	return toP(parser, result);
}

/* M1 */
static JagPJCBlock *l_block(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return l_block_ext(parser, priv->token->pos, 0);
}

/** BlockStatements = { BlockStatement }
 *  BlockStatement  = LocalVariableDeclarationStatement
 *                  | ClassOrInterfaceOrEnumDeclaration
 *                  | [Ident ":"] Statement
 *  LocalVariableDeclarationStatement
 *                  = { FINAL | '@' Annotation } Type VariableDeclarators ";"
 */
/* M1 */
static CatArrayWo /*<JCStatement>*/ *l_block_statements(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	/* todo: skip to anchor on error(?) */
	int lastErrPos = -1;
	CatArrayWo */*<JagPJCStatement>*/ stats = cat_array_wo_new();
	while (TRUE) {
		CatArrayWo */*<JCStatement>*/ stat = l_block_statement(parser);
		cat_log_debug("stat.size=%d", cat_array_wo_size(stat));
		if (cat_array_wo_size(stat)==0) {
			cat_unref_ptr(stat);
			return stats;
		} else {
			/* error recovery */
			if (priv->token->pos == lastErrPos) {
				cat_unref_ptr(stat);
				return stats;
			}
//			if (priv->token->pos <= endPosTable.errorEndPos) {
//				skip(FALSE, TRUE, TRUE, TRUE);
//				lastErrPos = priv->token->pos;
//			}
			cat_array_wo_append_all(stats, stat);
			cat_unref_ptr(stat);
		}
	}
}

/*
 * Parse a Statement (JLS 14.5). As an enhancement to improve error recovery,
 * this method will also recognize variable and class declarations (which are
 * not legal for a Statement) by delegating the parsing to BlockStatement (JLS 14.2).
 * If any illegal declarations are found, they will be wrapped in an erroneous tree,
 * and an error will be produced by this method.
 */
/* M1 */
static JagPJCStatement *l_parse_statement_as_block(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	CatArrayWo */*<JCStatement>*/ stats = l_block_statement(parser);
	if (cat_array_wo_size(stats)==0) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCErroneous *e = jagp_tree_maker_erroneous(priv->tree_maker, NULL);
//		error(e, "illegal.start.of.stmt");
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_exec(priv->tree_maker, (JagPJCExpression *) e);
		cat_unref_ptr(stats);
		cat_unref_ptr(e);
		return result;
	} else {
		JagPJCStatement *first = (JagPJCStatement *) cat_array_wo_get_first(stats);
		const char *error = NULL;
		JagPTag tag = jagp_jctree_get_tag((JagPJCTree *) first);
		switch (tag) {
			case JAGP_TAG_CLASSDEF:
				error = "class.not.allowed";
				break;
			case JAGP_TAG_VARDEF:
				error = "variable.not.allowed";
				break;
			default :
				break;
		}
		if (error != NULL) {
//			error(first, error);
			jagp_tree_maker_at(priv->tree_maker, first->parent.pos);
			JagPJCBlock *block = jagp_tree_maker_block(priv->tree_maker, 0, stats);
			CatArrayWo /*<JCBlock>*/ *blist = cat_array_wo_new();
			cat_array_wo_append(blist, (GObject *) block);
			cat_unref_ptr(block);
			jagp_tree_maker_at(priv->tree_maker, first->parent.pos);
			JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, blist);
			cat_unref_ptr(blist);

			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_exec(priv->tree_maker, (JagPJCExpression *) err);
			cat_unref_ptr(err);
			result = toP(parser, result);
			cat_unref_ptr(stats);
			return result;
		}
		cat_ref_ptr(first);
		cat_unref_ptr(stats);
		return first;
	}
}

/**This method parses a statement appearing inside a block.
 */
/* M1 */
static CatArrayWo */*<JCStatement>*/ l_block_statement(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_block_statement:token=%d", priv->token->kind);
	/* todo: skip to anchor on error(?) */
	int pos = priv->token->pos;
	switch (priv->token->kind) {
		case JAGP_KIND_RBRACE : case JAGP_KIND_CASE : case JAGP_KIND_DEFAULT : case JAGP_KIND_EOF :
			return cat_array_wo_new();
		case JAGP_KIND_LBRACE : case JAGP_KIND_IF : case JAGP_KIND_FOR : case JAGP_KIND_WHILE : case JAGP_KIND_DO : case JAGP_KIND_TRY :
		case JAGP_KIND_SWITCH : case JAGP_KIND_SYNCHRONIZED : case JAGP_KIND_RETURN : case JAGP_KIND_THROW : case JAGP_KIND_BREAK :
		case JAGP_KIND_CONTINUE : case JAGP_KIND_SEMI : case JAGP_KIND_ELSE : case JAGP_KIND_FINALLY : case JAGP_KIND_CATCH :
		case JAGP_KIND_ASSERT : {
			CatArrayWo *result = cat_array_wo_new();
			JagPJCStatement *pss = l_parse_simple_statement(parser);
			cat_array_wo_append(result, (GObject *) pss);
			cat_unref_ptr(pss);
			return result;
		}
		case JAGP_KIND_MONKEYS_AT :
		case JAGP_KIND_FINAL : {
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
			CatArrayWo *result = cat_array_wo_new();
			if (priv->token->kind == JAGP_KIND_INTERFACE ||
					priv->token->kind == JAGP_KIND_CLASS ||
					priv->token->kind == JAGP_KIND_ENUM) {
				GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
				cat_array_wo_append(result, clorinorende);
				cat_unref_ptr(clorinorende);
			} else {
				JagPJCExpression *declarator = l_parse_type(parser);
				l_variable_declarators(parser, mods, declarator, result);
				cat_unref_ptr(declarator);
				/* A "LocalVariableDeclarationStatement" subsumes the terminating semicolon */
				l_accept(parser, JAGP_KIND_SEMI);
				l_store_end((JagPJCTree *) cat_array_wo_get_last(result), jagp_ilexer_prev_token(priv->lexer)->endPos);
			}
			cat_unref_ptr(mods);
			cat_unref_ptr(dc);
			return result;
		}
		case JAGP_KIND_ABSTRACT : 
		case JAGP_KIND_STRICTFP : {
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			cat_unref_ptr(mods);
			cat_unref_ptr(dc);
			return result;
		}
// todo compare below block with block above (are they identical)
		case JAGP_KIND_INTERFACE :
		case JAGP_KIND_CLASS : {
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			cat_unref_ptr(mods);
			cat_unref_ptr(dc);
			return result;
		}
		case JAGP_KIND_ENUM : {
//			error(priv->token->pos, "local.enum");
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			cat_unref_ptr(mods);
			cat_unref_ptr(dc);
		return result;
		}
		default : {
			JagPToken *prevToken = priv->token;
			JagPJCExpression *termres = l_term(parser, JAGP_MODE_EXPR | JAGP_MODE_TYPE);
			CatArrayWo *result = cat_array_wo_new();
			if (priv->token->kind == JAGP_KIND_COLON && jagp_jctree_has_tag((JagPJCTree *) termres, JAGP_TAG_IDENT)) {
				l_next_token(parser);
				JagPJCStatement *stat = l_parse_statement_as_block(parser);
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCLabeledStatement *labstat = jagp_tree_maker_labelled(priv->tree_maker, jagp_token_name(prevToken), stat);
				cat_unref_ptr(stat);
				cat_array_wo_append(result, (GObject *) labstat);
				cat_unref_ptr(labstat);
			} else if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && lax_identifier_accepts(priv->token->kind)) {
				pos = priv->token->pos;
				jagp_tree_maker_at(priv->tree_maker, -1);
				JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, 0, cat_array_wo_new());
				jagp_tree_maker_at(priv->tree_maker, pos);
				l_variable_declarators(parser, mods, termres, result);
				/* A "LocalVariableDeclarationStatement" subsumes the terminating semicolon */
				cat_unref_ptr(mods);
				l_accept(parser, JAGP_KIND_SEMI);
				l_store_end((JagPJCTree *) cat_array_wo_get_last(result), jagp_ilexer_prev_token(priv->lexer)->endPos);
			} else {
				/* This Exec is an "ExpressionStatement"; it subsumes the terminating semicolon */
				JagPJCExpression *checked_expr = l_check_expr_stat(parser, termres);
				l_accept(parser, JAGP_KIND_SEMI);
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCExpressionStatement *expr = jagp_tree_maker_exec(priv->tree_maker, checked_expr);
				cat_unref_ptr(checked_expr);
				expr = toP(parser, expr);
				cat_array_wo_append(result, (GObject *) expr);
				cat_unref_ptr(expr);
			}
			cat_unref_ptr(termres);
			return result;
		}
	}
}


/** Statement =
 *       Block
 *     | IF ParExpression Statement [ELSE Statement]
 *     | FOR "(" ForInitOpt ";" [Expression] ";" ForUpdateOpt ")" Statement
 *     | FOR "(" FormalParameter : Expression ")" Statement
 *     | WHILE ParExpression Statement
 *     | DO Statement WHILE ParExpression ";"
 *     | TRY Block ( Catches | [Catches] FinallyPart )
 *     | TRY "(" ResourceSpecification ";"opt ")" Block [Catches] [FinallyPart]
 *     | SWITCH ParExpression "{" SwitchBlockStatementGroups "}"
 *     | SYNCHRONIZED ParExpression Block
 *     | RETURN [Expression] ";"
 *     | THROW Expression ";"
 *     | BREAK [Ident] ";"
 *     | CONTINUE [Ident] ";"
 *     | ASSERT Expression [ ":" Expression ] ";"
 *     | ";"
 */
/* M1 */
static JagPJCStatement *l_parse_simple_statement(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_parse_simple_statement:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	switch (priv->token->kind) {
		case JAGP_KIND_LBRACE:
			return (JagPJCStatement *) l_block(parser);
		case JAGP_KIND_IF: {
			l_next_token(parser);
			JagPJCExpression *cond = l_par_expression(parser);
			JagPJCStatement *thenpart = l_parse_statement_as_block(parser);
			JagPJCStatement *elsepart = NULL;
			if (priv->token->kind == JAGP_KIND_ELSE) {
				l_next_token(parser);
				elsepart = l_parse_statement_as_block(parser);
			}
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *t = (JagPJCStatement *) jagp_tree_maker_if(priv->tree_maker, cond, thenpart, elsepart);
			cat_unref_ptr(cond);
			cat_unref_ptr(thenpart);
			cat_unref_ptr(elsepart);
			return t;
		}
		case JAGP_KIND_FOR: {
			l_next_token(parser);
			l_accept(parser, JAGP_KIND_LPAREN);
			CatArrayWo */*<JCStatement>*/ inits = priv->token->kind == JAGP_KIND_SEMI ? cat_array_wo_new() : l_for_init(parser);
			JagPJCStatement *inits_head = (JagPJCStatement *) cat_array_wo_get_first(inits);
			JagPJCStatement *result = NULL;
			if (cat_array_wo_size(inits) == 1 &&
					jagp_jctree_has_tag((JagPJCTree *) inits_head, JAGP_TAG_VARDEF) &&
					((JagPJCVariableDecl *) inits_head)->init == NULL &&
					priv->token->kind == JAGP_KIND_COLON) {
				JagPJCVariableDecl *var = (JagPJCVariableDecl *) inits_head;
				l_accept(parser, JAGP_KIND_COLON);
				JagPJCExpression *expr = l_parse_expression(parser);
				l_accept(parser, JAGP_KIND_RPAREN);
				JagPJCStatement *body = l_parse_statement_as_block(parser);
				jagp_tree_maker_at(priv->tree_maker, pos);
				result = (JagPJCStatement *) jagp_tree_maker_foreach_loop(priv->tree_maker, var, expr, body);
				cat_unref_ptr(expr);
				cat_unref_ptr(body);
			} else {
				l_accept(parser, JAGP_KIND_SEMI);
				JagPJCExpression *cond = priv->token->kind == JAGP_KIND_SEMI ? NULL : l_parse_expression(parser);
				l_accept(parser, JAGP_KIND_SEMI);
				CatArrayWo /*<JCExpressionStatement>*/ *steps = priv->token->kind == JAGP_KIND_RPAREN ? cat_array_wo_new() : l_for_update(parser);
				l_accept(parser, JAGP_KIND_RPAREN);
				JagPJCStatement *body = l_parse_statement_as_block(parser);
				jagp_tree_maker_at(priv->tree_maker, pos);
				result = (JagPJCStatement *) jagp_tree_maker_for_loop(priv->tree_maker, inits, cond, steps, body);
				cat_unref_ptr(cond);
				cat_unref_ptr(steps);
				cat_unref_ptr(body);
			}
			cat_unref_ptr(inits);
			return result;
		}
		case JAGP_KIND_WHILE : {
			l_next_token(parser);
			JagPJCExpression *cond = l_par_expression(parser);
			JagPJCStatement *body = l_parse_statement_as_block(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_while_loop(priv->tree_maker, cond, body);
			cat_unref_ptr(cond);
			cat_unref_ptr(body);
			return result;
		}
		case JAGP_KIND_DO : {
			l_next_token(parser);
			JagPJCStatement *body = l_parse_statement_as_block(parser);
			l_accept(parser, JAGP_KIND_WHILE);
			JagPJCExpression *cond = l_par_expression(parser);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_do_loop(priv->tree_maker, body, cond);
			cat_unref_ptr(body);
			cat_unref_ptr(cond);
			result = toP(parser, result);
			return result;
		}
		case JAGP_KIND_TRY : {
			l_next_token(parser);
			CatArrayWo */*<JCTree>*/ resources = cat_array_wo_new();
			if (priv->token->kind == JAGP_KIND_LPAREN) {
				l_check_try_with_resources(parser);
				l_next_token(parser);
				cat_unref_ptr(resources);
				resources = l_resources(parser);
				l_accept(parser, JAGP_KIND_RPAREN);
			}
			JagPJCBlock *body = l_block(parser);
			CatArrayWo /*<JagPJCCatch>*/ *catchers = cat_array_wo_new();
			JagPJCBlock *finalizer = NULL;
			if (priv->token->kind == JAGP_KIND_CATCH || priv->token->kind == JAGP_KIND_FINALLY) {
				while (priv->token->kind == JAGP_KIND_CATCH) {
					GObject *cc = (GObject *) l_catch_clause(parser);
					cat_array_wo_append(catchers, cc);
					cat_unref_ptr(cc);
				}
				if (priv->token->kind == JAGP_KIND_FINALLY) {
					l_next_token(parser);
					finalizer = l_block(parser);
				}
			} else {
//				if (priv->allowTWR) {
//					if (resources.isEmpty())
//						error(pos, "try.without.catch.finally.or.resource.decls");
//				} else
//					error(pos, "try.without.catch.or.finally");
			}
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_try(priv->tree_maker, resources, body, catchers, finalizer);
			cat_unref_ptr(resources);
			cat_unref_ptr(body);
			cat_unref_ptr(catchers);
			cat_unref_ptr(finalizer);
			return result; 
		}
		case JAGP_KIND_SWITCH : {
			l_next_token(parser);
			JagPJCExpression *selector = l_par_expression(parser);
			l_accept(parser, JAGP_KIND_LBRACE);
			CatArrayWo /*<JCCase>*/ *cases = l_switch_block_statement_groups(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCSwitch *t = jagp_tree_maker_switch(priv->tree_maker, selector, cases);
			t = to(parser, t);
			l_accept(parser, JAGP_KIND_RBRACE);
			cat_unref_ptr(selector);
			cat_unref_ptr(cases);
			return (JagPJCStatement *) t;
		}
		case JAGP_KIND_SYNCHRONIZED : {
			l_next_token(parser);
			JagPJCExpression *lock = l_par_expression(parser);
			JagPJCBlock *body = l_block(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCSynchronized *jsync = jagp_tree_maker_synchronized(priv->tree_maker, lock, body);
			cat_unref_ptr(lock);
			cat_unref_ptr(body);
			return (JagPJCStatement *) jsync;
		}
		case JAGP_KIND_RETURN : {
			l_next_token(parser);
			JagPJCExpression *returnval = priv->token->kind == JAGP_KIND_SEMI ? NULL : l_parse_expression(parser);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCReturn *jret = jagp_tree_maker_return(priv->tree_maker, returnval);
			jret = toP(parser, jret);
			cat_unref_ptr(returnval);
			return (JagPJCStatement *) jret;
		}
		case JAGP_KIND_THROW : {
			l_next_token(parser);
			JagPJCExpression *exc = l_parse_expression(parser);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCThrow *jthrow = jagp_tree_maker_throw(priv->tree_maker, exc);
			cat_unref_ptr(exc);
			jthrow = toP(parser, jthrow);
			return (JagPJCStatement *) jthrow;
		}
		case JAGP_KIND_BREAK : {
			l_next_token(parser);
			JagPName *label = lax_identifier_accepts(priv->token->kind) ? l_ident(parser, FALSE) : NULL;
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCBreak *jbreak = jagp_tree_maker_break(priv->tree_maker, label);
			cat_unref_ptr(label);
			jbreak = toP(parser, jbreak);
			return (JagPJCStatement *) jbreak;
		}
		case JAGP_KIND_CONTINUE : {
			l_next_token(parser);
			JagPName *label = lax_identifier_accepts(priv->token->kind) ? l_ident(parser, FALSE) : NULL;
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCContinue *jcont = jagp_tree_maker_continue(priv->tree_maker, label);
			cat_unref_ptr(label);
			jcont =  toP(parser, jcont);
			return (JagPJCStatement *) jcont;
		}
		case JAGP_KIND_SEMI : {
			l_next_token(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCSkip *skip = jagp_tree_maker_skip(priv->tree_maker);
			skip = toP(parser, skip);
			return (JagPJCStatement *) skip;
		}
		case JAGP_KIND_ELSE : {
			int elsePos = priv->token->pos;
			l_next_token(parser);
			return l_do_recover(parser, elsePos, (JagPBasicErrorRecoveryAction) l_parse_statement_as_block, cat_string_wo_new_with("else.without.if"));
		}
		case JAGP_KIND_FINALLY : {
			int finallyPos = priv->token->pos;
			l_next_token(parser);
			return l_do_recover(parser, finallyPos, (JagPBasicErrorRecoveryAction) l_parse_statement_as_block, cat_string_wo_new_with("finally.without.try"));
		}
		case JAGP_KIND_CATCH :
			return l_do_recover(parser, priv->token->pos, (JagPBasicErrorRecoveryAction) l_catch_clause, cat_string_wo_new_with("catch.without.try"));
		case JAGP_KIND_ASSERT : {
			l_next_token(parser);
			JagPJCExpression *assertion = l_parse_expression(parser);
			JagPJCExpression *message = NULL;
			if (priv->token->kind == JAGP_KIND_COLON) {
				l_next_token(parser);
				message = l_parse_expression(parser);
			}
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCAssert *t = jagp_tree_maker_assert(priv->tree_maker, assertion, message);
			cat_unref_ptr(assertion);
			cat_unref_ptr(message);
			t = toP(parser, t);
			return (JagPJCStatement *) t;
		}
		default:
//			Assert.error();
			return NULL;
	}
}

static JagPJCStatement *l_do_recover(JagPParser *parser, int startPos, JagPBasicErrorRecoveryAction action, CatStringWo *c_key) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	int errPos = S.errPos();
	JagPJCTree *stm = action(parser);
//	S.errPos(errPos);
	CatArrayWo *errs = cat_array_wo_new();
	cat_array_wo_append(errs, (GObject *) stm);
	cat_unref_ptr(stm);
	JagPJCExpression *err = NULL; // syntaxError(startPos, errs, c_key);
	cat_unref_ptr(errs);
	JagPJCExpressionStatement *exprstat = jagp_tree_maker_exec(priv->tree_maker, err);
	exprstat = toP(parser, exprstat);
	cat_unref_ptr(c_key);
	return (JagPJCStatement *) exprstat;
}

/** CatchClause     = CATCH "(" FormalParameter ")" Block
 * TODO: the "FormalParameter" is not correct, it uses the special "catchTypes" rule below.
 */
/* M1 */
static JagPJCCatch *l_catch_clause(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_catch_clause:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_CATCH);
	l_accept(parser, JAGP_KIND_LPAREN);
	JagPJCModifiers *mods = l_opt_final(parser, JAGP_FLAG_PARAMETER);
	CatArrayWo */*<JagPJCExpression>*/ catchTypes = l_catch_types(parser);
	JagPJCExpression *paramType = NULL;
	if (cat_array_wo_size(catchTypes)>1) {
		jagp_tree_maker_at(priv->tree_maker, jagp_tree_info_get_end_pos((JagPJCTree *) cat_array_wo_get_first(catchTypes)));
		paramType = (JagPJCExpression *) jagp_tree_maker_type_union(priv->tree_maker, catchTypes);
		paramType = toP(parser, paramType);
	} else {
		paramType = (JagPJCExpression *) cat_array_wo_get_first(catchTypes);
		cat_ref_ptr(paramType);
	}
	cat_unref_ptr(catchTypes);
	JagPJCVariableDecl *formal = l_variable_declarator_id(parser, mods, paramType, FALSE);
	cat_unref_ptr(paramType);
	cat_unref_ptr(mods);
	l_accept(parser, JAGP_KIND_RPAREN);
	JagPJCBlock *body = l_block(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCCatch *result = jagp_tree_maker_catch(priv->tree_maker, formal, body);
	cat_unref_ptr(formal);
	cat_unref_ptr(body);
	return result;
}

/* M1 */
static CatArrayWo /*<JagPJCExpression>*/ *l_catch_types(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_catch_types:token=%d", priv->token->kind);
	CatArrayWo /*<JagPJCExpression>*/ *catchTypes = cat_array_wo_new();
	GObject *pt = (GObject *) l_parse_type(parser);
	cat_array_wo_append(catchTypes, pt);
	cat_unref_ptr(pt);
	while (priv->token->kind == JAGP_KIND_BAR) {
		l_check_multicatch(parser);
		l_next_token(parser);
		/* Instead of qualident this is now parseType.
		 * But would that allow too much, e.g. arrays or generics? */
		GObject *pt = (GObject *) l_parse_type(parser);
		cat_array_wo_append(catchTypes, pt);
		cat_unref_ptr(pt);
	}
	return catchTypes;
}

/** SwitchBlockStatementGroups = { SwitchBlockStatementGroup }
 *  SwitchBlockStatementGroup = SwitchLabel BlockStatements
 *  SwitchLabel = CASE ConstantExpression ":" | DEFAULT ":"
 */
/* M1 */
static CatArrayWo /*<JCCase>*/ *l_switch_block_statement_groups(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_switch_block_statement_groups:token=%d", priv->token->kind);
	CatArrayWo /*<JCCase>*/ *cases = cat_array_wo_new();
	while (TRUE) {
//		int pos = priv->token->pos;
		switch (priv->token->kind) {
			case JAGP_KIND_CASE :
			case JAGP_KIND_DEFAULT : {
				GObject *grp = (GObject *) l_switch_block_statement_group(parser);
				cat_array_wo_append(cases, grp);
				cat_unref_ptr(grp);
				break;
			}
			case JAGP_KIND_RBRACE : case JAGP_KIND_EOF :
				return cases;
			default:
				l_next_token(parser); /* to ensure progress */
//				syntaxError(pos, "expected3",
//						JAGP_KIND_CASE, JAGP_KIND_DEFAULT, JAGP_KIND_RBRACE);
		}
	}
}

/* M1 */
static JagPJCCase *l_switch_block_statement_group(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_switch_block_statement_group:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	CatArrayWo */*<JCStatement>*/ stats;
	JagPJCCase *c = NULL;
	JagPJCExpression *pat = NULL;
	switch (priv->token->kind) {
		case JAGP_KIND_CASE :
			l_next_token(parser);
			pat = l_parse_expression(parser);
			break;

		case JAGP_KIND_DEFAULT :
			l_next_token(parser);
			break;

		default :
			/* throw new AssertionError("should not reach here"); */
			break;
	}
	l_accept(parser, JAGP_KIND_COLON);
	stats = l_block_statements(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	c = jagp_tree_maker_case(priv->tree_maker, pat, stats);
	if (cat_array_wo_size(stats)==0) {
		l_store_end((JagPJCTree *) c, jagp_ilexer_prev_token(priv->lexer)->endPos);
	}
	cat_unref_ptr(stats);
	cat_unref_ptr(pat);
	return c;
}

/** MoreStatementExpressions = { JAGP_KIND_COMMA StatementExpression }
 */
/* M1 */
static CatArrayWo /*<? super JCExpressionStatement>>*/ *l_more_statement_expressions(JagPParser *parser, int pos, JagPJCExpression *first, CatArrayWo  *stats) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_more_statement_expressions:token=%d", priv->token->kind);
	/* This Exec is a "StatementExpression"; it subsumes no terminating token */
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *checked_expr = l_check_expr_stat(parser, first);
	JagPJCExpressionStatement *exprstat = jagp_tree_maker_exec(priv->tree_maker, checked_expr);
	cat_unref_ptr(checked_expr);
	exprstat = toP(parser, exprstat);
	cat_array_wo_append(stats, (GObject *) exprstat);
	cat_unref_ptr(exprstat);
	while (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);
		pos = priv->token->pos;
		JagPJCExpression *next = l_parse_expression(parser);
		/* This Exec is a "StatementExpression"; it subsumes no terminating token */
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *checked_expr = l_check_expr_stat(parser, next);
		cat_unref_ptr(next);
		JagPJCExpressionStatement *exprstat = jagp_tree_maker_exec(priv->tree_maker, checked_expr);
		cat_unref_ptr(checked_expr);
		exprstat = toP(parser, exprstat);
		cat_array_wo_append(stats, (GObject *) exprstat);
		cat_unref_ptr(exprstat);
	}
	return stats;
}

/** ForInit = StatementExpression MoreStatementExpressions
 *           |  { FINAL | '@' Annotation } Type VariableDeclarators
 */
/* M1 */
static CatArrayWo */*<JCStatement>*/ l_for_init(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_for_init:token=%d", priv->token->kind);
	CatArrayWo /*<JCStatement>*/ *result = cat_array_wo_new();
	int pos = priv->token->pos;
	if (priv->token->kind == JAGP_KIND_FINAL || priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		JagPJCModifiers *mods = l_opt_final(parser, 0);
		JagPJCExpression *varexpr = l_parse_type(parser);
		l_variable_declarators(parser, mods , varexpr, result);
		cat_unref_ptr(varexpr);
		cat_unref_ptr(mods);
	} else {
		JagPJCExpression *termres = l_term(parser, JAGP_MODE_EXPR | JAGP_MODE_TYPE);
		if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && lax_identifier_accepts(priv->token->kind)) {
			JagPJCModifiers *modsopt = l_modifiers_opt(parser, NULL);
			l_variable_declarators(parser, modsopt, termres, result);
			cat_unref_ptr(modsopt);
		} else if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && priv->token->kind == JAGP_KIND_COLON) {
//			error(pos, "bad.initializer", "for-loop");
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCVariableDecl *vardecl = jagp_tree_maker_var_def(priv->tree_maker, NULL, NULL, termres, NULL);
			cat_array_wo_append(result, (GObject *) vardecl);
			cat_unref_ptr(vardecl);
		} else {
			l_more_statement_expressions(parser, pos, termres, result);
		}
		cat_unref_ptr(termres);
	}
	return result;
}

/** ForUpdate = StatementExpression MoreStatementExpressions
 */
/* M1 */
static CatArrayWo /*<JCExpressionStatement>*/ *l_for_update(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_for_update:token=%d", priv->token->kind);
	JagPJCExpression *pexpr = l_parse_expression(parser);
	CatArrayWo *nillist = cat_array_wo_new();
	CatArrayWo *result = l_more_statement_expressions(parser, priv->token->pos, pexpr, nillist);
	cat_unref_ptr(pexpr);
	return result;
}

/** AnnotationsOpt = { '@' Annotation }
 *
 * @param kind Whether to parse an ANNOTATION or TYPE_ANNOTATION
 */
/* M1 */
static CatArrayWo */*<JagPJCAnnotation>*/ l_annotations_opt(JagPParser *parser, JagPTag kind) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_annotations_opt:token=%d", priv->token->kind);
	if (priv->token->kind != JAGP_KIND_MONKEYS_AT) {
		return cat_array_wo_new(); /* optimization */
	}
	CatArrayWo /*<JagPJCAnnotation>*/ *buf = cat_array_wo_new();
	int prevmode = priv->mode;
	while (priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		int pos = priv->token->pos;
		l_next_token(parser);
		GObject *anno = (GObject *) l_annotation(parser, pos, kind);
		cat_array_wo_append(buf, anno);
		cat_unref_ptr(anno);
	}
	priv->last_mode = priv->mode;
	priv->mode = prevmode;

	return buf;
}

/* M1 */
static CatArrayWo */*<JagPJCAnnotation>*/ l_type_annotations_opt(JagPParser *parser) {
	CatArrayWo */*<JagPJCAnnotation>*/ annotations = l_annotations_opt(parser, JAGP_TAG_TYPE_ANNOTATION);
	return annotations;
}

/** ModifiersOpt = { Modifier }
 *  Modifier = PUBLIC | PROTECTED | PRIVATE | STATIC | ABSTRACT | FINAL
 *           | NATIVE | SYNCHRONIZED | TRANSIENT | VOLATILE | "@"
 *           | "@" Annotation
 */
/* M1 */
static JagPJCModifiers *l_modifiers_opt(JagPParser *parser, JagPJCModifiers *partial) {
	cat_log_debug("l_modifiers_opt");
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	long flags;
	CatArrayWo /*<JagPJCAnnotation>*/ *annotations = cat_array_wo_new();
	int pos;
	if (partial == NULL) {
		flags = 0;
		pos = priv->token->pos;
	} else {
		flags = partial->flags;
		cat_array_wo_append_all(annotations, partial->annotations);
		pos = partial->parent.pos;
	}
//	if (token.deprecatedFlag()) {
//		flags |= JAGP_FLAG_DEPRECATED;
//	}
	int lastPos;
	gboolean keep_looping = TRUE;
	while (keep_looping) {
		long flag;
		switch (priv->token->kind) {
			case JAGP_KIND_PRIVATE     : flag = JAGP_FLAG_PRIVATE; break;
			case JAGP_KIND_PROTECTED   : flag = JAGP_FLAG_PROTECTED; break;
			case JAGP_KIND_PUBLIC      : flag = JAGP_FLAG_PUBLIC; break;
			case JAGP_KIND_STATIC      : flag = JAGP_FLAG_STATIC; break;
			case JAGP_KIND_TRANSIENT   : flag = JAGP_FLAG_TRANSIENT; break;
			case JAGP_KIND_FINAL       : flag = JAGP_FLAG_FINAL; break;
			case JAGP_KIND_ABSTRACT    : flag = JAGP_FLAG_ABSTRACT; break;
			case JAGP_KIND_NATIVE      : flag = JAGP_FLAG_NATIVE; break;
			case JAGP_KIND_VOLATILE    : flag = JAGP_FLAG_VOLATILE; break;
			case JAGP_KIND_SYNCHRONIZED: flag = JAGP_FLAG_SYNCHRONIZED; break;
			case JAGP_KIND_STRICTFP    : flag = JAGP_FLAG_STRICTFP; break;
			case JAGP_KIND_MONKEYS_AT  : flag = JAGP_FLAG_ANNOTATION; break;
			case JAGP_KIND_DEFAULT     : l_check_default_methods(parser); flag = JAGP_FLAG_DEFAULT; break;
			case JAGP_KIND_ERROR       : flag = 0; l_next_token(parser); break;
			default: keep_looping = FALSE; continue;
		}
//		if ((flags & flag) != 0) error(priv->token->pos, "repeated.modifier");
		lastPos = priv->token->pos;
		l_next_token(parser);
		if (flag == JAGP_FLAG_ANNOTATION) {
			if (priv->token->kind != JAGP_KIND_INTERFACE) {
				JagPJCAnnotation *ann = l_annotation(parser, lastPos, JAGP_TAG_ANNOTATION);
				/* if first modifier is an annotation, set pos to annotation's. */
				if (flags == 0 && cat_array_wo_size(annotations)==0) {
					pos = ann->parent.parent.pos;
				}
				cat_array_wo_append(annotations, (GObject *) ann);
				cat_unref_ptr(ann);
				flag = 0;
			}
		}
		flags |= flag;
	}
	switch (priv->token->kind) {
		case JAGP_KIND_ENUM: flags |= JAGP_FLAG_ENUM; break;
		case JAGP_KIND_INTERFACE: flags |= JAGP_FLAG_INTERFACE; break;
		default: break;
	}

	/* A modifiers tree with no modifier tokens or annotations
	 * has no text position. */
	if ((flags & (JAGP_FLAG_ModifierFlags | JAGP_FLAG_ANNOTATION)) == 0 && cat_array_wo_size(annotations)==0) {
		pos = -1;
	}

	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, flags, annotations);
	if (pos != -1) {
		l_store_end((JagPJCTree *) mods, jagp_ilexer_prev_token(priv->lexer)->endPos);
	}
	cat_unref_ptr(annotations);
	return mods;
}

/** Annotation              = "@" Qualident [ "(" AnnotationFieldValues ")" ]
 *
 * @param pos position of "@" token
 * @param kind Whether to parse an ANNOTATION or TYPE_ANNOTATION
 */
/* M1 */
static JagPJCAnnotation *l_annotation(JagPParser *parser, int pos, JagPTag kind) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_annotation:token=%d", priv->token->kind);
	// l_accept(parser, JAGP_KIND_AT); // AT consumed by caller
	if (kind == JAGP_TAG_TYPE_ANNOTATION) {
		l_check_type_annotations(parser);
	}
	JagPJCTree *ident = (JagPJCTree *) l_qualident(parser, FALSE);
	CatArrayWo */*<JagPJCExpression>*/ fieldValues = l_annotation_field_values_opt(parser);
	JagPJCAnnotation *ann;
	if (kind == JAGP_TAG_ANNOTATION) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		ann = jagp_tree_maker_annotation(priv->tree_maker, ident, fieldValues);
	} else if (kind == JAGP_TAG_TYPE_ANNOTATION) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		ann = jagp_tree_maker_type_annotation(priv->tree_maker, ident, fieldValues);
//	} else {
//		throw new AssertionError("Unhandled annotation kind: " + kind);
	}

	cat_unref_ptr(fieldValues);
	cat_unref_ptr(ident);
	l_store_end((JagPJCTree *) ann, jagp_ilexer_prev_token(priv->lexer)->endPos);
	return ann;
}

/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_annotation_field_values_opt(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return (priv->token->kind == JAGP_KIND_LPAREN) ? l_annotation_field_values(parser) : cat_array_wo_new();
}

/** AnnotationFieldValues   = "(" [ AnnotationFieldValue { "," AnnotationFieldValue } ] ")" */
/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_annotation_field_values(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_annotation_field_values:token=%d", priv->token->kind);
	l_accept(parser, JAGP_KIND_LPAREN);
	CatArrayWo /*<JagPJCExpression>*/ *buf = cat_array_wo_new();
	if (priv->token->kind != JAGP_KIND_RPAREN) {
		GObject *afv = (GObject *) l_annotation_field_value(parser);
		cat_array_wo_append(buf, afv);
		cat_unref_ptr(afv);
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			GObject *afv = (GObject *) l_annotation_field_value(parser);
			cat_array_wo_append(buf, afv);
			cat_unref_ptr(afv);
		}
	}
	l_accept(parser, JAGP_KIND_RPAREN);
	return buf;
}

/** AnnotationFieldValue    = AnnotationValue
 *                          | Identifier "=" AnnotationValue
 */
/* M1 */
static JagPJCExpression *l_annotation_field_value(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_annotation_field_value:token=%d", priv->token->kind);
	if (lax_identifier_accepts(priv->token->kind)) {
		priv->mode = JAGP_MODE_EXPR;
		JagPJCExpression *t1 = l_term1(parser);
		if (jagp_jctree_has_tag((JagPJCTree *) t1, JAGP_TAG_IDENT) && priv->token->kind == JAGP_KIND_EQ) {
			int pos = priv->token->pos;
			l_accept(parser, JAGP_KIND_EQ);
			JagPJCExpression *v = l_annotation_value(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCExpression *res = (JagPJCExpression *) jagp_tree_maker_assign(priv->tree_maker, t1, v);
			cat_unref_ptr(t1);
			cat_unref_ptr(v);
			res = toP(parser, res);
			return res;
		} else {
			return t1;
		}
	}
	return l_annotation_value(parser);
}

/* AnnotationValue          = ConditionalExpression
 *                          | Annotation
 *                          | "{" [ AnnotationValue { "," AnnotationValue } ] [","] "}"
 */
/* M1 */
static JagPJCExpression *l_annotation_value(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_annotation_value:token=%d", priv->token->kind);
	int pos;
	switch (priv->token->kind) {
	case JAGP_KIND_MONKEYS_AT:
		pos = priv->token->pos;
		l_next_token(parser);
		return (JagPJCExpression *) l_annotation(parser, pos, JAGP_TAG_ANNOTATION);
	case JAGP_KIND_LBRACE:
		pos = priv->token->pos;
		l_accept(parser, JAGP_KIND_LBRACE);
		CatArrayWo /*<JagPJCExpression>*/ *buf = cat_array_wo_new();
		if (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
		} else if (priv->token->kind != JAGP_KIND_RBRACE) {
			GObject *av = (GObject *) l_annotation_value(parser);
			cat_array_wo_append(buf, av);
			cat_unref_ptr(av);
			while (priv->token->kind == JAGP_KIND_COMMA) {
				l_next_token(parser);
				if (priv->token->kind == JAGP_KIND_RBRACE) break;
				GObject *av = (GObject *) l_annotation_value(parser);
				cat_array_wo_append(buf, av);
				cat_unref_ptr(av);
			}
		}
		l_accept(parser, JAGP_KIND_RBRACE);
		jagp_tree_maker_at(priv->tree_maker, pos);
		CatArrayWo *emptylist = cat_array_wo_new();
		JagPJCNewArray *na = jagp_tree_maker_new_array(priv->tree_maker, NULL, emptylist, buf);
		cat_unref_ptr(emptylist);
		cat_unref_ptr(buf);
		na = toP(parser, na);
		return (JagPJCExpression *) na;
	default:
		priv->mode = JAGP_MODE_EXPR;
		return l_term1(parser);
	}
}

/** VariableDeclarators = VariableDeclarator { "," VariableDeclarator }
 */
/* M1 */
static CatArrayWo /*<T extends ListBuffer<? super JCVariableDecl>>*/ *l_variable_declarators(JagPParser *parser, JagPJCModifiers *mods,
		JagPJCExpression *type, CatArrayWo * vdefs) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPName *id_name = l_ident(parser, FALSE);
	CatArrayWo *result = l_variable_declarators_rest(parser, priv->token->pos, mods, type, id_name , FALSE, NULL, vdefs);
	cat_unref_ptr(id_name);
	return result;
}

/** VariableDeclaratorsRest = VariableDeclaratorRest { "," VariableDeclarator }
 *  ConstantDeclaratorsRest = ConstantDeclaratorRest { "," ConstantDeclarator }
 *
 *  @param reqInit  Is an initializer always required?
 *  @param dc       The documentation comment for the variable declarations, or NULL.
 */
static CatArrayWo /*<T extends ListBuffer<? super JCVariableDecl>>*/ *l_variable_declarators_rest(JagPParser *parser, int pos,
							JagPJCModifiers *mods, JagPJCExpression *type,JagPName *name, gboolean reqInit,
							JagPComment *dc, CatArrayWo * vdefs) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_variable_declarators_rest:token=%d", priv->token->kind);
	GObject *varDecRest = (GObject *) l_variable_declarator_rest(parser, pos, mods, type, name, reqInit, dc);
	cat_array_wo_append(vdefs, varDecRest);
	cat_unref_ptr(varDecRest);
	while (priv->token->kind == JAGP_KIND_COMMA) {
		/* All but last of multiple declarators subsume a comma */
		l_store_end((JagPJCTree *) cat_array_wo_get_last(vdefs), priv->token->endPos);
		l_next_token(parser);
		GObject *varDecRest = (GObject *) l_variable_declarator(parser, mods, type, reqInit, dc);
		cat_array_wo_append(vdefs, varDecRest);
		cat_unref_ptr(varDecRest);

	}
	return vdefs;
}

/** VariableDeclarator = Ident VariableDeclaratorRest
 *  ConstantDeclarator = Ident ConstantDeclaratorRest
 */
/* M1 */
static JagPJCVariableDecl *l_variable_declarator(JagPParser *parser, JagPJCModifiers *mods, JagPJCExpression *type, gboolean reqInit, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPName *id_name = l_ident(parser, FALSE);
	JagPJCVariableDecl *result = l_variable_declarator_rest(parser, priv->token->pos, mods, type, id_name, reqInit, dc);
	cat_unref_ptr(id_name);
	return result;
}

/** VariableDeclaratorRest = BracketsOpt ["=" VariableInitializer]
 *  ConstantDeclaratorRest = BracketsOpt "=" VariableInitializer
 *
 *  @param reqInit  Is an initializer always required?
 *  @param dc       The documentation comment for the variable declarations, or NULL.
 */
/* M1 */
static JagPJCVariableDecl *l_variable_declarator_rest(JagPParser *parser, int pos, JagPJCModifiers *mods, JagPJCExpression *type, JagPName *name,
							  gboolean reqInit, JagPComment *dc ) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCExpression *brackstype = l_brackets_opt_simple(parser, type);
	JagPJCExpression *init = NULL;
	if (priv->token->kind == JAGP_KIND_EQ) {
		l_next_token(parser);
		init = l_variable_initializer(parser);
	}
//	else if (reqInit) syntaxError(priv->token->pos, "expected", JAGP_KIND_EQ);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCVariableDecl *result = jagp_tree_maker_var_def(priv->tree_maker, mods, name, brackstype, init);
	cat_unref_ptr(brackstype);
	cat_unref_ptr(init);
	result = toP(parser, result);
	l_attach(parser, (JagPJCTree *) result, dc);
	return result;
}

/** VariableDeclaratorId = Ident BracketsOpt
 */
/* where */
/* M1 */
static JagPJCVariableDecl *l_variable_declarator_id(JagPParser *parser, JagPJCModifiers *mods, JagPJCExpression *type, gboolean lambdaParameter) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_variable_declarator_id:token=%d, lambdaParameter=%d", priv->token->kind, lambdaParameter);
	int pos = priv->token->pos;
	JagPName *name;
	if (lambdaParameter && (priv->token->kind == JAGP_KIND_UNDERSCORE)) {
//		log.error(pos, "underscore.as.identifier.in.lambda");
//		name = priv->token->name();
		l_next_token(parser);
	} else {
		if (priv->allowThisIdent && !lambdaParameter) {
			JagPJCExpression *pn = l_qualident(parser, FALSE);
			if (jagp_jctree_has_tag((JagPJCTree *) pn, JAGP_TAG_IDENT) && ((JagPJCIdent *)pn)->name != priv->names_this) {
				name = cat_ref_ptr(((JagPJCIdent *)pn)->name);
			} else {
				if ((mods->flags & JAGP_FLAG_VARARGS) != 0) {
//					log.error(priv->token->pos, "varargs.and.receiver");
				}
				if (priv->token->kind == JAGP_KIND_LBRACKET) {
//					log.error(priv->token->pos, "array.and.receiver");
				}
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCVariableDecl *recvvardecl = jagp_tree_maker_receiver_var_def(priv->tree_maker, mods, pn, type);
				recvvardecl = toP(parser, recvvardecl);
				cat_unref_ptr(pn);
				return recvvardecl;
			}
			cat_unref_ptr(pn);
		} else {
			name = l_ident(parser, FALSE);
		}
	}
	cat_log_debug("l_variable_declarator_id:name=%O", name);
	if ((mods->flags & JAGP_FLAG_VARARGS) != 0 &&
			priv->token->kind == JAGP_KIND_LBRACKET) {
//		log.error(priv->token->pos, "varargs.and.old.array.syntax");
	}
	JagPJCExpression *opt_smpl = type = l_brackets_opt_simple(parser, type);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCVariableDecl *result = jagp_tree_maker_var_def(priv->tree_maker, mods, name, opt_smpl, NULL);
	cat_unref_ptr(name);
// M1	cat_unref_ptr(opt_smpl);
	result = toP(parser, result);
	cat_log_debug("l_variable_declarator_id:done:result=%O", result);
	cat_log_indent_level--;
	return result;
}

/** Resources = Resource { ";" Resources }
 */
static CatArrayWo */*<JCTree>*/ l_resources(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo /*<JagPJCTree>*/ *defs = cat_array_wo_new();
	GObject *res = (GObject *) l_resource(parser);
	cat_array_wo_append(defs, res);
	cat_unref_ptr(res);
	while (priv->token->kind == JAGP_KIND_SEMI) {
		/* All but last of multiple declarators must subsume a semicolon */
		l_store_end((JagPJCTree *) cat_array_wo_get_last(defs), priv->token->endPos);
//		int semiColonPos = priv->token->pos;
		l_next_token(parser);
		if (priv->token->kind == JAGP_KIND_RPAREN) { /* Optional trailing semicolon after last resource */
			break;
		}
		GObject *res = (GObject *) l_resource(parser);
		cat_array_wo_append(defs, res);
		cat_unref_ptr(res);
	}
	return defs;
}

/** Resource = VariableModifiersOpt Type VariableDeclaratorId "=" Expression
 *           | Expression
 */
/* M1 */
static JagPJCTree *l_resource(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int startPos = priv->token->pos;
	if (priv->token->kind == JAGP_KIND_FINAL || priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		JagPJCModifiers *mods = l_opt_final(parser, JAGP_FLAG_FINAL);
		JagPJCExpression *t = l_parse_type(parser);
		JagPName *id_name = l_ident(parser, FALSE);
		JagPJCTree *result = (JagPJCTree *) l_variable_declarator_rest(parser, priv->token->pos, mods, t, id_name, TRUE, NULL);
		cat_unref_ptr(mods);
		cat_unref_ptr(t);
		cat_unref_ptr(id_name);
		return result;
	}
	JagPJCExpression *t = l_term(parser, JAGP_MODE_EXPR | JAGP_MODE_TYPE);
	if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && lax_identifier_accepts(priv->token->kind)) {
		jagp_tree_maker_at(priv->tree_maker, startPos);
		JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, JAGP_FLAG_FINAL, NULL);
		JagPName *id_name = l_ident(parser, FALSE);
		JagPJCTree *result = (JagPJCTree *) l_variable_declarator_rest(parser, priv->token->pos, mods, t, id_name, TRUE, NULL);
		cat_unref_ptr(mods);
		cat_unref_ptr(id_name);
		return result;
	} else {
		l_check_variable_in_try_with_resources(parser, startPos);
//		if (!jagp_jctree_has_tag(t, JAGP_TAG_IDENT) && !jagp_jctree_has_tag(t, JAGP_TAG_SELECT)) {
//			log.error(t.pos(), "try.with.resources.expr.needs.var");
//		}

		return (JagPJCTree *) t;
	}
}

/** CompilationUnit = [ { "@" Annotation } PACKAGE Qualident ";"] {ImportDeclaration} {TypeDeclaration}
 */
/* M1 */
static JagPJCCompilationUnit *l_parse_compilation_unit(JagPParser *parser) {
	cat_log_debug("l_parse_compilation_unit");
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPToken *firstToken = priv->token;
	JagPJCModifiers *mods = NULL;
	gboolean consumedToplevelDoc = FALSE;
	CatArrayWo /*<JagPJCTree>*/ *defs = cat_array_wo_new();

	if (priv->token->kind == JAGP_KIND_IDENTIFIER && jagp_token_name(priv->token) == priv->names_module) {
//		cat_array_wo_append(defs, moduleDecl(jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC)));
		consumedToplevelDoc = TRUE;
	} else {
		gboolean seenImport = FALSE;
		gboolean seenPackage = FALSE;
		if (priv->token->kind == JAGP_KIND_MONKEYS_AT) {
			mods = l_modifiers_opt(parser, NULL);
		}

		if (priv->token->kind == JAGP_KIND_PACKAGE) {
			int packagePos = priv->token->pos;
			CatArrayWo */*<JagPJCAnnotation>*/ annotations;
			seenPackage = TRUE;
			if (mods != NULL) {
				l_check_no_mods(parser, mods->flags);
				annotations = cat_ref_ptr(mods->annotations);
				cat_unref_ptr(mods);
			} else {
				annotations = cat_array_wo_new();
			}
			l_next_token(parser);
			JagPJCExpression *pid = l_qualident(parser, FALSE);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, packagePos);
			JagPJCPackageDecl *pd = jagp_tree_maker_package_decl(priv->tree_maker, annotations, pid);
			cat_unref_ptr(pid);
			cat_unref_ptr(annotations);
			JagPComment *docComment = jagp_token_comment(firstToken, JAGP_COMMENT_STYLE_JAVADOC);
			l_attach(parser, (JagPJCTree *) pd, docComment);
			cat_unref_ptr(docComment);
			consumedToplevelDoc = TRUE;
			l_store_end((JagPJCTree *) pd, priv->token->pos);
			cat_array_wo_append(defs, (GObject *) pd);
			cat_unref_ptr(pd);
		}
		gboolean checkForImports = TRUE;
		gboolean firstTypeDecl = TRUE;
		while (priv->token->kind != JAGP_KIND_EOF) {
			cat_log_debug("priv->token->kind=%d, eof=%d", priv->token->kind, JAGP_KIND_EOF);

//			if (priv->token->pos <= endPosTable.errorEndPos) {
//				// error recovery
//				skip(checkForImports, FALSE, FALSE, FALSE);
//				if (priv->token->kind == EOF)
//					break;
//			}
			if (checkForImports && mods == NULL && priv->token->kind == JAGP_KIND_IMPORT) {
				seenImport = TRUE;
				GObject *imde = (GObject *) l_import_declaration(parser);
				cat_array_wo_append(defs, imde);
				cat_unref_ptr(imde);
			} else {
				JagPComment *docComment = NULL;
				if (firstTypeDecl && !seenImport && !seenPackage) {
					docComment = jagp_token_comment(firstToken, JAGP_COMMENT_STYLE_JAVADOC);
					consumedToplevelDoc = TRUE;
				} else {
					docComment = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
				}
				JagPJCTree *def = l_type_declaration(parser, mods, docComment);
				cat_unref_ptr(docComment);
				if (JAGP_IS_JCEXPRESSION_STATEMENT(def)) {
					JagPJCTree *sub_def = (JagPJCTree *) ((JagPJCExpressionStatement *)def)->expr;
					cat_ref_ptr(sub_def);
					cat_unref_ptr(def);
					def = sub_def;
				}
				cat_array_wo_append(defs, (GObject *) def);
				if (JAGP_IS_JCCLASS_DECL(def)) {
					checkForImports = FALSE;
				}
				cat_unref_ptr(mods);
				firstTypeDecl = FALSE;
				cat_unref_ptr(def);
			}
		}
	}
	cat_unref_ptr(mods);

	jagp_tree_maker_at(priv->tree_maker, firstToken->pos);
	JagPJCCompilationUnit *toplevel = jagp_tree_maker_top_level(priv->tree_maker, defs);
	if (!consumedToplevelDoc)
		l_attach(parser, (JagPJCTree *) toplevel, jagp_token_comment(firstToken, JAGP_COMMENT_STYLE_JAVADOC));
//	if (cat_array_wo_size(defs)==0);
		l_store_end((JagPJCTree *) toplevel, jagp_ilexer_prev_token(priv->lexer)->endPos);
//	if (priv->keepDocComments)
//		toplevel.docComments = docComments;
//	if (priv->keepLineMap)
//		toplevel.lineMap = S.getLineMap();
//	this.endPosTable.setParser(NULL); // remove reference to parser
//	toplevel.endPositions = this.endPosTable;
	cat_unref_ptr(defs);
	return toplevel;
}

//static JagPJCModuleDecl *moduleDecl(JagPParser *parser, JagPComment *dc) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	int pos = priv->token->pos;
//	if (!priv->allowModules) {
//		log.error(pos, Errors.ModulesNotSupportedInSource(source.name));
//		priv->allowModules = TRUE;
//	}
//
//	l_next_token(parser);
//	JagPJCExpression *name = l_qualident(parser, FALSE);
//	CatArrayWo /*<JCDirective>*/ *directives = NULL;
//
//	l_accept(parser, JAGP_KIND_LBRACE);
//	directives = l_module_directive_list(parser);
//	l_accept(parser, JAGP_KIND_RBRACE);
//	l_accept(parser, JAGP_KIND_EOF);
//
//	jagp_tree_maker_at(priv->tree_maker, pos);
//	JagPJCModuleDecl *result = toP(parser, F.ModuleDef(name, directives));
//	l_attach(parser, result, dc);
//	return result;
//}


static CatArrayWo /*<JagPJCDirective>*/ *l_module_directive_list(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo /*<JagPJCDirective>*/ *defs = cat_array_wo_new();
//	while (priv->token->kind == JAGP_KIND_IDENTIFIER) {
//		int pos = priv->token->pos;
//		if (jagp_token_name(priv->token) == priv->names_requires) {
//			l_next_token(parser);
//			gboolean isPublic = FALSE;
//			if (priv->token->kind == JAGP_KIND_PUBLIC) {
//				isPublic = TRUE;
//				l_next_token(parser);
//			}
//			JagPJCExpression *moduleName = l_qualident(parser, FALSE);
//			l_accept(parser, JAGP_KIND_SEMI);
//			cat_array_wo_append(defs, toP(parser, F.at(pos).Requires(isPublic, moduleName)));
//		} else if (jagp_token_name(priv->token) == priv->names_exports) {
//			l_next_token(parser);
//			JagPJCExpression *pkgName = l_qualident(parser, FALSE);
//			CatArrayWo */*<JagPJCExpression>*/ moduleNames = NULL;
//			if (priv->token->kind == JAGP_KIND_IDENTIFIER && jagp_token_name(priv->token) == priv->names_to) {
//				l_next_token(parser);
//				moduleNames = l_qualident_list(parser);
//			}
//			l_accept(parser, JAGP_KIND_SEMI);
//			cat_array_wo_append(defs, toP(parser, F.at(pos).Exports(pkgName, moduleNames)));
//		} else if (jagp_token_name(priv->token) == priv->names_provides) {
//			l_next_token(parser);
//			JagPJCExpression *serviceName = l_qualident(parser, FALSE);
//			if (priv->token->kind == JAGP_KIND_IDENTIFIER && jagp_token_name(priv->token) == priv->names_with) {
//				l_next_token(parser);
//				JagPJCExpression *implName = l_qualident(parser, FALSE);
//				l_accept(parser, JAGP_KIND_SEMI);
//				cat_array_wo_append(defs, toP(parser, F.at(pos).Provides(serviceName, implName)));
//			} else {
//				error(priv->token->pos, "expected", "'" + priv->names_with + "'");
//				skip(FALSE, FALSE, FALSE, FALSE);
//			}
//		} else if (jagp_token_name(priv->token) == priv->names_uses) {
//			l_next_token(parser);
//			JagPJCExpression *service = l_qualident(parser, FALSE);
//			l_accept(parser, JAGP_KIND_SEMI);
//			jagp_tree_maker_at(priv->tree_maker, pos);
//			GObject *uses = toP(parser, F.Uses(service));
//			cat_array_wo_append(defs, uses);
//		} else {
//			break;
//		}
//	}
	return defs;
}

/** ImportDeclaration = IMPORT [ STATIC ] Ident { "." Ident } [ "." "*" ] ";"
 */
/* M1 */
static JagPJCTree *l_import_declaration(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_next_token(parser);
	gboolean importStatic = FALSE;
	if (priv->token->kind == JAGP_KIND_STATIC) {
		importStatic = TRUE;
		l_next_token(parser);
	}
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPName *i_name = l_ident(parser, FALSE);
	JagPJCExpression *pid = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, i_name);
	cat_unref_ptr(i_name);
	pid = toP(parser, pid);
	do {
		int pos1 = priv->token->pos;
		l_accept(parser, JAGP_KIND_DOT);
		if (priv->token->kind == JAGP_KIND_STAR) {
			jagp_tree_maker_at(priv->tree_maker, pos1);
			JagPJCExpression *field = pid;
			pid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, field, priv->names_asterisk);
			cat_unref_ptr(field);
			pid = to(parser, pid);
			l_next_token(parser);
			break;
		} else {
			jagp_tree_maker_at(priv->tree_maker, pos1);
			i_name = l_ident(parser, FALSE);
			JagPJCExpression *field = pid;
			pid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, field, i_name);
			cat_unref_ptr(field);
			cat_unref_ptr(i_name);
			pid = to(parser, pid);
		}
	} while (priv->token->kind == JAGP_KIND_DOT);
	l_accept(parser, JAGP_KIND_SEMI);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTree *result = (JagPJCTree *) jagp_tree_maker_import(priv->tree_maker, (JagPJCTree *) pid, importStatic);
	cat_unref_ptr(pid);
	result = toP(parser, result);
	return result;
}

/** TypeDeclaration = ClassOrInterfaceOrEnumDeclaration
 *                  | ";"
 */
/* M1 */
static JagPJCTree *l_type_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *docComment) {
	cat_log_debug("l_type_declaration");
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	JagPJCTree *result = NULL;
	if (mods == NULL && priv->token->kind == JAGP_KIND_SEMI) {
		l_next_token(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCTree *) jagp_tree_maker_skip(priv->tree_maker);
		result = toP(parser, result);
	} else {
		JagPJCModifiers *mods_sub = l_modifiers_opt(parser, mods);
		result = (JagPJCTree *) l_class_or_interface_or_enum_declaration(parser, mods_sub, docComment);
		cat_unref_ptr(mods_sub);
	}
	return result;
}

/** ClassOrInterfaceOrEnumDeclaration = ModifiersOpt
 *           (ClassDeclaration | InterfaceDeclaration | EnumDeclaration)
 *  @param mods     Any modifiers starting the class or interface declaration
 *  @param dc       The documentation comment for the class, or NULL.
 */
/* M1 */
static JagPJCStatement *l_class_or_interface_or_enum_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc) {
	cat_log_debug("l_class_or_interface_or_enum_declaration");
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (priv->token->kind == JAGP_KIND_CLASS) {
		return (JagPJCStatement *) l_class_declaration(parser, mods, dc);
	} else if (priv->token->kind == JAGP_KIND_INTERFACE) {
		return (JagPJCStatement *) l_interface_declaration(parser, mods, dc);
	} else if (priv->token->kind == JAGP_KIND_ENUM) {
		return (JagPJCStatement *) l_enum_declaration(parser, mods, dc);
	} else {
		int pos = priv->token->pos;
		CatArrayWo */*<JCTree>*/ errs = cat_array_wo_new();
		if (lax_identifier_accepts(priv->token->kind)) {
			cat_array_wo_append(errs, (GObject *) mods);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPName *id = l_ident(parser, FALSE);
			JagPJCIdent *ident = jagp_tree_maker_ident(priv->tree_maker, id);
			cat_unref_ptr(id);
			ident = toP(parser, ident);
			cat_array_wo_append(errs, (GObject *) ident);
			cat_unref_ptr(ident);
			l_set_error_end_pos(parser, priv->token->pos);
		} else {
			cat_array_wo_append(errs, (GObject *) mods);
		}
		JagPJCExpression *err = NULL;
//		JagPJCExpression *err = syntaxError(pos, errs, "expected3",JAGP_KIND_CLASS, JAGP_KIND_INTERFACE, JAGP_KIND_ENUM);
		cat_unref_ptr(errs);
		JagPJCExpressionStatement *result = jagp_tree_maker_exec(priv->tree_maker, err);
		result = toP(parser, result);
		return (JagPJCStatement *) result;
	}
}

/** ClassDeclaration = CLASS Ident TypeParametersOpt [EXTENDS Type]
 *                     [IMPLEMENTS TypeList] ClassBody
 *  @param mods    The modifiers starting the class declaration
 *  @param dc       The documentation comment for the class, or NULL.
 */
/* M1 */
static JagPJCClassDecl *l_class_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc) {
	cat_log_debug("l_class_declaration");
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_CLASS);
	JagPName *name = l_ident(parser, FALSE);

	CatArrayWo /*<JagPJCTypeParameter>*/ * typarams = l_type_parameters_opt(parser);

	JagPJCExpression *extending = NULL;
	if (priv->token->kind == JAGP_KIND_EXTENDS) {
		l_next_token(parser);
		extending = l_parse_type(parser);
	}
	CatArrayWo */*<JagPJCExpression>*/ implementing = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_IMPLEMENTS) {
		l_next_token(parser);
		implementing = l_type_list(parser);
	}
	CatArrayWo */*<JCTree>*/ defs = l_class_or_interface_body(parser, name, FALSE);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCClassDecl *result = jagp_tree_maker_class_def(priv->tree_maker, mods, name, typarams, extending, implementing, defs);
	cat_unref_ptr(name);
	cat_unref_ptr(typarams);
	cat_unref_ptr(extending);
	cat_unref_ptr(implementing);
	cat_unref_ptr(defs);
	result = toP(parser, result);
	l_attach(parser, (JagPJCTree *) result, dc);
	return result;
}

/** InterfaceDeclaration = INTERFACE Ident TypeParametersOpt
 *                         [EXTENDS TypeList] InterfaceBody
 *  @param mods    The modifiers starting the interface declaration
 *  @param dc       The documentation comment for the interface, or NULL.
 */
/* M1 */
static JagPJCClassDecl *l_interface_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_INTERFACE);
	JagPName *name = l_ident(parser, FALSE);

	CatArrayWo /*<JagPJCTypeParameter>*/ * typarams = l_type_parameters_opt(parser);

	CatArrayWo */*<JagPJCExpression>*/ extending = NULL;
	if (priv->token->kind == JAGP_KIND_EXTENDS) {
		l_next_token(parser);
		extending = l_type_list(parser);
	} else {
		extending = cat_array_wo_new();
	}
	CatArrayWo */*<JCTree>*/ defs = l_class_or_interface_body(parser, name, TRUE);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCClassDecl *result = jagp_tree_maker_class_def(priv->tree_maker, mods, name, typarams,NULL,extending, defs);
	result = toP(parser, result);
	l_attach(parser, (JagPJCTree *) result, dc);
	cat_unref_ptr(name);
	cat_unref_ptr(typarams);
	cat_unref_ptr(extending);
	cat_unref_ptr(defs);
	return result;
}

/** EnumDeclaration = ENUM Ident [IMPLEMENTS TypeList] EnumBody
 *  @param mods    The modifiers starting the enum declaration
 *  @param dc       The documentation comment for the enum, or NULL.
 */
/* M1 */
static JagPJCClassDecl *l_enum_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_ENUM);
	JagPName *name = l_ident(parser, FALSE);

	CatArrayWo */*<JagPJCExpression>*/ implementing = NULL;
	if (priv->token->kind == JAGP_KIND_IMPLEMENTS) {
		l_next_token(parser);
		implementing = l_type_list(parser);
	} else {
		implementing = cat_array_wo_new();
	}

	CatArrayWo */*<JCTree>*/ defs = l_enum_body(parser, name);
	mods->flags |= JAGP_FLAG_ENUM;
	jagp_tree_maker_at(priv->tree_maker, pos);
	CatArrayWo *emptylist = cat_array_wo_new();
	JagPJCClassDecl *result = jagp_tree_maker_class_def(priv->tree_maker, mods, name, emptylist, NULL, implementing, defs);
	cat_unref_ptr(emptylist);
	result = toP(parser, result);
	l_attach(parser, (JagPJCTree *) result, dc);
	cat_unref_ptr(name);
	cat_unref_ptr(implementing);
	cat_unref_ptr(defs);
	return result;
}

/** EnumBody = "{" { EnumeratorDeclarationList } [","]
 *                  [ ";" {ClassBodyDeclaration} ] "}"
 */
/* M1 */
static CatArrayWo */*<JCTree>*/ l_enum_body(JagPParser *parser, JagPName *enumName) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_LBRACE);
	CatArrayWo /*<JagPJCTree>*/ *defs = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);
	} else if (priv->token->kind != JAGP_KIND_RBRACE && priv->token->kind != JAGP_KIND_SEMI) {
		GObject *enumdecl = (GObject *) l_enumerator_declaration(parser, enumName);
		cat_array_wo_append(defs, enumdecl);
		cat_unref_ptr(enumdecl);
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			if (priv->token->kind == JAGP_KIND_RBRACE || priv->token->kind == JAGP_KIND_SEMI) {
				break;
			}
			GObject *enumdecl = (GObject *) l_enumerator_declaration(parser, enumName);
			cat_array_wo_append(defs, enumdecl);
			cat_unref_ptr(enumdecl);
		}
		if (priv->token->kind != JAGP_KIND_SEMI && priv->token->kind != JAGP_KIND_RBRACE) {
//			GObject *err = syntaxError(priv->token->pos, "expected3", JAGP_KIND_COMMA, JAGP_KIND_RBRACE, JAGP_KIND_SEMI)
//			cat_array_wo_append(defs, err);
			l_next_token(parser);
		}
	}
	if (priv->token->kind == JAGP_KIND_SEMI) {
		l_next_token(parser);
		while (priv->token->kind != JAGP_KIND_RBRACE && priv->token->kind != JAGP_KIND_EOF) {
			GObject *clorinbode = (GObject *) l_class_or_interface_body_declaration(parser, enumName,FALSE);
			cat_array_wo_append(defs, clorinbode);
			cat_unref_ptr(clorinbode);
//			if (priv->token->pos <= endPosTable.errorEndPos) {
//				// error recovery
//			   skip(FALSE, TRUE, TRUE, FALSE);
//			}
		}
	}
	l_accept(parser, JAGP_KIND_RBRACE);
	return defs;
}

/** EnumeratorDeclaration = AnnotationsOpt [TypeArguments] IDENTIFIER [ Arguments ] [ "{" ClassBody "}" ]
 */
/* M1 */
static JagPJCTree *l_enumerator_declaration(JagPParser *parser, JagPName *enumName) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
	int flags = JAGP_FLAG_PUBLIC|JAGP_FLAG_STATIC|JAGP_FLAG_FINAL|JAGP_FLAG_ENUM;
//	if (token.deprecatedFlag()) {
//		flags |= JAGP_FLAG_DEPRECATED;
//	}
	int pos = priv->token->pos;
	CatArrayWo */*<JagPJCAnnotation>*/ annotations = l_annotations_opt(parser, JAGP_TAG_ANNOTATION);
	jagp_tree_maker_at(priv->tree_maker, cat_array_wo_size(annotations)==0 ? -1 : pos);
	JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, flags, annotations);
	cat_unref_ptr(annotations);
	CatArrayWo /*<JagPJCExpression>*/ *typeArgs = l_type_arguments_opt(parser, JAGP_MODE_TYPE);
	int identPos = priv->token->pos;
	JagPName *name = l_ident(parser, FALSE);
	int createPos = priv->token->pos;
	CatArrayWo */*<JagPJCExpression>*/ args = (priv->token->kind == JAGP_KIND_LPAREN)
		? l_arguments(parser) : cat_array_wo_new();
	JagPJCClassDecl *body = NULL;
	if (priv->token->kind == JAGP_KIND_LBRACE) {
		jagp_tree_maker_at(priv->tree_maker, -1);
		JagPJCModifiers *mods1 = jagp_tree_maker_modifiers(priv->tree_maker, JAGP_FLAG_ENUM, NULL);
		CatArrayWo */*<JCTree>*/ defs = l_class_or_interface_body(parser, priv->names_empty, FALSE);
		jagp_tree_maker_at(priv->tree_maker, identPos);
		body = jagp_tree_maker_anonymous_class_def(priv->tree_maker, mods1, priv->names_empty, defs);
		body = toP(parser, body);
		cat_unref_ptr(mods1);
	}
	if (cat_array_wo_size(args)==0 && body == NULL) {
		createPos = identPos;
	}
	jagp_tree_maker_at(priv->tree_maker, identPos);
	JagPJCIdent *ident = jagp_tree_maker_ident(priv->tree_maker, enumName);
	jagp_tree_maker_at(priv->tree_maker, createPos);
	JagPJCNewClass *create = jagp_tree_maker_new_class(priv->tree_maker, NULL, typeArgs, (JagPJCExpression *) ident, args, body);
	cat_unref_ptr(body);
	cat_unref_ptr(ident);
	if (createPos != identPos) {
		l_store_end(create, jagp_ilexer_prev_token(priv->lexer)->endPos);
	}
	cat_unref_ptr(args);
	jagp_tree_maker_at(priv->tree_maker, identPos);
	ident = jagp_tree_maker_ident(priv->tree_maker, enumName);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTree *result = (JagPJCTree *) jagp_tree_maker_var_def(priv->tree_maker, mods, name, (JagPJCExpression *) ident, (JagPJCExpression *) create);
	result = toP(parser, result);
	l_attach(parser, result, dc);
	cat_unref_ptr(ident);
	cat_unref_ptr(dc);
	cat_unref_ptr(create);
	cat_unref_ptr(mods);
	cat_unref_ptr(name);
	cat_unref_ptr(typeArgs);
	return result;
}

/** TypeList = Type {"," Type}
 */
/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_type_list(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo /*<JagPJCExpression>*/ *ts = cat_array_wo_new();
	GObject *pt = (GObject *) l_parse_type(parser);
	cat_array_wo_append(ts, pt);
	cat_unref_ptr(pt);
	while (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);
		GObject *pt = (GObject *) l_parse_type(parser);
		cat_array_wo_append(ts, pt);
		cat_unref_ptr(pt);
	}
	return ts;
}

/** ClassBody     = "{" {ClassBodyDeclaration} "}"
 *  InterfaceBody = "{" {InterfaceBodyDeclaration} "}"
 */
static CatArrayWo */*<JCTree>*/ l_class_or_interface_body(JagPParser *parser, JagPName *className, gboolean isInterface) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_class_or_interface_body:token=%d", priv->token->kind);
	l_accept(parser, JAGP_KIND_LBRACE);
//	if (priv->token->pos <= endPosTable.errorEndPos) {
//		// error recovery
//		skip(FALSE, TRUE, FALSE, FALSE);
//		if (priv->token->kind == JAGP_KIND_LBRACE)
//			l_next_token(parser);
//	}
	CatArrayWo /*<JagPJCTree>*/ *defs = cat_array_wo_new();
	while (priv->token->kind != JAGP_KIND_RBRACE && priv->token->kind != JAGP_KIND_EOF) {
		GObject *clorinbode = (GObject *) l_class_or_interface_body_declaration(parser, className, isInterface);
		cat_array_wo_append(defs, clorinbode);
		cat_unref_ptr(clorinbode);
//		if (priv->token->pos <= endPosTable.errorEndPos) {
//		   // error recovery
//		   skip(FALSE, TRUE, TRUE, FALSE);
//	   }
	}
	l_accept(parser, JAGP_KIND_RBRACE);
	return defs;
}

/** ClassBodyDeclaration =
 *      ";"
 *    | [STATIC] Block
 *    | ModifiersOpt
 *      ( Type Ident
 *        ( VariableDeclaratorsRest ";" | MethodDeclaratorRest )
 *      | VOID Ident VoidMethodDeclaratorRest
 *      | TypeParameters [Annotations]
 *        ( Type Ident MethodDeclaratorRest
 *        | VOID Ident VoidMethodDeclaratorRest
 *        )
 *      | Ident ConstructorDeclaratorRest
 *      | TypeParameters Ident ConstructorDeclaratorRest
 *      | ClassOrInterfaceOrEnumDeclaration
 *      )
 *  InterfaceBodyDeclaration =
 *      ";"
 *    | ModifiersOpt
 *      ( Type Ident
 *        ( ConstantDeclaratorsRest ";" | MethodDeclaratorRest )
 *      | VOID Ident MethodDeclaratorRest
 *      | TypeParameters [Annotations]
 *        ( Type Ident MethodDeclaratorRest
 *        | VOID Ident VoidMethodDeclaratorRest
 *        )
 *      | ClassOrInterfaceOrEnumDeclaration
 *      )
 *
 */
/* M1 */
static CatArrayWo /*<JCTree>*/ *l_class_or_interface_body_declaration(JagPParser *parser, JagPName *className, gboolean isInterface) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_class_or_interface_body_declaration:token=%d", priv->token->kind);
	CatArrayWo *result = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_SEMI) {
		l_next_token(parser);
	} else {
		JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
		int pos = priv->token->pos;
		JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
		if (priv->token->kind == JAGP_KIND_CLASS ||
				priv->token->kind == JAGP_KIND_INTERFACE ||
				priv->token->kind == JAGP_KIND_ENUM) {
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
		} else if (priv->token->kind == JAGP_KIND_LBRACE &&
				   (mods->flags & JAGP_FLAG_StandardFlags & ~JAGP_FLAG_STATIC) == 0 &&
				   cat_array_wo_size(mods->annotations)==0) {
//			if (isInterface) {
//				error(priv->token->pos, "initializer.not.allowed");
//			}
			GObject *block = (GObject *) l_block_ext(parser, pos, mods->flags);
			cat_array_wo_append(result, block);
			cat_unref_ptr(block);
		} else {
			pos = priv->token->pos;
			CatArrayWo /*<JagPJCTypeParameter>*/ * typarams = l_type_parameters_opt(parser);
			/* if there are type parameters but no modifiers, save the start
			 * position of the method in the modifiers. */
			if (cat_array_wo_size(typarams)>0 && mods->parent.pos == -1) {
				mods->parent.pos = pos;
				l_store_end(mods, pos);
			}
			CatArrayWo */*<JagPJCAnnotation>*/ annosAfterParams = l_annotations_opt(parser, JAGP_TAG_ANNOTATION);

			if (cat_array_wo_size(annosAfterParams)>0) {
				l_check_annotations_after_type_params(parser, l_list_head_pos(annosAfterParams));
				cat_array_wo_append_all(mods->annotations, annosAfterParams);
				if (mods->parent.pos == -1) {
					mods->parent.pos = l_list_head_pos(mods->annotations);
				}
			}

			JagPToken *tk = priv->token;
			pos = priv->token->pos;
			JagPJCExpression *type;
			gboolean isVoid = priv->token->kind == JAGP_KIND_VOID;
			if (isVoid) {
				jagp_tree_maker_at(priv->tree_maker, pos);
				type = (JagPJCExpression *) jagp_tree_maker_type_ident(priv->tree_maker, JAGP_TT_VOID);
				type = to(parser, type);
				l_next_token(parser);
			} else {
				/* method returns types are un-annotated types */
				type = l_unannotated_type(parser);
			}

			cat_log_debug("type=%O", type);

			if (priv->token->kind == JAGP_KIND_LPAREN && !isInterface && jagp_jctree_has_tag((JagPJCTree *) type, JAGP_TAG_IDENT)) {
//				if (isInterface || jagp_token_name(tk) != className) {
//					error(pos, "invalid.meth.decl.ret.type.req");
//				} else if (cat_array_wo_size(annosAfterParams)>0) {
//					illegal(l_list_head_pos(annosAfterParams));
//				}
				GObject *mdr = (GObject *) l_method_declarator_rest(parser, pos, mods, NULL, priv->names_init, typarams, isInterface, TRUE, dc);
				cat_array_wo_append(result, mdr);
				cat_unref_ptr(mdr);
			} else {
				pos = priv->token->pos;
				JagPName *name = l_ident(parser, FALSE);
				if (priv->token->kind == JAGP_KIND_LPAREN) {
					GObject *mdr = (GObject *) l_method_declarator_rest(parser, pos, mods, type, name, typarams, isInterface, isVoid, dc);
					cat_array_wo_append(result, mdr);
					cat_unref_ptr(mdr);
				} else if (!isVoid && cat_array_wo_size(typarams)==0) {
					l_variable_declarators_rest(parser, pos, mods, type, name, isInterface, dc, result);
					l_accept(parser, JAGP_KIND_SEMI);
					l_store_end((JagPJCTree *) cat_array_wo_get_last(result), jagp_ilexer_prev_token(priv->lexer)->endPos);
				} else {
					pos = priv->token->pos;
					CatArrayWo */*<JCTree>*/ err = NULL;
					if (isVoid) {
						err = cat_array_wo_new();
						jagp_tree_maker_at(priv->tree_maker, pos);
						GObject *ee = (GObject *) jagp_tree_maker_method_def1(priv->tree_maker, mods, name, type, typarams, cat_array_wo_new(), cat_array_wo_new(), NULL, NULL);
						ee = toP(parser, ee);
						cat_array_wo_append(err, ee);
						cat_unref_ptr(ee);
					}
//					cat_array_wo_append(result, syntaxError(priv->token->pos, err, "expected", JAGP_KIND_LPAREN));
					cat_unref_ptr(err);
				}
				cat_unref_ptr(name);
			}
			cat_unref_ptr(type);
			cat_unref_ptr(annosAfterParams);
			cat_unref_ptr(typarams);
		}
		cat_unref_ptr(dc);
		cat_unref_ptr(mods);
	}
	return result;
}

/** MethodDeclaratorRest =
 *      FormalParameters BracketsOpt [THROWS TypeList] ( MethodBody | [DEFAULT AnnotationValue] ";")
 *  VoidMethodDeclaratorRest =
 *      FormalParameters [THROWS TypeList] ( MethodBody | ";")
 *  ConstructorDeclaratorRest =
 *      "(" FormalParameterListOpt ")" [THROWS TypeList] MethodBody
 */
/* M1 */
static JagPJCTree *l_method_declarator_rest(JagPParser *parser, int pos, JagPJCModifiers *mods, JagPJCExpression *type, JagPName *name,
			CatArrayWo /*<JagPJCTypeParameter>*/ * typarams, gboolean isInterface, gboolean isVoid, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (isInterface) {
		if ((mods->flags & JAGP_FLAG_STATIC) != 0) {
			l_check_static_interface_methods(parser);
		}
		if ((mods->flags & JAGP_FLAG_PRIVATE) != 0) {
			l_check_private_interface_methods(parser);
		}
	}
	JagPJCVariableDecl *prevReceiverParam = priv->receiverParam;
	priv->receiverParam = NULL;
	/* Parsing formalParameters sets the receiverParam, if present */
	CatArrayWo */*JagPJCVariableDecl>*/ params = l_formal_parameters(parser, FALSE);
	if (!isVoid) {
		type = l_brackets_opt_simple(parser, type);
	} else {
		cat_ref_ptr(type);
	}
	CatArrayWo */*<JagPJCExpression>*/ thrown = NULL;
	if (priv->token->kind == JAGP_KIND_THROWS) {
		l_next_token(parser);
		thrown = l_qualident_list(parser);
	} else {
		thrown = cat_array_wo_new();
	}
	JagPJCBlock *body = NULL;
	JagPJCExpression *defaultValue = NULL;
	if (priv->token->kind == JAGP_KIND_LBRACE) {
		body = l_block(parser);
	} else {
		if (priv->token->kind == JAGP_KIND_DEFAULT) {
			l_accept(parser, JAGP_KIND_DEFAULT);
			defaultValue = l_annotation_value(parser);
		}
		l_accept(parser, JAGP_KIND_SEMI);
//		if (priv->token->pos <= endPosTable.errorEndPos) {
//			// error recovery
//			skip(FALSE, TRUE, FALSE, FALSE);
//			if (priv->token->kind == JAGP_KIND_LBRACE) {
//				body = l_block(parser);
//			}
//		}
	}

	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCMethodDecl *result = jagp_tree_maker_method_def2(priv->tree_maker, mods, name, type, typarams, priv->receiverParam, params, thrown, body, defaultValue);
	result = toP(parser, result);
	l_attach(parser, (JagPJCTree *) result, dc);
	priv->receiverParam = prevReceiverParam;
	cat_unref_ptr(params);
	cat_unref_ptr(thrown);
	cat_unref_ptr(body);
	cat_unref_ptr(type);
	cat_unref_ptr(defaultValue);
	return (JagPJCTree *) result;
}

/** QualidentList = [Annotations] Qualident {"," [Annotations] Qualident}
 */
/* M1 */
static CatArrayWo */*<JagPJCExpression>*/ l_qualident_list(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo /*<JagPJCExpression>*/ *ts = cat_array_wo_new();

	CatArrayWo */*<JagPJCAnnotation>*/ typeAnnos = l_type_annotations_opt(parser);
	JagPJCExpression *qi = l_qualident(parser, TRUE);
	if (cat_array_wo_size(typeAnnos)>0) {
		JagPJCExpression *at = l_insert_annotations_to_most_inner(parser, qi, typeAnnos, FALSE);
		cat_array_wo_append(ts, (GObject *) at);
		cat_unref_ptr(at);
	} else {
		cat_array_wo_append(ts, (GObject *) qi);
	}
	cat_unref_ptr(qi);
	cat_unref_ptr(typeAnnos);

	while (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);

		typeAnnos = l_type_annotations_opt(parser);
		qi = l_qualident(parser, TRUE);
		if (cat_array_wo_size(typeAnnos)>0) {
			JagPJCExpression *at = l_insert_annotations_to_most_inner(parser, qi, typeAnnos, FALSE);
			cat_array_wo_append(ts, (GObject *) at);
			cat_unref_ptr(at);
		} else {
			cat_array_wo_append(ts, (GObject *) qi);
		}
		cat_unref_ptr(qi);
	cat_unref_ptr(typeAnnos);
	}
	return ts;
}

/**
 *  {@literal
 *  TypeParametersOpt = ["<" TypeParameter {"," TypeParameter} ">"]
 *  }
 */
/* M1 */
static CatArrayWo /*<JagPJCTypeParameter>*/ *l_type_parameters_opt(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_type_parameters_opt:token=%d", priv->token->kind);
	CatArrayWo /*<JagPJCTypeParameter>*/ *typarams = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_LT) {
		l_next_token(parser);
		GObject *tprm = (GObject *) l_type_parameter(parser);
		cat_array_wo_append(typarams, tprm);
		cat_unref_ptr(tprm);
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			tprm = (GObject *) l_type_parameter(parser);
			cat_array_wo_append(typarams, tprm);
			cat_unref_ptr(tprm);
		}
		l_accept(parser, JAGP_KIND_GT);
	}
	return typarams;
}

/**
 *  {@literal
 *  TypeParameter = [Annotations] TypeVariable [TypeParameterBound]
 *  TypeParameterBound = EXTENDS Type {"&" Type}
 *  TypeVariable = Ident
 *  }
 */
/* M1 */
static JagPJCTypeParameter *l_type_parameter(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_type_paramater:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	CatArrayWo */*<JagPJCAnnotation>*/ annos = l_type_annotations_opt(parser);
	JagPName *name = l_ident(parser, FALSE);
	CatArrayWo /*<JagPJCExpression>*/ *bounds = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_EXTENDS) {
		l_next_token(parser);
		GObject *paty = (GObject *) l_parse_type(parser);
		cat_array_wo_append(bounds, paty);
		cat_unref_ptr(paty);
		while (priv->token->kind == JAGP_KIND_AMP) {
			l_next_token(parser);
			GObject *paty = (GObject *) l_parse_type(parser);
			cat_array_wo_append(bounds, paty);
			cat_unref_ptr(paty);
		}
	}
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTypeParameter *result = jagp_tree_maker_type_parameter(priv->tree_maker, name, bounds, annos);
	cat_unref_ptr(name);
	cat_unref_ptr(bounds);
	cat_unref_ptr(annos);
	result = toP(parser, result);
	return result;
}

/** FormalParameters = "(" [ FormalParameterList ] ")"
 *  FormalParameterList = [ FormalParameterListNovarargs , ] LastFormalParameter
 *  FormalParameterListNovarargs = [ FormalParameterListNovarargs , ] FormalParameter
 */
/* M1 */
static CatArrayWo */*JagPJCVariableDecl>*/ l_formal_parameters(JagPParser *parser, gboolean lambdaParameters) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_formal_parameters:token=%d", priv->token->kind);
	CatArrayWo /*<JagPJCVariableDecl>*/ *params = cat_array_wo_new();
	JagPJCVariableDecl *lastParam = NULL;
	l_accept(parser, JAGP_KIND_LPAREN);
	if (priv->token->kind != JAGP_KIND_RPAREN) {
		priv->allowThisIdent = TRUE;
		lastParam = l_formal_parameter(parser, lambdaParameters);
		if (lastParam->nameexpr != NULL) {
			cat_ref_swap(priv->receiverParam, lastParam);
		} else {
			cat_array_wo_append(params, (GObject *) lastParam);
		}
		cat_unref(lastParam);
		priv->allowThisIdent = FALSE;
		while (priv->token->kind == JAGP_KIND_COMMA) {
//			if ((lastParam->mods->flags & JAGP_FLAG_VARARGS) != 0) {
//				error(lastParam, "varargs.must.be.last");
//			}
			l_next_token(parser);
			lastParam = l_formal_parameter(parser, lambdaParameters);
			cat_array_wo_append(params, (GObject *) lastParam);
			cat_unref(lastParam);
		}
	}
	if (priv->token->kind == JAGP_KIND_RPAREN) {
		l_next_token(parser);
	} else {
		l_set_error_end_pos(parser, priv->token->pos);
//		reportSyntaxError(jagp_ilexer_prev_token(priv->lexer)->endPos, "expected3", JAGP_KIND_COMMA, JAGP_KIND_RPAREN, JAGP_KIND_LBRACKET);
	}
	return params;
}

static CatArrayWo */*JagPJCVariableDecl>*/ l_implicit_parameters(JagPParser *parser, gboolean hasParens) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_indent_level++;
	cat_log_debug("l_implicit_parameters:token=%d", priv->token->kind);
	if (hasParens) {
		l_accept(parser, JAGP_KIND_LPAREN);
	}
	CatArrayWo /*<JagPJCVariableDecl>*/ *params = cat_array_wo_new();
	if (priv->token->kind != JAGP_KIND_RPAREN && priv->token->kind != JAGP_KIND_ARROW) {
		GObject *ip = (GObject *) l_implicit_parameter(parser);
		cat_array_wo_append(params, ip);
		cat_unref_ptr(ip);
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			GObject *ip = (GObject *) l_implicit_parameter(parser);
			cat_array_wo_append(params, ip);
			cat_unref_ptr(ip);
		}
	}
	if (hasParens) {
		l_accept(parser, JAGP_KIND_RPAREN);
	}
	cat_log_debug("l_implicit_parameters:done:result=%O", params);
	cat_log_indent_level--;
	return params;
}

static JagPJCModifiers *l_opt_final(JagPParser *parser, long flags) {
	JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
	l_check_no_mods(parser, mods->flags & ~(JAGP_FLAG_FINAL | JAGP_FLAG_DEPRECATED));
	mods->flags |= flags;
	return mods;
}

/**
 * Inserts the annotations (and possibly a new array level)
 * to the left-most type in an array or nested type.
 *
 * When parsing a type like {@code @B Outer.Inner @A []}, the
 * {@code @A} annotation should target the array itself, while
 * {@code @B} targets the nested type {@code Outer}.
 *
 * Currently the parser parses the annotation first, then
 * the array, and then inserts the annotation to the left-most
 * nested type.
 *
 * When {@code createNewLevel} is true, then a new array
 * level is inserted as the most inner type, and have the
 * annotations target it.  This is useful in the case of
 * varargs, e.g. {@code String @A [] @B ...}, as the parser
 * first parses the type {@code String @A []} then inserts
 * a new array level with {@code @B} annotation.
 */
static JagPJCExpression *l_insert_annotations_to_most_inner(JagPParser *parser, JagPJCExpression *type, CatArrayWo */*<JagPJCAnnotation>*/ annos,
				gboolean createNewLevel) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int origEndPos = l_get_end_pos((JagPJCTree *) type);
	JagPJCExpression *mostInnerType = type;
	JagPJCArrayTypeTree *mostInnerArrayType = NULL;
	JagPJCExpression *tex = jagp_tree_info_type_in((JagPJCExpression *) mostInnerType);
	while (jagp_jctree_has_tag((JagPJCTree *) tex, JAGP_TAG_TYPEARRAY)) {
		mostInnerArrayType = (JagPJCArrayTypeTree *) tex;
		mostInnerType = mostInnerArrayType->elemtype;
	}

	if (createNewLevel) {
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		JagPJCExpression *elemtype = mostInnerType;
		mostInnerType = (JagPJCExpression *) jagp_tree_maker_type_array(priv->tree_maker, elemtype);
		cat_unref_ptr(elemtype);
		mostInnerType = to(parser, mostInnerType);
	}

	JagPJCExpression *mostInnerTypeToReturn = mostInnerType;
	if (cat_array_wo_size(annos)>0) {
		JagPJCExpression *lastToModify = mostInnerType;

		JagPJCExpression *tex = jagp_tree_info_type_in((JagPJCExpression *) mostInnerType);
		while (jagp_jctree_has_tag((JagPJCTree *) tex, JAGP_TAG_SELECT) ||
				jagp_jctree_has_tag((JagPJCTree *) tex, JAGP_TAG_TYPEAPPLY)) {
			while (jagp_jctree_has_tag((JagPJCTree *) tex, JAGP_TAG_SELECT)) {
				lastToModify = mostInnerType;
				mostInnerType = ((JagPJCFieldAccess *) tex)->selected;
				tex = jagp_tree_info_type_in((JagPJCExpression *) mostInnerType);
			}
			while (jagp_jctree_has_tag((JagPJCTree *) tex, JAGP_TAG_TYPEAPPLY)) {
				lastToModify = mostInnerType;
				mostInnerType = ((JagPJCTypeApply *) tex)->clazz;
				tex = jagp_tree_info_type_in((JagPJCExpression *) mostInnerType);
			}

			tex = jagp_tree_info_type_in((JagPJCExpression *) mostInnerType);
		}

		jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(annos));
		JagPJCExpression *underlying_type = mostInnerType;
		mostInnerType = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annos, underlying_type);

		JagPJCExpression *ltmex = jagp_tree_info_type_in(lastToModify);

		if (jagp_jctree_has_tag((JagPJCTree *) ltmex, JAGP_TAG_TYPEAPPLY)) {
			((JagPJCTypeApply *) ltmex)->clazz = mostInnerType;
		} else if (jagp_jctree_has_tag((JagPJCTree *) ltmex, JAGP_TAG_SELECT)) {
			((JagPJCFieldAccess *) ltmex)->selected = mostInnerType;
		} else {
			/* We never saw a SELECT or TYPEAPPLY, return the annotated type. */
			mostInnerTypeToReturn = mostInnerType;
		}
	}

	if (mostInnerArrayType == NULL) {
		return mostInnerTypeToReturn;
	} else {
		mostInnerArrayType->elemtype = mostInnerTypeToReturn;
		l_store_end(type, origEndPos);
		return type;
	}
}

/** FormalParameter = { FINAL | '@' Annotation } Type VariableDeclaratorId
 *  LastFormalParameter = { FINAL | '@' Annotation } Type '...' Ident | FormalParameter
 */
/* M1 */
static JagPJCVariableDecl *l_formal_parameter(JagPParser *parser, gboolean lambdaParameter) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCModifiers *mods = l_opt_final(parser, JAGP_FLAG_PARAMETER);
	/* need to distinguish between vararg annos and array annos
	 * look at typeAnnotationsPushedBack comment */
	priv->permitTypeAnnotationsPushBack = TRUE;
	JagPJCExpression *type = l_parse_type(parser);
	priv->permitTypeAnnotationsPushBack = FALSE;

	if (priv->token->kind == JAGP_KIND_ELLIPSIS) {
		CatArrayWo /*<JagPJCAnnotation>*/ *varargsAnnos = priv->typeAnnotationsPushedBack;
		priv->typeAnnotationsPushedBack = cat_array_wo_new();
		mods->flags |= JAGP_FLAG_VARARGS;
		/* insert var arg type annotations */
		JagPJCExpression *type_inner = type;
		type = l_insert_annotations_to_most_inner(parser, type_inner, varargsAnnos, TRUE);
		cat_unref_ptr(type_inner);
		cat_unref_ptr(varargsAnnos);
		l_next_token(parser);
	} else {
		/* if not a var arg, then typeAnnotationsPushedBack should be NULL */
		if (cat_array_wo_size(priv->typeAnnotationsPushedBack)>0) {
//			reportSyntaxError(l_list_head_pos(priv->typeAnnotationsPushedBack),
//					"illegal.start.of.type");
		}
		cat_unref_ptr(priv->typeAnnotationsPushedBack);
		priv->typeAnnotationsPushedBack = cat_array_wo_new();
	}
	JagPJCVariableDecl *result = l_variable_declarator_id(parser, mods, type, lambdaParameter);
	cat_unref_ptr(mods);
	cat_unref_ptr(type);
	return result;
}

static JagPJCVariableDecl *l_implicit_parameter(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, JAGP_FLAG_PARAMETER, NULL);
	JagPJCVariableDecl *result = l_variable_declarator_id(parser, mods, NULL, TRUE);
	return result;
}

/* ---------- auxiliary methods -------------- */


/** Check that given tree is a legal expression statement.
 */
/* M1 */
static JagPJCExpression *l_check_expr_stat(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!jagp_tree_info_is_expression_statement(t)) {
		jagp_tree_maker_at(priv->tree_maker, t->parent.pos);
		CatArrayWo *errs = cat_array_wo_new();
		cat_array_wo_append(errs, (GObject *) t);
		JagPJCExpression *ret = (JagPJCExpression *) jagp_tree_maker_erroneous(priv->tree_maker, errs);
		cat_unref_ptr(errs);
//		error(ret, "not.stmt");
		return ret;
	} else {
		return cat_ref_ptr(t);
	}
}

/** Return precedence of operator represented by token,
 *  -1 if token is not a binary operator. @see TreeInfo.opPrec
 */
static int l_prec(JagPTokenKind token) {
	JagPTag oc = l_optag(token);
	return (oc != JAGP_TAG_NO_TAG) ? jagp_tree_info_op_prec(oc) : -1;
}

/**
 * Return the lesser of two positions, making allowance for either one
 * being unset.
 */
static int l_earlier(int pos1, int pos2) {
	if (pos1 == -1)
		return pos2;
	if (pos2 == -1)
		return pos1;
	return (pos1 < pos2 ? pos1 : pos2);
}

/** Return operation tag of binary operator represented by token,
 *  No_TAG if token is not a binary operator.
 */
static JagPTag l_optag(JagPTokenKind token) {
	switch (token) {
		case JAGP_KIND_BARBAR:
			return JAGP_TAG_OR;
		case JAGP_KIND_AMPAMP:
			return JAGP_TAG_AND;
		case JAGP_KIND_BAR:
			return JAGP_TAG_BITOR;
		case JAGP_KIND_BAREQ:
			return JAGP_TAG_BITOR_ASG;
		case JAGP_KIND_CARET:
			return JAGP_TAG_BITXOR;
		case JAGP_KIND_CARETEQ:
			return JAGP_TAG_BITXOR_ASG;
		case JAGP_KIND_AMP:
			return JAGP_TAG_BITAND;
		case JAGP_KIND_AMPEQ:
			return JAGP_TAG_BITAND_ASG;
		case JAGP_KIND_EQEQ:
			return JAGP_TAG_EQ;
		case JAGP_KIND_BANGEQ:
			return JAGP_TAG_NE;
		case JAGP_KIND_LT:
			return JAGP_TAG_LT;
		case JAGP_KIND_GT:
			return JAGP_TAG_GT;
		case JAGP_KIND_LTEQ:
			return JAGP_TAG_LE;
		case JAGP_KIND_GTEQ:
			return JAGP_TAG_GE;
		case JAGP_KIND_LTLT:
			return JAGP_TAG_SL;
		case JAGP_KIND_LTLTEQ:
			return JAGP_TAG_SL_ASG;
		case JAGP_KIND_GTGT:
			return JAGP_TAG_SR;
		case JAGP_KIND_GTGTEQ:
			return JAGP_TAG_SR_ASG;
		case JAGP_KIND_GTGTGT:
			return JAGP_TAG_USR;
		case JAGP_KIND_GTGTGTEQ:
			return JAGP_TAG_USR_ASG;
		case JAGP_KIND_PLUS:
			return JAGP_TAG_PLUS;
		case JAGP_KIND_PLUSEQ:
			return JAGP_TAG_PLUS_ASG;
		case JAGP_KIND_SUB:
			return JAGP_TAG_MINUS;
		case JAGP_KIND_SUBEQ:
			return JAGP_TAG_MINUS_ASG;
		case JAGP_KIND_STAR:
			return JAGP_TAG_MUL;
		case JAGP_KIND_STAREQ:
			return JAGP_TAG_MUL_ASG;
		case JAGP_KIND_SLASH:
			return JAGP_TAG_DIV;
		case JAGP_KIND_SLASHEQ:
			return JAGP_TAG_DIV_ASG;
		case JAGP_KIND_PERCENT:
			return JAGP_TAG_MOD;
		case JAGP_KIND_PERCENTEQ:
			return JAGP_TAG_MOD_ASG;
		case JAGP_KIND_INSTANCEOF:
			return JAGP_TAG_TYPETEST;
		default:
			return JAGP_TAG_NO_TAG;
	}
}

/** Return operation tag of unary operator represented by token,
 *  No_TAG if token is not a binary operator.
 */
static JagPTag l_unoptag(JagPTokenKind token) {
	switch (token) {
	case JAGP_KIND_PLUS:
		return JAGP_TAG_POS;
	case JAGP_KIND_SUB:
		return JAGP_TAG_NEG;
	case JAGP_KIND_BANG:
		return JAGP_TAG_NOT;
	case JAGP_KIND_TILDE:
		return JAGP_TAG_COMPL;
	case JAGP_KIND_PLUSPLUS:
		return JAGP_TAG_PREINC;
	case JAGP_KIND_SUBSUB:
		return JAGP_TAG_PREDEC;
	default:
		return JAGP_TAG_NO_TAG;
	}
}

/** Return type tag of basic type represented by token,
 *  NONE if token is not a basic type identifier.
 */
static JagPTypeTag l_typetag(JagPTokenKind token) {
	switch (token) {
	case JAGP_KIND_BYTE:
		return JAGP_TT_BYTE;
	case JAGP_KIND_CHAR:
		return JAGP_TT_CHAR;
	case JAGP_KIND_SHORT:
		return JAGP_TT_SHORT;
	case JAGP_KIND_INT:
		return JAGP_TT_INT;
	case JAGP_KIND_LONG:
		return JAGP_TT_LONG;
	case JAGP_KIND_FLOAT:
		return JAGP_TT_FLOAT;
	case JAGP_KIND_DOUBLE:
		return JAGP_TT_DOUBLE;
	case JAGP_KIND_BOOLEAN:
		return JAGP_TT_BOOLEAN;
	default:
		return JAGP_TT_NONE;
	}
}

static void l_check_diamond(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowDiamond) {
//		error(priv->token->pos, "diamond.not.supported.in.source", source.name);
		priv->allowDiamond = TRUE;
	}
}

static void l_check_multicatch(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowMulticatch) {
//		error(priv->token->pos, "multicatch.not.supported.in.source", source.name);
		priv->allowMulticatch = TRUE;
	}
}

static void l_check_try_with_resources(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);

	if (!priv->allowTWR) {
//		error(priv->token->pos, "try.with.resources.not.supported.in.source", source.name);
		priv->allowTWR = TRUE;
	}
}

static void l_check_variable_in_try_with_resources(JagPParser *parser, int startPos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowEffectivelyFinalVariablesInTWR) {
//		error(startPos, "var.in.try.with.resources.not.supported.in.source", source.name);
		priv->allowEffectivelyFinalVariablesInTWR = TRUE;
	}
}

static void l_check_lambda(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);

	if (!priv->allowLambda) {
//		log.error(priv->token->pos, "lambda.not.supported.in.source", source.name);
		priv->allowLambda = TRUE;
	}
}

static void l_check_method_references(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);

	if (!priv->allowMethodReferences) {
//		log.error(priv->token->pos, "method.references.not.supported.in.source", source.name);
		priv->allowMethodReferences = TRUE;
	}
}

static void l_check_default_methods(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);

	if (!priv->allowDefaultMethods) {
//		log.error(priv->token->pos, "default.methods.not.supported.in.source", source.name);
		priv->allowDefaultMethods = TRUE;
	}
}

static void l_check_intersection_types_in_cast(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowIntersectionTypesInCast) {
//		log.error(priv->token->pos, "intersection.types.in.cast.not.supported.in.source", source.name);
		priv->allowIntersectionTypesInCast = TRUE;
	}
}

static void l_check_static_interface_methods(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowStaticInterfaceMethods) {
//		log.error(priv->token->pos, "static.intf.methods.not.supported.in.source", source.name);
		priv->allowStaticInterfaceMethods = TRUE;
	}
}

static void l_check_type_annotations(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowTypeAnnotations) {
//		log.error(priv->token->pos, "type.annotations.not.supported.in.source", source.name);
		priv->allowTypeAnnotations = TRUE;
	}
}

static void l_check_private_interface_methods(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowPrivateInterfaceMethods) {
//		log.error(priv->token->pos, CompilerProperties.Errors.PrivateIntfMethodsNotSupportedInSource(source.name));
		priv->allowPrivateInterfaceMethods = TRUE;
	}
}

static void l_check_annotations_after_type_params(JagPParser *parser, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (!priv->allowAnnotationsAfterTypeParams) {
//		log.error(pos, "annotations.after.type.params.not.supported.in.source", source.name);
		priv->allowAnnotationsAfterTypeParams = TRUE;
	}
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPParser *instance = JAGP_PARSER(self);
	JagPParserPrivate *priv = jagp_parser_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
