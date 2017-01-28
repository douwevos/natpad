#include "jagvirtualparsestack.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagVirtualParseStack"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagVirtualParseStack, jag_virtual_parse_stack, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void jag_virtual_parse_stack_class_init(JagVirtualParseStackClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void jag_virtual_parse_stack_init(JagVirtualParseStack *obj) {
}

static void _dispose(GObject *object) {
	JagVirtualParseStack *instance = JAG_VIRTUAL_PARSE_STACK(object);
	cat_unref_ptr(instance->e_real_stack);
	cat_unref_ptr(instance->e_vstack);
}


JagVirtualParseStack *jag_virtual_parse_stack_new(CatArrayWo *e_real) {
	JagVirtualParseStack *result = g_object_new(JAG_TYPE_VIRTUAL_PARSE_STACK, NULL);
	cat_ref_anounce(result);
	result->e_real_stack = cat_ref_ptr(e_real);
	result->e_vstack = cat_array_wo_new();
	result->real_next = 0;
	jag_virtual_parser_stack_get_from_real(result);
	return result;
}



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void jag_virtual_parser_stack_get_from_real(JagVirtualParseStack *virtual_stack) {
	JagToken *stack_sym;

	/* don't transfer if the real stack is empty */
	if (virtual_stack->real_next >= cat_array_wo_size(virtual_stack->e_real_stack)) {
		return;
	}

	/* get a copy of the first Symbol we have not transfered */
	int index = cat_array_wo_size(virtual_stack->e_real_stack)-1-virtual_stack->real_next;
	stack_sym = (JagToken *) cat_array_wo_get(virtual_stack->e_real_stack, index);

	/* record the transfer */
	virtual_stack->real_next++;

	/* put the state number from the Symbol onto the virtual stack */
	cat_array_wo_append(virtual_stack->e_vstack, (GObject *) stack_sym);
}

/** Indicate whether the stack is empty. */
gboolean jag_virtual_parser_stack_empty(JagVirtualParseStack *virtual_stack) {
	/* if e_vstack is empty then we were unable to transfer onto it and the whole thing is empty. */
	return cat_array_wo_size(virtual_stack->e_vstack)==0;
}


/** Return value on the top of the stack (without popping it). */
int jag_virtual_parser_stack_top(JagVirtualParseStack *virtual_stack) {
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		return 0;
//		throw new Exception("Internal parser error: top() called on empty virtual stack");
	}
	JagToken *top_token = (JagToken *) cat_array_wo_get(virtual_stack->e_vstack, cat_array_wo_size(virtual_stack->e_vstack)-1);
	return top_token->parse_state;
}

/** Pop the stack. */
void jag_virtual_parser_stack_pop(JagVirtualParseStack *virtual_stack) {
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		return;
//		throw new Exception("Internal parser error: pop from empty virtual stack");
	}

	/* pop it */
	cat_array_wo_remove(virtual_stack->e_vstack, cat_array_wo_size(virtual_stack->e_vstack)-1, NULL);

	/* if we are now empty transfer an element (if there is one) */
	if (cat_array_wo_size(virtual_stack->e_vstack)==0) {
		jag_virtual_parser_stack_get_from_real(virtual_stack);
	}
}


/** Push a state number onto the stack. */
void jag_virtual_parser_stack_push(JagVirtualParseStack *virtual_stack, JagToken *token) {
	cat_array_wo_append(virtual_stack->e_vstack, (GObject *) token);
}
