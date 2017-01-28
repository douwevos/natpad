#ifndef [%PRE%]VIRTUALPARSESTACK_H_
#define [%PRE%]VIRTUALPARSESTACK_H_

#include <caterpillar.h>
#include "[%pre%]token.h"

G_BEGIN_DECLS

#define [%PRE%]_TYPE_VIRTUAL_PARSE_STACK            ([%pre%]_virtual_parse_stack_get_type())
#define [%PRE%]_VIRTUAL_PARSE_STACK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), [%pre%]_virtual_parse_stack_get_type(), [%Pre%]VirtualParseStack))
#define [%PRE%]_VIRTUAL_PARSE_STACK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), [%PRE%]_TYPE_VIRTUAL_PARSE_STACK, [%Pre%]VirtualParseStackClass))
#define [%PRE%]_IS_VIRTUAL_PARSE_STACK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), [%PRE%]_TYPE_VIRTUAL_PARSE_STACK))
#define [%PRE%]_IS_VIRTUAL_PARSE_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), [%PRE%]_TYPE_VIRTUAL_PARSE_STACK))
#define [%PRE%]_VIRTUAL_PARSE_STACK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), [%PRE%]_TYPE_VIRTUAL_PARSE_STACK, [%Pre%]VirtualParseStackClass))

typedef struct _[%Pre%]VirtualParseStack       [%Pre%]VirtualParseStack;
typedef struct _[%Pre%]VirtualParseStackClass  [%Pre%]VirtualParseStackClass;


struct _[%Pre%]VirtualParseStack {
	GObject parent;

	/** The real stack that we shadow.  This is accessed when we move off
	 *  the bottom of the virtual portion of the stack, but is always left
	 *  unmodified.
	 */
	CatArrayWo *e_real_stack;

	/** The virtual top portion of the stack.  This stack contains Integer
	 *  objects with state numbers.  This stack shadows the top portion
	 *  of the real stack within the area that has been modified (via operations
	 *  on the virtual stack).  When this portion of the stack becomes empty we
	 *  transfer elements from the underlying stack onto this stack.
	 */
	CatArrayWo *e_vstack;

	/** Top of stack indicator for where we leave off in the real stack.
	 *  This is measured from top of stack, so 0 would indicate that no
	 *  elements have been "moved" from the real to virtual stack.
	 */
	int real_next;

};


struct _[%Pre%]VirtualParseStackClass {
	GObjectClass parent_class;
};


GType [%pre%]_virtual_parse_stack_get_type(void);

[%Pre%]VirtualParseStack *[%pre%]_virtual_parse_stack_new(CatArrayWo *e_real_array);



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void [%pre%]_virtual_parser_stack_get_from_real([%Pre%]VirtualParseStack *virtual_stack);

/** Indicate whether the stack is empty. */
gboolean [%pre%]_virtual_parser_stack_empty([%Pre%]VirtualParseStack *virtual_stack);


/** Return value on the top of the stack (without popping it). */
int [%pre%]_virtual_parser_stack_top([%Pre%]VirtualParseStack *virtual_stack);

/** Pop the stack. */
void [%pre%]_virtual_parser_stack_pop([%Pre%]VirtualParseStack *virtual_stack);

/** Push a state number onto the stack. */
void [%pre%]_virtual_parser_stack_push([%Pre%]VirtualParseStack *virtual_stack, [%Pre%]Token *token);


G_END_DECLS


#endif /* [%PRE%]VIRTUALPARSESTACK_H_ */
