/* This file was generated by Gross-CC on 2017.01.25 AD at 22:48:06 CET */
#include "mulrustactions.h"
#include <grossruntime.h>


	




#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulRustActions"
#include <logging/catlog.h>

static void l_action_handler_iface_init(GroRunIActionHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulRustActions, mulrust_actions, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_IACTION_HANDLER, l_action_handler_iface_init)
);

static void l_finalize(GObject *object);
static void mulrust_actions_class_init(MulRustActionsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->finalize = l_finalize;
}
static void mulrust_actions_init(MulRustActions *instance) {
}
static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mulrust_actions_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}
MulRustActions *mulrust_actions_new() {
	MulRustActions *result = g_object_new(MULRUST_TYPE_ACTIONS, NULL);
	cat_ref_anounce(result);
	return result;
}

static GroRunStackEntry *l_extract(GroRunFork *fork, GroRunReduceLayout *reduce_layout, int offset) {
	int rloff = grorun_reduce_layout_calculate_stack_offset(reduce_layout, offset);
	return  grorun_fork_get_entry(fork, rloff);
}


static GObject *l_run_action(GroRunIActionHandler *self, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id) {
	void *result = NULL;
	switch(action_id) {
		case 0 : { /* generic_args ⟶ L_LT generic_values GE */
		grorun_fork_push_back_token(fork, "L_IS");		 
		} break;
		case 1 : { /* generic_args ⟶ L_LT generic_values SHREQ */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 2 : { /* generic_args ⟶ L_LT generic_values SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 3 : { /* generic_params ⟶ L_LT lifetimes SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 4 : { /* generic_params ⟶ L_LT ty_params SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 5 : { /* generic_args ⟶ SHL ty_qualified_path_and_generic_values SHREQ */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 6 : { /* generic_args ⟶ SHL ty_qualified_path_and_generic_values GE */
		grorun_fork_push_back_token(fork, "L_IS");		 
		} break;
		case 7 : { /* generic_args ⟶ SHL ty_qualified_path_and_generic_values SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 8 : { /* generic_params ⟶ L_LT lifetimes L_COMMA SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 9 : { /* generic_params ⟶ L_LT ty_params L_COMMA SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 10 : { /* generic_params ⟶ L_LT lifetimes L_COMMA ty_params SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
		case 11 : { /* generic_params ⟶ L_LT lifetimes L_COMMA ty_params L_COMMA SHR */
		grorun_fork_push_back_token(fork, "L_GT");		 
		} break;
	}
	return (GObject *) result;
}

static void l_action_handler_iface_init(GroRunIActionHandlerInterface *iface) {
	iface->runAction = l_run_action;
}
