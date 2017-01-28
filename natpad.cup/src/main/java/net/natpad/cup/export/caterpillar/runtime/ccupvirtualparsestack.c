#include "[%pre%]virtualparsestack.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "[%Pre%]VirtualParseStack"
#include <logging/catlog.h>

G_DEFINE_TYPE([%Pre%]VirtualParseStack, [%pre%]_virtual_parse_stack, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void [%pre%]_virtual_parse_stack_class_init([%Pre%]VirtualParseStackClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void [%pre%]_virtual_parse_stack_init([%Pre%]VirtualParseStack *obj) {
}

static void _dispose(GObject *object) {
	[%Pre%]VirtualParseStack *instance = [%PRE%]_VIRTUAL_PARSE_STACK(object);
	cat_unref_ptr(instance->e_real_stack);
	cat_unref_ptr(instance->e_vstack);
}


[%Pre%]VirtualParseStack *[%pre%]_virtual_parse_stack_new(CatArrayWo *e_real) {
	[%Pre%]VirtualParseStack *result = g_object_new([%PRE%]_TYPE_VIRTUAL_PARSE_STACK, NULL);
	cat_ref_anounce(result);
	result->e_real_stack = cat_ref_ptr(e_real);
	result->e_vstack = cat_array_wo_new();
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
	if (virtual_stack->real_next >= cat_array_wo_size(virtual_stack->e_real_stack)) {
		return;
	}

	/* get a copy of the first Symbol we have not transfered */
	int index = cat_array_wo_size(virtual_stack->e_real_stack)-1-virtual_stack->real_next;
	stack_sym = ([%Pre%]Token *) cat_array_wo_get(virtual_stack->e_real_stack, index);

	/* record the transfer */
	virtual_stack->real_next++;

	/* put the state number from the Symbol onto the virtual stack */
	cat_array_wo_append(virtual_stack->e_vstack, (GObject *) stack_sym);
}

/** Indicate whether the stack is empty. */
gboolean [%pre%]_virtual_parser_stack_empty([%Pre%]VirtualParseStack *virtual_stack) {
	/* if e_vstack is empty then we were unable to transfer onto it and the whole thing is empty. */
	return cat_array_wo_size(virtual_stack->e_vstack)==0;
}


/** Return value on the top of the stack (without popping it). */
int [%pre%]_virtual_parser_stack_top([%Pre%]VirtualParseStack *virtual_stack) {
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		return 0;
//		throw new Exception("Internal parser error: top() called on empty virtual stack");
	}
	[%Pre%]Token *top_token = ([%Pre%]Token *) cat_array_wo_get(virtual_stack->e_vstack, cat_array_wo_size(virtual_stack->e_vstack)-1);
	return top_token->parse_state;
}

/** Pop the stack. */
void [%pre%]_virtual_parser_stack_pop([%Pre%]VirtualParseStack *virtual_stack) {
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		return;
//		throw new Exception("Internal parser error: pop from empty virtual stack");
	}

	/* pop it */
	cat_array_wo_remove(virtual_stack->e_vstack, cat_array_wo_size(virtual_stack->e_vstack)-1, NULL);

	/* if we are now empty transfer an element (if there is one) */
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		[%pre%]_virtual_parser_stack_get_from_real(virtual_stack);
	}
}


/** Push a state number onto the stack. */
void [%pre%]_virtual_parser_stack_push([%Pre%]VirtualParseStack *virtual_stack, [%Pre%]Token *token) {
	cat_array_wo_append(virtual_stack->e_vstack, (GObject *) token);
}
