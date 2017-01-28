#include "smplparseractions.h"


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
		case 0: { // $START ::= program EOF 
			String *RESULT = NULL;
				SmplToken *cup_start_val = context_class->getFromTop(parser_context, 1);
				String *start_val = (String *) (cup_start_val->value);
RESULT = start_val;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 0/*$START*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		/* ACCEPT */
		smpl_parser_context_done_parsing(parser_context);
		return cup_result;

		case 1: { // program ::= statements 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 1/*program*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 2: { // statements ::= statements statement 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 2/*statements*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 3: { // statements ::= statement 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 2/*statements*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 4: { // statement ::= assign_statement 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 3/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 5: { // assign_statement ::= qualified_name ASSIGN expr SEMICOLON 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 4/*assign_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 6: { // expr ::= expr OPPLUS term 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*expr*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 7: { // expr ::= expr OPMINUS term 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*expr*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 8: { // expr ::= term 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 5/*expr*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 9: { // term ::= fact OPMUL prim 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 6/*term*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 10: { // term ::= fact OPDIV prim 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 6/*term*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 11: { // term ::= prim 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 6/*term*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 12: { // prim ::= IDENTIFIER 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 8/*prim*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 13: { // prim ::= LEFTPAREN expr RIGHTPAREN 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 8/*prim*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 14: { // qualified_name ::= qualified_name DOT IDENTIFIER 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 9/*qualified_name*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

		case 15: { // qualified_name ::= IDENTIFIER 
			String *RESULT = NULL;
			cup_result  = smpl_iscanner_create_token(parser_context->scanner, 9/*qualified_name*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		return cup_result;

	}
	return cup_result;
}

