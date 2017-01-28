#include "shoparseractions.h"



#include "../../model/shomodel.h"
#include "../../model/shoreference.h"
#include "../../model/shoivalue.h"
#include "../../model/shoblockvalue.h"
#include "../../model/sholistvalue.h"
#include "../../model/shosimplevalue.h"
#include "../../model/shotemplate.h"
#include "../../model/shotemplateargument.h"
#include "../../model/shotemplatecall.h"



#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ERROR
#define CAT_LOG_CLAZZ "ShoParserActions"
#include <logging/catlog.h>

G_DEFINE_TYPE(ShoParserActions, sho_parser_actions, G_TYPE_OBJECT)


static void sho_parser_actions_class_init(ShoParserActionsClass *clazz) {
	clazz->run_action = sho_parser_actions_run_action;
}

static void sho_parser_actions_init(ShoParserActions *parser) {
}

ShoParserActions *sho_parser_actions_new() {
	ShoParserActions *result = g_object_new(SHO_TYPE_PARSER_ACTIONS, NULL);
	cat_ref_anounce(result);
	return result;
}

ShoToken *sho_parser_actions_run_action(ShoParserActions *parser_actions, ShoParserContext *parser_context, int cup_action_id) {	ShoParserContextClass *context_class = SHO_PARSER_CONTEXT_GET_CLASS(parser_context);
	/* Symbol object for return from actions */
	ShoToken *cup_result = NULL;

	/* select the action based on the action number */
	switch(cup_action_id) {
		case 0: { // $START ::= sht_file EOF 
			ShoModel *RESULT = NULL;
				ShoToken *cup_start_val = context_class->getFromTop(parser_context, 1);
				ShoModel *start_val = (ShoModel *) (cup_start_val->value);
RESULT = start_val;
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 0/*$START*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		/* ACCEPT */
		sho_parser_context_done_parsing(parser_context);
		cup_result->symbol_text = "$START ::= sht_file EOF ";
		return cup_result;

		case 1: { // sht_file ::= sht_block 
			ShoModel *RESULT = NULL;
				ShoToken *cup_b = context_class->getFromTop(parser_context, 0);
				ShoModel *b = (ShoModel *) (cup_b->value);
 RESULT=b; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 8/*sht_file*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "sht_file ::= sht_block ";
		return cup_result;

		case 2: { // sht_block ::= template_def 
			ShoModel *RESULT = NULL;
				ShoToken *cup_d = context_class->getFromTop(parser_context, 0);
				ShoTemplate *d = (ShoTemplate *) (cup_d->value);
 RESULT = sho_model_new(); sho_model_add_template(RESULT, d);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 7/*sht_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "sht_block ::= template_def ";
		return cup_result;

		case 3: { // sht_block ::= reference 
			ShoModel *RESULT = NULL;
				ShoToken *cup_r = context_class->getFromTop(parser_context, 0);
				ShoReference *r = (ShoReference *) (cup_r->value);
 RESULT = sho_model_new(); sho_reference_list_add_reference((ShoReferenceList *) RESULT, r);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 7/*sht_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "sht_block ::= reference ";
		return cup_result;

		case 4: { // sht_block ::= sht_block template_def 
			ShoModel *RESULT = NULL;
				ShoToken *cup_b = context_class->getFromTop(parser_context, 1);
				ShoModel *b = (ShoModel *) (cup_b->value);
				ShoToken *cup_d = context_class->getFromTop(parser_context, 0);
				ShoTemplate *d = (ShoTemplate *) (cup_d->value);
 RESULT = b; sho_model_add_template(RESULT, d); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 7/*sht_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "sht_block ::= sht_block template_def ";
		return cup_result;

		case 5: { // sht_block ::= sht_block reference 
			ShoModel *RESULT = NULL;
				ShoToken *cup_b = context_class->getFromTop(parser_context, 1);
				ShoModel *b = (ShoModel *) (cup_b->value);
				ShoToken *cup_r = context_class->getFromTop(parser_context, 0);
				ShoReference *r = (ShoReference *) (cup_r->value);
 RESULT = b; sho_reference_list_add_reference((ShoReferenceList *) RESULT, r); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 7/*sht_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "sht_block ::= sht_block reference ";
		return cup_result;

		case 6: { // template_def ::= ID LPARREN template_arguments RPARREN SEMICOLON 
			ShoTemplate *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 4);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_args = context_class->getFromTop(parser_context, 2);
				CatArrayWo *args = (CatArrayWo *) (cup_args->value);
 
			  RESULT = sho_template_new(i);
			  int idx;
			  for(idx=0; idx<cat_array_wo_size(args); idx++) { 
			  	ShoTemplateArgument *arg = (ShoTemplateArgument *) cat_array_wo_get(args, idx);
			  	sho_template_add_argument(RESULT, arg);
			  }
			  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 9/*template_def*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 4)->left, context_class->getFromTop(parser_context, 4)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
			  
		}
		cup_result->symbol_text = "template_def ::= ID LPARREN template_arguments RPARREN SEMICOLON ";
		return cup_result;

		case 7: { // template_def ::= ID LPARREN RPARREN SEMICOLON 
			ShoTemplate *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 3);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
 RESULT = sho_template_new(i);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 9/*template_def*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "template_def ::= ID LPARREN RPARREN SEMICOLON ";
		return cup_result;

		case 8: { // template_arguments ::= template_arg 
			CatArrayWo *RESULT = NULL;
				ShoToken *cup_arg = context_class->getFromTop(parser_context, 0);
				ShoTemplateArgument *arg = (ShoTemplateArgument *) (cup_arg->value);
 RESULT = cat_array_wo_new(); cat_array_wo_append(RESULT, (GObject *) arg);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 11/*template_arguments*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "template_arguments ::= template_arg ";
		return cup_result;

		case 9: { // template_arguments ::= template_arguments COMMA template_arg 
			CatArrayWo *RESULT = NULL;
				ShoToken *cup_args = context_class->getFromTop(parser_context, 2);
				CatArrayWo *args = (CatArrayWo *) (cup_args->value);
				ShoToken *cup_arg = context_class->getFromTop(parser_context, 0);
				ShoTemplateArgument *arg = (ShoTemplateArgument *) (cup_arg->value);
 RESULT = args; cat_array_wo_append(RESULT, (GObject *) arg); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 11/*template_arguments*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "template_arguments ::= template_arguments COMMA template_arg ";
		return cup_result;

		case 10: { // template_arg ::= ID 
			ShoTemplateArgument *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 0);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
 RESULT = sho_template_argument_new(i); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 10/*template_arg*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "template_arg ::= ID ";
		return cup_result;

		case 11: { // template_arg ::= ID value 
			ShoTemplateArgument *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 1);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_e = context_class->getFromTop(parser_context, 0);
				ShoIValue *e = (ShoIValue *) (cup_e->value);
 RESULT=sho_template_argument_new(i); sho_template_argument_set_default(RESULT, e);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 10/*template_arg*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "template_arg ::= ID value ";
		return cup_result;

		case 12: { // reference_block ::= reference_block reference 
			ShoBlockValue *RESULT = NULL;
				ShoToken *cup_rb = context_class->getFromTop(parser_context, 1);
				ShoBlockValue *rb = (ShoBlockValue *) (cup_rb->value);
				ShoToken *cup_ref = context_class->getFromTop(parser_context, 0);
				ShoReference *ref = (ShoReference *) (cup_ref->value);
 RESULT=rb; sho_reference_list_add_reference((ShoReferenceList *) RESULT, ref); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 5/*reference_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference_block ::= reference_block reference ";
		return cup_result;

		case 13: { // reference_block ::= reference 
			ShoBlockValue *RESULT = NULL;
				ShoToken *cup_ref = context_class->getFromTop(parser_context, 0);
				ShoReference *ref = (ShoReference *) (cup_ref->value);
 RESULT = sho_block_value_new(); sho_reference_list_add_reference((ShoReferenceList *) RESULT, ref);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 5/*reference_block*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "reference_block ::= reference ";
		return cup_result;

		case 14: { // reference ::= block_reference 
			ShoReference *RESULT = NULL;
				ShoToken *cup_r = context_class->getFromTop(parser_context, 0);
				ShoReference *r = (ShoReference *) (cup_r->value);
 RESULT=r; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 16/*reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference ::= block_reference ";
		return cup_result;

		case 15: { // reference ::= property_reference 
			ShoReference *RESULT = NULL;
				ShoToken *cup_r = context_class->getFromTop(parser_context, 0);
				ShoReference *r = (ShoReference *) (cup_r->value);
 RESULT=r; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 16/*reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference ::= property_reference ";
		return cup_result;

		case 17: { // simple_reference ::= ID simple_value SEMICOLON 
			ShoReference *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 2);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_v = context_class->getFromTop(parser_context, 1);
				ShoSimpleValue *v = (ShoSimpleValue *) (cup_v->value);
 RESULT = sho_reference_new(i); sho_reference_set_value(RESULT, (ShoIValue *) v); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 13/*simple_reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "simple_reference ::= ID simple_value SEMICOLON ";
		return cup_result;

		case 16: { // reference ::= simple_reference 
			ShoReference *RESULT = NULL;
				ShoToken *cup_r = context_class->getFromTop(parser_context, 0);
				ShoReference *r = (ShoReference *) (cup_r->value);
 RESULT=r; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 16/*reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "reference ::= simple_reference ";
		return cup_result;

		case 19: { // block_reference ::= ID list_value 
			ShoReference *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 1);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_s = context_class->getFromTop(parser_context, 0);
				ShoListValue *s = (ShoListValue *) (cup_s->value);
 RESULT = sho_reference_new(i); sho_reference_set_value(RESULT, (ShoIValue *) s); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 14/*block_reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "block_reference ::= ID list_value ";
		return cup_result;

		case 18: { // block_reference ::= ID block_value 
			ShoReference *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 1);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_s = context_class->getFromTop(parser_context, 0);
				ShoBlockValue *s = (ShoBlockValue *) (cup_s->value);
 RESULT = sho_reference_new(i); sho_reference_set_value(RESULT, (ShoIValue *) s); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 14/*block_reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "block_reference ::= ID block_value ";
		return cup_result;

		case 21: { // block_value ::= template_call block_body 
			ShoBlockValue *RESULT = NULL;
				ShoToken *cup_tc = context_class->getFromTop(parser_context, 1);
				ShoTemplateCall *tc = (ShoTemplateCall *) (cup_tc->value);
				ShoToken *cup_b = context_class->getFromTop(parser_context, 0);
				ShoBlockValue *b = (ShoBlockValue *) (cup_b->value);
 RESULT = b; sho_block_value_set_template_call(RESULT, tc); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 4/*block_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_value ::= template_call block_body ";
		return cup_result;

		case 20: { // property_reference ::= ID ASSIGN_VALUE_EOL 
			ShoReference *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 1);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_v = context_class->getFromTop(parser_context, 0);
				CatStringWo *v = (CatStringWo *) (cup_v->value);
 ShoSimpleValue *sv = sho_simple_value_new_string(v); RESULT=sho_reference_new(i); sho_reference_set_value(RESULT, (ShoIValue *) sv);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 15/*property_reference*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); cat_unref(sv); 
		}
		cup_result->symbol_text = "property_reference ::= ID ASSIGN_VALUE_EOL ";
		return cup_result;

		case 23: { // list_value ::= LBRACE values RBRACE 
			ShoListValue *RESULT = NULL;
				ShoToken *cup_el = context_class->getFromTop(parser_context, 1);
				ShoListValue *el = (ShoListValue *) (cup_el->value);
 RESULT=el; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 3/*list_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "list_value ::= LBRACE values RBRACE ";
		return cup_result;

		case 22: { // block_value ::= block_body 
			ShoBlockValue *RESULT = NULL;
				ShoToken *cup_b = context_class->getFromTop(parser_context, 0);
				ShoBlockValue *b = (ShoBlockValue *) (cup_b->value);
 RESULT = b; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 4/*block_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_value ::= block_body ";
		return cup_result;

		case 25: { // template_call ::= ID LPARREN values RPARREN 
			ShoTemplateCall *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 3);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
				ShoToken *cup_el = context_class->getFromTop(parser_context, 1);
				ShoListValue *el = (ShoListValue *) (cup_el->value);
 RESULT = sho_template_call_new(i); sho_template_call_set_argument_list(RESULT, el);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 12/*template_call*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 3)->left, context_class->getFromTop(parser_context, 3)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "template_call ::= ID LPARREN values RPARREN ";
		return cup_result;

		case 24: { // template_call ::= ID LPARREN RPARREN 
			ShoTemplateCall *RESULT = NULL;
				ShoToken *cup_i = context_class->getFromTop(parser_context, 2);
				CatStringWo *i = (CatStringWo *) (cup_i->value);
 RESULT = sho_template_call_new(i);  
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 12/*template_call*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "template_call ::= ID LPARREN RPARREN ";
		return cup_result;

		case 27: { // block_body ::= LBRACE reference_block RBRACE 
			ShoBlockValue *RESULT = NULL;
				ShoToken *cup_nb = context_class->getFromTop(parser_context, 1);
				ShoBlockValue *nb = (ShoBlockValue *) (cup_nb->value);
 RESULT = nb; 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 6/*block_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "block_body ::= LBRACE reference_block RBRACE ";
		return cup_result;

		case 26: { // block_body ::= LBRACE RBRACE 
			ShoBlockValue *RESULT = NULL;
 RESULT = sho_block_value_new(); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 6/*block_body*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 1)->left, context_class->getFromTop(parser_context, 1)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "block_body ::= LBRACE RBRACE ";
		return cup_result;

		case 29: { // values ::= value 
			ShoListValue *RESULT = NULL;
				ShoToken *cup_e = context_class->getFromTop(parser_context, 0);
				ShoIValue *e = (ShoIValue *) (cup_e->value);
 RESULT = sho_list_value_new(); sho_list_value_add_value(RESULT, e); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 2/*values*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
 cat_unref(RESULT); 
		}
		cup_result->symbol_text = "values ::= value ";
		return cup_result;

		case 28: { // values ::= values COMMA value 
			ShoListValue *RESULT = NULL;
				ShoToken *cup_el = context_class->getFromTop(parser_context, 2);
				ShoListValue *el = (ShoListValue *) (cup_el->value);
				ShoToken *cup_e = context_class->getFromTop(parser_context, 0);
				ShoIValue *e = (ShoIValue *) (cup_e->value);
 RESULT=el; sho_list_value_add_value(RESULT, e); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 2/*values*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 2)->left, context_class->getFromTop(parser_context, 2)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "values ::= values COMMA value ";
		return cup_result;

		case 31: { // value ::= list_value 
			ShoIValue *RESULT = NULL;
				ShoToken *cup_s = context_class->getFromTop(parser_context, 0);
				ShoListValue *s = (ShoListValue *) (cup_s->value);
 RESULT=SHO_IVALUE(s); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 1/*value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "value ::= list_value ";
		return cup_result;

		case 30: { // value ::= block_value 
			ShoIValue *RESULT = NULL;
				ShoToken *cup_s = context_class->getFromTop(parser_context, 0);
				ShoBlockValue *s = (ShoBlockValue *) (cup_s->value);
 RESULT=SHO_IVALUE(s); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 1/*value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "value ::= block_value ";
		return cup_result;

		case 34: { // simple_value ::= NUMBER 
			ShoSimpleValue *RESULT = NULL;
				ShoToken *cup_s = context_class->getFromTop(parser_context, 0);
				CatStringWo *s = (CatStringWo *) (cup_s->value);

				RESULT = sho_simple_value_new_string(s);
				
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 17/*simple_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));

				cat_unref(RESULT);
			
		}
		cup_result->symbol_text = "simple_value ::= NUMBER ";
		return cup_result;

		case 35: { // simple_value ::= NIL 
			ShoSimpleValue *RESULT = NULL;

				RESULT = sho_simple_value_new_string(NULL);
				
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 17/*simple_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));

				cat_unref(RESULT);
			
		}
		cup_result->symbol_text = "simple_value ::= NIL ";
		return cup_result;

		case 32: { // value ::= simple_value 
			ShoIValue *RESULT = NULL;
				ShoToken *cup_v = context_class->getFromTop(parser_context, 0);
				ShoSimpleValue *v = (ShoSimpleValue *) (cup_v->value);
 RESULT=SHO_IVALUE(v); 
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 1/*value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));
		}
		cup_result->symbol_text = "value ::= simple_value ";
		return cup_result;

		case 33: { // simple_value ::= STRING 
			ShoSimpleValue *RESULT = NULL;
				ShoToken *cup_s = context_class->getFromTop(parser_context, 0);
				CatStringWo *s = (CatStringWo *) (cup_s->value);

			  	RESULT = sho_simple_value_new_string(s);
			  	
			cup_result  = sho_iscanner_create_token(parser_context->scanner, 17/*simple_value*/, -1, FALSE, FALSE, FALSE, context_class->getFromTop(parser_context, 0)->left, context_class->getFromTop(parser_context, 0)->left_row, context_class->getFromTop(parser_context, 0)->right, context_class->getFromTop(parser_context, 0)->right_row, G_OBJECT(RESULT));

			  	cat_unref(RESULT);
			  
		}
		cup_result->symbol_text = "simple_value ::= STRING ";
		return cup_result;

	}
	return cup_result;
}

