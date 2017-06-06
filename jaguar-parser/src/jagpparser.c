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
//	JagPScanner *scanner;
	JagPILexer *lexer;
	GroRunITokenFactory *token_factory;
	GroRunModel *model;
	CatArrayWo *messages;
	JagPCompilationUnit *compilation_unit;
	CatArrayWo *token_cash;


	JagPToken *token;
	CatArrayWo *token_list;
	int token_index;
	JagPTreeMaker *tree_maker;
	int mode;
	int last_mode;


    /** Switch: Should diamond operator be recognized?
     */
    gboolean allowDiamond;

    /** Switch: Should multicatch clause be accepted?
     */
    gboolean allowMulticatch;

    /** Switch: should we recognize try-with-resources?
     */
    gboolean allowTWR;

    /** Switch: should we allow (effectively) final variables as resources in try-with-resources?
     */
    gboolean allowEffectivelyFinalVariablesInTWR;

    /** Switch: should we fold strings?
     */
    gboolean allowStringFolding;

    /** Switch: should we recognize lambda expressions?
     */
    gboolean allowLambda;

    /** Switch: should we allow method/constructor references?
     */
    gboolean allowMethodReferences;

    /** Switch: should we recognize modules?
     */
    gboolean allowModules;

    /** Switch: should we allow default methods in interfaces?
     */
    gboolean allowDefaultMethods;

    /** Switch: should we allow static methods in interfaces?
     */
    gboolean allowStaticInterfaceMethods;

    /** Switch: should we allow private (instance) methods in interfaces?
     */
    gboolean allowPrivateInterfaceMethods;

    /** Switch: should we allow intersection types in cast?
     */
    gboolean allowIntersectionTypesInCast;

    /** Switch: should we keep docComments?
     */
    gboolean keepDocComments;

    /** Switch: should we keep line table?
     */
    gboolean keepLineMap;

    /** Switch: should we recognize type annotations?
     */
    gboolean allowTypeAnnotations;

    /** Switch: should we allow annotations after the method type parameters?
     */
    gboolean allowAnnotationsAfterTypeParams;

    /** Switch: should we allow '_' as an identifier?
     */
    gboolean allowUnderscoreIdentifier;

    /** Switch: is "this" allowed as an identifier?
     * This is needed to parse receiver types.
     */
    gboolean allowThisIdent;


    /** The type of the method receiver, as specified by a first "this" parameter.
     */
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
//	JagPParser *instance = JAGP_PARSER(object);
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(instance);
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
//	int token_count = cat_array_wo_size(priv->token_list);
//	cat_log_detail("next_token:idx=%d, count=%d", priv->token_index, token_count);
//	if (priv->token_index<token_count) {
//		priv->token_index++;
//	}
//
//	if (priv->token_index==token_count) {
//		GroRunFullToken *full_token = (GroRunFullToken *) grorun_iscanner_next((GroRunIScanner *) priv->scanner);
//		cat_log_detail("full_token=%O", full_token);
//		JagPToken *token = jagp_token_new();
//		token->source = full_token;
//		int sym_index = grorun_full_token_get_user_index(full_token);
//		switch(sym_index) {
//			case JAGP_SYM_K_PACKAGE : token->kind = JAGP_KIND_PACKAGE; break;
//			case JAGP_SYM_K_CLASS : token->kind = JAGP_KIND_CLASS; break;
//			case JAGP_SYM_K_IMPORT: token->kind = JAGP_KIND_IMPORT; break;
//
//			case JAGP_SYM_K_PUBLIC: token->kind = JAGP_KIND_PUBLIC; break;
//			case JAGP_SYM_K_PRIVATE: token->kind = JAGP_KIND_PRIVATE; break;
//			case JAGP_SYM_K_PROTECTED: token->kind = JAGP_KIND_PROTECTED; break;
//			case JAGP_SYM_K_STATIC: token->kind = JAGP_KIND_STATIC; break;
//			case JAGP_SYM_K_FINAL: token->kind = JAGP_KIND_FINAL; break;
//			case JAGP_SYM_K_TRANSIENT: token->kind = JAGP_KIND_TRANSIENT; break;
//			case JAGP_SYM_K_VOLATILE: token->kind = JAGP_KIND_VOLATILE; break;
//			case JAGP_SYM_K_STRICTFP: token->kind = JAGP_KIND_STRICTFP; break;
//			case JAGP_SYM_K_ABSTRACT: token->kind = JAGP_KIND_ABSTRACT; break;
//
//			case JAGP_SYM_K_VOID: token->kind = JAGP_KIND_VOID; break;
//			case JAGP_SYM_K_BYTE: token->kind = JAGP_KIND_BYTE; break;
//			case JAGP_SYM_K_CHAR: token->kind = JAGP_KIND_CHAR; break;
//			case JAGP_SYM_K_SHORT: token->kind = JAGP_KIND_SHORT; break;
//			case JAGP_SYM_K_INT: token->kind = JAGP_KIND_INT; break;
//			case JAGP_SYM_K_LONG: token->kind = JAGP_KIND_LONG; break;
//			case JAGP_SYM_K_FLOAT: token->kind = JAGP_KIND_FLOAT; break;
//			case JAGP_SYM_K_DOUBLE: token->kind = JAGP_KIND_DOUBLE; break;
//
//			case JAGP_SYM_K_FOR: token->kind = JAGP_KIND_FOR; break;
//			case JAGP_SYM_K_DO: token->kind = JAGP_KIND_DO; break;
//			case JAGP_SYM_K_WHILE: token->kind = JAGP_KIND_WHILE; break;
//			case JAGP_SYM_K_SWITCH: token->kind = JAGP_KIND_SWITCH; break;
//			case JAGP_SYM_K_CASE: token->kind = JAGP_KIND_CASE; break;
//			case JAGP_SYM_K_IF: token->kind = JAGP_KIND_IF; break;
//			case JAGP_SYM_K_ELSE: token->kind = JAGP_KIND_ELSE; break;
//			case JAGP_SYM_K_BREAK: token->kind = JAGP_KIND_BREAK; break;
//			case JAGP_SYM_K_CONTINUE: token->kind = JAGP_KIND_CONTINUE; break;
//
//
//			case JAGP_SYM_IDENTIFIER : token->kind = JAGP_KIND_IDENTIFIER; token->value = (GObject *) jagp_name_new(); break;
//
//			case JAGP_SYM_FULL_COMMENT :
//				cat_array_wo_append(priv->token_list, (GObject *) token);
//				return l_next_token(parser);
//			case JAGP_SYM_L_DOT : token->kind = JAGP_KIND_DOT; break;
//			case JAGP_SYM_L_SEMI : token->kind = JAGP_KIND_SEMI; break;
//			case JAGP_SYM_L_COMMA : token->kind = JAGP_KIND_COMMA; break;
//			case JAGP_SYM_L_COLON : token->kind = JAGP_KIND_COLON; break;
//
//			case JAGP_SYM_END_OF_INPUT : token->kind = JAGP_KIND_EOF;  break;
//
//
//			default : {
//				token->kind = JAGP_KIND_EOF;
//			}
//		}
//		cat_array_wo_append(priv->token_list, (GObject *) token);
//		priv->token = token;
//	} else {
//		priv->token = (JagPToken *) cat_array_wo_get(priv->token_list, priv->token_index);
//	}
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


JagPParser *jagp_parser_new(JagPScanner *scanner, JagPILexer *lexer, GroRunITokenFactory *token_factory, GroRunModel *model) {
	JagPParser *result = g_object_new(JAGP_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	JagPParserPrivate *priv = jagp_parser_get_instance_private(result);
//	priv->scanner = cat_ref_ptr(scanner);
	priv->lexer = cat_ref_ptr(lexer);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
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

	JagPJCCompilationUnit *compilation_unit = l_parse_compilation_unit(parser);


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

static JagPJCErroneous *l_syntax_error(JagPParser *parser, int pos, CatArrayWo /*<JCTree>*/ *errs, const char *key/*, TokenKind... args */) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//        setErrorEndPos(pos);
	if (errs==NULL) {
		errs = cat_array_wo_new();
	}
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//        reportSyntaxError(err, key, (Object[])args);
	if (errs != NULL) {
		JagPJCTree *last = (JagPJCTree *) cat_array_wo_get_last(errs);
		if (last != NULL) {
//			storeEnd(last, pos);
		}
	}
//	return toP(err);
	return err;
}


/** If next input token matches given token, skip it, otherwise report
 *  an error.
 */
static void l_accept(JagPParser *parser, JagPTokenKind tk) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (priv->token->kind == tk) {
		l_next_token(parser);
	} else {
//		setErrorEndPos(priv->token->pos);
//		reportSyntaxError(S.prevToken().endPos, "expected", tk);
	}
}

/** Report an illegal start of expression/type error at given position.
 */
static JagPJCExpression *l_illegal(JagPParser *parser, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//    setErrorEndPos(pos);
    if ((priv->mode & JAGP_MODE_EXPR) != 0)
        return (JagPJCExpression *) l_syntax_error(parser, pos, NULL, "illegal.start.of.expr");
    else
        return (JagPJCExpression *) l_syntax_error(parser, pos, NULL, "illegal.start.of.type");
}


/** Diagnose a modifier flag from the set, if any. */
static void l_check_no_mods(JagPParser *parser, long long mods) {
	if (mods != 0) {
		long lowestMod = mods & -mods;
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

static void l_set_error_end_pos(JagPParser *parser, int err_pos) {
//	endPosTable.setErrorEndPos(errPos);
}

static void l_store_end(JagPJCTree *tree, int endpos) {
//	endPosTable.storeEnd(tree, endpos);
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

static JagPName *l_ident(JagPParser *parser, gboolean advance_on_errors) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (priv->token->kind == JAGP_KIND_IDENTIFIER) {
		JagPName *name = (JagPName *) priv->token->value;
		l_next_token(parser);
		return name;
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
		return priv->names_error;
	}
}

/**
 * Qualident = Ident { DOT [Annotations] Ident }
 */
static JagPJCExpression *l_qualident(JagPParser *parser, gboolean allow_annos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPName *ident_name = l_ident(parser, FALSE);
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, ident_name);
	while (priv->token->kind == JAGP_KIND_DOT) {
		int pos = priv->token->pos;
		l_next_token(parser);
		CatArrayWo *tyannos = NULL;
		if (allow_annos) {
//			tyannos = l_type_annotations_opt(parser);
		}
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *next_t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, t, l_ident(parser, FALSE));
		cat_unref_ptr(t);
		t = next_t;
//		if (tyannos != NULL && tyannos.nonEmpty()) {
//			t = toP(F.at(tyannos.head.pos).AnnotatedType(tyannos, t));
//		}
	}
	return t;
}

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
//	default:
//		Assert.error();
	}
//	if (t == errorTree)
//		t = F.at(pos).Erroneous();
//	storeEnd(t, token.endPos);
	l_next_token(parser);
	return t;
}


/** terms can be either expressions or types.
 */
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
static JagPJCExpression *l_parse_type(JagPParser *parser) {
	CatArrayWo * /*<JCAnnotation>*/ annotations = l_type_annotations_opt(parser);
	return l_parse_type_with_annotations(parser, annotations);
}

static JagPJCExpression *l_parse_type_with_annotations(JagPParser *parser, CatArrayWo * /* <JCAnnotation> */ annotations) {
	JagPJCExpression *result = l_unannotated_type(parser);

	if (cat_array_wo_size(annotations)>0) {
		result = l_insert_annotations_to_most_inner(parser, result, annotations, FALSE);
	}

	return result;
}

static JagPJCExpression *l_unannotated_type(JagPParser *parser) {
	return l_term(parser, JAGP_MODE_TYPE);
}

static JagPJCExpression *l_term(JagPParser *parser, int newmode) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term:token=%d", priv->token->kind);
	int prevmode = priv->mode;
	priv->mode = newmode;
	JagPJCExpression *t = l_term_raw(parser);
	priv->last_mode = priv->mode;
	priv->mode = prevmode;
	return t;
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
static JagPJCExpression *l_term_raw(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term_raw:token=%d", priv->token->kind);
    JagPJCExpression *t = l_term1(parser);
    JagPTokenKind tk = priv->token->kind;
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
        return l_term_rest(parser, t);
    } else {
        return t;
    }
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
			return (JagPJCExpression *) jagp_tree_maker_assign(priv->tree_maker, t, t1);
		}
		default:
			return t;
    }
}

/** Expression1   = Expression2 [Expression1Rest]
 *  Type1         = Type2
 *  TypeNoParams1 = TypeNoParams2
 */
