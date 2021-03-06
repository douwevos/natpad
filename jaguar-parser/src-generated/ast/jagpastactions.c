/* This file was generated by Gross-CC on 2017.04.30 AD at 13:51:01 CEST */
#include "jagpastactions.h"
#include <grossruntime.h>

static void l_iaction_handler_iface_init(GroRunIActionHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPAstActions, jagp_ast_actions, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_IACTION_HANDLER, l_iaction_handler_iface_init)
);

#define L_SAFE_UNREF_PTR(p) { void *ptr = p; p = NULL; if (ptr) { g_object_unref(ptr); } }
static G_GNUC_UNUSED void *L_SAFE_REF_PTR(void *p) { if (p) { g_object_ref(p); } return p; }

static void l_dispose(GObject *object);

static void jagp_ast_actions_class_init(JagPAstActionsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void jagp_ast_actions_init(JagPAstActions *instance) {
}

static void l_dispose(GObject *object) {
	G_OBJECT_CLASS(jagp_ast_actions_parent_class)->dispose(object);
}

JagPAstActions *jagp_ast_actions_new() {
	JagPAstActions *instance = g_object_new(JAGP_TYPE_AST_ACTIONS, NULL);
	return instance;
}


/******************** start GroRunIActionHandler implementation  ********************/

static GroRunIToken *l_extract(GroRunFork *fork, GroRunReduceLayout *reduce_layout, int offset) {
	GroRunIToken *result = NULL;
	int rloff = grorun_reduce_layout_calculate_stack_offset(reduce_layout, offset);
	GroRunStackEntry *fe = grorun_fork_get_entry(fork, rloff);
	if (fe!=NULL) {
		result = grorun_stack_entry_get_lookahead(fe);
	}
	return result;
}


static GObject *l_iaction_handler_run_action(GroRunIActionHandler *self, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id) {
	GObject *result = NULL;
	int prod_id = grorun_reduce_layout_get_production_id(reduce_layout);
	switch(prod_id) {

		case 1 : { // AnnotationsMultiOpt PackageDeclaration ImportDeclarationMultiOpt TypeDeclarationMultiOpt⇒CompilationUnit
			result = (GObject *) jagp_compilation_unit_new_1();
		} break;
	}
	return result;

}


static void l_iaction_handler_iface_init(GroRunIActionHandlerInterface *iface) {
	iface->runAction = l_iaction_handler_run_action;
}

/******************** end GroRunIActionHandler implementation ********************/

