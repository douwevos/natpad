#include "smplparseractions.h"



#include <caterpillar.h>
#include "grosymbol.h"
#include "../bnf/grobnfproduction.h"
#include "../bnf/grobnfrhs.h"
#include "../bnf/grobnfspec.h"
#include "../bnf/grobnfsymbolpart.h"
#include "../bnf/grobnfcodepart.h"
#include "../bnf/grobnfsymboldeflist.h"
#include "../bnf/grobnfstacktype.h"



#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ERROR
#define CAT_LOG_CLAZZ "SmplParserActions"
#include <logging/catlog.h>

G_DEFINE_TYPE(SmplParserActions, smpl_parser_actions, G_TYPE_OBJECT)


static void smpl_parser_actions_class_init(SmplParserActionsClass *clazz) {
	clazz->run_action = smpl_parser_actions_run_action;
}

static void smpl_parser_actions_init(SmplParserActions *parser) {
}

SmplParserActions *smpl_parser_actions_new() {
	SmplParserActions *result = g_object_new(SMPL_TYPE_PARSER_ACTIONS, NULL);
	cat_ref_anounce(result);
	return result;
}

SmplToken *smpl_parser_actions_run_action(SmplParserActions *parser_actions, SmplParserContext *parser_context, int cup_action_id) {	SmplParserContextClass *context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parser_context);
	/* Symbol object for return from actions */
	SmplToken *cup_result = NULL;

	/* select the action based on the action number */
	switch(cup_action_id) {
		case 0: { // $START ::= spec EOF 
			GObject *RESULT = NULL;
				SmplToken *cup_start_val = context_class->getFromTop(parser_context, 1);
				GObject *start_val = (GObject *) (cup_start_val->value);
RESULT = start_val;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 0/*$START*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		/* ACCEPT */
		smpl_parser_context_done_parsing(parser_context);
		return cup_result;

		case 1: { // spec ::= package_spec import_list code_parts symbol_list precedence_list start_spec production_list 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 26/*spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 2: { // spec ::= error symbol_list precedence_list start_spec production_list 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 26/*spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 3: { // package_spec ::= PACKAGE multipart_id SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 1/*package_spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 4: { // package_spec ::= empty 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 1/*package_spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 5: { // import_list ::= import_list import_spec 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 2/*import_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 6: { // import_list ::= empty 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 2/*import_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 7: { // import_spec ::= IMPORT import_id SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 10/*import_spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 8: { // code_part ::= action_code_part 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*code_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 9: { // code_part ::= parser_code_part 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*code_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 10: { // code_part ::= init_code 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*code_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 11: { // code_part ::= scan_code 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*code_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 12: { // code_parts ::= 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 4/*code_parts*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 13: { // code_parts ::= code_parts code_part 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 4/*code_parts*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 14: { // action_code_part ::= ACTION CODE CODE_STRING opt_semi 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 3/*action_code_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 15: { // parser_code_part ::= PARSER CODE CODE_STRING opt_semi 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 8/*parser_code_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 16: { // init_code ::= INIT WITH CODE_STRING opt_semi 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 12/*init_code*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 17: { // scan_code ::= SCAN WITH CODE_STRING opt_semi 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 13/*scan_code*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 18: { // symbol_list ::= symbol_list symbol 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 33/*symbol_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 19: { // symbol_list ::= symbol 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 33/*symbol_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 20: { // symbol ::= TERMINAL type_id declares_term 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 14/*symbol*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 21: { // symbol ::= TERMINAL declares_term 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 14/*symbol*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 22: { // symbol ::= non_terminal type_id declares_non_term 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 14/*symbol*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 23: { // symbol ::= non_terminal declares_non_term 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 14/*symbol*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 24: { // symbol ::= TERMINAL error SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 14/*symbol*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 25: { // symbol ::= non_terminal error SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 14/*symbol*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 26: { // declares_term ::= term_name_list SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 24/*declares_term*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 27: { // declares_non_term ::= non_term_name_list SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 25/*declares_non_term*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 28: { // term_name_list ::= term_name_list COMMA new_term_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 16/*term_name_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 29: { // term_name_list ::= new_term_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 16/*term_name_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 30: { // non_term_name_list ::= non_term_name_list COMMA new_non_term_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 17/*non_term_name_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 31: { // non_term_name_list ::= new_non_term_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 17/*non_term_name_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 32: { // precedence_list ::= precedence_l 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 20/*precedence_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 33: { // precedence_list ::= empty 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 20/*precedence_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 34: { // precedence_l ::= precedence_l preced 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 23/*precedence_l*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 35: { // precedence_l ::= preced 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 23/*precedence_l*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 36: { // preced ::= PRECEDENCE LEFT terminal_list SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 21/*preced*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 37: { // preced ::= PRECEDENCE RIGHT terminal_list SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 21/*preced*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 38: { // preced ::= PRECEDENCE NONASSOC terminal_list SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 21/*preced*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 39: { // terminal_list ::= terminal_list COMMA terminal_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 22/*terminal_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 40: { // terminal_list ::= terminal_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 22/*terminal_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 41: { // terminal_id ::= term_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 38/*terminal_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 42: { // term_id ::= symbol_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 39/*term_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 43: { // start_spec ::= START WITH nt_id SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 9/*start_spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 44: { // start_spec ::= empty 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 9/*start_spec*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 45: { // production_list ::= production_list production 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 32/*production_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 46: { // production_list ::= production 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 32/*production_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 47: { // production ::= nt_id COLON_COLON_EQUALS rhs_list SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 31/*production*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 48: { // production ::= error SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 31/*production*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 49: { // rhs_list ::= rhs_list BAR rhs 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 30/*rhs_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 50: { // rhs_list ::= rhs 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 30/*rhs_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 51: { // rhs ::= prod_part_list PERCENT_PREC term_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 27/*rhs*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 52: { // rhs ::= prod_part_list 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 27/*rhs*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 53: { // prod_part_list ::= prod_part_list prod_part 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 29/*prod_part_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 54: { // prod_part_list ::= empty 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 29/*prod_part_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 55: { // prod_part ::= symbol_id opt_label 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 28/*prod_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 56: { // prod_part ::= CODE_STRING 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 28/*prod_part*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 57: { // opt_label ::= COLON label_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 37/*opt_label*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 58: { // opt_label ::= empty 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 37/*opt_label*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 59: { // multipart_id ::= multipart_id DOT robust_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 42/*multipart_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 60: { // multipart_id ::= robust_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 42/*multipart_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 61: { // import_id ::= multipart_id DOT STAR 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 11/*import_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 62: { // import_id ::= multipart_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 11/*import_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 63: { // type_id ::= multipart_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 15/*type_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 64: { // type_id ::= type_id LBRACK RBRACK 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 15/*type_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 65: { // new_term_id ::= ID 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 41/*new_term_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 66: { // new_non_term_id ::= ID 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 18/*new_non_term_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 67: { // nt_id ::= ID 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 34/*nt_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 68: { // nt_id ::= error 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 34/*nt_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 69: { // symbol_id ::= ID 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 35/*symbol_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 70: { // symbol_id ::= error 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 35/*symbol_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 71: { // label_id ::= robust_id 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 36/*label_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 72: { // robust_id ::= ID 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 73: { // robust_id ::= CODE 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 74: { // robust_id ::= ACTION 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 75: { // robust_id ::= PARSER 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 76: { // robust_id ::= TERMINAL 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 77: { // robust_id ::= NON 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 78: { // robust_id ::= NONTERMINAL 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 79: { // robust_id ::= INIT 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 80: { // robust_id ::= SCAN 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 81: { // robust_id ::= WITH 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 82: { // robust_id ::= START 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 83: { // robust_id ::= PRECEDENCE 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 84: { // robust_id ::= LEFT 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 85: { // robust_id ::= RIGHT 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 86: { // robust_id ::= NONASSOC 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 87: { // robust_id ::= error 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 40/*robust_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 88: { // non_terminal ::= NON TERMINAL 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 7/*non_terminal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 89: { // non_terminal ::= NONTERMINAL 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 7/*non_terminal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 90: { // opt_semi ::= 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 6/*opt_semi*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 91: { // opt_semi ::= SEMI 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 6/*opt_semi*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 92: { // empty ::= 
			GObject *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 19/*empty*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

	}
	return cup_result;
}