static JagPJCExpression *l_term1(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term1:token=%d", priv->token->kind);
	JagPJCExpression *t = l_term2(parser);
	if ((priv->mode & JAGP_MODE_EXPR) != 0 && priv->token->kind == JAGP_KIND_QUES) {
		priv->mode = JAGP_MODE_EXPR;
		return l_term1_rest(parser, t);
	} else {
		return t;
	}
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
static JagPJCExpression *l_term2(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term2:token=%d", priv->token->kind);
	JagPJCExpression *t = l_term3(parser);
	if ((priv->mode & JAGP_MODE_EXPR) != 0 && l_prec(priv->token->kind) >= jagp_tree_info_orPrec) {
		priv->mode = JAGP_MODE_EXPR;
		return l_term2_rest(parser, t, jagp_tree_info_orPrec);
	} else {
		return t;
	}
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
	cat_log_debug("l_term2_rest:token=%d", priv->token->kind);
	JagPJCExpression **odStack = l_new_od_stack(parser);
	JagPToken **opStack = l_new_op_stack(parser);

	// optimization, was odStack = new Tree[...]; opStack = new Tree[...];
	int top = 0;
	odStack[0] = t;
	int startPos = priv->token->pos;
	JagPToken *topOp = NULL;
	while (l_prec(priv->token->kind) >= minprec) {
		cat_log_debug("l_term2_rest:token=%d, minprec=%d", priv->token->kind, minprec);
		opStack[top] = topOp;
		top++;
		topOp = priv->token;
		l_next_token(parser);
		odStack[top] = (topOp->kind == JAGP_KIND_INSTANCEOF) ? l_parse_type(parser) : l_term3(parser);
		while (top > 0 && l_prec(topOp->kind) >= l_prec(priv->token->kind)) {
			odStack[top-1] = l_make_op(parser, topOp->pos, topOp->kind, odStack[top-1],
									odStack[top]);
			top--;
			topOp = opStack[top];
		}
	}
//	Assert.check(top == 0);
	t = odStack[0];

//	if (jagp_jctree_has_tag(t, JAGP_TAG_PLUS)) {
//		t = foldStrings(t);
//	}

//	odStackSupply.add(odStack);
//	opStackSupply.add(opStack);
	cat_log_debug("l_term2_rest:done=%O", t);
	return t;
}
//where
	/** Construct a binary or type test node.
	 */
	static JagPJCExpression *l_make_op(JagPParser *parser, int pos, JagPTokenKind topOp,
								JagPJCExpression *od1, JagPJCExpression *od2) {
		JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
//		if (topOp == JAGP_KIND_INSTANCEOF) {
//			return jagp_tree_maker_type_test(priv->tree_maker, od1, od2);
//		} else {
//			return jagp_tree_maker_binary(priv->tree_maker, l_optag(topOp), od1, od2);
//		}
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



static int l_list_head_pos(CatArrayWo *tree_list) {
	JagPJCTree *ft = (JagPJCTree *) cat_array_wo_get_first(tree_list);
	return ft->pos;
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
static JagPJCExpression *l_term3(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term3:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	JagPJCExpression *t;
	CatArrayWo * /*JagPJCExpression*/ typeArgs = l_type_arguments_opt(parser, JAGP_MODE_EXPR);
	cat_log_debug("l_term3:token=%d", priv->token->kind);
	switch (priv->token->kind) {
		case JAGP_KIND_QUES:
			if ((priv->mode & JAGP_MODE_TYPE) != 0 && (priv->mode & (JAGP_MODE_TYPEARG|JAGP_MODE_NOPARAMS)) == JAGP_MODE_TYPEARG) {
				priv->mode = JAGP_MODE_TYPE;
				return l_type_argument(parser);
			} else
				return l_illegal(parser, priv->token->pos);
		case JAGP_KIND_PLUSPLUS : case JAGP_KIND_SUBSUB: case JAGP_KIND_BANG: case JAGP_KIND_TILDE: case JAGP_KIND_PLUS: case JAGP_KIND_SUB:
			if (typeArgs == NULL && (priv->mode & JAGP_MODE_EXPR) != 0) {
				JagPTokenKind tk = priv->token->kind;
				l_next_token(parser);
				priv->mode = JAGP_MODE_EXPR;
				if (tk == JAGP_KIND_SUB &&
					(priv->token->kind == JAGP_KIND_INTLITERAL || priv->token->kind == JAGP_KIND_LONGLITERAL) &&
					jagp_numeric_token_radix(priv->token) == 10) {
					priv->mode = JAGP_MODE_EXPR;
					t = l_literal_at(parser, priv->names_hyphen, pos);
				} else {
					t = l_term3(parser);
					jagp_tree_maker_at(priv->tree_maker, pos);
					return (JagPJCExpression *) jagp_tree_maker_unary(priv->tree_maker, l_unoptag(tk), t);
				}
			} else return l_illegal(parser, priv->token->pos);
			break;
		case JAGP_KIND_LPAREN :
			cat_log_debug("l_term3:priv->mode=%d", priv->mode);
			if (typeArgs == NULL && (priv->mode & JAGP_MODE_EXPR) != 0) {
				JagPParensResult pres = l_analyze_parens(parser);
				cat_log_debug("l_term3:pres=%d", pres);
				
				switch (pres) {
					case JAGP_PAR_CAST:
					   l_accept(parser, JAGP_KIND_LPAREN);
					   priv->mode = JAGP_MODE_TYPE;
					   int pos1 = pos;
					   CatArrayWo *targets = cat_array_wo_new();
					   t = l_term3(parser);
						cat_log_debug("l_term3:t=%O", t);

					   cat_array_wo_append(targets, (GObject *) t);
					   while (priv->token->kind == JAGP_KIND_AMP) {
						   l_check_intersection_types_in_cast(parser);
						   l_accept(parser, JAGP_KIND_AMP);
						   JagPJCExpression *sub = l_term3(parser);
						   cat_array_wo_append(targets, (GObject *) sub);
					   }
					   if (cat_array_wo_size(targets) > 1) {
						   jagp_tree_maker_at(priv->tree_maker, pos1);
						   t = (JagPJCExpression *) jagp_tree_maker_type_intersection(priv->tree_maker, targets);
					   }
					   l_accept(parser, JAGP_KIND_RPAREN);
					   priv->mode = JAGP_MODE_EXPR;
					   JagPJCExpression *t1 = l_term3(parser);
						cat_log_debug("l_term3:t1=%O", t1);
					   jagp_tree_maker_at(priv->tree_maker, pos);
					   JagPJCExpression *result = (JagPJCExpression *) jagp_tree_maker_type_cast(priv->tree_maker, (JagPJCTree *) t, t1);
						cat_log_debug("l_term3:done=%O", result);
						return result;

					case JAGP_PAR_IMPLICIT_LAMBDA:
					case JAGP_PAR_EXPLICIT_LAMBDA:
						t = l_lambda_expression_or_statement(parser, TRUE, pres == JAGP_PAR_EXPLICIT_LAMBDA, pos);
						break;
					default: //PARENS
						l_accept(parser, JAGP_KIND_LPAREN);
						priv->mode = JAGP_MODE_EXPR;
						t = l_term_rest(parser, l_term1_rest(parser, l_term2_rest(parser, l_term3(parser), jagp_tree_info_orPrec)));
						l_accept(parser, JAGP_KIND_RPAREN);
						jagp_tree_maker_at(priv->tree_maker, pos);
						JagPJCParens *parens = jagp_tree_maker_parens(priv->tree_maker, t);
						cat_unref_ptr(t);
						t = (JagPJCExpression *) parens;
						break;
				}
			} else {
				return l_illegal(parser, priv->token->pos);
			}
			break;
		case JAGP_KIND_THIS:
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				jagp_tree_maker_at(priv->tree_maker, pos);
				t = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, priv->names_this);
				l_next_token(parser);
				if (typeArgs == NULL)
					t = l_arguments_opt(parser, NULL, t);
				else
					t = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, t);
				typeArgs = NULL;
			} else return l_illegal(parser, priv->token->pos);
			break;
		case JAGP_KIND_SUPER:
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				jagp_tree_maker_at(priv->tree_maker, pos);
				t = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, priv->names_super);
				t = l_super_suffix(parser, typeArgs, t);
				typeArgs = NULL;
			} else return l_illegal(parser, priv->token->pos);
			break;
		case JAGP_KIND_INTLITERAL: case JAGP_KIND_LONGLITERAL: case JAGP_KIND_FLOATLITERAL: case JAGP_KIND_DOUBLELITERAL:
		case JAGP_KIND_CHARLITERAL: case JAGP_KIND_STRINGLITERAL:
		case JAGP_KIND_TRUE: case JAGP_KIND_FALSE: case JAGP_KIND_NULL:
			if (typeArgs == NULL && (priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				t = l_literal(parser, priv->names_empty);
			} else return l_illegal(parser, priv->token->pos);
			break;
		case JAGP_KIND_NEW:
			if (typeArgs != NULL) return l_illegal(parser, priv->token->pos);
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				priv->mode = JAGP_MODE_EXPR;
				l_next_token(parser);
				if (priv->token->kind == JAGP_KIND_LT) typeArgs = l_type_arguments(parser, FALSE);
				t = l_creator(parser, pos, typeArgs);
				typeArgs = NULL;
			} else return l_illegal(parser, priv->token->pos);
			break;
		case JAGP_KIND_MONKEYS_AT: {
			// Only annotated cast types and method references are valid
			CatArrayWo *type_annos = l_type_annotations_opt(parser);
			if (cat_array_wo_size(type_annos)==0) {
				// else there would be no '@'
				cat_log_fatal("Expected type annotations, but found none!");
//				throw new AssertionError("Expected type annotations, but found none!");
			}

			JagPJCExpression *expr = l_term3(parser);

			if ((priv->mode & JAGP_MODE_TYPE) == 0) {
				// Type annotations on class literals no longer legal
				switch (jagp_jctree_get_tag((JagPJCTree *) expr)) {
					case JAGP_TAG_REFERENCE: {
						JagPJCMemberReference *mref = (JagPJCMemberReference *) expr;
						jagp_tree_maker_at(priv->tree_maker, pos);
						JagPJCAnnotatedType *nat = jagp_tree_maker_annotated_type(priv->tree_maker, type_annos, mref->expr);
						cat_unref_ptr(mref->expr)
						mref->expr = (JagPJCExpression *) nat;
						cat_unref_ptr(t);
						t = (JagPJCExpression *) mref;
						break;
					}
					case JAGP_TAG_SELECT: {
						JagPJCFieldAccess *sel = (JagPJCFieldAccess *) expr;

						if (sel->name != priv->names_class) {
							return l_illegal(parser, priv->token->pos);
						} else {
	//						log.error(priv->token->pos, "no.annotations.on.dot.class");
							return expr;
						}
					}
					default:
						return l_illegal(parser, l_list_head_pos(type_annos));
				}

			} else {
				// Type annotations targeting a cast
				t = l_insert_annotations_to_most_inner(parser, expr, type_annos, FALSE);
			}
		}
		break;
		case JAGP_KIND_UNDERSCORE : case JAGP_KIND_IDENTIFIER : case JAGP_KIND_ASSERT : case JAGP_KIND_ENUM : {
			cat_log_debug("l_term3:priv->mode=%d", priv->mode);
			if (typeArgs != NULL) {
				return l_illegal(parser, priv->token->pos);
			}
			if ((priv->mode & JAGP_MODE_EXPR) != 0 && l_peek_token(parser, JAGP_KIND_ARROW)) {
				t = l_lambda_expression_or_statement(parser, FALSE, FALSE, pos);
			} else {
				jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
				t = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, l_ident(parser, FALSE));
				cat_log_debug("l_term3:t=%O", t);
				while (TRUE) {
					pos = priv->token->pos;
					CatArrayWo *annos = l_type_annotations_opt(parser);

					// need to report an error later if LBRACKET is for array
					// index access rather than array creation level
					if (cat_array_wo_size(annos)!=0 && priv->token->kind != JAGP_KIND_LBRACKET && priv->token->kind != JAGP_KIND_ELLIPSIS)
						return l_illegal(parser, l_list_head_pos(annos));

					switch (priv->token->kind) {
						case JAGP_KIND_LBRACKET:
							l_next_token(parser);
							if (priv->token->kind == JAGP_KIND_RBRACKET) {
								l_next_token(parser);
								t = l_brackets_opt(parser, t, cat_array_wo_new());
								jagp_tree_maker_at(priv->tree_maker, pos);
								JagPJCArrayTypeTree *tatt = jagp_tree_maker_type_array(priv->tree_maker, t);
								cat_unref_ptr(t);
								t = (JagPJCExpression *) tatt;
								if (cat_array_wo_size(annos)>0) {
									JagPJCAnnotatedType *tat = jagp_tree_maker_annotated_type(priv->tree_maker, annos, t);
									cat_unref_ptr(t);
									t = (JagPJCExpression *) tat;
								}
								t = l_brackets_suffix(parser, t);
							} else {
								if ((priv->mode & JAGP_MODE_EXPR) != 0) {
									priv->mode = JAGP_MODE_EXPR;
									JagPJCExpression *t1 = l_term_raw(parser);
									if (cat_array_wo_size(annos)>0) {
										t = l_illegal(parser, l_list_head_pos(annos));
									}
									jagp_tree_maker_at(priv->tree_maker, pos);
									JagPJCArrayAccess *tidx = jagp_tree_maker_indexed(priv->tree_maker, t, t1);
									cat_unref_ptr(t);
									t = (JagPJCExpression *) tidx;
								}
								l_accept(parser, JAGP_KIND_RBRACKET);
							}
							goto done;
						case JAGP_KIND_LPAREN:
							if ((priv->mode & JAGP_MODE_EXPR) != 0) {
								priv->mode = JAGP_MODE_EXPR;
								t = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, t);
								if (cat_array_wo_size(annos)>0) {
									cat_unref_ptr(t);
									t = l_illegal(parser, l_list_head_pos(annos));
								}
								typeArgs = NULL;
							}
							goto done;
						case JAGP_KIND_DOT:
							l_next_token(parser);
							int oldmode = priv->mode;
							priv->mode &= ~JAGP_MODE_NOPARAMS;
							typeArgs = l_type_arguments_opt(parser, JAGP_MODE_EXPR);
							priv->mode = oldmode;
							if ((priv->mode & JAGP_MODE_EXPR) != 0) {
								switch (priv->token->kind) {
								case JAGP_KIND_CLASS:
									if (typeArgs != NULL) return l_illegal(parser, priv->token->pos);
									priv->mode = JAGP_MODE_EXPR;
									jagp_tree_maker_at(priv->tree_maker, pos);
									t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, t, priv->names_class);
									l_next_token(parser);
									goto done;
								case JAGP_KIND_THIS:
									if (typeArgs != NULL) return l_illegal(parser, priv->token->pos);
									priv->mode = JAGP_MODE_EXPR;
									jagp_tree_maker_at(priv->tree_maker, pos);
									t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, t, priv->names_this);
									l_next_token(parser);
									goto done;
								case JAGP_KIND_SUPER:
									priv->mode = JAGP_MODE_EXPR;
									jagp_tree_maker_at(priv->tree_maker, pos);
									t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, t, priv->names_super);
									t = l_super_suffix(parser, typeArgs, t);
									typeArgs = NULL;
									goto done;
								case JAGP_KIND_NEW:
									if (typeArgs != NULL) {
										return l_illegal(parser, priv->token->pos);
									}
									priv->mode = JAGP_MODE_EXPR;
									int pos1 = priv->token->pos;
									l_next_token(parser);
									if (priv->token->kind == JAGP_KIND_LT) typeArgs = l_type_arguments(parser, FALSE);
									t = l_inner_creator(parser, pos1, typeArgs, t);
									typeArgs = NULL;
									goto done;
								}
							}
	
							CatArrayWo *tyannos = NULL;
							if ((priv->mode & JAGP_MODE_TYPE) != 0 && priv->token->kind == JAGP_KIND_MONKEYS_AT) {
								tyannos = l_type_annotations_opt(parser);
							}
							// typeArgs saved for next loop iteration.
							jagp_tree_maker_at(priv->tree_maker, pos);
							JagPJCFieldAccess *nfa = jagp_tree_maker_select(priv->tree_maker, t, l_ident(parser, FALSE));
							cat_unref_ptr(t);
							t = (JagPJCExpression *) nfa;
							if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
								jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
								JagPJCAnnotatedType *nat = jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, t);
								cat_unref_ptr(t);
								t = (JagPJCExpression *) nat;
							}
							break;
						case JAGP_KIND_ELLIPSIS:
	//						if (this.permitTypeAnnotationsPushBack) {
	//							this.typeAnnotationsPushedBack = annos;
	//						} else if (cat_array_wo_size(annos)>0) {
								// Don't return here -- error recovery attempt
							l_illegal(parser, l_list_head_pos(annos));
	//						}
							goto done;
						case JAGP_KIND_LT:
							if ((priv->mode & JAGP_MODE_TYPE) == 0 && l_is_unbound_member_ref(parser)) {
								//this is an unbound method reference whose qualifier
								//is a generic type i.e. A<S>::m
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
								JagPJCTypeApply *nta = jagp_tree_maker_type_apply(priv->tree_maker, (JagPJCTree *) t, args);
								cat_unref_ptr(t);
								t = (JagPJCExpression *) nta;
								while (priv->token->kind == JAGP_KIND_DOT) {
									l_next_token(parser);
									priv->mode = JAGP_MODE_TYPE;
									jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
									t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, t, l_ident(parser, FALSE));
									t = l_type_arguments_apply_opt(parser, t);
								}
								t = l_brackets_opt(parser, t, cat_array_wo_new());
								if (priv->token->kind != JAGP_KIND_COLCOL) {
									//method reference expected here
									t = l_illegal(parser, priv->token->pos);
								}
								priv->mode = JAGP_MODE_EXPR;
								return l_term3_rest(parser, t, typeArgs);
							}
							goto done;
						default:
							goto done;
					}
				}
			}
			done:
			if (typeArgs != NULL) {
				l_illegal(parser, priv->token->pos);
			}
			cat_log_debug("l_term3:t=%O", t);
			t = l_type_arguments_apply_opt(parser, t);
		}
		break;
		case JAGP_KIND_BYTE: case JAGP_KIND_SHORT: case JAGP_KIND_CHAR: case JAGP_KIND_INT: case JAGP_KIND_LONG: case JAGP_KIND_FLOAT:
		case JAGP_KIND_DOUBLE: case JAGP_KIND_BOOLEAN:
			if (typeArgs != NULL) {
				l_illegal(parser, priv->token->pos);
			}
			t = l_brackets_suffix(parser, l_brackets_opt(parser, (JagPJCExpression *) l_basic_type(parser), cat_array_wo_new()));
			break;
		case JAGP_KIND_VOID:
			if (typeArgs != NULL) {
				l_illegal(parser, priv->token->pos);
			}
			if ((priv->mode & JAGP_MODE_EXPR) != 0) {
				l_next_token(parser);
				if (priv->token->kind == JAGP_KIND_DOT) {
					jagp_tree_maker_at(priv->tree_maker, pos);
					JagPJCPrimitiveTypeTree *ti = jagp_tree_maker_type_ident(priv->tree_maker, JAGP_TT_VOID);
					t = l_brackets_suffix(parser, (JagPJCExpression *) ti);
				} else {
					return l_illegal(parser, pos);
				}
			} else {
				// Support the corner case of myMethodHandle.<void>invoke() by passing
				// a void type (like other primitive types) to the next phase.
				// The error will be reported in Attr.attribTypes or Attr.visitApply.
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCPrimitiveTypeTree *ti = jagp_tree_maker_type_ident(priv->tree_maker, JAGP_TT_VOID);
				l_next_token(parser);
				return (JagPJCExpression *) ti;
				//return l_illegal(parser, priv->token->pos);
			}
			break;
		default:
			cat_log_error("l_term3:returning illegal");
			return l_illegal(parser, priv->token->pos);
	}
	return l_term3_rest(parser, t, typeArgs);
}

static JagPJCExpression *l_term3_rest(JagPParser *parser, JagPJCExpression *t, CatArrayWo */*<JAgPJCExpression>*/ type_args) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_term3_rest:token=%d", priv->token->kind);
    if (type_args != NULL) {
    	l_illegal(parser, priv->token->pos);
    }
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
                    JagPJCExpression *nxt = l_brackets_opt(parser, t, cat_array_wo_new());
                    cat_unref_ptr(t);
                    jagp_tree_maker_at(priv->tree_maker, pos1);
                    t = (JagPJCExpression *) jagp_tree_maker_type_array(priv->tree_maker, nxt);
//                    t = toP(t);
                    cat_unref_ptr(nxt);
                    if (priv->token->kind == JAGP_KIND_COLCOL) {
                    	priv->mode = JAGP_MODE_EXPR;
                        continue;
                    }
                    if (cat_array_wo_size(annos)>0) {
                        jagp_tree_maker_at(priv->tree_maker, pos1);
                        JagPJCExpression *nxt = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annos, t);
                        cat_unref_ptr(t);
                        t = nxt;
//                        t = toP(t);
                    }
                    return t;
                }
                priv->mode = oldmode;
            }
            if ((priv->mode & JAGP_MODE_EXPR) != 0) {
            	priv->mode = JAGP_MODE_EXPR;
            	JagPJCExpression *indexed = t;
                JagPJCExpression *index = (JagPJCExpression *) l_term_raw(parser);
                jagp_tree_maker_at(priv->tree_maker, pos1);
                t = (JagPJCExpression *) jagp_tree_maker_indexed(priv->tree_maker, indexed, index);
//                t = to(t);
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
                JagPJCExpression *selected = t;
                t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, priv->names_super);
