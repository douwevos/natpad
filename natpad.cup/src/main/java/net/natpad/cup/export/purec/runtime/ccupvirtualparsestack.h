#ifndef [%PRE%]VIRTUALPARSESTACK_H_
#define [%PRE%]VIRTUALPARSESTACK_H_

#include "[%pre%]token.h"
#include "[%pre%]vector.h"


typedef struct _[%Pre%]VirtualParseStack       [%Pre%]VirtualParseStack;


struct _[%Pre%]VirtualParseStack {
	/** The real stack that we shadow.  This is accessed when we move off
	 *  the bottom of the virtual portion of the stack, but is always left
	 *  unmodified.
	 */
	[%Pre%]Vector *real_stack;

	/** The virtual top portion of the stack.  This stack contains Integer
	 *  objects with state numbers.  This stack shadows the top portion
	 *  of the real stack within the area that has been modified (via operations
	 *  on the virtual stack).  When this portion of the stack becomes empty we
	 *  transfer elements from the underlying stack onto this stack.
	 */
	[%Pre%]Vector *vstack;

	/** Top of stack indicator for where we leave off in the real stack.
	 *  This is measured from top of stack, so 0 would indicate that no
	 *  elements have been "moved" from the real to virtual stack.
	 */
	int real_next;

};


void [%pre%]_virtual_parse_stack_free([%Pre%]VirtualParseStack *instance);

[%Pre%]VirtualParseStack *[%pre%]_virtual_parse_stack_new([%Pre%]Vector *real_array);



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void [%pre%]_virtual_parser_stack_get_from_real([%Pre%]VirtualParseStack *virtual_stack);

/** Indicate whether the stack is empty. */
[%pre%]boolean [%pre%]_virtual_parser_stack_empty([%Pre%]VirtualParseStack *virtual_stack);


/** Return value on the top of the stack (without popping it). */
int [%pre%]_virtual_parser_stack_top([%Pre%]VirtualParseStack *virtual_stack);

/** Pop the stack. */
void [%pre%]_virtual_parser_stack_pop([%Pre%]VirtualParseStack *virtual_stack);

/** Push a state number onto the stack. */
void [%pre%]_virtual_parser_stack_push([%Pre%]VirtualParseStack *virtual_stack, [%Pre%]Token *token);



#endif /* [%PRE%]VIRTUALPARSESTACK_H_ */
