#include "grovirtualparsestack.h"


G_DEFINE_TYPE(GroVirtualParseStack, gro_virtual_parse_stack, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void gro_virtual_parse_stack_class_init(GroVirtualParseStackClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void gro_virtual_parse_stack_init(GroVirtualParseStack *obj) {
}

static void l_unref(gpointer data, gpointer user_data) { g_object_unref(data); }

static void _dispose(GObject *object) {
	GroVirtualParseStack *instance = GRO_VIRTUAL_PARSE_STACK(object);
	if (instance->e_real_stack) {
		g_ptr_array_foreach(instance->e_real_stack, l_unref, NULL);
		g_ptr_array_unref(instance->e_real_stack);
		instance->e_real_stack = NULL;
	}
	if (instance->e_vstack) {
		g_ptr_array_foreach(instance->e_vstack, l_unref, NULL);
		g_ptr_array_unref(instance->e_vstack);
		instance->e_vstack = NULL;
	}
}


GroVirtualParseStack *gro_virtual_parse_stack_new(GPtrArray *e_real) {
	GroVirtualParseStack *result = g_object_new(GRO_TYPE_VIRTUAL_PARSE_STACK, NULL);
	result->e_real_stack = g_ptr_array_ref(e_real);
	result->e_vstack = g_ptr_array_new();
	result->real_next = 0;
	gro_virtual_parser_stack_get_from_real(result);
	return result;
}



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void gro_virtual_parser_stack_get_from_real(GroVirtualParseStack *virtual_stack) {
	GroToken *stack_sym;

	/* don't transfer if the real stack is empty */
	if (virtual_stack->real_next >= virtual_stack->e_real_stack->len) {
		return;
	}

	/* get a copy of the first Symbol we have not transfered */
	int index = virtual_stack->e_real_stack->len-1-virtual_stack->real_next;
	stack_sym = (GroToken *) g_ptr_array_index(virtual_stack->e_real_stack, index);

	/* record the transfer */
	virtual_stack->real_next++;

	/* put the state number from the Symbol onto the virtual stack */
	g_ptr_array_add(virtual_stack->e_vstack, (GObject *) stack_sym);
}

/** Indicate whether the stack is empty. */
gboolean gro_virtual_parser_stack_empty(GroVirtualParseStack *virtual_stack) {
	/* if e_vstack is empty then we were unable to transfer onto it and the whole thing is empty. */
	return virtual_stack->e_vstack->len==0;
}


/** Return value on the top of the stack (without popping it). */
int gro_virtual_parser_stack_top(GroVirtualParseStack *virtual_stack) {
	if (virtual_stack->e_vstack->len==0) {
		return 0;
//		throw new Exception("Internal parser error: top() called on empty virtual stack");
	}
	GroToken *top_token = (GroToken *) g_ptr_array_index(virtual_stack->e_vstack, virtual_stack->e_vstack->len-1);
	return top_token->parse_state;
}

/** Pop the stack. */
void gro_virtual_parser_stack_pop(GroVirtualParseStack *virtual_stack) {
	if (virtual_stack->e_vstack->len==0) {
		return;
//		throw new Exception("Internal parser error: pop from empty virtual stack");
	}

	/* pop it */
	GObject *obj = g_ptr_array_index(virtual_stack->e_vstack, virtual_stack->e_vstack->len-1);
	g_object_unref(obj);
	g_ptr_array_remove_index(virtual_stack->e_vstack, virtual_stack->e_vstack->len-1);

	/* if we are now empty transfer an element (if there is one) */
	if (virtual_stack->e_vstack->len==0) {
		gro_virtual_parser_stack_get_from_real(virtual_stack);
	}
}


/** Push a state number onto the stack. */
void gro_virtual_parser_stack_push(GroVirtualParseStack *virtual_stack, GroToken *token) {
	g_ptr_array_add(virtual_stack->e_vstack, (GObject *) token);
	g_object_ref(token);
}
