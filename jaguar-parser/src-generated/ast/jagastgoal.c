/* This file was generated by Gross-CC on 2017.03.09 AD at 21:10:09 CET */
#include "jagastgoal.h"


G_DEFINE_TYPE_WITH_CODE(JagAstgoal, jagast_goal, G_TYPE_OBJECT,
);

#define L_SAFE_UNREF_PTR(p) { void *ptr = p; p = NULL; if (ptr) { g_object_unref(ptr); } }
static G_GNUC_UNUSED void *L_SAFE_REF_PTR(void *p) { if (p) { g_object_ref(p); } return p; }

static void l_dispose(GObject *object);

static void jagast_goal_class_init(JagAstgoalClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void jagast_goal_init(JagAstgoal *instance) {
}

static void l_dispose(GObject *object) {
	G_OBJECT_CLASS(jagast_goal_parent_class)->dispose(object);
}

JagAstgoal *jagast_goal_new_1() {
	JagAstgoal *instance = g_object_new(JAGAST_TYPE_GOAL, NULL);
	return instance;
}
