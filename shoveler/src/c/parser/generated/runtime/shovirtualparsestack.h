#ifndef SHOVIRTUALPARSESTACK_H_
#define SHOVIRTUALPARSESTACK_H_

#include <caterpillar.h>
#include "shotoken.h"

G_BEGIN_DECLS

#define SHO_TYPE_VIRTUAL_PARSE_STACK            (sho_virtual_parse_stack_get_type())
#define SHO_VIRTUAL_PARSE_STACK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_virtual_parse_stack_get_type(), ShoVirtualParseStack))
#define SHO_VIRTUAL_PARSE_STACK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_VIRTUAL_PARSE_STACK, ShoVirtualParseStackClass))
#define SHO_IS_VIRTUAL_PARSE_STACK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_VIRTUAL_PARSE_STACK))
#define SHO_IS_VIRTUAL_PARSE_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_VIRTUAL_PARSE_STACK))
#define SHO_VIRTUAL_PARSE_STACK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_VIRTUAL_PARSE_STACK, ShoVirtualParseStackClass))

typedef struct _ShoVirtualParseStack       ShoVirtualParseStack;
typedef struct _ShoVirtualParseStackClass  ShoVirtualParseStackClass;


struct _ShoVirtualParseStack {
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


struct _ShoVirtualParseStackClass {
	GObjectClass parent_class;
};


GType sho_virtual_parse_stack_get_type(void);

ShoVirtualParseStack *sho_virtual_parse_stack_new(CatArrayWo *e_real_array);



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void sho_virtual_parser_stack_get_from_real(ShoVirtualParseStack *virtual_stack);

/** Indicate whether the stack is empty. */
gboolean sho_virtual_parser_stack_empty(ShoVirtualParseStack *virtual_stack);


/** Return value on the top of the stack (without popping it). */
int sho_virtual_parser_stack_top(ShoVirtualParseStack *virtual_stack);

/** Pop the stack. */
void sho_virtual_parser_stack_pop(ShoVirtualParseStack *virtual_stack);

/** Push a state number onto the stack. */
void sho_virtual_parser_stack_push(ShoVirtualParseStack *virtual_stack, ShoToken *token);


G_END_DECLS


#endif /* SHOVIRTUALPARSESTACK_H_ */