//                t = to(t);
                cat_unref_ptr(selected);
                l_next_token(parser);
                t = (JagPJCExpression *) l_arguments_apply(parser, type_args, t);
                cat_unref_ptr(type_args);
            } else if (priv->token->kind == JAGP_KIND_NEW && (priv->mode & JAGP_MODE_EXPR) != 0) {
                if (type_args != NULL) {
                	return l_illegal(parser, priv->token->pos);
                }
                priv->mode = JAGP_MODE_EXPR;
                int pos2 = priv->token->pos;
                l_next_token(parser);
                if (priv->token->kind == JAGP_KIND_LT){
                	type_args = l_type_arguments(parser, FALSE);
                }
                t = l_inner_creator(parser, pos2, type_args, t);
                cat_unref_ptr(type_args);
            } else {
                CatArrayWo * /*<JAgPJCAnnotation>*/ tyannos = NULL;
                if ((priv->mode & JAGP_MODE_TYPE) != 0 && priv->token->kind == JAGP_KIND_MONKEYS_AT) {
                    // is the mode check needed?
                    tyannos = l_type_annotations_opt(parser);
                }

                jagp_tree_maker_at(priv->tree_maker, pos1);
                JagPJCExpression *selected = t;
                JagPName *selector = l_ident(parser, TRUE);
                t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, selector);
//                t = toP(t);
                cat_unref_ptr(selected);
                cat_unref_ptr(selector);
                if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
                    jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
                    JagPJCExpression *underlying_type = t;
                    t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, underlying_type);
                    cat_unref_ptr(t);
//                    t = toP(t);
                }
                t = l_arguments_opt(parser, type_args, l_type_arguments_apply_opt(parser, t));
                cat_unref_ptr(type_args);
            }
        } else if ((priv->mode & JAGP_MODE_EXPR) != 0 && priv->token->kind == JAGP_KIND_COLCOL) {
            priv->mode = JAGP_MODE_EXPR;
            if (type_args != NULL) {
            	return l_illegal(parser, priv->token->pos);
            }
            l_accept(parser, JAGP_KIND_COLCOL);
            t = l_member_reference_suffix_ext(parser, pos1, t);
        } else {
            if (cat_array_wo_size(annos)>0) {
//                if (permitTypeAnnotationsPushBack)
//                    typeAnnotationsPushedBack = annos;
//                else
                    return l_illegal(parser, l_list_head_pos(annos));
            }
            break;
        }
    }
	cat_log_debug("t=%O", t);
    while ((priv->token->kind == JAGP_KIND_PLUSPLUS || priv->token->kind == JAGP_KIND_SUBSUB) && (priv->mode & JAGP_MODE_EXPR) != 0) {
        priv->mode = JAGP_MODE_EXPR;
        jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
        JagPJCExpression *arg = t;
        t = (JagPJCExpression *) jagp_tree_maker_unary(priv->tree_maker, priv->token->kind == JAGP_KIND_PLUSPLUS ? JAGP_TAG_POSTINC : JAGP_TAG_POSTDEC, arg);
        cat_unref_ptr(arg);
//        t = to(t);
        l_next_token(parser);
    }
//    return toP(t);
	cat_log_debug("l_term3_rest:done=%O", t);

    return t;
}

/**
 * If we see an identifier followed by a '&lt;' it could be an unbound
 * method reference or a binary expression. To disambiguate, look for a
 * matching '&gt;' and see if the subsequent terminal is either '.' or '::'.
 */
static gboolean l_is_unbound_member_ref(JagPParser *parser) {
    return FALSE;
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//    int pos = 0;
//    int depth = 0;
//    outer:
//	for(JagPToken *t = S.token(pos) ; ; t = S.token(++pos)) {
//        switch (t.kind) {
//            case JAGP_KIND_IDENTIFIER: case JAGP_KIND_UNDERSCORE: case JAGP_KIND_QUES: case JAGP_KIND_EXTENDS: case JAGP_KIND_SUPER:
//            case JAGP_KIND_DOT: case JAGP_KIND_RBRACKET: case JAGP_KIND_LBRACKET: case JAGP_KIND_COMMA:
//            case JAGP_KIND_BYTE: case JAGP_KIND_SHORT: case JAGP_KIND_INT: case JAGP_KIND_LONG: case JAGP_KIND_FLOAT:
//            case JAGP_KIND_DOUBLE: case JAGP_KIND_BOOLEAN: case JAGP_KIND_CHAR:
//            case JAGP_KIND_MONKEYS_AT:
//                break;
//
//            case JAGP_KIND_LPAREN:
//                // skip annotation values
//                int nesting = 0;
//                for (; ; pos++) {
//                    TokenKind tk2 = S.token(pos).kind;
//                    switch (tk2) {
//                        case EOF:
//                            return FALSE;
//                        case JAGP_KIND_LPAREN:
//                            nesting++;
//                            break;
//                        case JAGP_KIND_RPAREN:
//                            nesting--;
//                            if (nesting == 0) {
//                                continue outer;
//                            }
//                            break;
//                    }
//                }
//
//            case JAGP_KIND_LT:
//                depth++; break;
//            case JAGP_KIND_GTGTGT:
//                depth--;
//            case JAGP_KIND_GTGT:
//                depth--;
//            case JAGP_KIND_GT:
//                depth--;
//                if (depth == 0) {
//                    JagPTokenKind nextKind = S.token(pos + 1).kind;
//                    return
//                        nextKind == JAGP_KIND_DOT ||
//                        nextKind == JAGP_KIND_LBRACKET ||
//                        nextKind == JAGP_KIND_COLCOL;
//                }
//                break;
//            default:
//                return FALSE;
//        }
//    }
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
		switch(lah_token->kind) {
			case JAGP_KIND_COMMA :
				type = TRUE;
			case JAGP_KIND_EXTENDS : case JAGP_KIND_SUPER : case JAGP_KIND_DOT : case JAGP_KIND_AMP :
				// skip
				break;

			case JAGP_KIND_QUES :
				if (l_peek_token_lah(parser, lookahead, JAGP_KIND_EXTENDS) || l_peek_token_lah(parser, lookahead, JAGP_KIND_SUPER)) {
					// wildcards
					type = TRUE;
				}
				break;

			case JAGP_KIND_BYTE : case JAGP_KIND_SHORT : case JAGP_KIND_INT : case JAGP_KIND_LONG : case JAGP_KIND_FLOAT :
			case JAGP_KIND_DOUBLE : case JAGP_KIND_BOOLEAN : case JAGP_KIND_CHAR : case JAGP_KIND_VOID :
				if (l_peek_token_lah(parser, lookahead, JAGP_KIND_RPAREN)) {
					// Type, ')' -> cast
					return JAGP_PAR_CAST;
//				} else if (l_peek_token_lah(parser, lookahead, JAGP_KIND_LAX_IDENTIFIER)) {
//					// Type, Identifier/'_'/'assert'/'enum' -> explicit lambda
//					return JAGP_PAR_EXPLICIT_LAMBDA;
				}
				break;
			case JAGP_KIND_LPAREN:
				if (lookahead != 0) {
					// '(' in a non-starting position -> parens
					return JAGP_PAR_PARENS;
				} else if (l_peek_token_lah(parser, lookahead, JAGP_KIND_RPAREN)) {
					// '(', ')' -> explicit lambda
					return JAGP_PAR_EXPLICIT_LAMBDA;
				}
				break;
			case JAGP_KIND_RPAREN:
				// if we have seen something that looks like a type,
				// then it's a cast expression
				if (type) {
					return JAGP_PAR_CAST;
				}
				// otherwise, disambiguate cast vs. parenthesized expression
				// based on subsequent token.
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
//            case UNDERSCORE:
//            case ASSERT:
//            case ENUM:
//            case IDENTIFIER:
//                if (peekToken(lookahead, LAX_IDENTIFIER)) {
//                    // Identifier, Identifier/'_'/'assert'/'enum' -> explicit lambda
//                    return JAGP_PAR_EXPLICIT_LAMBDA;
//                } else if (peekToken(lookahead, JAGP_KIND_RPAREN, ARROW)) {
//                    // Identifier, ')' '->' -> implicit lambda
//                    return ParensResult.IMPLICIT_LAMBDA;
//                }
//                type = FALSE;
//                break;
//            case FINAL:
//            case ELLIPSIS:
//                //those can only appear in explicit lambdas
//                return JAGP_PAR_EXPLICIT_LAMBDA;
//            case MONKEYS_AT:
//                type = true;
//                lookahead += 1; //skip '@'
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
//                break;
//            case LBRACKET:
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
//                    return JAGP_PAR_PARENS;
//                }
//            case JAGP_KIND_LT:
//                depth++; break;
//            case GTGTGT:
//                depth--;
//            case GTGT:
//                depth--;
//            case GT:
//                depth--;
//                if (depth == 0) {
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
//                } else if (depth < 0) {
//                    //unbalanced '<', '>' - not a generic type
//                    return JAGP_PAR_PARENS;
//                }
//                break;
			default : 
				// this includes EOF
				return JAGP_PAR_PARENS;
		}
	}
	return JAGP_PAR_PARENS;
}

/** Accepts all identifier-like tokens */
//static Filter<TokenKind> LAX_IDENTIFIER = new Filter<TokenKind>() {
//	public boolean accepts(TokenKind t) {
//		return t == IDENTIFIER || t == UNDERSCORE || t == ASSERT || t == ENUM;
//	}
//};

static JagPJCExpression *l_lambda_expression_or_statement(JagPParser *parser, gboolean hasParens, gboolean explicitParams, int pos) {
	CatArrayWo */*JagPJCVariableDecl>*/ params = explicitParams ?
			l_formal_parameters(parser, TRUE) :
			l_implicit_parameters(parser, hasParens);

	return l_lambda_expression_or_statement_rest(parser, params, pos);
}

static JagPJCExpression *l_lambda_expression_or_statement_rest(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_check_lambda(parser);
	l_accept(parser, JAGP_KIND_ARROW);

	return priv->token->kind == JAGP_KIND_LBRACE ?
		l_lambda_statement(parser, args, pos, pos) :
		l_lambda_expression(parser, args, pos);
}

static JagPJCExpression *l_lambda_statement(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos, int pos2) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCBlock *block = l_block_ext(parser, pos2, 0);
	jagp_tree_maker_at(priv->tree_maker, pos2);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_lambda(priv->tree_maker, args, (JagPJCTree *) block);
	cat_unref_ptr(block);
//	t = toP(t);
	return t;
}

static JagPJCExpression *l_lambda_expression(JagPParser *parser, CatArrayWo */*JagPJCVariableDecl>*/ args, int pos) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCTree *expr = (JagPJCTree *) l_parse_expression(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_lambda(priv->tree_maker, args, expr);
	cat_unref_ptr(expr);
	//	t = toP(t);
	return t;
}

/** SuperSuffix = Arguments | "." [TypeArguments] Ident [Arguments]
 */
static JagPJCExpression *l_super_suffix(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_next_token(parser);
	if (priv->token->kind == JAGP_KIND_LPAREN || typeArgs != NULL) {
		t = (JagPJCExpression *) l_arguments_apply(parser, typeArgs, t);
	} else if (priv->token->kind == JAGP_KIND_COLCOL) {
		if (typeArgs != NULL) {
			return l_illegal(parser, priv->token->pos);
		}
		t = l_member_reference_suffix(parser, t);
	} else {
		int pos = priv->token->pos;
		l_accept(parser, JAGP_KIND_DOT);
		typeArgs = (priv->token->kind == JAGP_KIND_LT) ? l_type_arguments(parser, FALSE) : NULL;
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *selected = t;
		JagPName *selector = l_ident(parser, FALSE);
		t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, selector);
//		t = toP(t);
		cat_unref_ptr(selected);
		cat_unref_ptr(selector);
		t = l_arguments_opt(parser, typeArgs, t);
	}
	return t;
}

/** BasicType = BYTE | SHORT | CHAR | INT | LONG | FLOAT | DOUBLE | BOOLEAN
 */
static JagPJCPrimitiveTypeTree *l_basic_type(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPJCPrimitiveTypeTree *t = jagp_tree_maker_type_ident(priv->tree_maker, l_typetag(priv->token->kind));
//	t= to(t);
	l_next_token(parser);
	return t;
}

/** ArgumentsOpt = [ Arguments ]
 */
static JagPJCExpression *l_arguments_opt(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (((priv->mode & JAGP_MODE_EXPR) != 0) && (priv->token->kind == JAGP_KIND_LPAREN || typeArgs != NULL)) {
		priv->mode = JAGP_MODE_EXPR;
		return (JagPJCExpression *) l_arguments_apply(parser, typeArgs, t);
	} else {
		return t;
	}
}

/** Arguments = "(" [Expression { COMMA Expression }] ")"
 */
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

static JagPJCMethodInvocation *l_arguments_apply(JagPParser *parser, CatArrayWo */*<JagPJCExpression>*/ typeArgs, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	CatArrayWo */*<JagPJCExpression>*/ args = l_arguments(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *fn = t;
	JagPJCMethodInvocation *result = jagp_tree_maker_apply(priv->tree_maker, typeArgs, fn, args);
	cat_unref_ptr(fn);
//	result = toP(result);
	return result;
}

/**  TypeArgumentsOpt = [ TypeArguments ]
 */
static JagPJCExpression *l_type_arguments_apply_opt(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	if (priv->token->kind == JAGP_KIND_LT &&
			(priv->mode & JAGP_MODE_TYPE) != 0 &&
			(priv->mode & JAGP_MODE_NOPARAMS) == 0) {
		priv->mode = JAGP_MODE_TYPE;
		return (JagPJCExpression *) l_type_arguments_apply(parser, t, FALSE);
	} else {
		return t;
	}
}

static CatArrayWo */*<JagPJCExpression>*/ l_type_arguments_opt(JagPParser *parser, int useMode) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_type_arguments_opt:token=%d", priv->token->kind);
	if (priv->token->kind == JAGP_KIND_LT) {
		if ((priv->mode & useMode) == 0 ||
			(priv->mode & JAGP_MODE_NOPARAMS) != 0) {
			l_illegal(parser, priv->token->pos);
		}
		priv->mode = useMode;
		return l_type_arguments(parser, FALSE);
	}
	return NULL;
}

/**
 *  {@literal
 *  TypeArguments  = "<" TypeArgument {"," TypeArgument} ">"
 *  }
 */
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
			return args;
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
static JagPJCExpression *l_type_argument(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCAnnotation>*/ annotations = l_type_annotations_opt(parser);
	if (priv->token->kind != JAGP_KIND_QUES) {
		return l_parse_type_with_annotations(parser, annotations);
	}
	int pos = priv->token->pos;
	l_next_token(parser);
	JagPJCExpression *result;
	if (priv->token->kind == JAGP_KIND_EXTENDS) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_EXTENDS);
//		t= to(t);
		l_next_token(parser);
		JagPJCExpression *bound = l_parse_type(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker, t, (JagPJCTree *) bound);
	} else if (priv->token->kind == JAGP_KIND_SUPER) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_SUPER);
//		t= to(t);
		l_next_token(parser);
		JagPJCExpression *bound = l_parse_type(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker,t, (JagPJCTree *) bound);
	} else if (lax_identifier_accepts(priv->token->kind)) {
		//error recovery
		jagp_tree_maker_at(priv->tree_maker, -1);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_UNBOUND);

		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *wc = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker,t,NULL);
//		wc = toP(wc);
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		JagPJCIdent *id = jagp_tree_maker_ident(priv->tree_maker, l_ident(parser, FALSE));
//		id = toP(id);
		jagp_tree_maker_at(priv->tree_maker, pos);
		CatArrayWo *errs = cat_array_wo_new();
		cat_array_wo_append(errs, (GObject *) wc);
		cat_array_wo_append(errs, (GObject *) id);
		JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
		cat_unref_ptr(errs);
//		reportSyntaxError(err, "expected3", JAGP_KIND_GT, JAGP_KIND_EXTENDS, JAGP_KIND_SUPER);
		result = (JagPJCExpression *) err;
	} else {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPTypeBoundKind *t = jagp_tree_maker_type_bound_kind(priv->tree_maker, JAGP_BOUND_KIND_UNBOUND);
//		t= toP(t);
		jagp_tree_maker_at(priv->tree_maker, pos);
		result = (JagPJCExpression *) jagp_tree_maker_wildcard(priv->tree_maker,t,NULL);
//		result = toP(result);
	}
	if (cat_array_wo_size(annotations)>0) {

		jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(annotations));
		JagPJCExpression *underlying_type = result;
		result = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annotations, underlying_type);
		cat_unref_ptr(annotations);
		cat_unref_ptr(underlying_type);
//		result = toP(result);
	}
	return result;
}

