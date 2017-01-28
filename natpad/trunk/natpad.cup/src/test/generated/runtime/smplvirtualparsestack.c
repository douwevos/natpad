#include "smplvirtualparsestack.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SmplVirtualParseStack"
#include <logging/catlog.h>

G_DEFINE_TYPE(SmplVirtualParseStack, smpl_virtual_parse_stack, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void smpl_virtual_parse_stack_class_init(SmplVirtualParseStackClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void smpl_virtual_parse_stack_init(SmplVirtualParseStack *obj) {
}

static void _dispose(GObject *object) {
	SmplVirtualParseStack *instance = SMPL_VIRTUAL_PARSE_STACK(object);
	cat_unref_ptr(instance->real_stack);
	cat_unref_ptr(instance->vstack);
}


SmplVirtualParseStack *smpl_virtual_parse_stack_new(CatArray *real) {
	SmplVirtualParseStack *result = g_object_new(SMPL_TYPE_VIRTUAL_PARSE_STACK, NULL);
	cat_ref_anounce(result);
	result->real_stack = cat_ref_ptr(real);
	result->vstack = cat_array_new();
	result->real_next = 0;
	smpl_virtual_parser_stack_get_from_real(result);
	return result;
}



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void smpl_virtual_parser_stack_get_from_real(SmplVirtualParseStack *virtual_stack) {
	SmplToken *stack_sym;

	/* don't transfer if the real stack is empty */
	if (virtual_stack->real_next >= cat_array_size(virtual_stack->real_stack)) {
		return;
	}

	/* get a copy of the first Symbol we have not transfered */
	int index = cat_array_size(virtual_stack->real_stack)-1-virtual_stack->real_next;
	stack_sym = (SmplToken *) cat_array_get(virtual_stack->real_stack, index);

	/* record the transfer */
	virtual_stack->real_next++;

	/* put the state number from the Symbol onto the virtual stack */
	cat_array_append(virtual_stack->vstack, (GObject *) stack_sym);
}

/** Indicate whether the stack is empty. */
gboolean smpl_virtual_parser_stack_empty(SmplVirtualParseStack *virtual_stack) {
	/* if vstack is empty then we were unable to transfer onto it and the whole thing is empty. */
	return cat_array_size(virtual_stack->vstack)==0;
}


/** Return value on the top of the stack (without popping it). */
int smpl_virtual_parser_stack_top(SmplVirtualParseStack *virtual_stack) {
	if (cat_array_size(virtual_stack->vstack)==0) {
		return 0;
//		throw new Exception("Internal parser error: top() called on empty virtual stack");
	}
	SmplToken *top_token = (SmplToken *) cat_array_get(virtual_stack->vstack, cat_array_size(virtual_stack->vstack)-1);
	return top_token->parse_state;
}

/** Pop the stack. */
void smpl_virtual_parser_stack_pop(SmplVirtualParseStack *virtual_stack) {
	if (cat_array_size(virtual_stack->vstack)==0) {
		return;
//		throw new Exception("Internal parser error: pop from empty virtual stack");
	}

	/* pop it */
	cat_array_remove(virtual_stack->vstack, cat_array_size(virtual_stack->vstack)-1, NULL);

	/* if we are now empty transfer an element (if there is one) */
	if (cat_array_size(virtual_stack->vstack)==0) {
		smpl_virtual_parser_stack_get_from_real(virtual_stack);
	}
}


/** Push a state number onto the stack. */
void smpl_virtual_parser_stack_push(SmplVirtualParseStack *virtual_stack, SmplToken *token) {
	cat_array_append(virtual_stack->vstack, (GObject *) token);
}
