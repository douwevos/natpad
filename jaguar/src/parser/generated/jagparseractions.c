#include "jagparseractions.h"




#include "jagsymbol.h"
#include "../jagasttoken.h"
#include "../../indexer/model/jagasttype.h"
#include "../../indexer/model/jagastindexermodifiers.h"
#include "../../indexer/model/jagastgenericstype.h"
#include "../../indexer/model/jagastplaintype.h"
#include "../../indexer/model/jagastdeclarator.h"



#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ERROR
#define CAT_LOG_CLAZZ "JagParserActions"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagParserActions, jag_parser_actions, G_TYPE_OBJECT)


static void jag_parser_actions_class_init(JagParserActionsClass *clazz) {
	clazz->run_action = jag_parser_actions_run_action;
}

static void jag_parser_actions_init(JagParserActions *parser) {
}

JagParserActions *jag_parser_actions_new() {
	JagParserActions *result = g_object_new(JAG_TYPE_PARSER_ACTIONS, NULL);
	cat_ref_anounce(result);
	return result;
}

JagToken *jag_parser_actions_run_action(JagParserActions *parser_actions, JagParserContext *parser_context, int cup_action_id) {	JagParserContextClass *context_class = JAG_PARSER_CONTEXT_GET_CLASS(parser_context);
	/* Symbol object for return from actions */
	JagToken *cup_result = NULL;

	/* select the action based on the action number */
	switch(cup_action_id) {
		case 0: { // $START ::= goal EOF 
			GObject *RESULT = NULL;
				JagToken *cup_start_val = context_class->getFromTop(parser_context, 1);
				GObject *start_val = (GObject *) (cup_start_val->value);
RESULT = start_val;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 0/*$START*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		/* ACCEPT */
		jag_parser_context_done_parsing(parser_context);
		cup_result->symbol_text = "$START ::= goal EOF ";
		return cup_result;

		case 1: { // goal ::= compilation_unit 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 1/*goal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "goal ::= compilation_unit ";
		return cup_result;

		case 2: { // literal ::= INTEGER_LITERAL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 2/*literal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "literal ::= INTEGER_LITERAL ";
		return cup_result;

		case 3: { // literal ::= FLOATING_POINT_LITERAL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 2/*literal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "literal ::= FLOATING_POINT_LITERAL ";
		return cup_result;

		case 4: { // literal ::= BOOLEAN_LITERAL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 2/*literal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "literal ::= BOOLEAN_LITERAL ";
		return cup_result;

		case 5: { // literal ::= CHARACTER_LITERAL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 2/*literal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "literal ::= CHARACTER_LITERAL ";
		return cup_result;

		case 6: { // literal ::= STRING_LITERAL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 2/*literal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "literal ::= STRING_LITERAL ";
		return cup_result;

		case 7: { // literal ::= NULL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 2/*literal*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "literal ::= NULL ";
		return cup_result;

		case 8: { // type ::= primitive_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 3/*type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type ::= primitive_type ";
		return cup_result;

		case 9: { // type ::= reference_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 3/*type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type ::= reference_type ";
		return cup_result;

		case 10: { // primitive_type ::= numeric_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 4/*primitive_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primitive_type ::= numeric_type ";
		return cup_result;

		case 11: { // primitive_type ::= BOOLEAN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 4/*primitive_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primitive_type ::= BOOLEAN ";
		return cup_result;

		case 12: { // numeric_type ::= integral_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 5/*numeric_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "numeric_type ::= integral_type ";
		return cup_result;

		case 13: { // numeric_type ::= floating_point_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 5/*numeric_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "numeric_type ::= floating_point_type ";
		return cup_result;

		case 14: { // integral_type ::= BYTE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 6/*integral_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "integral_type ::= BYTE ";
		return cup_result;

		case 15: { // integral_type ::= SHORT 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 6/*integral_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "integral_type ::= SHORT ";
		return cup_result;

		case 16: { // integral_type ::= INT 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 6/*integral_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "integral_type ::= INT ";
		return cup_result;

		case 17: { // integral_type ::= LONG 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 6/*integral_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "integral_type ::= LONG ";
		return cup_result;

		case 18: { // integral_type ::= CHAR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 6/*integral_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "integral_type ::= CHAR ";
		return cup_result;

		case 19: { // floating_point_type ::= FLOAT 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 7/*floating_point_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "floating_point_type ::= FLOAT ";
		return cup_result;

		case 20: { // floating_point_type ::= DOUBLE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 7/*floating_point_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "floating_point_type ::= DOUBLE ";
		return cup_result;

		case 21: { // reference_type ::= class_or_interface_type 
			GObject *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				GObject *t = (GObject *) (cup_t->value);
 RESULT = t; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 8/*reference_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type ::= class_or_interface_type ";
		return cup_result;

		case 22: { // reference_type ::= array_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 8/*reference_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type ::= array_type ";
		return cup_result;

		case 23: { // type_variable ::= IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 180/*type_variable*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_variable ::= IDENTIFIER ";
		return cup_result;

		case 24: { // class_or_interface ::= name 
			JagAstType *RESULT = NULL;
				JagToken *cup_nm = context_class->getFromTop(parser_context, 0);
				CatStringWo *nm = (CatStringWo *) (cup_nm->value);
 gboolean is_qualified = cat_string_wo_index_of(nm, '.')>=0; RESULT = (JagAstType*) jag_ast_plain_type_new(nm, is_qualified); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 179/*class_or_interface*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "class_or_interface ::= name ";
		return cup_result;

		case 25: { // class_or_interface ::= class_or_interface OP_LT type_argument_list_1 DOT name 
			JagAstType *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 4);
				JagAstType *t = (JagAstType *) (cup_t->value);
				JagToken *cup_g = context_class->getFromTop(parser_context, 2);
				CatArrayWo *g = (CatArrayWo *) (cup_g->value);

						cat_ref_ptr(g);
						CatArrayWo *a_generics_list = cat_array_wo_anchor(g, 0);
						RESULT=(JagAstType*) jag_ast_generics_type_new(t, a_generics_list);
						cat_unref_ptr(a_generics_list);
						
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 179/*class_or_interface*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 
						cat_unref(RESULT); 
					
		}
		cup_result->symbol_text = "class_or_interface ::= class_or_interface OP_LT type_argument_list_1 DOT name ";
		return cup_result;

		case 26: { // class_or_interface_type ::= class_or_interface 
			GObject *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				JagAstType *t = (JagAstType *) (cup_t->value);
 RESULT = t; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 9/*class_or_interface_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_or_interface_type ::= class_or_interface ";
		return cup_result;

		case 27: { // class_or_interface_type ::= class_or_interface OP_LT type_argument_list_1 
			GObject *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 2);
				JagAstType *t = (JagAstType *) (cup_t->value);
				JagToken *cup_g = context_class->getFromTop(parser_context, 0);
				CatArrayWo *g = (CatArrayWo *) (cup_g->value);

						cat_ref_ptr(g);
						CatArrayWo *a_generics_list = cat_array_wo_anchor(g, 0);
						JagAstGenericsType *gen_type = jag_ast_generics_type_new(t, a_generics_list);
						cat_unref_ptr(a_generics_list);
						cat_log_debug("gen_type=%p", gen_type);
						RESULT = JAG_AST_TYPE(gen_type);
						cat_log_debug("RESULT=%p", RESULT);
						
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 9/*class_or_interface_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 
						cat_unref(RESULT);  
					
		}
		cup_result->symbol_text = "class_or_interface_type ::= class_or_interface OP_LT type_argument_list_1 ";
		return cup_result;

		case 28: { // class_type ::= class_or_interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 10/*class_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_type ::= class_or_interface_type ";
		return cup_result;

		case 29: { // interface_type ::= class_or_interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 11/*interface_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_type ::= class_or_interface_type ";
		return cup_result;

		case 30: { // array_type ::= primitive_type dims 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 12/*array_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_type ::= primitive_type dims ";
		return cup_result;

		case 31: { // array_type ::= name dims 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 12/*array_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_type ::= name dims ";
		return cup_result;

		case 32: { // array_type ::= class_or_interface OP_LT type_argument_list_1 DOT name dims 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 12/*array_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_type ::= class_or_interface OP_LT type_argument_list_1 DOT name dims ";
		return cup_result;

		case 33: { // array_type ::= class_or_interface OP_LT type_argument_list_1 dims 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 12/*array_type*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_type ::= class_or_interface OP_LT type_argument_list_1 dims ";
		return cup_result;

		case 34: { // type_arguments_opt ::= type_arguments 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 182/*type_arguments_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_arguments_opt ::= type_arguments ";
		return cup_result;

		case 35: { // type_arguments_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 182/*type_arguments_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_arguments_opt ::= ";
		return cup_result;

		case 36: { // type_arguments ::= OP_LT type_argument_list_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 181/*type_arguments*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_arguments ::= OP_LT type_argument_list_1 ";
		return cup_result;

		case 37: { // wildcard ::= QUESTION 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 204/*wildcard*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard ::= QUESTION ";
		return cup_result;

		case 38: { // wildcard ::= QUESTION EXTENDS reference_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 204/*wildcard*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard ::= QUESTION EXTENDS reference_type ";
		return cup_result;

		case 39: { // wildcard ::= QUESTION SUPER reference_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 204/*wildcard*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard ::= QUESTION SUPER reference_type ";
		return cup_result;

		case 40: { // wildcard_1 ::= QUESTION OP_GT 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 205/*wildcard_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_1 ::= QUESTION OP_GT ";
		return cup_result;

		case 41: { // wildcard_1 ::= QUESTION EXTENDS reference_type_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 205/*wildcard_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_1 ::= QUESTION EXTENDS reference_type_1 ";
		return cup_result;

		case 42: { // wildcard_1 ::= QUESTION SUPER reference_type_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 205/*wildcard_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_1 ::= QUESTION SUPER reference_type_1 ";
		return cup_result;

		case 43: { // wildcard_2 ::= QUESTION OP_SHR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 206/*wildcard_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_2 ::= QUESTION OP_SHR ";
		return cup_result;

		case 44: { // wildcard_2 ::= QUESTION EXTENDS reference_type_2 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 206/*wildcard_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_2 ::= QUESTION EXTENDS reference_type_2 ";
		return cup_result;

		case 45: { // wildcard_2 ::= QUESTION SUPER reference_type_2 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 206/*wildcard_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_2 ::= QUESTION SUPER reference_type_2 ";
		return cup_result;

		case 46: { // wildcard_3 ::= QUESTION OP_SHRR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 207/*wildcard_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_3 ::= QUESTION OP_SHRR ";
		return cup_result;

		case 47: { // wildcard_3 ::= QUESTION EXTENDS reference_type_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 207/*wildcard_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_3 ::= QUESTION EXTENDS reference_type_3 ";
		return cup_result;

		case 48: { // wildcard_3 ::= QUESTION SUPER reference_type_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 207/*wildcard_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "wildcard_3 ::= QUESTION SUPER reference_type_3 ";
		return cup_result;

		case 49: { // reference_type_1 ::= reference_type OP_GT 
			JagAstType *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 1);
				GObject *t = (GObject *) (cup_t->value);
 RESULT=t; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 185/*reference_type_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type_1 ::= reference_type OP_GT ";
		return cup_result;

		case 50: { // reference_type_1 ::= class_or_interface OP_LT type_argument_list_2 
			JagAstType *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 185/*reference_type_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type_1 ::= class_or_interface OP_LT type_argument_list_2 ";
		return cup_result;

		case 51: { // reference_type_2 ::= reference_type OP_SHR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 187/*reference_type_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type_2 ::= reference_type OP_SHR ";
		return cup_result;

		case 52: { // reference_type_2 ::= class_or_interface OP_LT type_argument_list_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 187/*reference_type_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type_2 ::= class_or_interface OP_LT type_argument_list_3 ";
		return cup_result;

		case 53: { // reference_type_3 ::= reference_type OP_SHRR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 189/*reference_type_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_type_3 ::= reference_type OP_SHRR ";
		return cup_result;

		case 54: { // type_argument_list ::= type_argument 
			CatArrayWo *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				JagAstType *t = (JagAstType *) (cup_t->value);
 CatArrayWo *e_arguments = cat_array_wo_new(); cat_array_wo_append(e_arguments, (GObject *) t); RESULT=e_arguments; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 183/*type_argument_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "type_argument_list ::= type_argument ";
		return cup_result;

		case 55: { // type_argument_list ::= type_argument_list COMMA type_argument 
			CatArrayWo *RESULT = NULL;
				JagToken *cup_l = context_class->getFromTop(parser_context, 2);
				CatArrayWo *l = (CatArrayWo *) (cup_l->value);
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				JagAstType *t = (JagAstType *) (cup_t->value);
 cat_array_wo_append(l, (GObject *) t); RESULT=l; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 183/*type_argument_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_list ::= type_argument_list COMMA type_argument ";
		return cup_result;

		case 56: { // type_argument_list_1 ::= type_argument_1 
			CatArrayWo *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				JagAstType *t = (JagAstType *) (cup_t->value);
 CatArrayWo *arguments = cat_array_wo_new(); cat_array_wo_append(arguments, (GObject *) t); RESULT=arguments; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 184/*type_argument_list_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "type_argument_list_1 ::= type_argument_1 ";
		return cup_result;

		case 57: { // type_argument_list_1 ::= type_argument_list COMMA type_argument_1 
			CatArrayWo *RESULT = NULL;
				JagToken *cup_l = context_class->getFromTop(parser_context, 2);
				CatArrayWo *l = (CatArrayWo *) (cup_l->value);
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				JagAstType *t = (JagAstType *) (cup_t->value);
 cat_array_wo_append(l, (GObject *) t); RESULT=l; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 184/*type_argument_list_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_list_1 ::= type_argument_list COMMA type_argument_1 ";
		return cup_result;

		case 58: { // type_argument_list_2 ::= type_argument_2 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 186/*type_argument_list_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_list_2 ::= type_argument_2 ";
		return cup_result;

		case 59: { // type_argument_list_2 ::= type_argument_list COMMA type_argument_2 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 186/*type_argument_list_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_list_2 ::= type_argument_list COMMA type_argument_2 ";
		return cup_result;

		case 60: { // type_argument_list_3 ::= type_argument_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 188/*type_argument_list_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_list_3 ::= type_argument_3 ";
		return cup_result;

		case 61: { // type_argument_list_3 ::= type_argument_list COMMA type_argument_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 188/*type_argument_list_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_list_3 ::= type_argument_list COMMA type_argument_3 ";
		return cup_result;

		case 62: { // type_argument ::= reference_type 
			JagAstType *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 210/*type_argument*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument ::= reference_type ";
		return cup_result;

		case 63: { // type_argument ::= wildcard 
			JagAstType *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 210/*type_argument*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument ::= wildcard ";
		return cup_result;

		case 64: { // type_argument_1 ::= reference_type_1 
			JagAstType *RESULT = NULL;
				JagToken *cup_t = context_class->getFromTop(parser_context, 0);
				JagAstType *t = (JagAstType *) (cup_t->value);
 RESULT=t; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 211/*type_argument_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_1 ::= reference_type_1 ";
		return cup_result;

		case 65: { // type_argument_1 ::= wildcard_1 
			JagAstType *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 211/*type_argument_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_1 ::= wildcard_1 ";
		return cup_result;

		case 66: { // type_argument_2 ::= reference_type_2 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 208/*type_argument_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_2 ::= reference_type_2 ";
		return cup_result;

		case 67: { // type_argument_2 ::= wildcard_2 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 208/*type_argument_2*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_2 ::= wildcard_2 ";
		return cup_result;

		case 68: { // type_argument_3 ::= reference_type_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 209/*type_argument_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_3 ::= reference_type_3 ";
		return cup_result;

		case 69: { // type_argument_3 ::= wildcard_3 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 209/*type_argument_3*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_argument_3 ::= wildcard_3 ";
		return cup_result;

		case 70: { // name ::= simple_name 
			CatStringWo *RESULT = NULL;
				JagToken *cup_sn = context_class->getFromTop(parser_context, 0);
				CatStringWo *sn = (CatStringWo *) (cup_sn->value);
 RESULT = sn; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 14/*name*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "name ::= simple_name ";
		return cup_result;

		case 71: { // name ::= qualified_name 
			CatStringWo *RESULT = NULL;
				JagToken *cup_qn = context_class->getFromTop(parser_context, 0);
				CatStringWo *qn = (CatStringWo *) (cup_qn->value);

						RESULT = qn;
					
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 14/*name*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "name ::= qualified_name ";
		return cup_result;

		case 72: { // simple_name ::= IDENTIFIER 
			CatStringWo *RESULT = NULL;
				JagToken *cup_id = context_class->getFromTop(parser_context, 0);
				CatStringWo *id = (CatStringWo *) (cup_id->value);
 RESULT=id; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 13/*simple_name*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "simple_name ::= IDENTIFIER ";
		return cup_result;

		case 73: { // qualified_name ::= name DOT IDENTIFIER 
			CatStringWo *RESULT = NULL;
				JagToken *cup_nm = context_class->getFromTop(parser_context, 2);
				CatStringWo *nm = (CatStringWo *) (cup_nm->value);
				JagToken *cup_id = context_class->getFromTop(parser_context, 0);
				CatStringWo *id = (CatStringWo *) (cup_id->value);

					     CatStringWo *e_tmp = cat_string_wo_clone(nm, CAT_CLONE_DEPTH_MAIN);
					     cat_string_wo_append_unichar(e_tmp, '.');
					     cat_string_wo_append(e_tmp, id);
					  	 RESULT = cat_string_wo_anchor(e_tmp, 0);
					  	 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 15/*qualified_name*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 
					  	 cat_unref_ptr(RESULT);
					  
		}
		cup_result->symbol_text = "qualified_name ::= name DOT IDENTIFIER ";
		return cup_result;

		case 74: { // compilation_unit ::= simple_compilation_unit 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 16/*compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "compilation_unit ::= simple_compilation_unit ";
		return cup_result;

		case 75: { // compilation_unit ::= importing_compilation_unit 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 16/*compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "compilation_unit ::= importing_compilation_unit ";
		return cup_result;

		case 76: { // compilation_unit ::= package_compilation_unit 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 16/*compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "compilation_unit ::= package_compilation_unit ";
		return cup_result;

		case 77: { // compilation_unit ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 16/*compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "compilation_unit ::= ";
		return cup_result;

		case 78: { // simple_compilation_unit ::= type_declarations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 18/*simple_compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "simple_compilation_unit ::= type_declarations ";
		return cup_result;

		case 79: { // importing_compilation_unit ::= import_declarations type_declarations_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 19/*importing_compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "importing_compilation_unit ::= import_declarations type_declarations_opt ";
		return cup_result;

		case 80: { // package_compilation_unit ::= package_declaration import_declarations_opt type_declarations_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 17/*package_compilation_unit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "package_compilation_unit ::= package_declaration import_declarations_opt type_declarations_opt ";
		return cup_result;

		case 81: { // import_declarations_opt ::= import_declarations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 21/*import_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declarations_opt ::= import_declarations ";
		return cup_result;

		case 82: { // import_declarations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 21/*import_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declarations_opt ::= ";
		return cup_result;

		case 83: { // type_declarations_opt ::= type_declarations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 23/*type_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declarations_opt ::= type_declarations ";
		return cup_result;

		case 84: { // type_declarations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 23/*type_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declarations_opt ::= ";
		return cup_result;

		case 85: { // import_declarations ::= import_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 22/*import_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declarations ::= import_declaration ";
		return cup_result;

		case 86: { // import_declarations ::= import_declarations import_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 22/*import_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declarations ::= import_declarations import_declaration ";
		return cup_result;

		case 87: { // type_declarations ::= type_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 24/*type_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declarations ::= type_declaration ";
		return cup_result;

		case 88: { // type_declarations ::= type_declarations type_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 24/*type_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declarations ::= type_declarations type_declaration ";
		return cup_result;

		case 89: { // package_declaration ::= annotations_opt PACKAGE name SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 20/*package_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "package_declaration ::= annotations_opt PACKAGE name SEMICOLON ";
		return cup_result;

		case 90: { // import_declaration ::= single_type_import_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 25/*import_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declaration ::= single_type_import_declaration ";
		return cup_result;

		case 91: { // import_declaration ::= type_import_on_demand_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 25/*import_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declaration ::= type_import_on_demand_declaration ";
		return cup_result;

		case 92: { // import_declaration ::= static_single_type_import_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 25/*import_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declaration ::= static_single_type_import_declaration ";
		return cup_result;

		case 93: { // import_declaration ::= static_type_import_on_demand_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 25/*import_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "import_declaration ::= static_type_import_on_demand_declaration ";
		return cup_result;

		case 94: { // single_type_import_declaration ::= IMPORT name SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 26/*single_type_import_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "single_type_import_declaration ::= IMPORT name SEMICOLON ";
		return cup_result;

		case 95: { // static_single_type_import_declaration ::= IMPORT STATIC name SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 28/*static_single_type_import_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "static_single_type_import_declaration ::= IMPORT STATIC name SEMICOLON ";
		return cup_result;

		case 96: { // type_import_on_demand_declaration ::= IMPORT name DOT OP_MUL SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 27/*type_import_on_demand_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_import_on_demand_declaration ::= IMPORT name DOT OP_MUL SEMICOLON ";
		return cup_result;

		case 97: { // static_type_import_on_demand_declaration ::= IMPORT STATIC name DOT OP_MUL SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 29/*static_type_import_on_demand_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "static_type_import_on_demand_declaration ::= IMPORT STATIC name DOT OP_MUL SEMICOLON ";
		return cup_result;

		case 98: { // type_declaration ::= class_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 30/*type_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declaration ::= class_declaration ";
		return cup_result;

		case 99: { // type_declaration ::= enum_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 30/*type_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declaration ::= enum_declaration ";
		return cup_result;

		case 100: { // type_declaration ::= interface_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 30/*type_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declaration ::= interface_declaration ";
		return cup_result;

		case 101: { // type_declaration ::= SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 30/*type_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_declaration ::= SEMICOLON ";
		return cup_result;

		case 102: { // modifiers_opt ::= 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_NONE); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 32/*modifiers_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "modifiers_opt ::= ";
		return cup_result;

		case 103: { // modifiers_opt ::= modifiers 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_m = context_class->getFromTop(parser_context, 0);
				JagAstIndexerModifiers *m = (JagAstIndexerModifiers *) (cup_m->value);
 RESULT = m; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 32/*modifiers_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "modifiers_opt ::= modifiers ";
		return cup_result;

		case 104: { // modifiers_at ::= mixed_modifiers_at 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_mm = context_class->getFromTop(parser_context, 0);
				JagAstIndexerModifiers *mm = (JagAstIndexerModifiers *) (cup_mm->value);
 RESULT = mm; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 36/*modifiers_at*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "modifiers_at ::= mixed_modifiers_at ";
		return cup_result;

		case 105: { // modifiers_at ::= annotations_at 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_NONE); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 36/*modifiers_at*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "modifiers_at ::= annotations_at ";
		return cup_result;

		case 106: { // modifiers ::= mixed_modifiers 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_mm = context_class->getFromTop(parser_context, 0);
				JagAstIndexerModifiers *mm = (JagAstIndexerModifiers *) (cup_mm->value);
 RESULT = mm; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 31/*modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "modifiers ::= mixed_modifiers ";
		return cup_result;

		case 107: { // modifiers ::= annotations 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_NONE); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 31/*modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "modifiers ::= annotations ";
		return cup_result;

		case 108: { // mixed_modifiers_at ::= mixed_modifiers AT 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_mm = context_class->getFromTop(parser_context, 1);
				JagAstIndexerModifiers *mm = (JagAstIndexerModifiers *) (cup_mm->value);
 RESULT = mm; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 35/*mixed_modifiers_at*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "mixed_modifiers_at ::= mixed_modifiers AT ";
		return cup_result;

		case 109: { // mixed_modifiers ::= adhoc_modifier 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_a = context_class->getFromTop(parser_context, 0);
				JagAstIndexerModifiers *a = (JagAstIndexerModifiers *) (cup_a->value);
 RESULT = a; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 34/*mixed_modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "mixed_modifiers ::= adhoc_modifier ";
		return cup_result;

		case 110: { // mixed_modifiers ::= annotations adhoc_modifier 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_a = context_class->getFromTop(parser_context, 0);
				JagAstIndexerModifiers *a = (JagAstIndexerModifiers *) (cup_a->value);
 RESULT = a; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 34/*mixed_modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "mixed_modifiers ::= annotations adhoc_modifier ";
		return cup_result;

		case 111: { // mixed_modifiers ::= mixed_modifiers adhoc_modifier 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_mm = context_class->getFromTop(parser_context, 1);
				JagAstIndexerModifiers *mm = (JagAstIndexerModifiers *) (cup_mm->value);
				JagToken *cup_a = context_class->getFromTop(parser_context, 0);
				JagAstIndexerModifiers *a = (JagAstIndexerModifiers *) (cup_a->value);
 RESULT = jag_ast_indexer_modifiers_include(mm, a); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 34/*mixed_modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "mixed_modifiers ::= mixed_modifiers adhoc_modifier ";
		return cup_result;

		case 112: { // mixed_modifiers ::= mixed_modifiers_at annotation_body 
			JagAstIndexerModifiers *RESULT = NULL;
				JagToken *cup_mm = context_class->getFromTop(parser_context, 1);
				JagAstIndexerModifiers *mm = (JagAstIndexerModifiers *) (cup_mm->value);
 RESULT = mm; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 34/*mixed_modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "mixed_modifiers ::= mixed_modifiers_at annotation_body ";
		return cup_result;

		case 113: { // adhoc_modifier ::= PUBLIC 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_PUBLIC); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= PUBLIC ";
		return cup_result;

		case 114: { // adhoc_modifier ::= PROTECTED 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_PROTECTED); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= PROTECTED ";
		return cup_result;

		case 115: { // adhoc_modifier ::= PRIVATE 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_PRIVATE); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= PRIVATE ";
		return cup_result;

		case 116: { // adhoc_modifier ::= STATIC 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_STATIC); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= STATIC ";
		return cup_result;

		case 117: { // adhoc_modifier ::= ABSTRACT 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_ABSTRACT); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= ABSTRACT ";
		return cup_result;

		case 118: { // adhoc_modifier ::= FINAL 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_FINAL); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= FINAL ";
		return cup_result;

		case 119: { // adhoc_modifier ::= NATIVE 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_NATIVE); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= NATIVE ";
		return cup_result;

		case 120: { // adhoc_modifier ::= SYNCHRONIZED 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_SYNCHRONIZED); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= SYNCHRONIZED ";
		return cup_result;

		case 121: { // adhoc_modifier ::= TRANSIENT 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_TRANSIENT); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= TRANSIENT ";
		return cup_result;

		case 122: { // adhoc_modifier ::= VOLATILE 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_VOLATILE); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= VOLATILE ";
		return cup_result;

		case 123: { // adhoc_modifier ::= STRICTFP 
			JagAstIndexerModifiers *RESULT = NULL;
 RESULT = jag_ast_indexer_modifiers_new(JAG_AST_INDEXER_MODIFIER_STRICTFP); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 33/*adhoc_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "adhoc_modifier ::= STRICTFP ";
		return cup_result;

		case 124: { // annotations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 230/*annotations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotations_opt ::= ";
		return cup_result;

		case 125: { // annotations_opt ::= annotations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 230/*annotations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotations_opt ::= annotations ";
		return cup_result;

		case 126: { // annotations ::= AT annotation_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 231/*annotations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotations ::= AT annotation_body ";
		return cup_result;

		case 127: { // annotations ::= annotations_at annotation_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 231/*annotations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotations ::= annotations_at annotation_body ";
		return cup_result;

		case 128: { // annotations_at ::= annotations AT 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 232/*annotations_at*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotations_at ::= annotations AT ";
		return cup_result;

		case 129: { // annotation ::= AT annotation_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 233/*annotation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation ::= AT annotation_body ";
		return cup_result;

		case 130: { // annotation_body ::= normal_annotation_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 234/*annotation_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_body ::= normal_annotation_body ";
		return cup_result;

		case 131: { // annotation_body ::= marker_annotation_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 234/*annotation_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_body ::= marker_annotation_body ";
		return cup_result;

		case 132: { // annotation_body ::= single_element_annotation_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 234/*annotation_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_body ::= single_element_annotation_body ";
		return cup_result;

		case 133: { // normal_annotation_body ::= IDENTIFIER LPAREN element_value_pairs_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 235/*normal_annotation_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "normal_annotation_body ::= IDENTIFIER LPAREN element_value_pairs_opt RPAREN ";
		return cup_result;

		case 134: { // marker_annotation_body ::= IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 236/*marker_annotation_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "marker_annotation_body ::= IDENTIFIER ";
		return cup_result;

		case 135: { // single_element_annotation_body ::= IDENTIFIER LPAREN element_value RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 237/*single_element_annotation_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "single_element_annotation_body ::= IDENTIFIER LPAREN element_value RPAREN ";
		return cup_result;

		case 136: { // element_value_pairs_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 244/*element_value_pairs_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value_pairs_opt ::= ";
		return cup_result;

		case 137: { // element_value_pairs_opt ::= element_value_pairs 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 244/*element_value_pairs_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value_pairs_opt ::= element_value_pairs ";
		return cup_result;

		case 138: { // element_value_pairs ::= element_value_pair 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 245/*element_value_pairs*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value_pairs ::= element_value_pair ";
		return cup_result;

		case 139: { // element_value_pairs ::= element_value_pairs COMMA element_value_pair 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 245/*element_value_pairs*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value_pairs ::= element_value_pairs COMMA element_value_pair ";
		return cup_result;

		case 140: { // element_value_pair ::= IDENTIFIER ASS_IS element_value 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 246/*element_value_pair*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value_pair ::= IDENTIFIER ASS_IS element_value ";
		return cup_result;

		case 141: { // element_value ::= annotation 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 249/*element_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value ::= annotation ";
		return cup_result;

		case 142: { // element_value ::= element_value_array_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 249/*element_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value ::= element_value_array_initializer ";
		return cup_result;

		case 143: { // element_value ::= conditional_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 249/*element_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value ::= conditional_expression ";
		return cup_result;

		case 144: { // element_value_array_initializer ::= LBRACE element_values_opt RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 250/*element_value_array_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_value_array_initializer ::= LBRACE element_values_opt RBRACE ";
		return cup_result;

		case 145: { // element_values_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 247/*element_values_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_values_opt ::= ";
		return cup_result;

		case 146: { // element_values_opt ::= element_values 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 247/*element_values_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_values_opt ::= element_values ";
		return cup_result;

		case 147: { // element_values ::= element_value 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 248/*element_values*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_values ::= element_value ";
		return cup_result;

		case 148: { // element_values ::= element_values COMMA element_value 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 248/*element_values*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "element_values ::= element_values COMMA element_value ";
		return cup_result;

		case 149: { // class_declaration ::= class_header class_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 37/*class_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_declaration ::= class_header class_body ";
		return cup_result;

		case 150: { // class_header ::= modifiers_opt CLASS IDENTIFIER type_parameters_opt super_opt interfaces_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 38/*class_header*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_header ::= modifiers_opt CLASS IDENTIFIER type_parameters_opt super_opt interfaces_opt ";
		return cup_result;

		case 151: { // super_ext ::= EXTENDS class_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 39/*super_ext*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "super_ext ::= EXTENDS class_type ";
		return cup_result;

		case 152: { // super_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 40/*super_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "super_opt ::= ";
		return cup_result;

		case 153: { // super_opt ::= super_ext 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 40/*super_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "super_opt ::= super_ext ";
		return cup_result;

		case 154: { // interfaces ::= IMPLEMENTS interface_type_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 41/*interfaces*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interfaces ::= IMPLEMENTS interface_type_list ";
		return cup_result;

		case 155: { // interfaces_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 42/*interfaces_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interfaces_opt ::= ";
		return cup_result;

		case 156: { // interfaces_opt ::= interfaces 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 42/*interfaces_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interfaces_opt ::= interfaces ";
		return cup_result;

		case 157: { // interface_type_list ::= interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 43/*interface_type_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_type_list ::= interface_type ";
		return cup_result;

		case 158: { // interface_type_list ::= interface_type_list COMMA interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 43/*interface_type_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_type_list ::= interface_type_list COMMA interface_type ";
		return cup_result;

		case 159: { // class_body ::= LBRACE class_body_declarations_opt RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 44/*class_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body ::= LBRACE class_body_declarations_opt RBRACE ";
		return cup_result;

		case 160: { // class_body_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 45/*class_body_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_opt ::= ";
		return cup_result;

		case 161: { // class_body_opt ::= class_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 45/*class_body_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_opt ::= class_body ";
		return cup_result;

		case 162: { // class_body_declarations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 47/*class_body_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declarations_opt ::= ";
		return cup_result;

		case 163: { // class_body_declarations_opt ::= class_body_declarations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 47/*class_body_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declarations_opt ::= class_body_declarations ";
		return cup_result;

		case 164: { // class_body_declarations ::= class_body_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 46/*class_body_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declarations ::= class_body_declaration ";
		return cup_result;

		case 165: { // class_body_declarations ::= class_body_declarations class_body_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 46/*class_body_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declarations ::= class_body_declarations class_body_declaration ";
		return cup_result;

		case 166: { // class_body_declaration ::= class_member_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 48/*class_body_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declaration ::= class_member_declaration ";
		return cup_result;

		case 167: { // class_body_declaration ::= static_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 48/*class_body_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declaration ::= static_initializer ";
		return cup_result;

		case 168: { // class_body_declaration ::= constructor_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 48/*class_body_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declaration ::= constructor_declaration ";
		return cup_result;

		case 169: { // class_body_declaration ::= block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 48/*class_body_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_body_declaration ::= block ";
		return cup_result;

		case 170: { // class_member_declaration ::= field_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 49/*class_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_member_declaration ::= field_declaration ";
		return cup_result;

		case 171: { // class_member_declaration ::= method_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 49/*class_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_member_declaration ::= method_declaration ";
		return cup_result;

		case 172: { // class_member_declaration ::= modifiers_opt CLASS IDENTIFIER type_parameters_opt super_opt interfaces_opt class_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 49/*class_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_member_declaration ::= modifiers_opt CLASS IDENTIFIER type_parameters_opt super_opt interfaces_opt class_body ";
		return cup_result;

		case 173: { // class_member_declaration ::= enum_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 49/*class_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_member_declaration ::= enum_declaration ";
		return cup_result;

		case 174: { // class_member_declaration ::= interface_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 49/*class_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_member_declaration ::= interface_declaration ";
		return cup_result;

		case 175: { // class_member_declaration ::= SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 49/*class_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_member_declaration ::= SEMICOLON ";
		return cup_result;

		case 176: { // enum_declaration ::= modifiers_opt ENUM IDENTIFIER interfaces_opt enum_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 50/*enum_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_declaration ::= modifiers_opt ENUM IDENTIFIER interfaces_opt enum_body ";
		return cup_result;

		case 177: { // enum_body ::= LBRACE enum_constants_opt enum_body_declarations_opt RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 51/*enum_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_body ::= LBRACE enum_constants_opt enum_body_declarations_opt RBRACE ";
		return cup_result;

		case 178: { // enum_constants_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 52/*enum_constants_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constants_opt ::= ";
		return cup_result;

		case 179: { // enum_constants_opt ::= enum_constants 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 52/*enum_constants_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constants_opt ::= enum_constants ";
		return cup_result;

		case 180: { // enum_constants_opt ::= enum_constants COMMA 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 52/*enum_constants_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constants_opt ::= enum_constants COMMA ";
		return cup_result;

		case 181: { // enum_constants ::= enum_constant 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 53/*enum_constants*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constants ::= enum_constant ";
		return cup_result;

		case 182: { // enum_constants ::= enum_constants COMMA enum_constant 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 53/*enum_constants*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constants ::= enum_constants COMMA enum_constant ";
		return cup_result;

		case 183: { // enum_constant ::= annotations_opt IDENTIFIER enum_arguments_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 54/*enum_constant*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constant ::= annotations_opt IDENTIFIER enum_arguments_opt ";
		return cup_result;

		case 184: { // enum_constant ::= annotations_opt IDENTIFIER enum_arguments_opt class_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 54/*enum_constant*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_constant ::= annotations_opt IDENTIFIER enum_arguments_opt class_body ";
		return cup_result;

		case 185: { // enum_arguments_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 55/*enum_arguments_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_arguments_opt ::= ";
		return cup_result;

		case 186: { // enum_arguments_opt ::= LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 55/*enum_arguments_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_arguments_opt ::= LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 187: { // enum_body_declarations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 56/*enum_body_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_body_declarations_opt ::= ";
		return cup_result;

		case 188: { // enum_body_declarations_opt ::= SEMICOLON class_body_declarations_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 56/*enum_body_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "enum_body_declarations_opt ::= SEMICOLON class_body_declarations_opt ";
		return cup_result;

		case 189: { // field_declaration ::= modifiers_opt type variable_declarators SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 57/*field_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "field_declaration ::= modifiers_opt type variable_declarators SEMICOLON ";
		return cup_result;

		case 190: { // variable_declarators ::= variable_declarator 
			CatArrayWo *RESULT = NULL;
				JagToken *cup_vd = context_class->getFromTop(parser_context, 0);
				JagAstDeclarator *vd = (JagAstDeclarator *) (cup_vd->value);
 RESULT = cat_array_wo_new(); cat_array_wo_append(RESULT, (GObject *) vd); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 58/*variable_declarators*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "variable_declarators ::= variable_declarator ";
		return cup_result;

		case 191: { // variable_declarators ::= variable_declarators COMMA variable_declarator 
			CatArrayWo *RESULT = NULL;
				JagToken *cup_vds = context_class->getFromTop(parser_context, 2);
				CatArrayWo *vds = (CatArrayWo *) (cup_vds->value);
				JagToken *cup_vd = context_class->getFromTop(parser_context, 0);
				JagAstDeclarator *vd = (JagAstDeclarator *) (cup_vd->value);
 RESULT = vds; cat_array_wo_append(RESULT, (GObject *) vd); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 58/*variable_declarators*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_declarators ::= variable_declarators COMMA variable_declarator ";
		return cup_result;

		case 192: { // variable_declarator ::= variable_declarator_id 
			JagAstDeclarator *RESULT = NULL;
				JagToken *cup_vdi = context_class->getFromTop(parser_context, 0);
				JagAstDeclarator *vdi = (JagAstDeclarator *) (cup_vdi->value);
 RESULT = vdi; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 59/*variable_declarator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_declarator ::= variable_declarator_id ";
		return cup_result;

		case 193: { // variable_declarator ::= variable_declarator_id ASS_IS variable_initializer 
			JagAstDeclarator *RESULT = NULL;
				JagToken *cup_vdi = context_class->getFromTop(parser_context, 2);
				JagAstDeclarator *vdi = (JagAstDeclarator *) (cup_vdi->value);
 RESULT = vdi; 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 59/*variable_declarator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_declarator ::= variable_declarator_id ASS_IS variable_initializer ";
		return cup_result;

		case 194: { // variable_declarator_id ::= IDENTIFIER 
			JagAstDeclarator *RESULT = NULL;
				JagToken *cup_id = context_class->getFromTop(parser_context, 0);
				CatStringWo *id = (CatStringWo *) (cup_id->value);
 RESULT = jag_ast_declarator_new(id); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 60/*variable_declarator_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "variable_declarator_id ::= IDENTIFIER ";
		return cup_result;

		case 195: { // variable_declarator_id ::= variable_declarator_id LBRACK RBRACK 
			JagAstDeclarator *RESULT = NULL;
				JagToken *cup_di = context_class->getFromTop(parser_context, 2);
				JagAstDeclarator *di = (JagAstDeclarator *) (cup_di->value);
 RESULT = di; jag_ast_declarator_add_dimension(di); 
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 60/*variable_declarator_id*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_declarator_id ::= variable_declarator_id LBRACK RBRACK ";
		return cup_result;

		case 196: { // variable_initializer ::= expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 61/*variable_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_initializer ::= expression ";
		return cup_result;

		case 197: { // variable_initializer ::= array_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 61/*variable_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_initializer ::= array_initializer ";
		return cup_result;

		case 198: { // method_declaration ::= method_header method_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 62/*method_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_declaration ::= method_header method_body ";
		return cup_result;

		case 199: { // method_header ::= modifiers_opt type method_declarator throws_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 63/*method_header*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_header ::= modifiers_opt type method_declarator throws_opt ";
		return cup_result;

		case 200: { // method_header ::= modifiers_opt OP_LT type_parameter_list_1 type method_declarator throws_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 63/*method_header*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_header ::= modifiers_opt OP_LT type_parameter_list_1 type method_declarator throws_opt ";
		return cup_result;

		case 201: { // method_header ::= modifiers_opt VOID method_declarator throws_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 63/*method_header*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_header ::= modifiers_opt VOID method_declarator throws_opt ";
		return cup_result;

		case 202: { // method_header ::= modifiers_opt OP_LT type_parameter_list_1 VOID method_declarator throws_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 63/*method_header*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_header ::= modifiers_opt OP_LT type_parameter_list_1 VOID method_declarator throws_opt ";
		return cup_result;

		case 203: { // method_declarator ::= IDENTIFIER LPAREN formal_parameter_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 64/*method_declarator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_declarator ::= IDENTIFIER LPAREN formal_parameter_list_opt RPAREN ";
		return cup_result;

		case 204: { // method_declarator ::= method_declarator LBRACK RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 64/*method_declarator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_declarator ::= method_declarator LBRACK RBRACK ";
		return cup_result;

		case 205: { // formal_parameter_list_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 65/*formal_parameter_list_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_list_opt ::= ";
		return cup_result;

		case 206: { // formal_parameter_list_opt ::= formal_parameter_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 65/*formal_parameter_list_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_list_opt ::= formal_parameter_list ";
		return cup_result;

		case 207: { // formal_parameter_list ::= formal_parameter 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 66/*formal_parameter_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_list ::= formal_parameter ";
		return cup_result;

		case 208: { // formal_parameter_list ::= formal_parameter_list COMMA formal_parameter 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 66/*formal_parameter_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_list ::= formal_parameter_list COMMA formal_parameter ";
		return cup_result;

		case 209: { // formal_parameter ::= formal_parameter_modifiers_opt type variable_declarator_id 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 67/*formal_parameter*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter ::= formal_parameter_modifiers_opt type variable_declarator_id ";
		return cup_result;

		case 210: { // formal_parameter ::= formal_parameter_modifiers_opt type ELLIPSIS IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 67/*formal_parameter*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter ::= formal_parameter_modifiers_opt type ELLIPSIS IDENTIFIER ";
		return cup_result;

		case 211: { // formal_parameter_modifiers_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 68/*formal_parameter_modifiers_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_modifiers_opt ::= ";
		return cup_result;

		case 212: { // formal_parameter_modifiers_opt ::= formal_parameter_modifiers 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 68/*formal_parameter_modifiers_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_modifiers_opt ::= formal_parameter_modifiers ";
		return cup_result;

		case 213: { // formal_parameter_modifiers ::= formal_parameter_modifier 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 69/*formal_parameter_modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_modifiers ::= formal_parameter_modifier ";
		return cup_result;

		case 214: { // formal_parameter_modifiers ::= formal_parameter_modifiers formal_parameter_modifier 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 69/*formal_parameter_modifiers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_modifiers ::= formal_parameter_modifiers formal_parameter_modifier ";
		return cup_result;

		case 215: { // formal_parameter_modifier ::= FINAL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 70/*formal_parameter_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_modifier ::= FINAL ";
		return cup_result;

		case 216: { // formal_parameter_modifier ::= annotation 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 70/*formal_parameter_modifier*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "formal_parameter_modifier ::= annotation ";
		return cup_result;

		case 217: { // throws_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 71/*throws_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "throws_opt ::= ";
		return cup_result;

		case 218: { // throws_opt ::= throws 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 71/*throws_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "throws_opt ::= throws ";
		return cup_result;

		case 219: { // throws ::= THROWS class_type_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 72/*throws*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "throws ::= THROWS class_type_list ";
		return cup_result;

		case 220: { // class_type_list ::= class_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 73/*class_type_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_type_list ::= class_type ";
		return cup_result;

		case 221: { // class_type_list ::= class_type_list COMMA class_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 73/*class_type_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_type_list ::= class_type_list COMMA class_type ";
		return cup_result;

		case 222: { // method_body ::= block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 74/*method_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_body ::= block ";
		return cup_result;

		case 223: { // method_body ::= SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 74/*method_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_body ::= SEMICOLON ";
		return cup_result;

		case 224: { // static_initializer ::= STATIC block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 75/*static_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "static_initializer ::= STATIC block ";
		return cup_result;

		case 225: { // constructor_declaration ::= modifiers_opt constructor_declarator throws_opt constructor_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 76/*constructor_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_declaration ::= modifiers_opt constructor_declarator throws_opt constructor_body ";
		return cup_result;

		case 226: { // constructor_declaration ::= modifiers_opt OP_LT type_parameter_list_1 constructor_declarator throws_opt constructor_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 76/*constructor_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_declaration ::= modifiers_opt OP_LT type_parameter_list_1 constructor_declarator throws_opt constructor_body ";
		return cup_result;

		case 227: { // constructor_declarator ::= simple_name LPAREN formal_parameter_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 77/*constructor_declarator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_declarator ::= simple_name LPAREN formal_parameter_list_opt RPAREN ";
		return cup_result;

		case 228: { // constructor_body ::= LBRACE explicit_constructor_invocation block_statements RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 78/*constructor_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_body ::= LBRACE explicit_constructor_invocation block_statements RBRACE ";
		return cup_result;

		case 229: { // constructor_body ::= LBRACE explicit_constructor_invocation RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 78/*constructor_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_body ::= LBRACE explicit_constructor_invocation RBRACE ";
		return cup_result;

		case 230: { // constructor_body ::= LBRACE block_statements RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 78/*constructor_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_body ::= LBRACE block_statements RBRACE ";
		return cup_result;

		case 231: { // constructor_body ::= LBRACE RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 78/*constructor_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constructor_body ::= LBRACE RBRACE ";
		return cup_result;

		case 232: { // explicit_constructor_invocation ::= THIS LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= THIS LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 233: { // explicit_constructor_invocation ::= type_arguments THIS LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= type_arguments THIS LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 234: { // explicit_constructor_invocation ::= SUPER LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= SUPER LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 235: { // explicit_constructor_invocation ::= type_arguments SUPER LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= type_arguments SUPER LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 236: { // explicit_constructor_invocation ::= primary DOT SUPER LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= primary DOT SUPER LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 237: { // explicit_constructor_invocation ::= primary DOT type_arguments SUPER LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 7)->left, context_class->getFromTop(parser_context, 7)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= primary DOT type_arguments SUPER LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 238: { // explicit_constructor_invocation ::= name DOT SUPER LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= name DOT SUPER LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 239: { // explicit_constructor_invocation ::= name DOT type_arguments SUPER LPAREN argument_list_opt RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 79/*explicit_constructor_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 7)->left, context_class->getFromTop(parser_context, 7)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "explicit_constructor_invocation ::= name DOT type_arguments SUPER LPAREN argument_list_opt RPAREN SEMICOLON ";
		return cup_result;

		case 240: { // interface_declaration ::= normal_interface_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 80/*interface_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_declaration ::= normal_interface_declaration ";
		return cup_result;

		case 241: { // interface_declaration ::= annotation_type_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 80/*interface_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_declaration ::= annotation_type_declaration ";
		return cup_result;

		case 242: { // normal_interface_declaration ::= modifiers_opt INTERFACE IDENTIFIER type_parameters_opt extends_interfaces_opt interface_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 81/*normal_interface_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "normal_interface_declaration ::= modifiers_opt INTERFACE IDENTIFIER type_parameters_opt extends_interfaces_opt interface_body ";
		return cup_result;

		case 243: { // annotation_type_declaration ::= AT INTERFACE IDENTIFIER annotation_type_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 82/*annotation_type_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_declaration ::= AT INTERFACE IDENTIFIER annotation_type_body ";
		return cup_result;

		case 244: { // annotation_type_declaration ::= modifiers_at INTERFACE IDENTIFIER annotation_type_body 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 82/*annotation_type_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_declaration ::= modifiers_at INTERFACE IDENTIFIER annotation_type_body ";
		return cup_result;

		case 245: { // annotation_type_body ::= LBRACE annotation_type_element_declarations_opt RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 238/*annotation_type_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_body ::= LBRACE annotation_type_element_declarations_opt RBRACE ";
		return cup_result;

		case 246: { // annotation_type_element_declarations_opt ::= annotation_type_element_declarations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 240/*annotation_type_element_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declarations_opt ::= annotation_type_element_declarations ";
		return cup_result;

		case 247: { // annotation_type_element_declarations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 240/*annotation_type_element_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declarations_opt ::= ";
		return cup_result;

		case 248: { // annotation_type_element_declarations ::= annotation_type_element_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 239/*annotation_type_element_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declarations ::= annotation_type_element_declaration ";
		return cup_result;

		case 249: { // annotation_type_element_declarations ::= annotation_type_element_declarations annotation_type_element_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 239/*annotation_type_element_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declarations ::= annotation_type_element_declarations annotation_type_element_declaration ";
		return cup_result;

		case 250: { // annotation_type_element_declaration ::= constant_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 241/*annotation_type_element_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declaration ::= constant_declaration ";
		return cup_result;

		case 251: { // annotation_type_element_declaration ::= modifiers_opt type IDENTIFIER LPAREN RPAREN default_value_opt SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 241/*annotation_type_element_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declaration ::= modifiers_opt type IDENTIFIER LPAREN RPAREN default_value_opt SEMICOLON ";
		return cup_result;

		case 252: { // annotation_type_element_declaration ::= class_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 241/*annotation_type_element_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declaration ::= class_declaration ";
		return cup_result;

		case 253: { // annotation_type_element_declaration ::= enum_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 241/*annotation_type_element_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declaration ::= enum_declaration ";
		return cup_result;

		case 254: { // annotation_type_element_declaration ::= interface_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 241/*annotation_type_element_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declaration ::= interface_declaration ";
		return cup_result;

		case 255: { // annotation_type_element_declaration ::= SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 241/*annotation_type_element_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "annotation_type_element_declaration ::= SEMICOLON ";
		return cup_result;

		case 256: { // default_value_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 242/*default_value_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "default_value_opt ::= ";
		return cup_result;

		case 257: { // default_value_opt ::= default_value 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 242/*default_value_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "default_value_opt ::= default_value ";
		return cup_result;

		case 258: { // default_value ::= DEFAULT element_value 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 243/*default_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "default_value ::= DEFAULT element_value ";
		return cup_result;

		case 259: { // extends_interfaces_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 83/*extends_interfaces_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "extends_interfaces_opt ::= ";
		return cup_result;

		case 260: { // extends_interfaces_opt ::= extends_interfaces 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 83/*extends_interfaces_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "extends_interfaces_opt ::= extends_interfaces ";
		return cup_result;

		case 261: { // extends_interfaces ::= EXTENDS interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 84/*extends_interfaces*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "extends_interfaces ::= EXTENDS interface_type ";
		return cup_result;

		case 262: { // extends_interfaces ::= extends_interfaces COMMA interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 84/*extends_interfaces*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "extends_interfaces ::= extends_interfaces COMMA interface_type ";
		return cup_result;

		case 263: { // interface_body ::= LBRACE interface_member_declarations_opt RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 85/*interface_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_body ::= LBRACE interface_member_declarations_opt RBRACE ";
		return cup_result;

		case 264: { // interface_member_declarations_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 86/*interface_member_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declarations_opt ::= ";
		return cup_result;

		case 265: { // interface_member_declarations_opt ::= interface_member_declarations 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 86/*interface_member_declarations_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declarations_opt ::= interface_member_declarations ";
		return cup_result;

		case 266: { // interface_member_declarations ::= interface_member_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 87/*interface_member_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declarations ::= interface_member_declaration ";
		return cup_result;

		case 267: { // interface_member_declarations ::= interface_member_declarations interface_member_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 87/*interface_member_declarations*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declarations ::= interface_member_declarations interface_member_declaration ";
		return cup_result;

		case 268: { // interface_member_declaration ::= constant_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 88/*interface_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declaration ::= constant_declaration ";
		return cup_result;

		case 269: { // interface_member_declaration ::= abstract_method_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 88/*interface_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declaration ::= abstract_method_declaration ";
		return cup_result;

		case 270: { // interface_member_declaration ::= class_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 88/*interface_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declaration ::= class_declaration ";
		return cup_result;

		case 271: { // interface_member_declaration ::= enum_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 88/*interface_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declaration ::= enum_declaration ";
		return cup_result;

		case 272: { // interface_member_declaration ::= interface_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 88/*interface_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declaration ::= interface_declaration ";
		return cup_result;

		case 273: { // interface_member_declaration ::= SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 88/*interface_member_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "interface_member_declaration ::= SEMICOLON ";
		return cup_result;

		case 274: { // constant_declaration ::= field_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 89/*constant_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constant_declaration ::= field_declaration ";
		return cup_result;

		case 275: { // abstract_method_declaration ::= method_header SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 90/*abstract_method_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "abstract_method_declaration ::= method_header SEMICOLON ";
		return cup_result;

		case 276: { // array_initializer ::= LBRACE variable_initializers COMMA RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 91/*array_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_initializer ::= LBRACE variable_initializers COMMA RBRACE ";
		return cup_result;

		case 277: { // array_initializer ::= LBRACE variable_initializers RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 91/*array_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_initializer ::= LBRACE variable_initializers RBRACE ";
		return cup_result;

		case 278: { // array_initializer ::= LBRACE COMMA RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 91/*array_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_initializer ::= LBRACE COMMA RBRACE ";
		return cup_result;

		case 279: { // array_initializer ::= LBRACE RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 91/*array_initializer*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_initializer ::= LBRACE RBRACE ";
		return cup_result;

		case 280: { // variable_initializers ::= variable_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 92/*variable_initializers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_initializers ::= variable_initializer ";
		return cup_result;

		case 281: { // variable_initializers ::= variable_initializers COMMA variable_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 92/*variable_initializers*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "variable_initializers ::= variable_initializers COMMA variable_initializer ";
		return cup_result;

		case 282: { // block ::= LBRACE block_statements_opt RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 93/*block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block ::= LBRACE block_statements_opt RBRACE ";
		return cup_result;

		case 283: { // block ::= LBRACE error RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 93/*block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block ::= LBRACE error RBRACE ";
		return cup_result;

		case 284: { // block_statements_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 94/*block_statements_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statements_opt ::= ";
		return cup_result;

		case 285: { // block_statements_opt ::= block_statements 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 94/*block_statements_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statements_opt ::= block_statements ";
		return cup_result;

		case 286: { // block_statements ::= block_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 95/*block_statements*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statements ::= block_statement ";
		return cup_result;

		case 287: { // block_statements ::= block_statements block_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 95/*block_statements*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statements ::= block_statements block_statement ";
		return cup_result;

		case 288: { // block_statements ::= block_statements error 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 95/*block_statements*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statements ::= block_statements error ";
		return cup_result;

		case 289: { // block_statement ::= local_variable_declaration_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 96/*block_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statement ::= local_variable_declaration_statement ";
		return cup_result;

		case 290: { // block_statement ::= statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 96/*block_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statement ::= statement ";
		return cup_result;

		case 291: { // block_statement ::= class_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 96/*block_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statement ::= class_declaration ";
		return cup_result;

		case 292: { // block_statement ::= enum_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 96/*block_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statement ::= enum_declaration ";
		return cup_result;

		case 293: { // block_statement ::= interface_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 96/*block_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_statement ::= interface_declaration ";
		return cup_result;

		case 294: { // local_variable_declaration_statement ::= local_variable_declaration SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 97/*local_variable_declaration_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "local_variable_declaration_statement ::= local_variable_declaration SEMICOLON ";
		return cup_result;

		case 295: { // local_variable_declaration ::= type variable_declarators 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 98/*local_variable_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "local_variable_declaration ::= type variable_declarators ";
		return cup_result;

		case 296: { // local_variable_declaration ::= modifiers type variable_declarators 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 98/*local_variable_declaration*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "local_variable_declaration ::= modifiers type variable_declarators ";
		return cup_result;

		case 297: { // statement ::= statement_without_trailing_substatement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= statement_without_trailing_substatement ";
		return cup_result;

		case 298: { // statement ::= labeled_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= labeled_statement ";
		return cup_result;

		case 299: { // statement ::= if_then_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= if_then_statement ";
		return cup_result;

		case 300: { // statement ::= if_then_else_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= if_then_else_statement ";
		return cup_result;

		case 301: { // statement ::= while_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= while_statement ";
		return cup_result;

		case 302: { // statement ::= for_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= for_statement ";
		return cup_result;

		case 303: { // statement ::= foreach_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 99/*statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement ::= foreach_statement ";
		return cup_result;

		case 304: { // statement_no_short_if ::= statement_without_trailing_substatement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 100/*statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_no_short_if ::= statement_without_trailing_substatement ";
		return cup_result;

		case 305: { // statement_no_short_if ::= labeled_statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 100/*statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_no_short_if ::= labeled_statement_no_short_if ";
		return cup_result;

		case 306: { // statement_no_short_if ::= if_then_else_statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 100/*statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_no_short_if ::= if_then_else_statement_no_short_if ";
		return cup_result;

		case 307: { // statement_no_short_if ::= while_statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 100/*statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_no_short_if ::= while_statement_no_short_if ";
		return cup_result;

		case 308: { // statement_no_short_if ::= for_statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 100/*statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_no_short_if ::= for_statement_no_short_if ";
		return cup_result;

		case 309: { // statement_no_short_if ::= foreach_statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 100/*statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_no_short_if ::= foreach_statement_no_short_if ";
		return cup_result;

		case 310: { // statement_without_trailing_substatement ::= block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= block ";
		return cup_result;

		case 311: { // statement_without_trailing_substatement ::= empty_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= empty_statement ";
		return cup_result;

		case 312: { // statement_without_trailing_substatement ::= expression_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= expression_statement ";
		return cup_result;

		case 313: { // statement_without_trailing_substatement ::= switch_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= switch_statement ";
		return cup_result;

		case 314: { // statement_without_trailing_substatement ::= do_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= do_statement ";
		return cup_result;

		case 315: { // statement_without_trailing_substatement ::= break_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= break_statement ";
		return cup_result;

		case 316: { // statement_without_trailing_substatement ::= continue_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= continue_statement ";
		return cup_result;

		case 317: { // statement_without_trailing_substatement ::= return_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= return_statement ";
		return cup_result;

		case 318: { // statement_without_trailing_substatement ::= synchronized_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= synchronized_statement ";
		return cup_result;

		case 319: { // statement_without_trailing_substatement ::= throw_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= throw_statement ";
		return cup_result;

		case 320: { // statement_without_trailing_substatement ::= try_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= try_statement ";
		return cup_result;

		case 321: { // statement_without_trailing_substatement ::= assert_statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 101/*statement_without_trailing_substatement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_without_trailing_substatement ::= assert_statement ";
		return cup_result;

		case 322: { // empty_statement ::= SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 102/*empty_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "empty_statement ::= SEMICOLON ";
		return cup_result;

		case 323: { // labeled_statement ::= IDENTIFIER COLON statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 103/*labeled_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "labeled_statement ::= IDENTIFIER COLON statement ";
		return cup_result;

		case 324: { // labeled_statement_no_short_if ::= IDENTIFIER COLON statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 104/*labeled_statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "labeled_statement_no_short_if ::= IDENTIFIER COLON statement_no_short_if ";
		return cup_result;

		case 325: { // expression_statement ::= statement_expression SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 105/*expression_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "expression_statement ::= statement_expression SEMICOLON ";
		return cup_result;

		case 326: { // statement_expression ::= assignment 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= assignment ";
		return cup_result;

		case 327: { // statement_expression ::= preincrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= preincrement_expression ";
		return cup_result;

		case 328: { // statement_expression ::= predecrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= predecrement_expression ";
		return cup_result;

		case 329: { // statement_expression ::= postincrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= postincrement_expression ";
		return cup_result;

		case 330: { // statement_expression ::= postdecrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= postdecrement_expression ";
		return cup_result;

		case 331: { // statement_expression ::= method_invocation 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= method_invocation ";
		return cup_result;

		case 332: { // statement_expression ::= class_instance_creation_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 106/*statement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression ::= class_instance_creation_expression ";
		return cup_result;

		case 333: { // if_then_statement ::= IF LPAREN expression RPAREN statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 107/*if_then_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "if_then_statement ::= IF LPAREN expression RPAREN statement ";
		return cup_result;

		case 334: { // if_then_else_statement ::= IF LPAREN expression RPAREN statement_no_short_if ELSE statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 108/*if_then_else_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "if_then_else_statement ::= IF LPAREN expression RPAREN statement_no_short_if ELSE statement ";
		return cup_result;

		case 335: { // if_then_else_statement_no_short_if ::= IF LPAREN expression RPAREN statement_no_short_if ELSE statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 109/*if_then_else_statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "if_then_else_statement_no_short_if ::= IF LPAREN expression RPAREN statement_no_short_if ELSE statement_no_short_if ";
		return cup_result;

		case 336: { // switch_statement ::= SWITCH LPAREN expression RPAREN switch_block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 110/*switch_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_statement ::= SWITCH LPAREN expression RPAREN switch_block ";
		return cup_result;

		case 337: { // switch_block ::= LBRACE switch_block_statement_groups switch_labels RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 111/*switch_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block ::= LBRACE switch_block_statement_groups switch_labels RBRACE ";
		return cup_result;

		case 338: { // switch_block ::= LBRACE switch_block_statement_groups RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 111/*switch_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block ::= LBRACE switch_block_statement_groups RBRACE ";
		return cup_result;

		case 339: { // switch_block ::= LBRACE switch_labels RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 111/*switch_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block ::= LBRACE switch_labels RBRACE ";
		return cup_result;

		case 340: { // switch_block ::= LBRACE RBRACE 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 111/*switch_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block ::= LBRACE RBRACE ";
		return cup_result;

		case 341: { // switch_block_statement_groups ::= switch_block_statement_group 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 112/*switch_block_statement_groups*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block_statement_groups ::= switch_block_statement_group ";
		return cup_result;

		case 342: { // switch_block_statement_groups ::= switch_block_statement_groups switch_block_statement_group 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 112/*switch_block_statement_groups*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block_statement_groups ::= switch_block_statement_groups switch_block_statement_group ";
		return cup_result;

		case 343: { // switch_block_statement_group ::= switch_labels block_statements 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 113/*switch_block_statement_group*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_block_statement_group ::= switch_labels block_statements ";
		return cup_result;

		case 344: { // switch_labels ::= switch_label 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 114/*switch_labels*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_labels ::= switch_label ";
		return cup_result;

		case 345: { // switch_labels ::= switch_labels switch_label 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 114/*switch_labels*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_labels ::= switch_labels switch_label ";
		return cup_result;

		case 346: { // switch_label ::= CASE constant_expression COLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 115/*switch_label*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_label ::= CASE constant_expression COLON ";
		return cup_result;

		case 347: { // switch_label ::= DEFAULT COLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 115/*switch_label*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "switch_label ::= DEFAULT COLON ";
		return cup_result;

		case 348: { // while_statement ::= WHILE LPAREN expression RPAREN statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 116/*while_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "while_statement ::= WHILE LPAREN expression RPAREN statement ";
		return cup_result;

		case 349: { // while_statement_no_short_if ::= WHILE LPAREN expression RPAREN statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 117/*while_statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "while_statement_no_short_if ::= WHILE LPAREN expression RPAREN statement_no_short_if ";
		return cup_result;

		case 350: { // do_statement ::= DO statement WHILE LPAREN expression RPAREN SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 118/*do_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "do_statement ::= DO statement WHILE LPAREN expression RPAREN SEMICOLON ";
		return cup_result;

		case 351: { // foreach_statement ::= FOR LPAREN type variable_declarator_id COLON expression RPAREN statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 119/*foreach_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 7)->left, context_class->getFromTop(parser_context, 7)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "foreach_statement ::= FOR LPAREN type variable_declarator_id COLON expression RPAREN statement ";
		return cup_result;

		case 352: { // foreach_statement ::= FOR IDENTIFIER LPAREN type variable_declarator_id IDENTIFIER expression RPAREN statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 119/*foreach_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 8)->left, context_class->getFromTop(parser_context, 8)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "foreach_statement ::= FOR IDENTIFIER LPAREN type variable_declarator_id IDENTIFIER expression RPAREN statement ";
		return cup_result;

		case 353: { // foreach_statement_no_short_if ::= FOR LPAREN type variable_declarator_id COLON expression RPAREN statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 120/*foreach_statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 7)->left, context_class->getFromTop(parser_context, 7)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "foreach_statement_no_short_if ::= FOR LPAREN type variable_declarator_id COLON expression RPAREN statement_no_short_if ";
		return cup_result;

		case 354: { // foreach_statement_no_short_if ::= FOR IDENTIFIER LPAREN type variable_declarator_id IDENTIFIER expression RPAREN statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 120/*foreach_statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 8)->left, context_class->getFromTop(parser_context, 8)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "foreach_statement_no_short_if ::= FOR IDENTIFIER LPAREN type variable_declarator_id IDENTIFIER expression RPAREN statement_no_short_if ";
		return cup_result;

		case 355: { // for_statement ::= FOR LPAREN for_init_opt SEMICOLON expression_opt SEMICOLON for_update_opt RPAREN statement 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 121/*for_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 8)->left, context_class->getFromTop(parser_context, 8)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_statement ::= FOR LPAREN for_init_opt SEMICOLON expression_opt SEMICOLON for_update_opt RPAREN statement ";
		return cup_result;

		case 356: { // for_statement_no_short_if ::= FOR LPAREN for_init_opt SEMICOLON expression_opt SEMICOLON for_update_opt RPAREN statement_no_short_if 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 122/*for_statement_no_short_if*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 8)->left, context_class->getFromTop(parser_context, 8)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_statement_no_short_if ::= FOR LPAREN for_init_opt SEMICOLON expression_opt SEMICOLON for_update_opt RPAREN statement_no_short_if ";
		return cup_result;

		case 357: { // for_init_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 123/*for_init_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_init_opt ::= ";
		return cup_result;

		case 358: { // for_init_opt ::= for_init 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 123/*for_init_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_init_opt ::= for_init ";
		return cup_result;

		case 359: { // for_init ::= statement_expression_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 124/*for_init*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_init ::= statement_expression_list ";
		return cup_result;

		case 360: { // for_init ::= local_variable_declaration 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 124/*for_init*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_init ::= local_variable_declaration ";
		return cup_result;

		case 361: { // for_update_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 125/*for_update_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_update_opt ::= ";
		return cup_result;

		case 362: { // for_update_opt ::= for_update 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 125/*for_update_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_update_opt ::= for_update ";
		return cup_result;

		case 363: { // for_update ::= statement_expression_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 126/*for_update*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "for_update ::= statement_expression_list ";
		return cup_result;

		case 364: { // statement_expression_list ::= statement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 127/*statement_expression_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression_list ::= statement_expression ";
		return cup_result;

		case 365: { // statement_expression_list ::= statement_expression_list COMMA statement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 127/*statement_expression_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "statement_expression_list ::= statement_expression_list COMMA statement_expression ";
		return cup_result;

		case 366: { // identifier_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 128/*identifier_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "identifier_opt ::= ";
		return cup_result;

		case 367: { // identifier_opt ::= IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 128/*identifier_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "identifier_opt ::= IDENTIFIER ";
		return cup_result;

		case 368: { // break_statement ::= BREAK identifier_opt SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 129/*break_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "break_statement ::= BREAK identifier_opt SEMICOLON ";
		return cup_result;

		case 369: { // continue_statement ::= CONTINUE identifier_opt SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 130/*continue_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "continue_statement ::= CONTINUE identifier_opt SEMICOLON ";
		return cup_result;

		case 370: { // return_statement ::= RETURN expression_opt SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 131/*return_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "return_statement ::= RETURN expression_opt SEMICOLON ";
		return cup_result;

		case 371: { // throw_statement ::= THROW expression SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 132/*throw_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "throw_statement ::= THROW expression SEMICOLON ";
		return cup_result;

		case 372: { // synchronized_statement ::= SYNCHRONIZED LPAREN expression RPAREN block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 133/*synchronized_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "synchronized_statement ::= SYNCHRONIZED LPAREN expression RPAREN block ";
		return cup_result;

		case 373: { // try_statement ::= TRY block catches 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 134/*try_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "try_statement ::= TRY block catches ";
		return cup_result;

		case 374: { // try_statement ::= TRY block catches_opt finally 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 134/*try_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "try_statement ::= TRY block catches_opt finally ";
		return cup_result;

		case 375: { // catches_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 135/*catches_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "catches_opt ::= ";
		return cup_result;

		case 376: { // catches_opt ::= catches 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 135/*catches_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "catches_opt ::= catches ";
		return cup_result;

		case 377: { // catches ::= catch_clause 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 136/*catches*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "catches ::= catch_clause ";
		return cup_result;

		case 378: { // catches ::= catches catch_clause 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 136/*catches*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "catches ::= catches catch_clause ";
		return cup_result;

		case 379: { // catch_clause ::= CATCH LPAREN formal_parameter RPAREN block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 137/*catch_clause*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "catch_clause ::= CATCH LPAREN formal_parameter RPAREN block ";
		return cup_result;

		case 380: { // finally ::= FINALLY block 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 138/*finally*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "finally ::= FINALLY block ";
		return cup_result;

		case 381: { // assert_statement ::= ASSERT expression SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 139/*assert_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assert_statement ::= ASSERT expression SEMICOLON ";
		return cup_result;

		case 382: { // assert_statement ::= ASSERT expression COLON expression SEMICOLON 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 139/*assert_statement*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assert_statement ::= ASSERT expression COLON expression SEMICOLON ";
		return cup_result;

		case 383: { // primary ::= primary_no_new_array 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 140/*primary*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary ::= primary_no_new_array ";
		return cup_result;

		case 384: { // primary ::= array_creation_init 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 140/*primary*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary ::= array_creation_init ";
		return cup_result;

		case 385: { // primary ::= array_creation_uninit 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 140/*primary*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary ::= array_creation_uninit ";
		return cup_result;

		case 386: { // primary_no_new_array ::= literal 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= literal ";
		return cup_result;

		case 387: { // primary_no_new_array ::= THIS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= THIS ";
		return cup_result;

		case 388: { // primary_no_new_array ::= LPAREN name RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= LPAREN name RPAREN ";
		return cup_result;

		case 389: { // primary_no_new_array ::= LPAREN expression_nn RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= LPAREN expression_nn RPAREN ";
		return cup_result;

		case 390: { // primary_no_new_array ::= class_instance_creation_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= class_instance_creation_expression ";
		return cup_result;

		case 391: { // primary_no_new_array ::= field_access 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= field_access ";
		return cup_result;

		case 392: { // primary_no_new_array ::= method_invocation 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= method_invocation ";
		return cup_result;

		case 393: { // primary_no_new_array ::= array_access 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= array_access ";
		return cup_result;

		case 394: { // primary_no_new_array ::= name DOT THIS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= name DOT THIS ";
		return cup_result;

		case 395: { // primary_no_new_array ::= VOID DOT CLASS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= VOID DOT CLASS ";
		return cup_result;

		case 396: { // primary_no_new_array ::= primitive_type DOT CLASS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= primitive_type DOT CLASS ";
		return cup_result;

		case 397: { // primary_no_new_array ::= primitive_type dims DOT CLASS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= primitive_type dims DOT CLASS ";
		return cup_result;

		case 398: { // primary_no_new_array ::= name DOT CLASS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= name DOT CLASS ";
		return cup_result;

		case 399: { // primary_no_new_array ::= name dims DOT CLASS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 141/*primary_no_new_array*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "primary_no_new_array ::= name dims DOT CLASS ";
		return cup_result;

		case 400: { // class_instance_creation_expression ::= NEW class_or_interface_type LPAREN argument_list_opt RPAREN class_body_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 142/*class_instance_creation_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_instance_creation_expression ::= NEW class_or_interface_type LPAREN argument_list_opt RPAREN class_body_opt ";
		return cup_result;

		case 401: { // class_instance_creation_expression ::= NEW type_arguments class_or_interface_type LPAREN argument_list_opt RPAREN class_body_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 142/*class_instance_creation_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_instance_creation_expression ::= NEW type_arguments class_or_interface_type LPAREN argument_list_opt RPAREN class_body_opt ";
		return cup_result;

		case 402: { // class_instance_creation_expression ::= primary DOT NEW type_arguments_opt IDENTIFIER type_arguments_opt LPAREN argument_list_opt RPAREN class_body_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 142/*class_instance_creation_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 9)->left, context_class->getFromTop(parser_context, 9)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_instance_creation_expression ::= primary DOT NEW type_arguments_opt IDENTIFIER type_arguments_opt LPAREN argument_list_opt RPAREN class_body_opt ";
		return cup_result;

		case 403: { // class_instance_creation_expression ::= name DOT NEW type_arguments_opt IDENTIFIER type_arguments_opt LPAREN argument_list_opt RPAREN class_body_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 142/*class_instance_creation_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 9)->left, context_class->getFromTop(parser_context, 9)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "class_instance_creation_expression ::= name DOT NEW type_arguments_opt IDENTIFIER type_arguments_opt LPAREN argument_list_opt RPAREN class_body_opt ";
		return cup_result;

		case 404: { // argument_list_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 143/*argument_list_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "argument_list_opt ::= ";
		return cup_result;

		case 405: { // argument_list_opt ::= argument_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 143/*argument_list_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "argument_list_opt ::= argument_list ";
		return cup_result;

		case 406: { // argument_list ::= expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 144/*argument_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "argument_list ::= expression ";
		return cup_result;

		case 407: { // argument_list ::= argument_list COMMA expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 144/*argument_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "argument_list ::= argument_list COMMA expression ";
		return cup_result;

		case 408: { // array_creation_uninit ::= NEW primitive_type dim_exprs dims_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 146/*array_creation_uninit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_creation_uninit ::= NEW primitive_type dim_exprs dims_opt ";
		return cup_result;

		case 409: { // array_creation_uninit ::= NEW class_or_interface_type dim_exprs dims_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 146/*array_creation_uninit*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_creation_uninit ::= NEW class_or_interface_type dim_exprs dims_opt ";
		return cup_result;

		case 410: { // array_creation_init ::= NEW primitive_type dims array_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 145/*array_creation_init*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_creation_init ::= NEW primitive_type dims array_initializer ";
		return cup_result;

		case 411: { // array_creation_init ::= NEW class_or_interface_type dims array_initializer 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 145/*array_creation_init*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_creation_init ::= NEW class_or_interface_type dims array_initializer ";
		return cup_result;

		case 412: { // dim_exprs ::= dim_expr 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 147/*dim_exprs*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dim_exprs ::= dim_expr ";
		return cup_result;

		case 413: { // dim_exprs ::= dim_exprs dim_expr 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 147/*dim_exprs*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dim_exprs ::= dim_exprs dim_expr ";
		return cup_result;

		case 414: { // dim_expr ::= LBRACK expression RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 148/*dim_expr*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dim_expr ::= LBRACK expression RBRACK ";
		return cup_result;

		case 415: { // dims_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 149/*dims_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dims_opt ::= ";
		return cup_result;

		case 416: { // dims_opt ::= dims 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 149/*dims_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dims_opt ::= dims ";
		return cup_result;

		case 417: { // dims ::= LBRACK RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 150/*dims*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dims ::= LBRACK RBRACK ";
		return cup_result;

		case 418: { // dims ::= dims LBRACK RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 150/*dims*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "dims ::= dims LBRACK RBRACK ";
		return cup_result;

		case 419: { // field_access ::= primary DOT IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 151/*field_access*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "field_access ::= primary DOT IDENTIFIER ";
		return cup_result;

		case 420: { // field_access ::= SUPER DOT IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 151/*field_access*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "field_access ::= SUPER DOT IDENTIFIER ";
		return cup_result;

		case 421: { // field_access ::= name DOT SUPER DOT IDENTIFIER 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 151/*field_access*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "field_access ::= name DOT SUPER DOT IDENTIFIER ";
		return cup_result;

		case 422: { // method_invocation ::= name LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= name LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 423: { // method_invocation ::= primary DOT IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= primary DOT IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 424: { // method_invocation ::= primary DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= primary DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 425: { // method_invocation ::= name DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= name DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 426: { // method_invocation ::= SUPER DOT IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 5)->left, context_class->getFromTop(parser_context, 5)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= SUPER DOT IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 427: { // method_invocation ::= SUPER DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= SUPER DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 428: { // method_invocation ::= name DOT SUPER DOT IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 7)->left, context_class->getFromTop(parser_context, 7)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= name DOT SUPER DOT IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 429: { // method_invocation ::= name DOT SUPER DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 152/*method_invocation*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 8)->left, context_class->getFromTop(parser_context, 8)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "method_invocation ::= name DOT SUPER DOT type_arguments IDENTIFIER LPAREN argument_list_opt RPAREN ";
		return cup_result;

		case 430: { // array_access ::= name LBRACK expression RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 153/*array_access*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_access ::= name LBRACK expression RBRACK ";
		return cup_result;

		case 431: { // array_access ::= primary_no_new_array LBRACK expression RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 153/*array_access*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_access ::= primary_no_new_array LBRACK expression RBRACK ";
		return cup_result;

		case 432: { // array_access ::= array_creation_init LBRACK expression RBRACK 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 153/*array_access*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "array_access ::= array_creation_init LBRACK expression RBRACK ";
		return cup_result;

		case 433: { // postfix_expression ::= primary 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 154/*postfix_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression ::= primary ";
		return cup_result;

		case 434: { // postfix_expression ::= name 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 154/*postfix_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression ::= name ";
		return cup_result;

		case 435: { // postfix_expression ::= postincrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 154/*postfix_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression ::= postincrement_expression ";
		return cup_result;

		case 436: { // postfix_expression ::= postdecrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 154/*postfix_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression ::= postdecrement_expression ";
		return cup_result;

		case 437: { // postincrement_expression ::= postfix_expression OP_INC 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 155/*postincrement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postincrement_expression ::= postfix_expression OP_INC ";
		return cup_result;

		case 438: { // postdecrement_expression ::= postfix_expression OP_DEC 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 156/*postdecrement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postdecrement_expression ::= postfix_expression OP_DEC ";
		return cup_result;

		case 439: { // unary_expression ::= preincrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 157/*unary_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression ::= preincrement_expression ";
		return cup_result;

		case 440: { // unary_expression ::= predecrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 157/*unary_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression ::= predecrement_expression ";
		return cup_result;

		case 441: { // unary_expression ::= OP_ADD unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 157/*unary_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression ::= OP_ADD unary_expression ";
		return cup_result;

		case 442: { // unary_expression ::= OP_SUB unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 157/*unary_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression ::= OP_SUB unary_expression ";
		return cup_result;

		case 443: { // unary_expression ::= unary_expression_not_plus_minus 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 157/*unary_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression ::= unary_expression_not_plus_minus ";
		return cup_result;

		case 444: { // preincrement_expression ::= OP_INC unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 159/*preincrement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "preincrement_expression ::= OP_INC unary_expression ";
		return cup_result;

		case 445: { // predecrement_expression ::= OP_DEC unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 160/*predecrement_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "predecrement_expression ::= OP_DEC unary_expression ";
		return cup_result;

		case 446: { // unary_expression_not_plus_minus ::= postfix_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 158/*unary_expression_not_plus_minus*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus ::= postfix_expression ";
		return cup_result;

		case 447: { // unary_expression_not_plus_minus ::= OP_NEG unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 158/*unary_expression_not_plus_minus*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus ::= OP_NEG unary_expression ";
		return cup_result;

		case 448: { // unary_expression_not_plus_minus ::= OP_NOT unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 158/*unary_expression_not_plus_minus*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus ::= OP_NOT unary_expression ";
		return cup_result;

		case 449: { // unary_expression_not_plus_minus ::= cast_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 158/*unary_expression_not_plus_minus*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus ::= cast_expression ";
		return cup_result;

		case 450: { // cast_expression ::= LPAREN primitive_type dims_opt RPAREN unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 161/*cast_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "cast_expression ::= LPAREN primitive_type dims_opt RPAREN unary_expression ";
		return cup_result;

		case 451: { // cast_expression ::= LPAREN name RPAREN unary_expression_not_plus_minus 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 161/*cast_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "cast_expression ::= LPAREN name RPAREN unary_expression_not_plus_minus ";
		return cup_result;

		case 452: { // cast_expression ::= LPAREN name dims RPAREN unary_expression_not_plus_minus 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 161/*cast_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "cast_expression ::= LPAREN name dims RPAREN unary_expression_not_plus_minus ";
		return cup_result;

		case 453: { // cast_expression ::= LPAREN name OP_LT type_argument_list_1 dims_opt RPAREN unary_expression_not_plus_minus 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 161/*cast_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 6)->left, context_class->getFromTop(parser_context, 6)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "cast_expression ::= LPAREN name OP_LT type_argument_list_1 dims_opt RPAREN unary_expression_not_plus_minus ";
		return cup_result;

		case 454: { // cast_expression ::= LPAREN name OP_LT type_argument_list_1 DOT class_or_interface_type dims_opt RPAREN unary_expression_not_plus_minus 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 161/*cast_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 8)->left, context_class->getFromTop(parser_context, 8)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "cast_expression ::= LPAREN name OP_LT type_argument_list_1 DOT class_or_interface_type dims_opt RPAREN unary_expression_not_plus_minus ";
		return cup_result;

		case 455: { // multiplicative_expression ::= unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 162/*multiplicative_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression ::= unary_expression ";
		return cup_result;

		case 456: { // multiplicative_expression ::= multiplicative_expression OP_MUL unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 162/*multiplicative_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression ::= multiplicative_expression OP_MUL unary_expression ";
		return cup_result;

		case 457: { // multiplicative_expression ::= multiplicative_expression OP_DIV unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 162/*multiplicative_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression ::= multiplicative_expression OP_DIV unary_expression ";
		return cup_result;

		case 458: { // multiplicative_expression ::= multiplicative_expression OP_MOD unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 162/*multiplicative_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression ::= multiplicative_expression OP_MOD unary_expression ";
		return cup_result;

		case 459: { // additive_expression ::= multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 163/*additive_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression ::= multiplicative_expression ";
		return cup_result;

		case 460: { // additive_expression ::= additive_expression OP_ADD multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 163/*additive_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression ::= additive_expression OP_ADD multiplicative_expression ";
		return cup_result;

		case 461: { // additive_expression ::= additive_expression OP_SUB multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 163/*additive_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression ::= additive_expression OP_SUB multiplicative_expression ";
		return cup_result;

		case 462: { // shift_expression ::= additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 164/*shift_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression ::= additive_expression ";
		return cup_result;

		case 463: { // shift_expression ::= shift_expression OP_SHL additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 164/*shift_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression ::= shift_expression OP_SHL additive_expression ";
		return cup_result;

		case 464: { // shift_expression ::= shift_expression OP_SHR additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 164/*shift_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression ::= shift_expression OP_SHR additive_expression ";
		return cup_result;

		case 465: { // shift_expression ::= shift_expression OP_SHRR additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 164/*shift_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression ::= shift_expression OP_SHRR additive_expression ";
		return cup_result;

		case 466: { // relational_expression ::= shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 165/*relational_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression ::= shift_expression ";
		return cup_result;

		case 467: { // relational_expression ::= relational_expression OP_LT shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 165/*relational_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression ::= relational_expression OP_LT shift_expression ";
		return cup_result;

		case 468: { // relational_expression ::= relational_expression OP_GT shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 165/*relational_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression ::= relational_expression OP_GT shift_expression ";
		return cup_result;

		case 469: { // relational_expression ::= relational_expression OP_LE shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 165/*relational_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression ::= relational_expression OP_LE shift_expression ";
		return cup_result;

		case 470: { // relational_expression ::= relational_expression OP_GE shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 165/*relational_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression ::= relational_expression OP_GE shift_expression ";
		return cup_result;

		case 471: { // instanceof_expression ::= relational_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 212/*instanceof_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "instanceof_expression ::= relational_expression ";
		return cup_result;

		case 472: { // instanceof_expression ::= instanceof_expression INSTANCEOF reference_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 212/*instanceof_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "instanceof_expression ::= instanceof_expression INSTANCEOF reference_type ";
		return cup_result;

		case 473: { // equality_expression ::= instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 166/*equality_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression ::= instanceof_expression ";
		return cup_result;

		case 474: { // equality_expression ::= equality_expression OP_EQ instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 166/*equality_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression ::= equality_expression OP_EQ instanceof_expression ";
		return cup_result;

		case 475: { // equality_expression ::= equality_expression OP_NE instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 166/*equality_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression ::= equality_expression OP_NE instanceof_expression ";
		return cup_result;

		case 476: { // and_expression ::= equality_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 167/*and_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "and_expression ::= equality_expression ";
		return cup_result;

		case 477: { // and_expression ::= and_expression OP_AND equality_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 167/*and_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "and_expression ::= and_expression OP_AND equality_expression ";
		return cup_result;

		case 478: { // exclusive_or_expression ::= and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 168/*exclusive_or_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "exclusive_or_expression ::= and_expression ";
		return cup_result;

		case 479: { // exclusive_or_expression ::= exclusive_or_expression OP_XOR and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 168/*exclusive_or_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "exclusive_or_expression ::= exclusive_or_expression OP_XOR and_expression ";
		return cup_result;

		case 480: { // inclusive_or_expression ::= exclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 169/*inclusive_or_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "inclusive_or_expression ::= exclusive_or_expression ";
		return cup_result;

		case 481: { // inclusive_or_expression ::= inclusive_or_expression OP_OR exclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 169/*inclusive_or_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "inclusive_or_expression ::= inclusive_or_expression OP_OR exclusive_or_expression ";
		return cup_result;

		case 482: { // conditional_and_expression ::= inclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 170/*conditional_and_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_and_expression ::= inclusive_or_expression ";
		return cup_result;

		case 483: { // conditional_and_expression ::= conditional_and_expression OP_LAND inclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 170/*conditional_and_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_and_expression ::= conditional_and_expression OP_LAND inclusive_or_expression ";
		return cup_result;

		case 484: { // conditional_or_expression ::= conditional_and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 171/*conditional_or_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_or_expression ::= conditional_and_expression ";
		return cup_result;

		case 485: { // conditional_or_expression ::= conditional_or_expression OP_LOR conditional_and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 171/*conditional_or_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_or_expression ::= conditional_or_expression OP_LOR conditional_and_expression ";
		return cup_result;

		case 486: { // conditional_expression ::= conditional_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 172/*conditional_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_expression ::= conditional_or_expression ";
		return cup_result;

		case 487: { // conditional_expression ::= conditional_or_expression QUESTION expression COLON conditional_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 172/*conditional_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_expression ::= conditional_or_expression QUESTION expression COLON conditional_expression ";
		return cup_result;

		case 488: { // assignment_expression ::= conditional_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 173/*assignment_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_expression ::= conditional_expression ";
		return cup_result;

		case 489: { // assignment_expression ::= assignment 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 173/*assignment_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_expression ::= assignment ";
		return cup_result;

		case 490: { // assignment ::= postfix_expression assignment_operator assignment_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 174/*assignment*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment ::= postfix_expression assignment_operator assignment_expression ";
		return cup_result;

		case 491: { // assignment_operator ::= ASS_IS 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_IS ";
		return cup_result;

		case 492: { // assignment_operator ::= ASS_MUL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_MUL ";
		return cup_result;

		case 493: { // assignment_operator ::= ASS_DIV 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_DIV ";
		return cup_result;

		case 494: { // assignment_operator ::= ASS_MOD 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_MOD ";
		return cup_result;

		case 495: { // assignment_operator ::= ASS_ADD 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_ADD ";
		return cup_result;

		case 496: { // assignment_operator ::= ASS_SUB 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_SUB ";
		return cup_result;

		case 497: { // assignment_operator ::= ASS_SHL 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_SHL ";
		return cup_result;

		case 498: { // assignment_operator ::= ASS_SHR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_SHR ";
		return cup_result;

		case 499: { // assignment_operator ::= ASS_SHRR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_SHRR ";
		return cup_result;

		case 500: { // assignment_operator ::= ASS_AND 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_AND ";
		return cup_result;

		case 501: { // assignment_operator ::= ASS_XOR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_XOR ";
		return cup_result;

		case 502: { // assignment_operator ::= ASS_OR 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 175/*assignment_operator*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_operator ::= ASS_OR ";
		return cup_result;

		case 503: { // expression_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 176/*expression_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "expression_opt ::= ";
		return cup_result;

		case 504: { // expression_opt ::= expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 176/*expression_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "expression_opt ::= expression ";
		return cup_result;

		case 505: { // expression ::= assignment_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 177/*expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "expression ::= assignment_expression ";
		return cup_result;

		case 506: { // constant_expression ::= expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 178/*constant_expression*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "constant_expression ::= expression ";
		return cup_result;

		case 507: { // type_parameters_opt ::= type_parameters 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 191/*type_parameters_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameters_opt ::= type_parameters ";
		return cup_result;

		case 508: { // type_parameters_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 191/*type_parameters_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameters_opt ::= ";
		return cup_result;

		case 509: { // type_parameters ::= OP_LT type_parameter_list_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 190/*type_parameters*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameters ::= OP_LT type_parameter_list_1 ";
		return cup_result;

		case 510: { // type_parameter_list ::= type_parameter_list COMMA type_parameter 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 193/*type_parameter_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter_list ::= type_parameter_list COMMA type_parameter ";
		return cup_result;

		case 511: { // type_parameter_list ::= type_parameter 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 193/*type_parameter_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter_list ::= type_parameter ";
		return cup_result;

		case 512: { // type_parameter_list_1 ::= type_parameter_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 195/*type_parameter_list_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter_list_1 ::= type_parameter_1 ";
		return cup_result;

		case 513: { // type_parameter_list_1 ::= type_parameter_list COMMA type_parameter_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 195/*type_parameter_list_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter_list_1 ::= type_parameter_list COMMA type_parameter_1 ";
		return cup_result;

		case 514: { // type_parameter ::= type_variable type_bound_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 192/*type_parameter*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter ::= type_variable type_bound_opt ";
		return cup_result;

		case 515: { // type_parameter_1 ::= type_variable OP_GT 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 194/*type_parameter_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter_1 ::= type_variable OP_GT ";
		return cup_result;

		case 516: { // type_parameter_1 ::= type_variable type_bound_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 194/*type_parameter_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_parameter_1 ::= type_variable type_bound_1 ";
		return cup_result;

		case 517: { // type_bound_opt ::= type_bound 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 197/*type_bound_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_bound_opt ::= type_bound ";
		return cup_result;

		case 518: { // type_bound_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 197/*type_bound_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_bound_opt ::= ";
		return cup_result;

		case 519: { // type_bound ::= EXTENDS reference_type additional_bound_list_opt 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 196/*type_bound*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_bound ::= EXTENDS reference_type additional_bound_list_opt ";
		return cup_result;

		case 520: { // type_bound_1 ::= EXTENDS reference_type_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 198/*type_bound_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_bound_1 ::= EXTENDS reference_type_1 ";
		return cup_result;

		case 521: { // type_bound_1 ::= EXTENDS reference_type additional_bound_list_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 198/*type_bound_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "type_bound_1 ::= EXTENDS reference_type additional_bound_list_1 ";
		return cup_result;

		case 522: { // additional_bound_list_opt ::= additional_bound_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 200/*additional_bound_list_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_list_opt ::= additional_bound_list ";
		return cup_result;

		case 523: { // additional_bound_list_opt ::= 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 200/*additional_bound_list_opt*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_list_opt ::= ";
		return cup_result;

		case 524: { // additional_bound_list ::= additional_bound additional_bound_list 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 199/*additional_bound_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_list ::= additional_bound additional_bound_list ";
		return cup_result;

		case 525: { // additional_bound_list ::= additional_bound 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 199/*additional_bound_list*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_list ::= additional_bound ";
		return cup_result;

		case 526: { // additional_bound_list_1 ::= additional_bound additional_bound_list_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 201/*additional_bound_list_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_list_1 ::= additional_bound additional_bound_list_1 ";
		return cup_result;

		case 527: { // additional_bound_list_1 ::= additional_bound_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 201/*additional_bound_list_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_list_1 ::= additional_bound_1 ";
		return cup_result;

		case 528: { // additional_bound ::= OP_AND interface_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 202/*additional_bound*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound ::= OP_AND interface_type ";
		return cup_result;

		case 529: { // additional_bound_1 ::= OP_AND reference_type_1 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 203/*additional_bound_1*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additional_bound_1 ::= OP_AND reference_type_1 ";
		return cup_result;

		case 530: { // postfix_expression_nn ::= primary 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 213/*postfix_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression_nn ::= primary ";
		return cup_result;

		case 531: { // postfix_expression_nn ::= postincrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 213/*postfix_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression_nn ::= postincrement_expression ";
		return cup_result;

		case 532: { // postfix_expression_nn ::= postdecrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 213/*postfix_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "postfix_expression_nn ::= postdecrement_expression ";
		return cup_result;

		case 533: { // unary_expression_nn ::= preincrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 214/*unary_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_nn ::= preincrement_expression ";
		return cup_result;

		case 534: { // unary_expression_nn ::= predecrement_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 214/*unary_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_nn ::= predecrement_expression ";
		return cup_result;

		case 535: { // unary_expression_nn ::= OP_ADD unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 214/*unary_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_nn ::= OP_ADD unary_expression ";
		return cup_result;

		case 536: { // unary_expression_nn ::= OP_SUB unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 214/*unary_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_nn ::= OP_SUB unary_expression ";
		return cup_result;

		case 537: { // unary_expression_nn ::= unary_expression_not_plus_minus_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 214/*unary_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_nn ::= unary_expression_not_plus_minus_nn ";
		return cup_result;

		case 538: { // unary_expression_not_plus_minus_nn ::= postfix_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 215/*unary_expression_not_plus_minus_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus_nn ::= postfix_expression_nn ";
		return cup_result;

		case 539: { // unary_expression_not_plus_minus_nn ::= OP_NEG unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 215/*unary_expression_not_plus_minus_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus_nn ::= OP_NEG unary_expression ";
		return cup_result;

		case 540: { // unary_expression_not_plus_minus_nn ::= OP_NOT unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 215/*unary_expression_not_plus_minus_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus_nn ::= OP_NOT unary_expression ";
		return cup_result;

		case 541: { // unary_expression_not_plus_minus_nn ::= cast_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 215/*unary_expression_not_plus_minus_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "unary_expression_not_plus_minus_nn ::= cast_expression ";
		return cup_result;

		case 542: { // multiplicative_expression_nn ::= unary_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= unary_expression_nn ";
		return cup_result;

		case 543: { // multiplicative_expression_nn ::= name OP_MUL unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= name OP_MUL unary_expression ";
		return cup_result;

		case 544: { // multiplicative_expression_nn ::= multiplicative_expression_nn OP_MUL unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= multiplicative_expression_nn OP_MUL unary_expression ";
		return cup_result;

		case 545: { // multiplicative_expression_nn ::= name OP_DIV unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= name OP_DIV unary_expression ";
		return cup_result;

		case 546: { // multiplicative_expression_nn ::= multiplicative_expression_nn OP_DIV unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= multiplicative_expression_nn OP_DIV unary_expression ";
		return cup_result;

		case 547: { // multiplicative_expression_nn ::= name OP_MOD unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= name OP_MOD unary_expression ";
		return cup_result;

		case 548: { // multiplicative_expression_nn ::= multiplicative_expression_nn OP_MOD unary_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 216/*multiplicative_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "multiplicative_expression_nn ::= multiplicative_expression_nn OP_MOD unary_expression ";
		return cup_result;

		case 549: { // additive_expression_nn ::= multiplicative_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 217/*additive_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression_nn ::= multiplicative_expression_nn ";
		return cup_result;

		case 550: { // additive_expression_nn ::= name OP_ADD multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 217/*additive_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression_nn ::= name OP_ADD multiplicative_expression ";
		return cup_result;

		case 551: { // additive_expression_nn ::= additive_expression_nn OP_ADD multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 217/*additive_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression_nn ::= additive_expression_nn OP_ADD multiplicative_expression ";
		return cup_result;

		case 552: { // additive_expression_nn ::= name OP_SUB multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 217/*additive_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression_nn ::= name OP_SUB multiplicative_expression ";
		return cup_result;

		case 553: { // additive_expression_nn ::= additive_expression_nn OP_SUB multiplicative_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 217/*additive_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "additive_expression_nn ::= additive_expression_nn OP_SUB multiplicative_expression ";
		return cup_result;

		case 554: { // shift_expression_nn ::= additive_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= additive_expression_nn ";
		return cup_result;

		case 555: { // shift_expression_nn ::= name OP_SHL additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= name OP_SHL additive_expression ";
		return cup_result;

		case 556: { // shift_expression_nn ::= shift_expression_nn OP_SHL additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= shift_expression_nn OP_SHL additive_expression ";
		return cup_result;

		case 557: { // shift_expression_nn ::= name OP_SHR additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= name OP_SHR additive_expression ";
		return cup_result;

		case 558: { // shift_expression_nn ::= shift_expression_nn OP_SHR additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= shift_expression_nn OP_SHR additive_expression ";
		return cup_result;

		case 559: { // shift_expression_nn ::= name OP_SHRR additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= name OP_SHRR additive_expression ";
		return cup_result;

		case 560: { // shift_expression_nn ::= shift_expression_nn OP_SHRR additive_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 218/*shift_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "shift_expression_nn ::= shift_expression_nn OP_SHRR additive_expression ";
		return cup_result;

		case 561: { // relational_expression_nn ::= shift_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= shift_expression_nn ";
		return cup_result;

		case 562: { // relational_expression_nn ::= name OP_LT shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= name OP_LT shift_expression ";
		return cup_result;

		case 563: { // relational_expression_nn ::= shift_expression_nn OP_LT shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= shift_expression_nn OP_LT shift_expression ";
		return cup_result;

		case 564: { // relational_expression_nn ::= name OP_GT shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= name OP_GT shift_expression ";
		return cup_result;

		case 565: { // relational_expression_nn ::= shift_expression_nn OP_GT shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= shift_expression_nn OP_GT shift_expression ";
		return cup_result;

		case 566: { // relational_expression_nn ::= name OP_LE shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= name OP_LE shift_expression ";
		return cup_result;

		case 567: { // relational_expression_nn ::= relational_expression_nn OP_LE shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= relational_expression_nn OP_LE shift_expression ";
		return cup_result;

		case 568: { // relational_expression_nn ::= name OP_GE shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= name OP_GE shift_expression ";
		return cup_result;

		case 569: { // relational_expression_nn ::= relational_expression_nn OP_GE shift_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 219/*relational_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "relational_expression_nn ::= relational_expression_nn OP_GE shift_expression ";
		return cup_result;

		case 570: { // instanceof_expression_nn ::= relational_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 220/*instanceof_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "instanceof_expression_nn ::= relational_expression_nn ";
		return cup_result;

		case 571: { // instanceof_expression_nn ::= name INSTANCEOF reference_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 220/*instanceof_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "instanceof_expression_nn ::= name INSTANCEOF reference_type ";
		return cup_result;

		case 572: { // instanceof_expression_nn ::= instanceof_expression_nn INSTANCEOF reference_type 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 220/*instanceof_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "instanceof_expression_nn ::= instanceof_expression_nn INSTANCEOF reference_type ";
		return cup_result;

		case 573: { // equality_expression_nn ::= instanceof_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 221/*equality_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression_nn ::= instanceof_expression_nn ";
		return cup_result;

		case 574: { // equality_expression_nn ::= name OP_EQ instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 221/*equality_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression_nn ::= name OP_EQ instanceof_expression ";
		return cup_result;

		case 575: { // equality_expression_nn ::= equality_expression_nn OP_EQ instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 221/*equality_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression_nn ::= equality_expression_nn OP_EQ instanceof_expression ";
		return cup_result;

		case 576: { // equality_expression_nn ::= name OP_NE instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 221/*equality_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression_nn ::= name OP_NE instanceof_expression ";
		return cup_result;

		case 577: { // equality_expression_nn ::= equality_expression_nn OP_NE instanceof_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 221/*equality_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "equality_expression_nn ::= equality_expression_nn OP_NE instanceof_expression ";
		return cup_result;

		case 578: { // and_expression_nn ::= equality_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 222/*and_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "and_expression_nn ::= equality_expression_nn ";
		return cup_result;

		case 579: { // and_expression_nn ::= name OP_AND equality_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 222/*and_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "and_expression_nn ::= name OP_AND equality_expression ";
		return cup_result;

		case 580: { // and_expression_nn ::= and_expression_nn OP_AND equality_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 222/*and_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "and_expression_nn ::= and_expression_nn OP_AND equality_expression ";
		return cup_result;

		case 581: { // exclusive_or_expression_nn ::= and_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 223/*exclusive_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "exclusive_or_expression_nn ::= and_expression_nn ";
		return cup_result;

		case 582: { // exclusive_or_expression_nn ::= name OP_XOR and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 223/*exclusive_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "exclusive_or_expression_nn ::= name OP_XOR and_expression ";
		return cup_result;

		case 583: { // exclusive_or_expression_nn ::= exclusive_or_expression_nn OP_XOR and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 223/*exclusive_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "exclusive_or_expression_nn ::= exclusive_or_expression_nn OP_XOR and_expression ";
		return cup_result;

		case 584: { // inclusive_or_expression_nn ::= exclusive_or_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 224/*inclusive_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "inclusive_or_expression_nn ::= exclusive_or_expression_nn ";
		return cup_result;

		case 585: { // inclusive_or_expression_nn ::= name OP_OR exclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 224/*inclusive_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "inclusive_or_expression_nn ::= name OP_OR exclusive_or_expression ";
		return cup_result;

		case 586: { // inclusive_or_expression_nn ::= inclusive_or_expression_nn OP_OR exclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 224/*inclusive_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "inclusive_or_expression_nn ::= inclusive_or_expression_nn OP_OR exclusive_or_expression ";
		return cup_result;

		case 587: { // conditional_and_expression_nn ::= inclusive_or_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 225/*conditional_and_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_and_expression_nn ::= inclusive_or_expression_nn ";
		return cup_result;

		case 588: { // conditional_and_expression_nn ::= name OP_LAND inclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 225/*conditional_and_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_and_expression_nn ::= name OP_LAND inclusive_or_expression ";
		return cup_result;

		case 589: { // conditional_and_expression_nn ::= conditional_and_expression_nn OP_LAND inclusive_or_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 225/*conditional_and_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_and_expression_nn ::= conditional_and_expression_nn OP_LAND inclusive_or_expression ";
		return cup_result;

		case 590: { // conditional_or_expression_nn ::= conditional_and_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 226/*conditional_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_or_expression_nn ::= conditional_and_expression_nn ";
		return cup_result;

		case 591: { // conditional_or_expression_nn ::= name OP_LOR conditional_and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 226/*conditional_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_or_expression_nn ::= name OP_LOR conditional_and_expression ";
		return cup_result;

		case 592: { // conditional_or_expression_nn ::= conditional_or_expression_nn OP_LOR conditional_and_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 226/*conditional_or_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_or_expression_nn ::= conditional_or_expression_nn OP_LOR conditional_and_expression ";
		return cup_result;

		case 593: { // conditional_expression_nn ::= conditional_or_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 227/*conditional_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_expression_nn ::= conditional_or_expression_nn ";
		return cup_result;

		case 594: { // conditional_expression_nn ::= name QUESTION expression COLON conditional_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 227/*conditional_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_expression_nn ::= name QUESTION expression COLON conditional_expression ";
		return cup_result;

		case 595: { // conditional_expression_nn ::= conditional_or_expression_nn QUESTION expression COLON conditional_expression 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 227/*conditional_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "conditional_expression_nn ::= conditional_or_expression_nn QUESTION expression COLON conditional_expression ";
		return cup_result;

		case 596: { // assignment_expression_nn ::= conditional_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 228/*assignment_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_expression_nn ::= conditional_expression_nn ";
		return cup_result;

		case 597: { // assignment_expression_nn ::= assignment 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 228/*assignment_expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "assignment_expression_nn ::= assignment ";
		return cup_result;

		case 598: { // expression_nn ::= assignment_expression_nn 
			GObject *RESULT = NULL;
			cup_result  = jag_iscanner_create_token(parser_context->scanner, 229/*expression_nn*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "expression_nn ::= assignment_expression_nn ";
		return cup_result;

	}
	return cup_result;
}

