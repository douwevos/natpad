#ifndef SMPLVIRTUALPARSESTACK_H_
#define SMPLVIRTUALPARSESTACK_H_

#include <caterpillar.h>
#include "smpltoken.h"

G_BEGIN_DECLS

#define SMPL_TYPE_VIRTUAL_PARSE_STACK            (smpl_virtual_parse_stack_get_type())
#define SMPL_VIRTUAL_PARSE_STACK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_virtual_parse_stack_get_type(), SmplVirtualParseStack))
#define SMPL_VIRTUAL_PARSE_STACK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_VIRTUAL_PARSE_STACK, SmplVirtualParseStackClass))
#define SMPL_IS_VIRTUAL_PARSE_STACK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_VIRTUAL_PARSE_STACK))
#define SMPL_IS_VIRTUAL_PARSE_STACK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_VIRTUAL_PARSE_STACK))
#define SMPL_VIRTUAL_PARSE_STACK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_VIRTUAL_PARSE_STACK, SmplVirtualParseStackClass))

typedef struct _SmplVirtualParseStack       SmplVirtualParseStack;
typedef struct _SmplVirtualParseStackClass  SmplVirtualParseStackClass;


struct _SmplVirtualParseStack {
	GObject parent;

	/** The real stack that we shadow.  This is accessed when we move off
	 *  the bottom of the virtual portion of the stack, but is always left
	 *  unmodified.
	 */
	CatArray *real_stack;

	/** The virtual top portion of the stack.  This stack contains Integer
	 *  objects with state numbers.  This stack shadows the top portion
	 *  of the real stack within the area that has been modified (via operations
	 *  on the virtual stack).  When this portion of the stack becomes empty we
	 *  transfer elements from the underlying stack onto this stack.
	 */
	CatArray *vstack;

	/** Top of stack indicator for where we leave off in the real stack.
	 *  This is measured from top of stack, so 0 would indicate that no
	 *  elements have been "moved" from the real to virtual stack.
	 */
	int real_next;

};


struct _SmplVirtualParseStackClass {
	GObjectClass parent_class;
};


GType smpl_virtual_parse_stack_get_type(void);

SmplVirtualParseStack *smpl_virtual_parse_stack_new(CatArray *real_array);



/** Transfer an element from the real to the virtual stack.  This assumes
 *  that the virtual stack is currently empty.
 */
void smpl_virtual_parser_stack_get_from_real(SmplVirtualParseStack *virtual_stack);

/** Indicate whether the stack is empty. */
gboolean smpl_virtual_parser_stack_empty(SmplVirtualParseStack *virtual_stack);


/** Return value on the top of the stack (without popping it). */
int smpl_virtual_parser_stack_top(SmplVirtualParseStack *virtual_stack);

/** Pop the stack. */
void smpl_virtual_parser_stack_pop(SmplVirtualParseStack *virtual_stack);

/** Push a state number onto the stack. */
void smpl_virtual_parser_stack_push(SmplVirtualParseStack *virtual_stack, SmplToken *token);


G_END_DECLS


#endif /* SMPLVIRTUALPARSESTACK_H_ */