static JagPJCTypeApply *l_type_arguments_apply(JagPParser *parser, JagPJCExpression *t, gboolean diamondAllowed) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	CatArrayWo */*<JagPJCExpression>*/ args = l_type_arguments(parser, diamondAllowed);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTypeApply *result = jagp_tree_maker_type_apply(priv->tree_maker, (JagPJCTree *) t, args);
	cat_unref_ptr(t);
//	result= toP(result);
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
static JagPJCExpression *l_brackets_opt(JagPParser *parser, JagPJCExpression *t, CatArrayWo */*<JagPJCAnnotation>*/ annotations) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCAnnotation>*/ nextLevelAnnotations = l_type_annotations_opt(parser);

	if (priv->token->kind == JAGP_KIND_LBRACKET) {
		int pos = priv->token->pos;
		l_next_token(parser);
		t = l_brackets_opt_cont(parser, t, pos, nextLevelAnnotations);
	} else if (cat_array_wo_size(nextLevelAnnotations)>0) {
//		if (permitTypeAnnotationsPushBack) {
//			this.typeAnnotationsPushedBack = nextLevelAnnotations;
//		} else {
			return l_illegal(parser, l_list_head_pos(nextLevelAnnotations));
//		}
	}

	if (cat_array_wo_size(annotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
		JagPJCExpression *underlying_type = t;
		t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annotations, underlying_type);
		cat_unref_ptr(annotations);
		cat_unref_ptr(underlying_type);
//		t = toP(t);
	}
	return t;
}

/** BracketsOpt = [ "[" "]" { [Annotations] "[" "]"} ]
 */
static JagPJCExpression *l_brackets_opt_simple(JagPParser *parser, JagPJCExpression *t) {
	CatArrayWo *nil_list = cat_array_wo_new();
	JagPJCExpression *result = l_brackets_opt(parser, t, nil_list);
	cat_unref_ptr(nil_list);
	return result;
}

static JagPJCExpression *l_brackets_opt_cont(JagPParser *parser, JagPJCExpression *t, int pos, CatArrayWo */*<JagPJCAnnotation>*/ annotations) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_RBRACKET);
	t = l_brackets_opt_simple(parser, t);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *elemtype = t;
	t = (JagPJCExpression *) jagp_tree_maker_type_array(priv->tree_maker, elemtype);
//	t = toP(t);
	cat_unref_ptr(elemtype)
	if (cat_array_wo_size(annotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *underlying_type = t;
		t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, annotations, underlying_type);
//		t = toP(t);
		cat_unref_ptr(underlying_type);
	}
	return t;
}

/** BracketsSuffixExpr = "." CLASS
 *  BracketsSuffixType =
 */
static JagPJCExpression *l_brackets_suffix(JagPParser *parser, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
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
//			fldacc = toP(fldacc);
//			cat_array_wo_append(errs, fldacc);
//			jagp_tree_maker_at(priv->tree_maker, pos);
//			t = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//			cat_unref_ptr(errs);
//		} else {
			JagPTag tag = jagp_jctree_get_tag((JagPJCTree *) t);

			// Type annotations are illegal on class literals. Annotated non array class literals
			// are complained about directly in term3(), Here check for type annotations on dimensions
			// taking care to handle some interior dimension(s) being annotated.
//			if ((tag == JAGP_TAG_TYPEARRAY && TreeInfo.containsTypeAnnotation(t)) || tag == JAGP_TAG_ANNOTATED_TYPE) {
//				syntaxError("no.annotations.on.dot.class");
//			}
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCExpression *selected = t;
			t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, priv->names_class);
			cat_unref_ptr(selected);
//			t = toP(t);
//		}
	} else if ((priv->mode & JAGP_MODE_TYPE) != 0) {
		if (priv->token->kind != JAGP_KIND_COLCOL) {
			priv->mode = JAGP_MODE_TYPE;
		}
//	} else if (priv->token->kind != JAGP_KIND_COLCOL) {
//		syntaxError(priv->token->pos, "dot.class.expected");
	}
	return t;
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
		refName = priv->names_init;
		l_next_token(parser);
	} else {
		refMode = JAGP_REFERENCE_MODE_INVOKE;
		refName = l_ident(parser, FALSE);
	}
	jagp_tree_maker_at(priv->tree_maker, jagp_jctree_get_start_position((JagPJCTree *) t));
	JagPJCExpression *expr = t;
	t = (JagPJCExpression *) jagp_tree_maker_reference(priv->tree_maker, refMode, refName, expr, typeArgs);
	cat_unref_ptr(expr);
//	t = toP(t);
	return t;
}

/** Creator = [Annotations] Qualident [TypeArguments] ( ArrayCreatorRest | ClassCreatorRest )
 */
static JagPJCExpression *l_creator(JagPParser *parser, int newpos, CatArrayWo */*<JagPJCExpression>*/ typeArgs) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	CatArrayWo */*<JagPJCAnnotation>*/ newAnnotations = l_type_annotations_opt(parser);

	switch (priv->token->kind) {
		case JAGP_KIND_BYTE: case JAGP_KIND_SHORT: case JAGP_KIND_CHAR: case JAGP_KIND_INT: case JAGP_KIND_LONG: case JAGP_KIND_FLOAT:
		case JAGP_KIND_DOUBLE: case JAGP_KIND_BOOLEAN : {
			if (typeArgs == NULL) {
				if (cat_array_wo_size(newAnnotations)==0) {
					return l_array_creator_rest(parser, newpos, (JagPJCExpression *) l_basic_type(parser));
				} else {
					jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(newAnnotations));
					JagPJCAnnotatedType *annotype = jagp_tree_maker_annotated_type(priv->tree_maker, newAnnotations, (JagPJCExpression *) l_basic_type(parser));
	//				annotype = toP(annotype);
					return l_array_creator_rest(parser, newpos, (JagPJCExpression *) annotype);
				}
			}
			break;
		}
		default : {
		}
	}
	JagPJCExpression *t = l_qualident(parser, TRUE);

	int oldmode = priv->mode;
	priv->mode = JAGP_MODE_TYPE;
	gboolean diamondFound = FALSE;
	int lastTypeargsPos = -1;
	if (priv->token->kind == JAGP_KIND_LT) {
		lastTypeargsPos = priv->token->pos;
		t = (JagPJCExpression *) l_type_arguments_apply(parser, t, TRUE);
		diamondFound = (priv->mode & JAGP_MODE_DIAMOND) != 0;
	}
	while (priv->token->kind == JAGP_KIND_DOT) {
		if (diamondFound) {
			//cannot select after a diamond
			l_illegal(parser, priv->token->pos);
		}
		int pos = priv->token->pos;
		l_next_token(parser);
		CatArrayWo */*<JagPJCAnnotation>*/ tyannos = l_type_annotations_opt(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpression *selected = t;
		JagPName *selector = l_ident(parser, FALSE);
		t = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, selected, selector);
		cat_unref_ptr(selected);
		cat_unref_ptr(selector);
//		t = toP(t);

		if (tyannos != NULL && cat_array_wo_size(tyannos)>0) {
			jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(tyannos));
			JagPJCExpression *underlying_type = t;
			t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, tyannos, underlying_type);
			cat_unref_ptr(tyannos);
			cat_unref_ptr(underlying_type);
//			t = toP(t);
		}

		if (priv->token->kind == JAGP_KIND_LT) {
			lastTypeargsPos = priv->token->pos;
			t = (JagPJCExpression *) l_type_arguments_apply(parser, t, TRUE);
			diamondFound = (priv->mode & JAGP_MODE_DIAMOND) != 0;
		}
	}
	priv->mode = oldmode;
	if (priv->token->kind == JAGP_KIND_LBRACKET || priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		// handle type annotations for non primitive arrays
		if (cat_array_wo_size(newAnnotations)>0) {
			t = l_insert_annotations_to_most_inner(parser, t, newAnnotations, FALSE);
		}

		JagPJCExpression *e = l_array_creator_rest(parser, newpos, t);
		if (diamondFound) {
//			reportSyntaxError(lastTypeargsPos, "cannot.create.array.with.diamond");
			jagp_tree_maker_at(priv->tree_maker, newpos);
			CatArrayWo *errs = cat_array_wo_new();
			cat_array_wo_append(errs, (GObject *) e);
			JagPJCErroneous *res = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//			res = toP(res);
			return (JagPJCExpression *) res;
		}
		else if (typeArgs != NULL) {
			int pos = newpos;
			if (cat_array_wo_size(typeArgs)>0 && l_list_head_pos(typeArgs) != -1) {
				// note: this should always happen but we should
				// not rely on this as the parser is continuously
				// modified to improve error recovery.
				pos = l_list_head_pos(typeArgs);
			}
//			setErrorEndPos(S.prevToken().endPos);
			jagp_tree_maker_at(priv->tree_maker, pos);
			CatArrayWo *errs = cat_array_wo_clone(typeArgs, CAT_CLONE_DEPTH_FULL);
			cat_array_wo_insert(errs, (GObject *) e, 0);
			JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//			reportSyntaxError(err, "cannot.create.array.with.type.arguments");
//			err = toP(err);
			return (JagPJCExpression *) err;
		}
		return e;
	} else if (priv->token->kind == JAGP_KIND_LPAREN) {
		JagPJCNewClass *newClass = l_class_creator_rest(parser, newpos, NULL, typeArgs, t);
		if (newClass->def != NULL) {
			if (cat_array_wo_size(newAnnotations)>0) {
				// Add type and declaration annotations to the new class;
				// com.sun.tools.javac.code.TypeAnnotations.TypeAnnotationPositions.visitNewClass(JCNewClass)
				// will later remove all type annotations and only leave the
				// declaration annotations.
				newClass->def->modifiers->parent.pos = l_earlier(newClass->def->modifiers->parent.pos, l_list_head_pos(newAnnotations));
				cat_unref_ptr(newClass->def->modifiers->annotations);
				newClass->def->modifiers->annotations = newAnnotations;
			}
		} else {
			// handle type annotations for instantiations
			if (cat_array_wo_size(newAnnotations)>0) {
				t = l_insert_annotations_to_most_inner(parser, t, newAnnotations, FALSE);
				newClass->clazz = t;
			}
		}
		return (JagPJCExpression *) newClass;
	} else {
//		setErrorEndPos(priv->token->pos);
//		reportSyntaxError(priv->token->pos, "expected2", JAGP_KIND_LPAREN, JAGP_KIND_LBRACKET);
		jagp_tree_maker_at(priv->tree_maker, newpos);
		JagPJCExpression *expr = t;
		t = (JagPJCExpression *) jagp_tree_maker_new_class(priv->tree_maker, NULL, typeArgs, expr, cat_array_wo_new(), NULL);
		cat_unref_ptr(expr);
//		t = toP(t);
		CatArrayWo *errs = cat_array_wo_new();
		cat_array_wo_append(errs, (GObject *) t);
		jagp_tree_maker_at(priv->tree_maker, newpos);
		JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, errs);
//		err = toP(err);
		return (JagPJCExpression *) err;
	}
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
//	t = toP(t);

	if (cat_array_wo_size(newAnnotations)>0) {
		jagp_tree_maker_at(priv->tree_maker, l_list_head_pos(newAnnotations));
		JagPJCExpression *underlying_type = t;
		t = (JagPJCExpression *) jagp_tree_maker_annotated_type(priv->tree_maker, newAnnotations, underlying_type);
		cat_unref_ptr(underlying_type);
		//	t = toP(t);
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
				na->annotations = annotated->annotations;
				na->elemtype = annotated->underlying_type;
			}
			return (JagPJCExpression *) na;
		} else {
			jagp_tree_maker_at(priv->tree_maker, newpos);
			JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_new_array(priv->tree_maker, elemtype, cat_array_wo_new(), NULL);
//			t = toP(t);
			CatArrayWo *lst = cat_array_wo_new();
			cat_array_wo_append(lst, (GObject *) t);
//			return syntaxError(priv->token->pos, lst, "array.dimension.missing");
			return NULL;
		}
	} else {
		CatArrayWo /*<JagPJCExpression>*/ *dims = cat_array_wo_new();

		// maintain array dimension type annotations
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
				if (priv->token->kind == JAGP_KIND_RBRACKET) { // no dimension
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
//		na = toP(na);
		cat_unref_ptr(na->dimAnnotations);
		na->dimAnnotations = dimAnnotations;
		return (JagPJCExpression *) na;
	}
}

/** ClassCreatorRest = Arguments [ClassBody]
 */
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
//		body = toP(body);
	}
	jagp_tree_maker_at(priv->tree_maker, newpos);
	JagPJCNewClass *ncls = jagp_tree_maker_new_class(priv->tree_maker, encl, typeArgs, t, args, body);
//	ncls = toP(ncls);
	return ncls;
}

/** ArrayInitializer = "{" [VariableInitializer {"," VariableInitializer}] [","] "}"
 */
static JagPJCExpression *l_array_initializer(JagPParser *parser, int newpos, JagPJCExpression *t) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_LBRACE);
	CatArrayWo *elems = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);
	} else if (priv->token->kind != JAGP_KIND_RBRACE) {
		cat_array_wo_append(elems, (GObject *) l_variable_initializer(parser));
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			if (priv->token->kind == JAGP_KIND_RBRACE) break;
			cat_array_wo_append(elems, (GObject *) l_variable_initializer(parser));
		}
	}
	l_accept(parser, JAGP_KIND_RBRACE);
	jagp_tree_maker_at(priv->tree_maker, newpos);
	JagPJCNewArray *res = jagp_tree_maker_new_array(priv->tree_maker, t, cat_array_wo_new(), elems);
//	res = toP(res);
	return (JagPJCExpression *) res;
}

/** VariableInitializer = ArrayInitializer | Expression
 */
static JagPJCExpression *l_variable_initializer(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return priv->token->kind == JAGP_KIND_LBRACE ? l_array_initializer(parser, priv->token->pos, NULL) : l_parse_expression(parser);
}

/** ParExpression = "(" Expression ")"
 */
static JagPJCExpression *l_par_expression(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_LPAREN);
	JagPJCExpression *expr = l_parse_expression(parser);
	l_accept(parser, JAGP_KIND_RPAREN);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpression *t = (JagPJCExpression *) jagp_tree_maker_parens(priv->tree_maker, expr);
	cat_unref_ptr(expr);
//	t = toP(t);
	return t;
}

/** Block = "{" BlockStatements "}"
 */
