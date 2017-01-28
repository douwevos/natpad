#include "grotbaglrparseractions.h"

GroContent *grotba_glr_parser_actions_run_action(GroGlrParserContext *context, int action_id, GroContent *content, int lhs, int rhs_count) {
	/* Symbol object for return from actions */
	GroContent *cup_result = NULL;

	/* select the action based on the action number */
	switch(action_id) {
		case 0 : { // $START ::= u EOF 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			GroContent *gro_start_val = gro_content_get_linked_at(super_content, 1);
			void *start_val = (void *) gro_content_get_value(gro_start_val);
			gro_content_get_left(gro_start_val, &left_row, &left_column);
			super_content = gro_content_get_linked(gro_start_val);
RESULT = start_val;			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 1 : { // u ::= A A q F F 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 4);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 2 : { // u ::= A A q F D 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 4);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 3 : { // u ::= A A A v F F 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 5);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 4 : { // u ::= A A A error F F 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 5);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 5 : { // q ::= r E 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 1);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 6 : { // q ::= s 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 7 : { // q ::= t A 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 1);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 8 : { // r ::= A B C 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 2);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 9 : { // v ::= B C E 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 2);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 10 : { // s ::= A B C E 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 3);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
		case 11 : { // t ::= A B C 
			void *RESULT = NULL;
			long long left_row, right_row;
			int left_column, right_column;
			GroContent *super_content = content;
			gro_content_get_right(super_content, &right_row, &right_column);
			super_content = gro_content_get_linked_at(super_content, 2);
			gro_content_get_left(super_content, &left_row, &left_column);
			super_content = gro_content_get_linked(super_content);
			cup_result = gro_content_new(super_content, (GObject *) RESULT, left_row, left_column, right_row, right_column);
			return cup_result;
		}
	}
	return NULL;
}
