#include "shovirtualparsestack.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoVirtualParseStack"
#include <logging/catlog.h>

G_DEFINE_TYPE(ShoVirtualParseStack, sho_virtual_parse_stack, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void sho_virtual_parse_stack_class_init(ShoVirtualParseStackClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void sho_virtual_parse_stack_init(ShoVirtualParseStack *obj) {
}

static void _dispose(GObject *object) {
	ShoVirtualParseStack *instance = SHO_VIRTUAL_PARSE_STACK(object);
	cat_unref_ptr(instance->e_real_stack);
	cat_unref_ptr(instance->e_vstack);
}


ShoVirtualParseStack *sho_virtual_parse_stack_new(CatArrayWo *e_real) {
	ShoVirtualParseStack *result = g_object_new(SHO_TYPE_VIRTUAL_PARSE_STACK, NULL);
	cat_ref_anounce(result);
	result->e_real_stack = cat_ref_ptr(e_real);
	result->e_vstack = cat_array_wo_new();
	result->real_next = 0;
	sho_virtual_parser_stack_get_from_real(result);
	return result;
}



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void sho_virtual_parser_stack_get_from_real(ShoVirtualParseStack *virtual_stack) {
	ShoToken *stack_sym;

	/* don't transfer if the real stack is empty */
	if (virtual_stack->real_next >= cat_array_wo_size(virtual_stack->e_real_stack)) {
		return;
	}

	/* get a copy of the first Symbol we have not transfered */
	int index = cat_array_wo_size(virtual_stack->e_real_stack)-1-virtual_stack->real_next;
	stack_sym = (ShoToken *) cat_array_wo_get(virtual_stack->e_real_stack, index);

	/* record the transfer */
	virtual_stack->real_next++;

	/* put the state number from the Symbol onto the virtual stack */
	cat_array_wo_append(virtual_stack->e_vstack, (GObject *) stack_sym);
}

/** Indicate whether the stack is empty. */
gboolean sho_virtual_parser_stack_empty(ShoVirtualParseStack *virtual_stack) {
	/* if vstack is empty then we were unable to transfer onto it and the whole thing is empty. */
	return cat_array_wo_size(virtual_stack->e_vstack)==0;
}


/** Return value on the top of the stack (without popping it). */
int sho_virtual_parser_stack_top(ShoVirtualParseStack *virtual_stack) {
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		return 0;
//		throw new Exception("Internal parser error: top() called on empty virtual stack");
	}
	ShoToken *top_token = (ShoToken *) cat_array_wo_get(virtual_stack->e_vstack, cat_array_wo_size(virtual_stack->e_vstack)-1);
	return top_token->parse_state;
}

/** Pop the stack. */
void sho_virtual_parser_stack_pop(ShoVirtualParseStack *virtual_stack) {
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		return;
//		throw new Exception("Internal parser error: pop from empty virtual stack");
	}

	/* pop it */
	cat_array_wo_remove(virtual_stack->e_vstack, cat_array_wo_size(virtual_stack->e_vstack)-1, NULL);

	/* if we are now empty transfer an element (if there is one) */
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		sho_virtual_parser_stack_get_from_real(virtual_stack);
	}
}


/** Push a state number onto the stack. */
void sho_virtual_parser_stack_push(ShoVirtualParseStack *virtual_stack, ShoToken *token) {
	cat_array_wo_append(virtual_stack->e_vstack, (GObject *) token);
}
