#include "[%pre%]virtualparsestack.h"
#include <stdlib.h>

void [%pre%]_virtual_parse_stack_free([%Pre%]VirtualParseStack *instance) {
	instance->real_stack = NULL;
	if (instance->vstack) {
		[%pre%]_vector_free(instance->vstack);
		instance->vstack = NULL;
	}
}


[%Pre%]VirtualParseStack *[%pre%]_virtual_parse_stack_new([%Pre%]Vector *real) {
	[%Pre%]VirtualParseStack *result = malloc(sizeof([%Pre%]VirtualParseStack));
	result->real_stack = real;
	result->vstack = [%pre%]_vector_new(NULL);
	result->real_next = 0;
	[%pre%]_virtual_parser_stack_get_from_real(result);
	return result;
}



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void [%pre%]_virtual_parser_stack_get_from_real([%Pre%]VirtualParseStack *virtual_stack) {
	[%Pre%]Token *stack_sym;

	/* don't transfer if the real stack is empty */
	if (virtual_stack->real_next >= [%pre%]_vector_count(virtual_stack->real_stack)) {
		return;
	}

	/* get a copy of the first Symbol we have not transfered */
	int index = [%pre%]_vector_count(virtual_stack->real_stack)-1-virtual_stack->real_next;
	stack_sym = ([%Pre%]Token *) [%pre%]_vector_get_at(virtual_stack->real_stack, index);

	/* record the transfer */
	virtual_stack->real_next++;

	/* put the state number from the Symbol onto the virtual stack */
	[%pre%]_vector_add(virtual_stack->vstack, (void *) stack_sym);
}

/** Indicate whether the stack is empty. */
[%pre%]boolean [%pre%]_virtual_parser_stack_empty([%Pre%]VirtualParseStack *virtual_stack) {
	/* if vstack is empty then we were unable to transfer onto it and the whole thing is empty. */
	return [%pre%]_vector_count(virtual_stack->vstack)==0;
}


/** Return value on the top of the stack (without popping it). */
int [%pre%]_virtual_parser_stack_top([%Pre%]VirtualParseStack *virtual_stack) {
	if ([%pre%]_vector_count(virtual_stack->vstack)==0) {
		return 0;
//		throw new Exception("Internal parser error: top() called on empty virtual stack");
	}
	[%Pre%]Token *top_token = ([%Pre%]Token *) [%pre%]_vector_get_at(virtual_stack->vstack, [%pre%]_vector_count(virtual_stack->vstack)-1);
	return top_token->parse_state;
}

/** Pop the stack. */
void [%pre%]_virtual_parser_stack_pop([%Pre%]VirtualParseStack *virtual_stack) {
	if ([%pre%]_vector_count(virtual_stack->vstack)==0) {
		return;
//		throw new Exception("Internal parser error: pop from empty virtual stack");
	}

	/* pop it */
	[%pre%]_vector_pop(virtual_stack->vstack);

	/* if we are now empty transfer an element (if there is one) */
	if ([%pre%]_vector_count(virtual_stack->vstack)==0) {
		[%pre%]_virtual_parser_stack_get_from_real(virtual_stack);
	}
}


/** Push a state number onto the stack. */
void [%pre%]_virtual_parser_stack_push([%Pre%]VirtualParseStack *virtual_stack, [%Pre%]Token *token) {
	[%pre%]_vector_add(virtual_stack->vstack, (void *) token);
}