static JagPJCBlock *l_block_ext(JagPParser *parser, int pos, long flags) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	l_accept(parser, JAGP_KIND_LBRACE);
	CatArrayWo */*<JCStatement>*/ stats = l_block_statements(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCBlock *t = jagp_tree_maker_block(priv->tree_maker, flags, stats);
	while (priv->token->kind == JAGP_KIND_CASE || priv->token->kind == JAGP_KIND_DEFAULT) {
//		syntaxError("orphaned", priv->token->kind);
		l_switch_block_statement_groups(parser);
	}
	// the Block node has a field "endpos" for first char of last token, which is
	// usually but not necessarily the last char of the last token.
	t->endpos = priv->token->pos;
	l_accept(parser, JAGP_KIND_RBRACE);
//	return toP(t);
	return t;
}

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
static CatArrayWo /*<JCStatement>*/ *l_block_statements(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	//todo: skip to anchor on error(?)
	int lastErrPos = -1;
	CatArrayWo */*<JagPJCStatement>*/ stats = cat_array_wo_new();
	while (TRUE) {
		CatArrayWo */*<JCStatement>*/ stat = l_block_statement(parser);
		cat_log_debug("stat.size=%d", cat_array_wo_size(stat));
		if (cat_array_wo_size(stat)==0) {
			cat_unref_ptr(stat);
			return stats;
		} else {
			// error recovery
			if (priv->token->pos == lastErrPos)
				return stats;
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
static JagPJCStatement *l_parse_statement_as_block(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	CatArrayWo */*<JCStatement>*/ stats = l_block_statement(parser);
	if (cat_array_wo_size(stats)==0) {
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCErroneous *e = jagp_tree_maker_erroneous(priv->tree_maker, NULL);
//		error(e, "illegal.start.of.stmt");
		jagp_tree_maker_at(priv->tree_maker, pos);
		return (JagPJCStatement *) jagp_tree_maker_exec(priv->tree_maker, (JagPJCExpression *) e);
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
		}
		if (error != NULL) {
//			error(first, error);
			jagp_tree_maker_at(priv->tree_maker, first->parent.pos);
			JagPJCBlock *block = jagp_tree_maker_block(priv->tree_maker, 0, stats);
			CatArrayWo /*<JCBlock>*/ *blist = cat_array_wo_new();
			cat_array_wo_append(blist, (GObject *) block);
			jagp_tree_maker_at(priv->tree_maker, first->parent.pos);
			JagPJCErroneous *err = jagp_tree_maker_erroneous(priv->tree_maker, blist);

			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_exec(priv->tree_maker, (JagPJCExpression *) err);
//			result = toP(result);
			return result;
		}
		return first;
	}
}

/**This method parses a statement appearing inside a block.
 */
static CatArrayWo */*<JCStatement>*/ l_block_statement(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_block_statement:token=%d", priv->token->kind);
	//todo: skip to anchor on error(?)
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
			if (priv->token->kind == JAGP_KIND_INTERFACE ||
					priv->token->kind == JAGP_KIND_CLASS ||
					priv->token->kind == JAGP_KIND_ENUM) {
				GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
				CatArrayWo *result = cat_array_wo_new();
				cat_array_wo_append(result, clorinorende);
				cat_unref_ptr(clorinorende);
				return result;
			} else {
				JagPJCExpression *t = l_parse_type(parser);
				CatArrayWo /*<JCStatement> */ *stats =
						l_variable_declarators(parser, mods, t, cat_array_wo_new());
				// A "LocalVariableDeclarationStatement" subsumes the terminating semicolon
				l_accept(parser, JAGP_KIND_SEMI);
	//			storeEnd(stats.last(), S.prevToken().endPos);
				return stats;
			}
		}
		case JAGP_KIND_ABSTRACT : case JAGP_KIND_STRICTFP : {
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			return result;
		}
		case JAGP_KIND_INTERFACE :
		case JAGP_KIND_CLASS : {
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, l_modifiers_opt(parser, NULL), dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			return result;
		}
		case JAGP_KIND_ENUM : {
//			error(priv->token->pos, "local.enum");
			JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, l_modifiers_opt(parser, NULL), dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			return result;
		}
		default : {
			JagPToken *prevToken = priv->token;
			JagPJCExpression *t = l_term(parser, JAGP_MODE_EXPR | JAGP_MODE_TYPE);
			if (priv->token->kind == JAGP_KIND_COLON && jagp_jctree_has_tag((JagPJCTree *) t, JAGP_TAG_IDENT)) {
				l_next_token(parser);
				JagPJCStatement *stat = l_parse_statement_as_block(parser);
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCLabeledStatement *labstat = jagp_tree_maker_labelled(priv->tree_maker, jagp_token_name(prevToken), stat);
				CatArrayWo *result = cat_array_wo_new();
				cat_array_wo_append(result, (GObject *) labstat);
				return result;
			} else if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && lax_identifier_accepts(priv->token->kind)) {
				pos = priv->token->pos;
				jagp_tree_maker_at(priv->tree_maker, -1);
				JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, 0, cat_array_wo_new());
				jagp_tree_maker_at(priv->tree_maker, pos);
				CatArrayWo /*<JagPJCStatement>*/ *stats =
						l_variable_declarators(parser, mods, t, cat_array_wo_new());
				// A "LocalVariableDeclarationStatement" subsumes the terminating semicolon
				l_accept(parser, JAGP_KIND_SEMI);
	//			storeEnd(stats.last(), S.prevToken().endPos);
				return stats;
			} else {
				// This Exec is an "ExpressionStatement"; it subsumes the terminating semicolon
				t = l_check_expr_stat(parser, t);
				l_accept(parser, JAGP_KIND_SEMI);
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCExpressionStatement *expr = jagp_tree_maker_exec(priv->tree_maker, t);
	//			expr = toP(expr);
				CatArrayWo *result = cat_array_wo_new();
				cat_array_wo_append(result, (GObject *) expr);
				cat_unref_ptr(expr);
				return result;
			}
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
				JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_foreach_loop(priv->tree_maker, var, expr, body);
				return result;
			} else {
				l_accept(parser, JAGP_KIND_SEMI);
				JagPJCExpression *cond = priv->token->kind == JAGP_KIND_SEMI ? NULL : l_parse_expression(parser);
				l_accept(parser, JAGP_KIND_SEMI);
				CatArrayWo /*<JCExpressionStatement>*/ *steps = priv->token->kind == JAGP_KIND_RPAREN ? cat_array_wo_new() : l_for_update(parser);
				l_accept(parser, JAGP_KIND_RPAREN);
				JagPJCStatement *body = l_parse_statement_as_block(parser);
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_for_loop(priv->tree_maker, inits, cond, steps, body);
				return result;
			}
		}
		case JAGP_KIND_WHILE : {
			l_next_token(parser);
			JagPJCExpression *cond = l_par_expression(parser);
			JagPJCStatement *body = l_parse_statement_as_block(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCStatement *result = (JagPJCStatement *) jagp_tree_maker_while_loop(priv->tree_maker, cond, body);
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
//			result = toP(result);
			return result;
		}
		case JAGP_KIND_TRY : {
			l_next_token(parser);
			CatArrayWo */*<JCTree>*/ resources = cat_array_wo_new();
			if (priv->token->kind == JAGP_KIND_LPAREN) {
				l_check_try_with_resources(parser);
				l_next_token(parser);
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
			return (JagPJCStatement *) jagp_tree_maker_try(priv->tree_maker, resources, body, catchers, finalizer);
		}
		case JAGP_KIND_SWITCH : {
			l_next_token(parser);
			JagPJCExpression *selector = l_par_expression(parser);
			l_accept(parser, JAGP_KIND_LBRACE);
			CatArrayWo /*<JCCase>*/ *cases = l_switch_block_statement_groups(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCSwitch *t = jagp_tree_maker_switch(priv->tree_maker, selector, cases);
//			t = to(t);
			l_accept(parser, JAGP_KIND_RBRACE);
			return (JagPJCStatement *) t;
		}
		case JAGP_KIND_SYNCHRONIZED : {
			l_next_token(parser);
			JagPJCExpression *lock = l_par_expression(parser);
			JagPJCBlock *body = l_block(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCSynchronized *jsync = jagp_tree_maker_synchronized(priv->tree_maker, lock, body);
			return (JagPJCStatement *) jsync;
		}
		case JAGP_KIND_RETURN : {
			l_next_token(parser);
			JagPJCExpression *result = priv->token->kind == JAGP_KIND_SEMI ? NULL : l_parse_expression(parser);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCReturn *t = jagp_tree_maker_return(priv->tree_maker, result);
//			t = toP(t);
			return (JagPJCStatement *) t;
		}
		case JAGP_KIND_THROW : {
			l_next_token(parser);
			JagPJCExpression *exc = l_parse_expression(parser);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCThrow *t = jagp_tree_maker_throw(priv->tree_maker, exc);
//			t = toP(t);
			return (JagPJCStatement *) t;
		}
		case JAGP_KIND_BREAK : {
			l_next_token(parser);
			JagPName *label = lax_identifier_accepts(priv->token->kind) ? l_ident(parser, FALSE) : NULL;
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCBreak *t = jagp_tree_maker_break(priv->tree_maker, label);
//			t = toP(t);
			return (JagPJCStatement *) t;
		}
		case JAGP_KIND_CONTINUE : {
			l_next_token(parser);
			JagPName *label = lax_identifier_accepts(priv->token->kind) ? l_ident(parser, FALSE) : NULL;
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCContinue *t = jagp_tree_maker_continue(priv->tree_maker, label);
//			t =  toP(t);
			return (JagPJCStatement *) t;
		}
		case JAGP_KIND_SEMI : {
			l_next_token(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCSkip *skip = jagp_tree_maker_skip(priv->tree_maker);
//			skip = toP(skip);
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
//			t = toP(t);
			return (JagPJCStatement *) t;
		}
		default:
//			Assert.error();
			return NULL;
	}
}

static JagPJCStatement *l_do_recover(JagPParser *parser, int startPos, JagPBasicErrorRecoveryAction action, CatStringWo *key) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	int errPos = S.errPos();
	JagPJCTree *stm = action(parser);
//	S.errPos(errPos);
	CatArrayWo *errs = cat_array_wo_new();
	cat_array_wo_append(errs, (GObject *) stm);
	JagPJCExpression *err = NULL; // syntaxError(startPos, errs, key);
	JagPJCExpressionStatement *exprstat = jagp_tree_maker_exec(priv->tree_maker, err);
//	exprstat = toP(exprstat);
	return (JagPJCStatement *) exprstat;
}

/** CatchClause     = CATCH "(" FormalParameter ")" Block
 * TODO: the "FormalParameter" is not correct, it uses the special "catchTypes" rule below.
 */
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
//		paramType = toP(paramType);
	} else {
		paramType = (JagPJCExpression *) cat_array_wo_get_first(catchTypes);
	}
	JagPJCVariableDecl *formal = l_variable_declarator_id(parser, mods, paramType, FALSE);
	l_accept(parser, JAGP_KIND_RPAREN);
	JagPJCBlock *body = l_block(parser);
	jagp_tree_maker_at(priv->tree_maker, pos);
	return jagp_tree_maker_catch(priv->tree_maker, formal, body);
}

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
		// Instead of qualident this is now parseType.
		// But would that allow too much, e.g. arrays or generics?
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
static CatArrayWo /*<JCCase>*/ *l_switch_block_statement_groups(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_switch_block_statement_groups:token=%d", priv->token->kind);
	CatArrayWo /*<JCCase>*/ *cases = cat_array_wo_new();
	while (TRUE) {
		int pos = priv->token->pos;
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
				l_next_token(parser); // to ensure progress
//				syntaxError(pos, "expected3",
//						JAGP_KIND_CASE, JAGP_KIND_DEFAULT, JAGP_KIND_RBRACE);
		}
	}
}

static JagPJCCase *l_switch_block_statement_group(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_switch_block_statement_group:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	CatArrayWo */*<JCStatement>*/ stats;
	JagPJCCase *c;
	switch (priv->token->kind) {
		case JAGP_KIND_CASE:
			l_next_token(parser);
			JagPJCExpression *pat = l_parse_expression(parser);
			l_accept(parser, JAGP_KIND_COLON);
			stats = l_block_statements(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			c = jagp_tree_maker_case(priv->tree_maker, pat, stats);
			if (cat_array_wo_size(stats)==0) {
//				storeEnd(c, S.prevToken().endPos);
			}
			return c;
		case JAGP_KIND_DEFAULT:
			l_next_token(parser);
			l_accept(parser, JAGP_KIND_COLON);
			stats = l_block_statements(parser);
			jagp_tree_maker_at(priv->tree_maker, pos);
			c = jagp_tree_maker_case(priv->tree_maker, NULL, stats);
			if (cat_array_wo_size(stats)==0) {
//				storeEnd(c, S.prevToken().endPos);
			}
			return c;
	}
//	throw new AssertionError("should not reach here");
	return NULL;
}

/** MoreStatementExpressions = { JAGP_KIND_COMMA StatementExpression }
 */
static CatArrayWo /*<? super JCExpressionStatement>>*/ *l_more_statement_expressions(JagPParser *parser, int pos, JagPJCExpression *first, CatArrayWo  *stats) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_more_statement_expressions:token=%d", priv->token->kind);
	// This Exec is a "StatementExpression"; it subsumes no terminating token
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCExpressionStatement *exprstat = jagp_tree_maker_exec(priv->tree_maker, l_check_expr_stat(parser, first));
//	exprstat = toP(exprstat);
	cat_array_wo_append(stats, (GObject *) exprstat);
	cat_unref_ptr(exprstat);
	while (priv->token->kind == JAGP_KIND_COMMA) {
		l_next_token(parser);
		pos = priv->token->pos;
		JagPJCExpression *t = l_parse_expression(parser);
		// This Exec is a "StatementExpression"; it subsumes no terminating token
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCExpressionStatement *exprstat = jagp_tree_maker_exec(priv->tree_maker, l_check_expr_stat(parser, t));
	//	exprstat = toP(exprstat);
		cat_array_wo_append(stats, (GObject *) exprstat);
		cat_unref_ptr(exprstat);
	}
	return stats;
}

/** ForInit = StatementExpression MoreStatementExpressions
 *           |  { FINAL | '@' Annotation } Type VariableDeclarators
 */
static CatArrayWo */*<JCStatement>*/ l_for_init(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_for_init:token=%d", priv->token->kind);
	CatArrayWo /*<JCStatement>*/ *stats = cat_array_wo_new();
	int pos = priv->token->pos;
	if (priv->token->kind == JAGP_KIND_FINAL || priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		return l_variable_declarators(parser, l_opt_final(parser, 0), l_parse_type(parser), stats);
	} else {
		JagPJCExpression *t = l_term(parser, JAGP_MODE_EXPR | JAGP_MODE_TYPE);
		if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && lax_identifier_accepts(priv->token->kind)) {
			return l_variable_declarators(parser, l_modifiers_opt(parser, NULL), t, stats);
		} else if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && priv->token->kind == JAGP_KIND_COLON) {
//			error(pos, "bad.initializer", "for-loop");
			jagp_tree_maker_at(priv->tree_maker, pos);
			JagPJCVariableDecl *vardecl = jagp_tree_maker_var_def(priv->tree_maker, NULL, NULL, t, NULL);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, (GObject *) vardecl);
			cat_unref_ptr(vardecl);
			return result;
		} else {
			return l_more_statement_expressions(parser, pos, t, stats);
		}
	}
}

/** ForUpdate = StatementExpression MoreStatementExpressions
 */
static CatArrayWo /*<JCExpressionStatement>*/ *l_for_update(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_for_update:token=%d", priv->token->kind);
	return l_more_statement_expressions(parser, priv->token->pos, l_parse_expression(parser), cat_array_wo_new());
}

/** AnnotationsOpt = { '@' Annotation }
 *
 * @param kind Whether to parse an ANNOTATION or TYPE_ANNOTATION
 */
static CatArrayWo */*<JagPJCAnnotation>*/ l_annotations_opt(JagPParser *parser, JagPTag kind) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_annotations_opt:token=%d", priv->token->kind);
	if (priv->token->kind != JAGP_KIND_MONKEYS_AT) {
		return cat_array_wo_new(); // optimization
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

static CatArrayWo */*<JagPJCAnnotation>*/ l_type_annotations_opt(JagPParser *parser) {
	CatArrayWo */*<JagPJCAnnotation>*/ annotations = l_annotations_opt(parser, JAGP_TAG_TYPE_ANNOTATION);
	return annotations;
}

/** ModifiersOpt = { Modifier }
 *  Modifier = PUBLIC | PROTECTED | PRIVATE | STATIC | ABSTRACT | FINAL
 *           | NATIVE | SYNCHRONIZED | TRANSIENT | VOLATILE | "@"
 *           | "@" Annotation
 */
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
loop:
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
				// if first modifier is an annotation, set pos to annotation's.
				if (flags == 0 && cat_array_wo_size(annotations)==0) {
					pos = ann->parent.parent.pos;
				}
				cat_array_wo_append(annotations, (GObject *) ann);
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
	if ((flags & (JAGP_FLAG_ModifierFlags | JAGP_FLAG_ANNOTATION)) == 0 && cat_array_wo_size(annotations)==0)
		pos = -1;

	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, flags, annotations);
	if (pos != -1) {
//		storeEnd(mods, S.prevToken().endPos);
	}
	return mods;
}

/** Annotation              = "@" Qualident [ "(" AnnotationFieldValues ")" ]
 *
 * @param pos position of "@" token
 * @param kind Whether to parse an ANNOTATION or TYPE_ANNOTATION
 */
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

//	storeEnd(ann, S.prevToken().endPos);
	return ann;
}

static CatArrayWo */*<JagPJCExpression>*/ l_annotation_field_values_opt(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return (priv->token->kind == JAGP_KIND_LPAREN) ? l_annotation_field_values(parser) : cat_array_wo_new();
}

/** AnnotationFieldValues   = "(" [ AnnotationFieldValue { "," AnnotationFieldValue } ] ")" */
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
//			res = toP(res);
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
		JagPJCNewArray *na = jagp_tree_maker_new_array(priv->tree_maker, NULL, cat_array_wo_new(), buf);
//		na = toP(na);
		return (JagPJCExpression *) na;
	default:
		priv->mode = JAGP_MODE_EXPR;
		return l_term1(parser);
	}
}

/** VariableDeclarators = VariableDeclarator { "," VariableDeclarator }
 */
static CatArrayWo /*<T extends ListBuffer<? super JCVariableDecl>>*/ *l_variable_declarators(JagPParser *parser, JagPJCModifiers *mods,
		JagPJCExpression *type, CatArrayWo * vdefs) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return l_variable_declarators_rest(parser, priv->token->pos, mods, type, l_ident(parser, FALSE), FALSE, NULL, vdefs);
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
		// All but last of multiple declarators subsume a comma
//		storeEnd((JagPJCTree *)vdefs.last(), token.endPos);
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
static JagPJCVariableDecl *l_variable_declarator(JagPParser *parser, JagPJCModifiers *mods, JagPJCExpression *type, gboolean reqInit, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	return l_variable_declarator_rest(parser, priv->token->pos, mods, type, l_ident(parser, FALSE), reqInit, dc);
}

/** VariableDeclaratorRest = BracketsOpt ["=" VariableInitializer]
 *  ConstantDeclaratorRest = BracketsOpt "=" VariableInitializer
 *
 *  @param reqInit  Is an initializer always required?
 *  @param dc       The documentation comment for the variable declarations, or NULL.
 */
