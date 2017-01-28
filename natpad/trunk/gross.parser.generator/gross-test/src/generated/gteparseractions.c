#include "gteparseractions.h"

GroToken *gte_parser_actions_run_action(GroParserContext *parser_context, int cup_action_id) {
	GroParserContextClass *context_class = GRO_PARSER_CONTEXT_GET_CLASS(parser_context);
	/* Symbol object for return from actions */
	GroToken *cup_result = NULL;

	/* select the action based on the action number */
	switch(cup_action_id) {
		case 0 : { // $START ::= spec EOF 
			void *RESULT = NULL;
			GroToken *cup_start_val = context_class->getFromTop(parser_context, 1);
			void *start_val = (void *) (cup_start_val->value);
			cup_result = gro_token_new_minimal(0 /*$START*/, RESULT);
			/* ACCEPT */
			parser_context->done_parsing = TRUE;
			return cup_result;
		}
		case 1 : { // spec ::= package_spec import_list code_parts symbol_list precedence_list start_spec production_list 
			void *RESULT = NULL;
			GroToken *cup_sl = context_class->getFromTop(parser_context, 3);
			void *sl = (void *) (cup_sl->value);
			GroToken *cup_pl = context_class->getFromTop(parser_context, 0);
			void *pl = (void *) (cup_pl->value);
			cup_result = gro_token_new_minimal(26 /*spec*/, RESULT);
			return cup_result;
		}
		case 2 : { // spec ::= error symbol_list precedence_list start_spec production_list 
			void *RESULT = NULL;
			GroToken *cup_sl = context_class->getFromTop(parser_context, 3);
			void *sl = (void *) (cup_sl->value);
			GroToken *cup_pl = context_class->getFromTop(parser_context, 0);
			void *pl = (void *) (cup_pl->value);
			cup_result = gro_token_new_minimal(26 /*spec*/, RESULT);
			return cup_result;
		}
		case 3 : { // package_spec ::= PACKAGE multipart_id SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(1 /*package_spec*/, RESULT);
			return cup_result;
		}
		case 4 : { // package_spec ::= empty 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(1 /*package_spec*/, RESULT);
			return cup_result;
		}
		case 5 : { // import_list ::= import_list import_spec 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(2 /*import_list*/, RESULT);
			return cup_result;
		}
		case 6 : { // import_list ::= empty 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(2 /*import_list*/, RESULT);
			return cup_result;
		}
		case 7 : { // import_spec ::= IMPORT import_id SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(10 /*import_spec*/, RESULT);
			return cup_result;
		}
		case 8 : { // code_part ::= action_code_part 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(5 /*code_part*/, RESULT);
			return cup_result;
		}
		case 9 : { // code_part ::= parser_code_part 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(5 /*code_part*/, RESULT);
			return cup_result;
		}
		case 10 : { // code_part ::= init_code 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(5 /*code_part*/, RESULT);
			return cup_result;
		}
		case 11 : { // code_part ::= scan_code 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(5 /*code_part*/, RESULT);
			return cup_result;
		}
		case 12 : { // code_parts ::= 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(4 /*code_parts*/, RESULT);
			return cup_result;
		}
		case 13 : { // code_parts ::= code_parts code_part 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(4 /*code_parts*/, RESULT);
			return cup_result;
		}
		case 14 : { // action_code_part ::= ACTION CODE CODE_STRING opt_semi 
			void *RESULT = NULL;
			GroToken *cup_user_code = context_class->getFromTop(parser_context, 1);
			void *user_code = (void *) (cup_user_code->value);
			cup_result = gro_token_new_minimal(3 /*action_code_part*/, RESULT);
			return cup_result;
		}
		case 15 : { // parser_code_part ::= PARSER CODE CODE_STRING opt_semi 
			void *RESULT = NULL;
			GroToken *cup_user_code = context_class->getFromTop(parser_context, 1);
			void *user_code = (void *) (cup_user_code->value);
			cup_result = gro_token_new_minimal(8 /*parser_code_part*/, RESULT);
			return cup_result;
		}
		case 16 : { // init_code ::= INIT WITH CODE_STRING opt_semi 
			void *RESULT = NULL;
			GroToken *cup_user_code = context_class->getFromTop(parser_context, 1);
			void *user_code = (void *) (cup_user_code->value);
			cup_result = gro_token_new_minimal(12 /*init_code*/, RESULT);
			return cup_result;
		}
		case 17 : { // scan_code ::= SCAN WITH CODE_STRING opt_semi 
			void *RESULT = NULL;
			GroToken *cup_user_code = context_class->getFromTop(parser_context, 1);
			void *user_code = (void *) (cup_user_code->value);
			cup_result = gro_token_new_minimal(13 /*scan_code*/, RESULT);
			return cup_result;
		}
		case 18 : { // symbol_list ::= symbol_list symbol 
			void *RESULT = NULL;
			GroToken *cup_sl = context_class->getFromTop(parser_context, 1);
			void *sl = (void *) (cup_sl->value);
			GroToken *cup_s = context_class->getFromTop(parser_context, 0);
			void *s = (void *) (cup_s->value);
			cup_result = gro_token_new_minimal(33 /*symbol_list*/, RESULT);
			return cup_result;
		}
		case 19 : { // symbol_list ::= symbol 
			void *RESULT = NULL;
			GroToken *cup_s = context_class->getFromTop(parser_context, 0);
			void *s = (void *) (cup_s->value);
			cup_result = gro_token_new_minimal(33 /*symbol_list*/, RESULT);
			return cup_result;
		}
		case 20 : { // symbol ::= TERMINAL type_id declares_term 
			void *RESULT = NULL;
			GroToken *cup_ti = context_class->getFromTop(parser_context, 1);
			void *ti = (void *) (cup_ti->value);
			GroToken *cup_tl = context_class->getFromTop(parser_context, 0);
			void *tl = (void *) (cup_tl->value);
			cup_result = gro_token_new_minimal(14 /*symbol*/, RESULT);
			return cup_result;
		}
		case 21 : { // symbol ::= TERMINAL declares_term 
			void *RESULT = NULL;
			GroToken *cup_tl = context_class->getFromTop(parser_context, 0);
			void *tl = (void *) (cup_tl->value);
			cup_result = gro_token_new_minimal(14 /*symbol*/, RESULT);
			return cup_result;
		}
		case 22 : { // symbol ::= non_terminal type_id declares_non_term 
			void *RESULT = NULL;
			GroToken *cup_ti = context_class->getFromTop(parser_context, 1);
			void *ti = (void *) (cup_ti->value);
			GroToken *cup_tl = context_class->getFromTop(parser_context, 0);
			void *tl = (void *) (cup_tl->value);
			cup_result = gro_token_new_minimal(14 /*symbol*/, RESULT);
			return cup_result;
		}
		case 23 : { // symbol ::= non_terminal declares_non_term 
			void *RESULT = NULL;
			GroToken *cup_tl = context_class->getFromTop(parser_context, 0);
			void *tl = (void *) (cup_tl->value);
			cup_result = gro_token_new_minimal(14 /*symbol*/, RESULT);
			return cup_result;
		}
		case 24 : { // symbol ::= TERMINAL error SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(14 /*symbol*/, RESULT);
			return cup_result;
		}
		case 25 : { // symbol ::= non_terminal error SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(14 /*symbol*/, RESULT);
			return cup_result;
		}
		case 26 : { // declares_term ::= term_name_list SEMI 
			void *RESULT = NULL;
			GroToken *cup_tnl = context_class->getFromTop(parser_context, 1);
			void *tnl = (void *) (cup_tnl->value);
			cup_result = gro_token_new_minimal(24 /*declares_term*/, RESULT);
			return cup_result;
		}
		case 27 : { // declares_non_term ::= non_term_name_list SEMI 
			void *RESULT = NULL;
			GroToken *cup_tnl = context_class->getFromTop(parser_context, 1);
			void *tnl = (void *) (cup_tnl->value);
			cup_result = gro_token_new_minimal(25 /*declares_non_term*/, RESULT);
			return cup_result;
		}
		case 28 : { // term_name_list ::= term_name_list COMMA new_term_id 
			void *RESULT = NULL;
			GroToken *cup_tnl = context_class->getFromTop(parser_context, 2);
			void *tnl = (void *) (cup_tnl->value);
			GroToken *cup_nti = context_class->getFromTop(parser_context, 0);
			void *nti = (void *) (cup_nti->value);
			cup_result = gro_token_new_minimal(16 /*term_name_list*/, RESULT);
			return cup_result;
		}
		case 29 : { // term_name_list ::= new_term_id 
			void *RESULT = NULL;
			GroToken *cup_nti = context_class->getFromTop(parser_context, 0);
			void *nti = (void *) (cup_nti->value);
			cup_result = gro_token_new_minimal(16 /*term_name_list*/, RESULT);
			return cup_result;
		}
		case 30 : { // non_term_name_list ::= non_term_name_list COMMA new_non_term_id 
			void *RESULT = NULL;
			GroToken *cup_tnl = context_class->getFromTop(parser_context, 2);
			void *tnl = (void *) (cup_tnl->value);
			GroToken *cup_nti = context_class->getFromTop(parser_context, 0);
			void *nti = (void *) (cup_nti->value);
			cup_result = gro_token_new_minimal(17 /*non_term_name_list*/, RESULT);
			return cup_result;
		}
		case 31 : { // non_term_name_list ::= new_non_term_id 
			void *RESULT = NULL;
			GroToken *cup_nti = context_class->getFromTop(parser_context, 0);
			void *nti = (void *) (cup_nti->value);
			cup_result = gro_token_new_minimal(17 /*non_term_name_list*/, RESULT);
			return cup_result;
		}
		case 32 : { // precedence_list ::= precedence_l 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(20 /*precedence_list*/, RESULT);
			return cup_result;
		}
		case 33 : { // precedence_list ::= empty 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(20 /*precedence_list*/, RESULT);
			return cup_result;
		}
		case 34 : { // precedence_l ::= precedence_l preced 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(23 /*precedence_l*/, RESULT);
			return cup_result;
		}
		case 35 : { // precedence_l ::= preced 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(23 /*precedence_l*/, RESULT);
			return cup_result;
		}
		case 36 : { // preced ::= PRECEDENCE LEFT terminal_list SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(21 /*preced*/, RESULT);
			return cup_result;
		}
		case 37 : { // preced ::= PRECEDENCE RIGHT terminal_list SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(21 /*preced*/, RESULT);
			return cup_result;
		}
		case 38 : { // preced ::= PRECEDENCE NONASSOC terminal_list SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(21 /*preced*/, RESULT);
			return cup_result;
		}
		case 39 : { // terminal_list ::= terminal_list COMMA terminal_id 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(22 /*terminal_list*/, RESULT);
			return cup_result;
		}
		case 40 : { // terminal_list ::= terminal_id 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(22 /*terminal_list*/, RESULT);
			return cup_result;
		}
		case 41 : { // terminal_id ::= term_id 
			void *RESULT = NULL;
			GroToken *cup_sym = context_class->getFromTop(parser_context, 0);
			void *sym = (void *) (cup_sym->value);
			cup_result = gro_token_new_minimal(38 /*terminal_id*/, RESULT);
			return cup_result;
		}
		case 42 : { // term_id ::= symbol_id 
			void *RESULT = NULL;
			GroToken *cup_sym = context_class->getFromTop(parser_context, 0);
			void *sym = (void *) (cup_sym->value);
			cup_result = gro_token_new_minimal(39 /*term_id*/, RESULT);
			return cup_result;
		}
		case 43 : { // start_spec ::= START WITH nt_id SEMI 
			void *RESULT = NULL;
			GroToken *cup_start_name = context_class->getFromTop(parser_context, 1);
			void *start_name = (void *) (cup_start_name->value);
			cup_result = gro_token_new_minimal(9 /*start_spec*/, RESULT);
			return cup_result;
		}
		case 44 : { // start_spec ::= empty 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(9 /*start_spec*/, RESULT);
			return cup_result;
		}
		case 45 : { // production_list ::= production_list production 
			void *RESULT = NULL;
			GroToken *cup_pl = context_class->getFromTop(parser_context, 1);
			void *pl = (void *) (cup_pl->value);
			GroToken *cup_p = context_class->getFromTop(parser_context, 0);
			void *p = (void *) (cup_p->value);
			cup_result = gro_token_new_minimal(32 /*production_list*/, RESULT);
			return cup_result;
		}
		case 46 : { // production_list ::= production 
			void *RESULT = NULL;
			GroToken *cup_p = context_class->getFromTop(parser_context, 0);
			void *p = (void *) (cup_p->value);
			cup_result = gro_token_new_minimal(32 /*production_list*/, RESULT);
			return cup_result;
		}
		case 47 : { // production ::= nt_id COLON_COLON_EQUALS rhs_list SEMI 
			void *RESULT = NULL;
			GroToken *cup_lhs_id = context_class->getFromTop(parser_context, 3);
			void *lhs_id = (void *) (cup_lhs_id->value);
			GroToken *cup_rhsl = context_class->getFromTop(parser_context, 1);
			void *rhsl = (void *) (cup_rhsl->value);
			cup_result = gro_token_new_minimal(31 /*production*/, RESULT);
			return cup_result;
		}
		case 48 : { // production ::= error SEMI 
			void *RESULT = NULL;
			GroToken *cup_s = context_class->getFromTop(parser_context, 0);
			void *s = (void *) (cup_s->value);
			cup_result = gro_token_new_minimal(31 /*production*/, RESULT);
			return cup_result;
		}
		case 49 : { // rhs_list ::= rhs_list BAR rhs 
			void *RESULT = NULL;
			GroToken *cup_rl = context_class->getFromTop(parser_context, 2);
			void *rl = (void *) (cup_rl->value);
			GroToken *cup_r = context_class->getFromTop(parser_context, 0);
			void *r = (void *) (cup_r->value);
			cup_result = gro_token_new_minimal(30 /*rhs_list*/, RESULT);
			return cup_result;
		}
		case 50 : { // rhs_list ::= rhs 
			void *RESULT = NULL;
			GroToken *cup_r = context_class->getFromTop(parser_context, 0);
			void *r = (void *) (cup_r->value);
			cup_result = gro_token_new_minimal(30 /*rhs_list*/, RESULT);
			return cup_result;
		}
		case 51 : { // rhs ::= prod_part_list PERCENT_PREC term_id 
			void *RESULT = NULL;
			GroToken *cup_ppl = context_class->getFromTop(parser_context, 2);
			void *ppl = (void *) (cup_ppl->value);
			GroToken *cup_term_name = context_class->getFromTop(parser_context, 0);
			void *term_name = (void *) (cup_term_name->value);
			cup_result = gro_token_new_minimal(27 /*rhs*/, RESULT);
			return cup_result;
		}
		case 52 : { // rhs ::= prod_part_list 
			void *RESULT = NULL;
			GroToken *cup_ppl = context_class->getFromTop(parser_context, 0);
			void *ppl = (void *) (cup_ppl->value);
			cup_result = gro_token_new_minimal(27 /*rhs*/, RESULT);
			return cup_result;
		}
		case 53 : { // prod_part_list ::= prod_part_list prod_part 
			void *RESULT = NULL;
			GroToken *cup_ppl = context_class->getFromTop(parser_context, 1);
			void *ppl = (void *) (cup_ppl->value);
			GroToken *cup_pp = context_class->getFromTop(parser_context, 0);
			void *pp = (void *) (cup_pp->value);
			cup_result = gro_token_new_minimal(29 /*prod_part_list*/, RESULT);
			return cup_result;
		}
		case 54 : { // prod_part_list ::= empty 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(29 /*prod_part_list*/, RESULT);
			return cup_result;
		}
		case 55 : { // prod_part ::= symbol_id opt_label 
			void *RESULT = NULL;
			GroToken *cup_symid = context_class->getFromTop(parser_context, 1);
			void *symid = (void *) (cup_symid->value);
			GroToken *cup_labid = context_class->getFromTop(parser_context, 0);
			void *labid = (void *) (cup_labid->value);
			cup_result = gro_token_new_minimal(28 /*prod_part*/, RESULT);
			return cup_result;
		}
		case 56 : { // prod_part ::= CODE_STRING 
			void *RESULT = NULL;
			GroToken *cup_code_str = context_class->getFromTop(parser_context, 0);
			void *code_str = (void *) (cup_code_str->value);
			cup_result = gro_token_new_minimal(28 /*prod_part*/, RESULT);
			return cup_result;
		}
		case 57 : { // opt_label ::= COLON label_id 
			void *RESULT = NULL;
			GroToken *cup_labid = context_class->getFromTop(parser_context, 0);
			void *labid = (void *) (cup_labid->value);
			cup_result = gro_token_new_minimal(37 /*opt_label*/, RESULT);
			return cup_result;
		}
		case 58 : { // opt_label ::= empty 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(37 /*opt_label*/, RESULT);
			return cup_result;
		}
		case 59 : { // multipart_id ::= multipart_id DOT robust_id 
			void *RESULT = NULL;
			GroToken *cup_ba = context_class->getFromTop(parser_context, 2);
			void *ba = (void *) (cup_ba->value);
			GroToken *cup_ad = context_class->getFromTop(parser_context, 0);
			void *ad = (void *) (cup_ad->value);
			cup_result = gro_token_new_minimal(42 /*multipart_id*/, RESULT);
			return cup_result;
		}
		case 60 : { // multipart_id ::= robust_id 
			void *RESULT = NULL;
			GroToken *cup_an_id = context_class->getFromTop(parser_context, 0);
			void *an_id = (void *) (cup_an_id->value);
			cup_result = gro_token_new_minimal(42 /*multipart_id*/, RESULT);
			return cup_result;
		}
		case 61 : { // import_id ::= multipart_id DOT STAR 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(11 /*import_id*/, RESULT);
			return cup_result;
		}
		case 62 : { // import_id ::= multipart_id 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(11 /*import_id*/, RESULT);
			return cup_result;
		}
		case 63 : { // type_id ::= multipart_id 
			void *RESULT = NULL;
			GroToken *cup_mi = context_class->getFromTop(parser_context, 0);
			void *mi = (void *) (cup_mi->value);
			cup_result = gro_token_new_minimal(15 /*type_id*/, RESULT);
			return cup_result;
		}
		case 64 : { // type_id ::= type_id LBRACK RBRACK 
			void *RESULT = NULL;
			GroToken *cup_ti = context_class->getFromTop(parser_context, 2);
			void *ti = (void *) (cup_ti->value);
			cup_result = gro_token_new_minimal(15 /*type_id*/, RESULT);
			return cup_result;
		}
		case 65 : { // new_term_id ::= ID 
			void *RESULT = NULL;
			GroToken *cup_term_id = context_class->getFromTop(parser_context, 0);
			void *term_id = (void *) (cup_term_id->value);
			cup_result = gro_token_new_minimal(41 /*new_term_id*/, RESULT);
			return cup_result;
		}
		case 66 : { // new_non_term_id ::= ID 
			void *RESULT = NULL;
			GroToken *cup_non_term_id = context_class->getFromTop(parser_context, 0);
			void *non_term_id = (void *) (cup_non_term_id->value);
			cup_result = gro_token_new_minimal(18 /*new_non_term_id*/, RESULT);
			return cup_result;
		}
		case 67 : { // nt_id ::= ID 
			void *RESULT = NULL;
			GroToken *cup_the_id = context_class->getFromTop(parser_context, 0);
			void *the_id = (void *) (cup_the_id->value);
			cup_result = gro_token_new_minimal(34 /*nt_id*/, RESULT);
			return cup_result;
		}
		case 68 : { // nt_id ::= error 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(34 /*nt_id*/, RESULT);
			return cup_result;
		}
		case 69 : { // symbol_id ::= ID 
			void *RESULT = NULL;
			GroToken *cup_the_id = context_class->getFromTop(parser_context, 0);
			void *the_id = (void *) (cup_the_id->value);
			cup_result = gro_token_new_minimal(35 /*symbol_id*/, RESULT);
			return cup_result;
		}
		case 70 : { // symbol_id ::= error 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(35 /*symbol_id*/, RESULT);
			return cup_result;
		}
		case 71 : { // label_id ::= robust_id 
			void *RESULT = NULL;
			GroToken *cup_the_id = context_class->getFromTop(parser_context, 0);
			void *the_id = (void *) (cup_the_id->value);
			cup_result = gro_token_new_minimal(36 /*label_id*/, RESULT);
			return cup_result;
		}
		case 72 : { // robust_id ::= ID 
			void *RESULT = NULL;
			GroToken *cup_the_id = context_class->getFromTop(parser_context, 0);
			void *the_id = (void *) (cup_the_id->value);
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 73 : { // robust_id ::= CODE 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 74 : { // robust_id ::= ACTION 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 75 : { // robust_id ::= PARSER 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 76 : { // robust_id ::= TERMINAL 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 77 : { // robust_id ::= NON 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 78 : { // robust_id ::= NONTERMINAL 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 79 : { // robust_id ::= INIT 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 80 : { // robust_id ::= SCAN 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 81 : { // robust_id ::= WITH 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 82 : { // robust_id ::= START 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 83 : { // robust_id ::= PRECEDENCE 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 84 : { // robust_id ::= LEFT 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 85 : { // robust_id ::= RIGHT 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 86 : { // robust_id ::= NONASSOC 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 87 : { // robust_id ::= error 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(40 /*robust_id*/, RESULT);
			return cup_result;
		}
		case 88 : { // non_terminal ::= NON TERMINAL 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(7 /*non_terminal*/, RESULT);
			return cup_result;
		}
		case 89 : { // non_terminal ::= NONTERMINAL 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(7 /*non_terminal*/, RESULT);
			return cup_result;
		}
		case 90 : { // opt_semi ::= 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(6 /*opt_semi*/, RESULT);
			return cup_result;
		}
		case 91 : { // opt_semi ::= SEMI 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(6 /*opt_semi*/, RESULT);
			return cup_result;
		}
		case 92 : { // empty ::= 
			void *RESULT = NULL;
			cup_result = gro_token_new_minimal(19 /*empty*/, RESULT);
			return cup_result;
		}
	}
}
