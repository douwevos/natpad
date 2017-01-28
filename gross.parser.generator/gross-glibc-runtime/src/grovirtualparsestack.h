#ifndef GROVIRTUALPARSESTACK_H_
#define GROVIRTUALPARSESTACK_H_

#include "grotoken.h"

G_BEGIN_DECLS

#define GRO_TYPE_VIRTUAL_PARSE_STACK            (gro_virtual_parse_stack_get_type())
#define GRO_VIRTUAL_PARSE_STACK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_virtual_parse_stack_get_type(), GroVirtualParseStack))
#define GRO_VIRTUAL_PARSE_STACK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_VIRTUAL_PARSE_STACK, GroVirtualParseStackClass))
#define GRO_IS_VIRTUAL_PARSE_STACK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_VIRTUAL_PARSE_STACK))
#define GRO_IS_VIRTUAL_PARSE_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_VIRTUAL_PARSE_STACK))
#define GRO_VIRTUAL_PARSE_STACK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_VIRTUAL_PARSE_STACK, GroVirtualParseStackClass))

typedef struct _GroVirtualParseStack       GroVirtualParseStack;
typedef struct _GroVirtualParseStackClass  GroVirtualParseStackClass;


struct _GroVirtualParseStack {
	GObject parent;

	/** The real stack that we shadow.  This is accessed when we move off
	 *  the bottom of the virtual portion of the stack, but is always left
	 *  unmodified.
	 */
	GPtrArray *e_real_stack;

	/** The virtual top portion of the stack.  This stack contains Integer
	 *  objects with state numbers.  This stack shadows the top portion
	 *  of the real stack within the area that has been modified (via operations
	 *  on the virtual stack).  When this portion of the stack becomes empty we
	 *  transfer elements from the underlying stack onto this stack.
	 */
	GPtrArray *e_vstack;

	/** Top of stack indicator for where we leave off in the real stack.
	 *  This is measured from top of stack, so 0 would indicate that no
	 *  elements have been "moved" from the real to virtual stack.
	 */
	int real_next;

};


struct _GroVirtualParseStackClass {
	GObjectClass parent_class;
};


GType gro_virtual_parse_stack_get_type(void);

GroVirtualParseStack *gro_virtual_parse_stack_new(GPtrArray *e_real_array);



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void gro_virtual_parser_stack_get_from_real(GroVirtualParseStack *virtual_stack);

/** Indicate whether the stack is empty. */
gboolean gro_virtual_parser_stack_empty(GroVirtualParseStack *virtual_stack);


/** Return value on the top of the stack (without popping it). */
int gro_virtual_parser_stack_top(GroVirtualParseStack *virtual_stack);

/** Pop the stack. */
void gro_virtual_parser_stack_pop(GroVirtualParseStack *virtual_stack);

/** Push a state number onto the stack. */
void gro_virtual_parser_stack_push(GroVirtualParseStack *virtual_stack, GroToken *token);


G_END_DECLS


#endif /* GROVIRTUALPARSESTACK_H_ */