static JagPJCVariableDecl *l_variable_declarator_rest(JagPParser *parser, int pos, JagPJCModifiers *mods, JagPJCExpression *type, JagPName *name,
							  gboolean reqInit, JagPComment *dc ) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	type = l_brackets_opt_simple(parser, type);
	JagPJCExpression *init = NULL;
	if (priv->token->kind == JAGP_KIND_EQ) {
		l_next_token(parser);
		init = l_variable_initializer(parser);
	}
//	else if (reqInit) syntaxError(priv->token->pos, "expected", JAGP_KIND_EQ);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCVariableDecl *result = jagp_tree_maker_var_def(priv->tree_maker, mods, name, type, init);
//	result = toP(result);
	l_attach(parser, (JagPJCTree *) result, dc);
	return result;
}

/** VariableDeclaratorId = Ident BracketsOpt
 */
//where
static JagPJCVariableDecl *l_variable_declarator_id(JagPParser *parser, JagPJCModifiers *mods, JagPJCExpression *type, gboolean lambdaParameter) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_variable_declarator_id:token=%d", priv->token->kind);
	int pos = priv->token->pos;
	JagPName *name;
	if (lambdaParameter && priv->token->kind == JAGP_KIND_UNDERSCORE) {
//		log.error(pos, "underscore.as.identifier.in.lambda");
//		name = priv->token->name();
		l_next_token(parser);
	} else {
		if (priv->allowThisIdent && !lambdaParameter) {
			JagPJCExpression *pn = l_qualident(parser, FALSE);
			if (jagp_jctree_has_tag((JagPJCTree *) pn, JAGP_TAG_IDENT) && ((JagPJCIdent *)pn)->name != priv->names_this) {
				name = ((JagPJCIdent *)pn)->name;
			} else {
				if ((mods->flags & JAGP_FLAG_VARARGS) != 0) {
//					log.error(priv->token->pos, "varargs.and.receiver");
				}
				if (priv->token->kind == JAGP_KIND_LBRACKET) {
//					log.error(priv->token->pos, "array.and.receiver");
				}
				jagp_tree_maker_at(priv->tree_maker, pos);
				JagPJCVariableDecl *recvvardecl = jagp_tree_maker_receiver_var_def(priv->tree_maker, mods, pn, type);
//				recvvardecl = toP(recvvardecl);
				return recvvardecl;
			}
		} else {
			name = l_ident(parser, FALSE);
		}
	}
	if ((mods->flags & JAGP_FLAG_VARARGS) != 0 &&
			priv->token->kind == JAGP_KIND_LBRACKET) {
//		log.error(priv->token->pos, "varargs.and.old.array.syntax");
	}
	type = l_brackets_opt_simple(parser, type);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCVariableDecl *result = jagp_tree_maker_var_def(priv->tree_maker, mods, name, type, NULL);
//	result = toP(result);
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
		// All but last of multiple declarators must subsume a semicolon
//		storeEnd(defs.last(), token.endPos);
		int semiColonPos = priv->token->pos;
		l_next_token(parser);
		if (priv->token->kind == JAGP_KIND_RPAREN) { // Optional trailing semicolon
								   // after last resource
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
static JagPJCTree *l_resource(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int startPos = priv->token->pos;
	if (priv->token->kind == JAGP_KIND_FINAL || priv->token->kind == JAGP_KIND_MONKEYS_AT) {
		JagPJCModifiers *mods = l_opt_final(parser, JAGP_FLAG_FINAL);
		JagPJCExpression *t = l_parse_type(parser);
		return (JagPJCTree *) l_variable_declarator_rest(parser, priv->token->pos, mods, t, l_ident(parser, FALSE), TRUE, NULL);
	}
	JagPJCExpression *t = l_term(parser, JAGP_MODE_EXPR | JAGP_MODE_TYPE);
	if ((priv->last_mode & JAGP_MODE_TYPE) != 0 && lax_identifier_accepts(priv->token->kind)) {
		jagp_tree_maker_at(priv->tree_maker, startPos);
		JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, JAGP_FLAG_FINAL, NULL);
		return (JagPJCTree *) l_variable_declarator_rest(parser, priv->token->pos, mods, t, l_ident(parser, FALSE), TRUE, NULL );
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
		if (priv->token->kind == JAGP_KIND_MONKEYS_AT)
			mods = l_modifiers_opt(parser, NULL);

		if (priv->token->kind == JAGP_KIND_PACKAGE) {
			int packagePos = priv->token->pos;
			CatArrayWo */*<JagPJCAnnotation>*/ annotations = cat_array_wo_new();
			seenPackage = TRUE;
			if (mods != NULL) {
				l_check_no_mods(parser, mods->flags);
				annotations = mods->annotations;
				mods = NULL;
			}
			l_next_token(parser);
			JagPJCExpression *pid = l_qualident(parser, FALSE);
			l_accept(parser, JAGP_KIND_SEMI);
			jagp_tree_maker_at(priv->tree_maker, packagePos);
			JagPJCPackageDecl *pd = jagp_tree_maker_package_decl(priv->tree_maker, annotations, pid);
			l_attach(parser, (JagPJCTree *) pd, jagp_token_comment(firstToken, JAGP_COMMENT_STYLE_JAVADOC));
			consumedToplevelDoc = TRUE;
//			storeEnd(pd, priv->token->pos);
			cat_array_wo_append(defs, (GObject *) pd);
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
				JagPComment *docComment = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
				if (firstTypeDecl && !seenImport && !seenPackage) {
					docComment = jagp_token_comment(firstToken, JAGP_COMMENT_STYLE_JAVADOC);
					consumedToplevelDoc = TRUE;
				}
				JagPJCTree *def = l_type_declaration(parser, mods, docComment);
				if (JAGP_IS_JCEXPRESSION_STATEMENT(def)) {
					def = (JagPJCTree *) ((JagPJCExpressionStatement *)def)->expr;
				}
				cat_array_wo_append(defs, (GObject *) def);
				if (JAGP_IS_JCCLASS_DECL(def)) {
					checkForImports = FALSE;
				}
				mods = NULL;
				firstTypeDecl = FALSE;
			}
		}
	}

	jagp_tree_maker_at(priv->tree_maker, firstToken->pos);
	JagPJCCompilationUnit *toplevel = jagp_tree_maker_top_level(priv->tree_maker, defs);
	if (!consumedToplevelDoc)
		l_attach(parser, (JagPJCTree *) toplevel, jagp_token_comment(firstToken, JAGP_COMMENT_STYLE_JAVADOC));
//	if (cat_array_wo_size(defs)==0);
//		storeEnd(toplevel, S.prevToken().endPos);
//	if (priv->keepDocComments)
//		toplevel.docComments = docComments;
//	if (priv->keepLineMap)
//		toplevel.lineMap = S.getLineMap();
//	this.endPosTable.setParser(NULL); // remove reference to parser
//	toplevel.endPositions = this.endPosTable;
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
//	JagPJCModuleDecl *result = toP(F.ModuleDef(name, directives));
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
//			cat_array_wo_append(defs, toP(F.at(pos).Requires(isPublic, moduleName)));
//		} else if (jagp_token_name(priv->token) == priv->names_exports) {
//			l_next_token(parser);
//			JagPJCExpression *pkgName = l_qualident(parser, FALSE);
//			CatArrayWo */*<JagPJCExpression>*/ moduleNames = NULL;
//			if (priv->token->kind == JAGP_KIND_IDENTIFIER && jagp_token_name(priv->token) == priv->names_to) {
//				l_next_token(parser);
//				moduleNames = l_qualident_list(parser);
//			}
//			l_accept(parser, JAGP_KIND_SEMI);
//			cat_array_wo_append(defs, toP(F.at(pos).Exports(pkgName, moduleNames)));
//		} else if (jagp_token_name(priv->token) == priv->names_provides) {
//			l_next_token(parser);
//			JagPJCExpression *serviceName = l_qualident(parser, FALSE);
//			if (priv->token->kind == JAGP_KIND_IDENTIFIER && jagp_token_name(priv->token) == priv->names_with) {
//				l_next_token(parser);
//				JagPJCExpression *implName = l_qualident(parser, FALSE);
//				l_accept(parser, JAGP_KIND_SEMI);
//				cat_array_wo_append(defs, toP(F.at(pos).Provides(serviceName, implName)));
//			} else {
//				error(priv->token->pos, "expected", "'" + priv->names_with + "'");
//				skip(FALSE, FALSE, FALSE, FALSE);
//			}
//		} else if (jagp_token_name(priv->token) == priv->names_uses) {
//			l_next_token(parser);
//			JagPJCExpression *service = l_qualident(parser, FALSE);
//			l_accept(parser, JAGP_KIND_SEMI);
//			jagp_tree_maker_at(priv->tree_maker, pos);
//			GObject *uses = toP(F.Uses(service));
//			cat_array_wo_append(defs, uses);
//		} else {
//			break;
//		}
//	}
	return defs;
}

/** ImportDeclaration = IMPORT [ STATIC ] Ident { "." Ident } [ "." "*" ] ";"
 */
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
	JagPJCExpression *pid = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, l_ident(parser, FALSE));
//	pid = toP(pid);
	do {
		int pos1 = priv->token->pos;
		l_accept(parser, JAGP_KIND_DOT);
		if (priv->token->kind == JAGP_KIND_STAR) {
			jagp_tree_maker_at(priv->tree_maker, pos1);
			pid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, pid, priv->names_asterisk);
//			pid = to(pid);
			l_next_token(parser);
			break;
		} else {
			jagp_tree_maker_at(priv->tree_maker, pos1);
			pid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, pid, l_ident(parser, FALSE));
//			pid = to(pid);
		}
	} while (priv->token->kind == JAGP_KIND_DOT);
	l_accept(parser, JAGP_KIND_SEMI);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTree *result = (JagPJCTree *) jagp_tree_maker_import(priv->tree_maker, (JagPJCTree *) pid, importStatic);
//	result = toP(result);
	return result;
}

/** TypeDeclaration = ClassOrInterfaceOrEnumDeclaration
 *                  | ";"
 */
static JagPJCTree *l_type_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *docComment) {
	cat_log_debug("l_type_declaration");
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	if (mods == NULL && priv->token->kind == JAGP_KIND_SEMI) {
		l_next_token(parser);
		jagp_tree_maker_at(priv->tree_maker, pos);
		JagPJCTree *result = (JagPJCTree *) jagp_tree_maker_skip(priv->tree_maker);
//		result = toP(result);
		return result;
	} else {
		return (JagPJCTree *) l_class_or_interface_or_enum_declaration(parser, l_modifiers_opt(parser, mods), docComment);
	}
}

/** ClassOrInterfaceOrEnumDeclaration = ModifiersOpt
 *           (ClassDeclaration | InterfaceDeclaration | EnumDeclaration)
 *  @param mods     Any modifiers starting the class or interface declaration
 *  @param dc       The documentation comment for the class, or NULL.
 */
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
			JagPJCIdent *ident = jagp_tree_maker_ident(priv->tree_maker, l_ident(parser, FALSE));
//			ident = toP(ident);
			cat_array_wo_append(errs, (GObject *) ident);
			cat_unref_ptr(ident);
//			setErrorEndPos(priv->token->pos);
		} else {
			cat_array_wo_append(errs, (GObject *) mods);
		}
		JagPJCExpression *err = NULL;
//		JagPJCExpression *err = syntaxError(pos, errs, "expected3",JAGP_KIND_CLASS, JAGP_KIND_INTERFACE, JAGP_KIND_ENUM);
		JagPJCExpressionStatement *result = jagp_tree_maker_exec(priv->tree_maker, err);
//		result = toP(result);
		return (JagPJCStatement *) result;
	}
}

/** ClassDeclaration = CLASS Ident TypeParametersOpt [EXTENDS Type]
 *                     [IMPLEMENTS TypeList] ClassBody
 *  @param mods    The modifiers starting the class declaration
 *  @param dc       The documentation comment for the class, or NULL.
 */
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
//	result = toP(result);
	l_attach(parser, (JagPJCTree *) result, dc);
	return result;
}

/** InterfaceDeclaration = INTERFACE Ident TypeParametersOpt
 *                         [EXTENDS TypeList] InterfaceBody
 *  @param mods    The modifiers starting the interface declaration
 *  @param dc       The documentation comment for the interface, or NULL.
 */
static JagPJCClassDecl *l_interface_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_INTERFACE);
	JagPName *name = l_ident(parser, FALSE);

	CatArrayWo /*<JagPJCTypeParameter>*/ * typarams = l_type_parameters_opt(parser);

	CatArrayWo */*<JagPJCExpression>*/ extending = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_EXTENDS) {
		l_next_token(parser);
		extending = l_type_list(parser);
	}
	CatArrayWo */*<JCTree>*/ defs = l_class_or_interface_body(parser, name, TRUE);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCClassDecl *result = jagp_tree_maker_class_def(priv->tree_maker, mods, name, typarams,NULL,extending, defs);
//	result = toP(result);
	l_attach(parser, (JagPJCTree *) result, dc);
	return result;
}

/** EnumDeclaration = ENUM Ident [IMPLEMENTS TypeList] EnumBody
 *  @param mods    The modifiers starting the enum declaration
 *  @param dc       The documentation comment for the enum, or NULL.
 */
static JagPJCClassDecl *l_enum_declaration(JagPParser *parser, JagPJCModifiers *mods, JagPComment *dc) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	int pos = priv->token->pos;
	l_accept(parser, JAGP_KIND_ENUM);
	JagPName *name = l_ident(parser, FALSE);

	CatArrayWo */*<JagPJCExpression>*/ implementing = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_IMPLEMENTS) {
		l_next_token(parser);
		implementing = l_type_list(parser);
	}

	CatArrayWo */*<JCTree>*/ defs = l_enum_body(parser, name);
	mods->flags |= JAGP_FLAG_ENUM;
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCClassDecl *result = jagp_tree_maker_class_def(priv->tree_maker, mods, name, cat_array_wo_new(), NULL, implementing, defs);
//	result = toP(result);
	l_attach(parser, (JagPJCTree *) result, dc);
	return result;
}

/** EnumBody = "{" { EnumeratorDeclarationList } [","]
 *                  [ ";" {ClassBodyDeclaration} ] "}"
 */
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
		JagPJCClassDecl *clsdec = jagp_tree_maker_anonymous_class_def(priv->tree_maker, mods1, priv->names_empty, defs);
//		clsdec = toP(clsdec);
		body = clsdec;
	}
	if (cat_array_wo_size(args)==0 && body == NULL) {
		createPos = identPos;
	}
	jagp_tree_maker_at(priv->tree_maker, identPos);
	JagPJCIdent *ident = jagp_tree_maker_ident(priv->tree_maker, enumName);
	jagp_tree_maker_at(priv->tree_maker, createPos);
	JagPJCNewClass *create = jagp_tree_maker_new_class(priv->tree_maker, NULL, typeArgs, (JagPJCExpression *) ident, args, body);
	if (createPos != identPos) {
//		storeEnd(create, S.prevToken().endPos);
	}
	jagp_tree_maker_at(priv->tree_maker, identPos);
	ident = jagp_tree_maker_ident(priv->tree_maker, enumName);
	jagp_tree_maker_at(priv->tree_maker, pos);
	JagPJCTree *result = (JagPJCTree *) jagp_tree_maker_var_def(priv->tree_maker, mods, name, (JagPJCExpression *) ident, (JagPJCExpression *) create);
//	result = toP(result);
	l_attach(parser, result, dc);
	return result;
}

/** TypeList = Type {"," Type}
 */
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
static CatArrayWo /*<JCTree>*/ *l_class_or_interface_body_declaration(JagPParser *parser, JagPName *className, gboolean isInterface) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_class_or_interface_body_declaration:token=%d", priv->token->kind);
	if (priv->token->kind == JAGP_KIND_SEMI) {
		l_next_token(parser);
		return cat_array_wo_new();
	} else {
		JagPComment *dc = jagp_token_comment(priv->token, JAGP_COMMENT_STYLE_JAVADOC);
		int pos = priv->token->pos;
		JagPJCModifiers *mods = l_modifiers_opt(parser, NULL);
		if (priv->token->kind == JAGP_KIND_CLASS ||
				priv->token->kind == JAGP_KIND_INTERFACE ||
				priv->token->kind == JAGP_KIND_ENUM) {
			GObject *clorinorende = (GObject *) l_class_or_interface_or_enum_declaration(parser, mods, dc);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, clorinorende);
			cat_unref_ptr(clorinorende);
			return result;
		} else if (priv->token->kind == JAGP_KIND_LBRACE &&
				   (mods->flags & JAGP_FLAG_StandardFlags & ~JAGP_FLAG_STATIC) == 0 &&
				   cat_array_wo_size(mods->annotations)==0) {
//			if (isInterface) {
//				error(priv->token->pos, "initializer.not.allowed");
//			}
			GObject *block = (GObject *) l_block_ext(parser, pos, mods->flags);
			CatArrayWo *result = cat_array_wo_new();
			cat_array_wo_append(result, block);
			cat_unref_ptr(block);
			return result;
		} else {
			pos = priv->token->pos;
			CatArrayWo /*<JagPJCTypeParameter>*/ * typarams = l_type_parameters_opt(parser);
			// if there are type parameters but no modifiers, save the start
			// position of the method in the modifiers.
			if (cat_array_wo_size(typarams)>0 && mods->parent.pos == -1) {
				mods->parent.pos = pos;
//				storeEnd(mods, pos);
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
//				type = to(type);
				l_next_token(parser);
			} else {
				// method returns types are un-annotated types
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
				CatArrayWo *result = cat_array_wo_new();
				cat_array_wo_append(result, mdr);
				cat_unref_ptr(mdr);
				return result;
			} else {
				pos = priv->token->pos;
				JagPName *name = l_ident(parser, FALSE);
				if (priv->token->kind == JAGP_KIND_LPAREN) {
					GObject *mdr = (GObject *) l_method_declarator_rest(parser, pos, mods, type, name, typarams, isInterface, isVoid, dc);
					CatArrayWo *result = cat_array_wo_new();
					cat_array_wo_append(result, mdr);
					cat_unref_ptr(mdr);
					return result;
				} else if (!isVoid && cat_array_wo_size(typarams)==0) {
					CatArrayWo */*<JCTree>*/ defs =
							l_variable_declarators_rest(parser, pos, mods, type, name, isInterface, dc,cat_array_wo_new());
					l_accept(parser, JAGP_KIND_SEMI);
//					storeEnd(defs.last(), S.prevToken().endPos);
					return defs;
				} else {
					pos = priv->token->pos;
					CatArrayWo */*<JCTree>*/ err = NULL;
					if (isVoid) {
						err = cat_array_wo_new();
						jagp_tree_maker_at(priv->tree_maker, pos);
						GObject *ee = (GObject *) jagp_tree_maker_method_def1(priv->tree_maker, mods, name, type, typarams, cat_array_wo_new(), cat_array_wo_new(), NULL, NULL);
//						ee = toP(ee);
						cat_array_wo_append(err, ee);
						cat_unref_ptr(ee);
					}
					CatArrayWo *nres = cat_array_wo_new();
//					cat_array_wo_append(nres, syntaxError(priv->token->pos, err, "expected", JAGP_KIND_LPAREN));
					return nres;
				}
			}
		}
	}
}

/** MethodDeclaratorRest =
 *      FormalParameters BracketsOpt [THROWS TypeList] ( MethodBody | [DEFAULT AnnotationValue] ";")
 *  VoidMethodDeclaratorRest =
 *      FormalParameters [THROWS TypeList] ( MethodBody | ";")
 *  ConstructorDeclaratorRest =
 *      "(" FormalParameterListOpt ")" [THROWS TypeList] MethodBody
 */
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
	// Parsing formalParameters sets the receiverParam, if present
	CatArrayWo */*JagPJCVariableDecl>*/ params = l_formal_parameters(parser, FALSE);
	if (!isVoid) type = l_brackets_opt_simple(parser, type);
	CatArrayWo */*<JagPJCExpression>*/ thrown = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_THROWS) {
		l_next_token(parser);
		thrown = l_qualident_list(parser);
	}
	JagPJCBlock *body = NULL;
	JagPJCExpression *defaultValue;
	if (priv->token->kind == JAGP_KIND_LBRACE) {
		body = l_block(parser);
		defaultValue = NULL;
	} else {
		if (priv->token->kind == JAGP_KIND_DEFAULT) {
			l_accept(parser, JAGP_KIND_DEFAULT);
			defaultValue = l_annotation_value(parser);
		} else {
			defaultValue = NULL;
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
//		result = toP(result);
	l_attach(parser, (JagPJCTree *) result, dc);
	priv->receiverParam = prevReceiverParam;
	return (JagPJCTree *) result;
}

/** QualidentList = [Annotations] Qualident {"," [Annotations] Qualident}
 */
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
		cat_unref_ptr(qi);
	}
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
			cat_unref_ptr(qi);
		}
	}
	return ts;
}

/**
 *  {@literal
 *  TypeParametersOpt = ["<" TypeParameter {"," TypeParameter} ">"]
 *  }
 */
static CatArrayWo /*<JagPJCTypeParameter>*/ *l_type_parameters_opt(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_type_parameters_opt:token=%d", priv->token->kind);
	CatArrayWo /*<JagPJCTypeParameter>*/ *typarams = cat_array_wo_new();
	if (priv->token->kind == JAGP_KIND_LT) {
		l_next_token(parser);
		cat_array_wo_append(typarams, (GObject *) l_type_parameter(parser));
		while (priv->token->kind == JAGP_KIND_COMMA) {
			l_next_token(parser);
			cat_array_wo_append(typarams, (GObject *) l_type_parameter(parser));
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
//	result = toP(result);
	return result;
}

/** FormalParameters = "(" [ FormalParameterList ] ")"
 *  FormalParameterList = [ FormalParameterListNovarargs , ] LastFormalParameter
 *  FormalParameterListNovarargs = [ FormalParameterListNovarargs , ] FormalParameter
 */
static CatArrayWo */*JagPJCVariableDecl>*/ l_formal_parameters(JagPParser *parser, gboolean lambdaParameters) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	cat_log_debug("l_formal_parameters:token=%d", priv->token->kind);
	CatArrayWo /*<JagPJCVariableDecl>*/ *params = cat_array_wo_new();
	JagPJCVariableDecl *lastParam;
	l_accept(parser, JAGP_KIND_LPAREN);
	if (priv->token->kind != JAGP_KIND_RPAREN) {
		priv->allowThisIdent = TRUE;
		lastParam = l_formal_parameter(parser, lambdaParameters);
		if (lastParam->nameexpr != NULL) {
			cat_ref_swap(priv->receiverParam, lastParam);
		} else {
			cat_array_wo_append(params, (GObject *) lastParam);
		}
		priv->allowThisIdent = FALSE;
		while (priv->token->kind == JAGP_KIND_COMMA) {
//			if ((lastParam->mods->flags & JAGP_FLAG_VARARGS) != 0) {
//				error(lastParam, "varargs.must.be.last");
//			}
			l_next_token(parser);
			lastParam = l_formal_parameter(parser, lambdaParameters);
			cat_array_wo_append(params, (GObject *) lastParam);
		}
	}
	if (priv->token->kind == JAGP_KIND_RPAREN) {
		l_next_token(parser);
	} else {
//		setErrorEndPos(priv->token->pos);
//		reportSyntaxError(S.prevToken().endPos, "expected3", JAGP_KIND_COMMA, JAGP_KIND_RPAREN, JAGP_KIND_LBRACKET);
	}
	return params;
}

static CatArrayWo */*JagPJCVariableDecl>*/ l_implicit_parameters(JagPParser *parser, gboolean hasParens) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
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
//		mostInnerType = to(mostInnerType);
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
			// We never saw a SELECT or TYPEAPPLY, return the annotated type.
			mostInnerTypeToReturn = mostInnerType;
		}
	}

	if (mostInnerArrayType == NULL) {
		return mostInnerTypeToReturn;
	} else {
		mostInnerArrayType->elemtype = mostInnerTypeToReturn;
//		storeEnd(type, origEndPos);
		return type;
	}
}

/** FormalParameter = { FINAL | '@' Annotation } Type VariableDeclaratorId
 *  LastFormalParameter = { FINAL | '@' Annotation } Type '...' Ident | FormalParameter
 */
static JagPJCVariableDecl *l_formal_parameter(JagPParser *parser, gboolean lambdaParameter) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	JagPJCModifiers *mods = l_opt_final(parser, JAGP_FLAG_PARAMETER);
	// need to distinguish between vararg annos and array annos
	// look at typeAnnotationsPushedBack comment
	priv->permitTypeAnnotationsPushBack = TRUE;
	JagPJCExpression *type = l_parse_type(parser);
	priv->permitTypeAnnotationsPushBack = FALSE;

	if (priv->token->kind == JAGP_KIND_ELLIPSIS) {
		CatArrayWo /*<JagPJCAnnotation>*/ *varargsAnnos = priv->typeAnnotationsPushedBack;
		priv->typeAnnotationsPushedBack = cat_array_wo_new();
		mods->flags |= JAGP_FLAG_VARARGS;
		// insert var arg type annotations
		type = l_insert_annotations_to_most_inner(parser, type, varargsAnnos, TRUE);
		l_next_token(parser);
	} else {
		// if not a var arg, then typeAnnotationsPushedBack should be NULL
		if (cat_array_wo_size(priv->typeAnnotationsPushedBack)>0) {
//			reportSyntaxError(l_list_head_pos(priv->typeAnnotationsPushedBack),
//					"illegal.start.of.type");
		}
		priv->typeAnnotationsPushedBack = cat_array_wo_new();
	}
	return l_variable_declarator_id(parser, mods, type, lambdaParameter);
}

static JagPJCVariableDecl *l_implicit_parameter(JagPParser *parser) {
	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
	jagp_tree_maker_at(priv->tree_maker, priv->token->pos);
	JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, JAGP_FLAG_PARAMETER, NULL);
	return l_variable_declarator_id(parser, mods, NULL, TRUE);
}

/* ---------- auxiliary methods -------------- */


/** Check that given tree is a legal expression statement.
 */
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












































































//
//
///** CompilationUnit = [ { "@" Annotation } PACKAGE Qualident ";"] {ImportDeclaration} {TypeDeclaration}
// */
//static JagPCompilationUnit *l_parse_compilation_unit(JagPParser *parser) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	cat_log_debug("compilation_unit:%o", priv->token);
//	JagPToken *first_token = priv->token;
//	JagPJCModifiers *mods = NULL;
//	gboolean consumed_toplevel_doc = FALSE;
//	CatArrayWo *defs = cat_array_wo_new();	// <JagPJCTree *>
//
////	if (priv->token->kind == IDENTIFIER && priv->token->name() == names.module) {
////		defs.append(moduleDecl(priv->token->comment(CommentStyle.JAVADOC)));
////		consumed_toplevel_doc = TRUE;
////	} else {
//		gboolean seen_import = FALSE;
//		gboolean seen_package = FALSE;
//		if (priv->token->kind == JAGP_KIND_MONKEYS_AT) {
//			mods = l_modifiers_opt(parser, NULL);
//		}
//
//		if (priv->token->kind == JAGP_KIND_PACKAGE) {
////			int packagePos = priv->token->pos;
////			CatArrayWo */*<JagPJCAnnotation>*/ annotations = cat_array_wo_new();
////			seen_package = TRUE;
////			if (mods != NULL) {
////				checkNoMods(mods.flags);
////				annotations = mods.annotations;
////				mods = NULL;
////			}
//			l_next_token(parser);
//			JagPJCExpression *pid = l_qualident(parser, FALSE);
//			l_accept(parser, JAGP_KIND_SEMI);
////			JCPackageDecl pd = F.at(packagePos).PackageDecl(annotations, pid);
//			l_attach(parser, pd, firstToken.comment(CommentStyle.JAVADOC));
////			consumed_toplevel_doc = TRUE;
////			storeEnd(pd, priv->token->pos);
////			defs.append(pd);
//		}
//		gboolean check_for_imports = TRUE;
//		gboolean first_type_decl = TRUE;
//		while (priv->token->kind != JAGP_KIND_EOF) {
////			if (priv->token->pos <= endPosTable.errorEndPos) {
////				// error recovery
////				skip(check_for_imports, FALSE, FALSE, FALSE);
////				if (priv->token->kind == EOF)
////					break;
////			}
//			if (check_for_imports && mods == NULL && priv->token->kind == JAGP_KIND_IMPORT) {
//				seen_import = TRUE;
//				JagPJCImport *import_decl = l_import_declaration(parser);
//				cat_array_wo_append(defs, (GObject *) import_decl);
//			} else {
////				JagPComment *docComment = priv->token->comment(CommentStyle.JAVADOC);
////				if (first_type_decl && !seen_import && !seen_package) {
////					docComment = firstToken.comment(CommentStyle.JAVADOC);
////					consumed_toplevel_doc = TRUE;
////				}
//				JagPJCTree *def = l_type_declaration(parser, mods /*, docComment */ );
////				JCTree def = l_type_declaration(parser, mods, docComment);
////				if (def instanceof JCExpressionStatement)
////					def = ((JCExpressionStatement)def).expr;
//				cat_array_wo_append(defs, (GObject *) def);
////				if (def instanceof JCClassDecl)
////					check_for_imports = FALSE;
//				cat_unref_ptr(mods);
//				first_type_decl = FALSE;
//			}
//		}
////	}
////
//	JagPCompilationUnit *toplevel = NULL;
////	JCTree.JCCompilationUnit toplevel = F.at(firstToken.pos).TopLevel(defs.toList());
////	if (!consumed_toplevel_doc)
//		l_attach(parser, toplevel, firstToken.comment(CommentStyle.JAVADOC));
////	if (defs.isEmpty())
////		storeEnd(toplevel, S.prevToken().endPos);
////	if (keepDocComments)
////		toplevel.docComments = docComments;
////	if (keepLineMap)
////		toplevel.lineMap = S.getLineMap();
////	this.endPosTable.setParser(NULL); // remove reference to parser
////	toplevel.endPositions = this.endPosTable;
//	return toplevel;
//}
//
//
///** ImportDeclaration = IMPORT [ STATIC ] Ident { "." Ident } [ "." "*" ] ";"
// */
//static JagPJCImport *l_import_declaration(JagPParser *parser) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	int pos = priv->token->pos;
//	l_next_token(parser);
//	gboolean import_static = FALSE;
//	if (priv->token->kind == JAGP_KIND_STATIC) {
//		import_static = TRUE;
//		l_next_token(parser);
//	}
//
//	jagp_tree_maker_at(priv->tree_maker, pos);
//	JagPJCExpression *pid = (JagPJCExpression *) jagp_tree_maker_ident(priv->tree_maker, l_ident(parser, FALSE));
//	do {
//		int pos1 = priv->token->pos;
//		l_accept(parser, JAGP_KIND_DOT);
//		if (priv->token->kind == JAGP_KIND_STAR) {
//			jagp_tree_maker_at(priv->tree_maker, pos1);
//			JagPJCExpression *next_pid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, pid, priv->names_asterisk);
//			l_next_token(parser);
//			break;
//		} else {
//			jagp_tree_maker_at(priv->tree_maker, pos1);
//			JagPJCExpression *next_pid = (JagPJCExpression *) jagp_tree_maker_select(priv->tree_maker, pid, l_ident(parser, FALSE));
//			cat_unref_ptr(pid);
//			pid = next_pid;
//		}
//	} while (priv->token->kind == JAGP_KIND_DOT);
//	l_accept(parser, JAGP_KIND_SEMI);
//	jagp_tree_maker_at(priv->tree_maker, pos);
//	return jagp_tree_maker_import(priv->tree_maker, (JagPJCTree *) pid, import_static);
//}
//
//
///** TypeDeclaration = ClassOrInterfaceOrEnumDeclaration
// *                  | ";"
// */
//static JagPJCTree *l_type_declaration(JagPParser *parser, JagPJCModifiers *mods/*, JagPComment *docComment */) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	int pos = priv->token->pos;
//    if (mods == NULL && priv->token->kind == JAGP_KIND_SEMI) {
//        l_next_token(parser);
//        jagp_tree_maker_at(priv->tree_maker, pos);
//        return (JagPJCTree *) jagp_tree_maker_skip(priv->tree_maker);
//    } else {
//        return (JagPJCTree *) l_class_or_interface_or_enum_declaration(parser, l_modifiers_opt(parser, mods)/* ,docComment */);
//    }
//}
//
///** ClassOrInterfaceOrEnumDeclaration = ModifiersOpt
// *           (ClassDeclaration | InterfaceDeclaration | EnumDeclaration)
// *  @param mods     Any modifiers starting the class or interface declaration
// *  @param dc       The documentation comment for the class, or NULL.
// */
//static JagPJCStatement *l_class_or_interface_or_enum_declaration(JagPParser *parser, JagPJCModifiers *mods/*, JagPComment *dc*/) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	if (priv->token->kind == JAGP_KIND_CLASS) {
//		return (JagPJCStatement *) l_class_declaration(parser, mods/*, dc */);
////	} else if (priv->token->kind== JAGP_KIND_INTERFACE) {
////		return l_interface_declaration(parser, mods/*, dc */);
////	} else if (priv->token->kind== JAGP_KIND_ENUM) {
////		return l_enum_declaration(parser, mods/*, dc */);
//	} else {
//		return NULL;
////		int pos = priv->token->pos;
////		CatArrayWo */*<JCTree>*/ errs;
////		if (lax_identifier_accepts(priv->token->kind)) {
////			errs = List.<JCTree>of(mods, toP(F.at(pos).Ident(l_ident(parser, FALSE))));
////			setErrorEndPos(priv->token->pos);
////		} else {
////			errs = List.<JCTree>of(mods);
////		}
////		return toP(F.Exec(syntaxError(pos, errs, "expected3",
////									  CLASS, INTERFACE, ENUM)));
//	}
//}
//
//
///** ClassDeclaration = CLASS Ident TypeParametersOpt [EXTENDS Type]
// *                     [IMPLEMENTS TypeList] ClassBody
// *  @param mods    The modifiers starting the class declaration
// *  @param dc       The documentation comment for the class, or NULL.
// */
//static JagPJCClassDecl *l_class_declaration(JagPParser *parser, JagPJCModifiers *mods/*, JagPComment *dc */) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	int pos = priv->token->pos;
//	l_accept(parser, JAGP_KIND_CLASS);
//	JagPName *name = l_ident(parser, FALSE);
//
//	CatArrayWo *typarams = /*l_type_paramerts_opt(parser)*/ NULL;
//
//	JagPJCExpression *extending = NULL;
//	if (priv->token->kind == JAGP_KIND_EXTENDS) {
//		l_next_token(parser);
////		extending = l_parse_type(parser);
//	}
//	CatArrayWo *implementing = NULL;
////	CatArrayWo */*<JagPJCExpression>*/ implementing = cat_array_wo_new();
////	if (priv->token->kind == IMPLEMENTS) {
////		l_next_token(parser);
////		implementing = l_type_list(parser);
////	}
//	CatArrayWo *defs = l_class_or_interface_body(parser, name, FALSE);
////	CatArrayWo */*<JCTree>*/ defs = l_class_or_interface_body(parser, name, FALSE);
//	jagp_tree_maker_at(priv->tree_maker, pos);
//	JagPJCClassDecl *result = jagp_tree_maker_class_def(priv->tree_maker, mods, name, typarams, extending, implementing, defs);
//	l_attach(parser, result, dc);
//	return result;
//}
//
///** ClassBody     = "{" {ClassBodyDeclaration} "}"
// *  InterfaceBody = "{" {InterfaceBodyDeclaration} "}"
// */
//static CatArrayWo */*<JCTree>*/ l_class_or_interface_body(JagPParser *parser, JagPName *class_name, gboolean is_interface) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//    l_accept(parser, JAGP_KIND_LBRACE);
////    if (priv->token->pos <= endPosTable.errorEndPos) {
////        // error recovery
////        skip(FALSE, true, FALSE, FALSE);
////        if (priv->token->kind == JAGP_KIND_LBRACE)
////            l_next_token(parser);
////    }
//    CatArrayWo *defs = cat_array_wo_new();
//    while (priv->token->kind != JAGP_KIND_RBRACE && priv->token->kind != JAGP_KIND_EOF) {
//    	CatArrayWo *body_statements = l_class_or_interface_body_declaration(parser, class_name, is_interface);
//    	cat_array_wo_append_all(defs, body_statements);
//    	cat_unref_ptr(body_statements);
////        if (priv->priv->token->pos <= endPosTable.errorEndPos) {
////           // error recovery
////           skip(FALSE, true, true, FALSE);
////       }
//    }
//    l_accept(parser, JAGP_KIND_RBRACE);
//    return defs;
//}
//
//
///** ClassBodyDeclaration =
// *      ";"
// *    | [STATIC] Block
// *    | ModifiersOpt
// *      ( Type Ident
// *        ( VariableDeclaratorsRest ";" | MethodDeclaratorRest )
// *      | VOID Ident VoidMethodDeclaratorRest
// *      | TypeParameters [Annotations]
// *        ( Type Ident MethodDeclaratorRest
// *        | VOID Ident VoidMethodDeclaratorRest
// *        )
// *      | Ident ConstructorDeclaratorRest
// *      | TypeParameters Ident ConstructorDeclaratorRest
// *      | ClassOrInterfaceOrEnumDeclaration
// *      )
// *  InterfaceBodyDeclaration =
// *      ";"
// *    | ModifiersOpt
// *      ( Type Ident
// *        ( ConstantDeclaratorsRest ";" | MethodDeclaratorRest )
// *      | VOID Ident MethodDeclaratorRest
// *      | TypeParameters [Annotations]
// *        ( Type Ident MethodDeclaratorRest
// *        | VOID Ident VoidMethodDeclaratorRest
// *        )
// *      | ClassOrInterfaceOrEnumDeclaration
// *      )
// *
// */
//static CatArrayWo /* <JCTree> */ *l_class_or_interface_body_declaration(JagPParser *parser, JagPName *class_name, gboolean is_interface) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	if (priv->token->kind == JAGP_KIND_SEMI) {
//		l_next_token(parser);
//		return cat_array_wo_new();
//	} else {
////		JagPComment *dc = priv->token->comment(CommentStyle.JAVADOC);
//		int pos = priv->token->pos;
//		JagPJCModifiers *mods = l_modifiers_opt(FALSE);
//		if (priv->token->kind == JAGP_KIND_CLASS ||
//				priv->token->kind == JAGP_KIND_INTERFACE ||
//				priv->token->kind == JAGP_KIND_ENUM) {
//			CatArrayWo *result = cat_array_wo_new();
//			JagPJCStatement *statement = l_class_or_interface_or_enum_declaration(parser, mods/*, dc*/);
//			cat_array_wo_append(result, (GObject *) statement);
//			cat_unref_ptr(statement);
//			return result;
//		} else if (priv->token->kind == JAGP_KIND_LBRACE &&
//				   (mods.flags & JAGP_FLAG_StandardFlags & ~JAGP_FLAG_STATIC) == 0 &&
//				   cat_array_wo_size(mods.annotations)==0) {
//			if (is_interface) {
//				error(priv->token->pos, "initializer.not.allowed");
//			}
//			l_block_ext(parser, pos, mods->flags);
//			return List.<JCTree>of(l_block_ext(parser, pos, mods.flags));
//		} else {
//			pos = priv->token->pos;
//			CatArrayWo /*<JagPJCTypeParameter>*/ * typarams = l_type_parameters_opt(parser);
//			// if there are type parameters but no modifiers, save the start
//			// position of the method in the modifiers.
//			if (typarams.nonEmpty() && mods.pos == Position.NOPOS) {
//				mods.pos = pos;
//				storeEnd(mods, pos);
//			}
//			CatArrayWo */*<JagPJCAnnotation>*/ annosAfterParams = l_annotations_opt(parser, JAGP_TAG_ANNOTATION);
//
//			if (annosAfterParams.nonEmpty()) {
//				l_check_annotations_after_type_params(parser, annosAfterParams.head.pos);
//				mods.annotations = mods.annotations.appendList(annosAfterParams);
//				if (mods.pos == Position.NOPOS)
//					mods.pos = mods.annotations.head.pos;
//			}
//
//			Token tk = token;
//			pos = priv->token->pos;
//			JCExpression type;
//			boolean isVoid = priv->token->kind == VOID;
//			if (isVoid) {
//				type = to(F.at(pos).TypeIdent(TypeTag.VOID));
//				l_next_token(parser);
//			} else {
//				// method returns types are un-annotated types
//				type = unannotatedType();
//			}
//			if (priv->token->kind == JAGP_KIND_LPAREN && !is_interface && jagp_jctree_has_tag(type, JAGP_TAG_IDENT)) {
//				if (isInterface || tk.name() != className)
//					error(pos, "invalid.meth.decl.ret.type.req");
//				else if (annosAfterParams.nonEmpty())
//					illegal(annosAfterParams.head.pos);
//				return List.of(l_method_declarator_rest(parser,
//					pos, mods, NULL, names.init, typarams,
//					isInterface, true, dc));
//			} else {
//				pos = priv->token->pos;
//				JagPName *name = l_ident(parser, FALSE);
//				if (priv->token->kind == JAGP_KIND_LPAREN) {
//					return List.of(l_method_declarator_rest(parser,
//						pos, mods, type, name, typarams,
//						isInterface, isVoid, dc));
//				} else if (!isVoid && typarams.isEmpty()) {
//					CatArrayWo */*<JCTree>*/ defs =
//						l_variable_declarators_rest(parser, pos, mods, type, name, isInterface, dc,
//												cat_array_wo_new()).toList();
//					l_accept(parser, JAGP_KIND_SEMI);
//					storeEnd(defs.last(), S.prevToken().endPos);
//					return defs;
//				} else {
//					pos = priv->token->pos;
//					CatArrayWo */*<JCTree>*/ err = isVoid
//						? List.<JCTree>of(toP(F.at(pos).MethodDef(mods, name, type, typarams,
//							List.<JCVariableDecl>nil(), cat_array_wo_new(), NULL, NULL)))
//						: NULL;
//					return List.<JCTree>of(syntaxError(priv->token->pos, err, "expected", JAGP_KIND_LPAREN));
//				}
//			}
//		}
//	}
//}
//
//
//
///**
// *  {@literal
// *  TypeParametersOpt = ["<" TypeParameter {"," TypeParameter} ">"]
// *  }
// */
//static CatArrayWo */* <JCTypeParameter> */ l_type_parameters_opt(JagPParser parser) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	CatArrayWo *typarams = cat_array_wo_new();
//	if (priv->token->kind == JAGP_KIND_LT) {
//		l_next_token(parser);
//		JagJCTypeParameter *typaram = l_type_paramater(parser);
//		cat_array_wo_append(typarams, (GObject *) typaram);
//		cat_unref_ptr(typaram);
//		while (priv->token->kind == JAGP_KIND_COMMA) {
//			l_next_token(parser);
//			JagJCTypeParameter *typaram = l_type_paramater(parser);
//			cat_array_wo_append(typarams, (GObject *) typaram);
//			cat_unref_ptr(typaram);
//		}
//		l_accept(parser, JAGP_KIND_GT);
//	}
//	return typarams;
//}
//
//
///**
// *  {@literal
// *  TypeParameter = [Annotations] TypeVariable [TypeParameterBound]
// *  TypeParameterBound = EXTENDS Type {"&" Type}
// *  TypeVariable = Ident
// *  }
// */
//static JagPJCTypeParameter *l_type_parameter(JagPParser *parser) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//    int pos = priv->token->pos;
//    CatArrayWo *annos = l_annotations_opt(parser, TRUE);
//    JagPName *name = l_ident(parser, FALSE);
//    CatArrayWo *bounds = cat_array_wo_new(); 	//<JCExpression>
//    if (priv->token->kind == JAGP_KIND_EXTENDS) {
//        l_next_token(parser);
//        l_parse_type(parser);
//        bounds.append(l_parse_type(parser));
//        while (priv->token->kind == JAGP_KIND_AMP) {
//        	l_next_token(parser);
//            bounds.append(l_parse_type(parser));
//        }
//    }
//    return toP(F.at(pos).TypeParameter(name, bounds.toList(), annos));
//}
//
//
///** AnnotationsOpt = { '@' Annotation }
// *
// * @param kind Whether to parse an ANNOTATION or TYPE_ANNOTATION
// */
//static CatArrayWo * /*<JCAnnotation> */ l_annotations_opt(JagPParser *parser, gboolean parse_type_annotation) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	if (priv->priv->token->kind != JAGP_KIND_MONKEYS_AT) {
//		return cat_array_wo_new(); // optimization
//	}
//	CatArrayWo * /* <JCAnnotation> */ buf = cat_array_wo_new();
//	int prevmode = priv->mode;
//	while (priv->token->kind == JAGP_KIND_MONKEYS_AT) {
//		int pos = priv->token->pos;
//		l_next_token(parser);
//		JagPJCAnnotation *annotation = l_annotation(parser, pos, parse_type_annotation);
//		cat_array_wo_append(buf, (GObject *) annotation);
//		cat_unref_ptr(annotation)
//	}
//	priv->last_mode = priv->mode;
//	priv->mode = prevmode;
//
//	return buf;
//}
//
//
///** Annotation              = "@" Qualident [ "(" AnnotationFieldValues ")" ]
// *
// * @param pos position of "@" token
// * @param kind Whether to parse an ANNOTATION or TYPE_ANNOTATION
// */
//static JagPJCAnnotation *l_annotation(JagPParser *parser, int pos, gboolean parse_type_annotation) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//    // accept(AT); // AT consumed by caller
//    if (parse_type_annotation) {
//        l_check_type_annotations(parser);
//    }
//    JagPJCTree *ident = l_qualident(parser, FALSE);
//    CatArrayWo * /* <JCExpression> */ *fieldValues = NULL;
////    CatArrayWo * /* <JCExpression> */ *fieldValues = l_annotation_field_values_opt(parser);
//    JagPJCAnnotation *ann;
//   	jagp_tree_maker_at(priv->tree_maker, pos);
//    if (!parse_type_annotation) {
//    	ann = jagp_tree_maker_annotation(priv->tree_maker, ident, fieldValues);
//    } else {
//    	ann = jagp_tree_maker_type_annotation(priv->tree_maker, ident, fieldValues);
//    }
//
////    storeEnd(ann, S.prevToken().endPos);
//    return ann;
//}
//
//
///** ModifiersOpt = { Modifier }
// *  Modifier = PUBLIC | PROTECTED | PRIVATE | STATIC | ABSTRACT | FINAL
// *           | NATIVE | SYNCHRONIZED | TRANSIENT | VOLATILE | "@"
// *           | "@" Annotation
// */
//static JagPJCModifiers *l_modifiers_opt(JagPParser *parser, JagPJCModifiers *partial) {
//	JagPParserPrivate *priv = jagp_parser_get_instance_private(parser);
//	long long flags;
//	CatArrayWo *annotations = cat_array_wo_new();
//	int pos;
//	if (partial == NULL) {
//		flags = 0;
//		pos = priv->token->pos;
//	} else {
//		flags = partial->flags;
//		cat_array_wo_append_all(annotations, partial->annotations);
//		pos = partial->parent.pos;
//	}
////	if (priv->token->deprecatedFlag()) {
////		flags |= JAGP_FLAG_DEPRECATED;
////	}
//	int lastPos;
//	gboolean keep_looping = TRUE;
//	while (keep_looping) {
//		long flag;
//		switch (priv->token->kind) {
//		case JAGP_KIND_PRIVATE     : flag = JAGP_FLAG_PRIVATE; break;
//		case JAGP_KIND_PROTECTED   : flag = JAGP_FLAG_PROTECTED; break;
//		case JAGP_KIND_PUBLIC      : flag = JAGP_FLAG_PUBLIC; break;
//		case JAGP_KIND_STATIC      : flag = JAGP_FLAG_STATIC; break;
//		case JAGP_KIND_TRANSIENT   : flag = JAGP_FLAG_TRANSIENT; break;
//		case JAGP_KIND_FINAL       : flag = JAGP_FLAG_FINAL; break;
//		case JAGP_KIND_ABSTRACT    : flag = JAGP_FLAG_ABSTRACT; break;
//		case JAGP_KIND_NATIVE      : flag = JAGP_FLAG_NATIVE; break;
//		case JAGP_KIND_VOLATILE    : flag = JAGP_FLAG_VOLATILE; break;
//		case JAGP_KIND_SYNCHRONIZED: flag = JAGP_FLAG_SYNCHRONIZED; break;
//		case JAGP_KIND_STRICTFP    : flag = JAGP_FLAG_STRICTFP; break;
//		case JAGP_KIND_MONKEYS_AT  : flag = JAGP_FLAG_ANNOTATION; break;
////		case JAGP_KIND_DEFAULT     : checkDefaultMethods(); flag = JAGP_FLAG_DEFAULT; break;
//		case JAGP_KIND_ERROR       : flag = 0; l_next_token(parser); break;
//		default:
//			keep_looping = FALSE;
//			continue;
//		}
//		if ((flags & flag) != 0) {
////			cat_log_error(priv->token->pos, "repeated.modifier");
//		}
//		lastPos = priv->token->pos;
//		l_next_token(parser);
////		if (flag == JAGP_FLAG_ANNOTATION) {
////			if (priv->token->kind != JAGP_KIND_INTERFACE) {
////				JagPJCAnnotation *ann = l_annotation(parser, lastPos, JAGP_TAG_ANNOTATION);
////				// if first modifier is an annotation, set pos to annotation's.
////				if (flags == 0 && annotations.isEmpty()) {
////					pos = ann.pos;
////				}
////				annotations.append(ann);
////				flag = 0;
////			}
////		}
//		flags |= flag;
//	}
//	switch (priv->token->kind) {
//		case JAGP_KIND_ENUM :
//			flags |= JAGP_FLAG_ENUM;
//			break;
//		case JAGP_KIND_INTERFACE:
//			flags |= JAGP_FLAG_INTERFACE;
//			break;
//		default: break;
//	}
//
//	/* A modifiers tree with no modifier tokens or annotations
//	 * has no text position. */
//	if ((flags & (JAGP_FLAG_ModifierFlags | JAGP_FLAG_ANNOTATION)) == 0 && (cat_array_wo_size(annotations)==0)) {
//		pos = -1;
//	}
//
//	jagp_tree_maker_at(priv->tree_maker, pos);
//	JagPJCModifiers *mods = jagp_tree_maker_modifiers(priv->tree_maker, flags, annotations);
//
//	if (pos != -1) {
////		storeEnd(mods, S.prevToken().endPos);
//	}
//	return mods;
//}
//
//
//





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
