/* This file was generated by Gross-CC on 2017.04.13 n. Chr. at 15:51:33 MESZ */
#include "jagpcompilationunit.h"


G_DEFINE_TYPE_WITH_CODE(JagPCompilationUnit, jagp_compilation_unit, G_TYPE_OBJECT,
);

#define L_SAFE_UNREF_PTR(p) { void *ptr = p; p = NULL; if (ptr) { g_object_unref(ptr); } }
static G_GNUC_UNUSED void *L_SAFE_REF_PTR(void *p) { if (p) { g_object_ref(p); } return p; }

static void l_dispose(GObject *object);

static void jagp_compilation_unit_class_init(JagPCompilationUnitClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void jagp_compilation_unit_init(JagPCompilationUnit *instance) {
}

static void l_dispose(GObject *object) {
	G_OBJECT_CLASS(jagp_compilation_unit_parent_class)->dispose(object);
}

JagPCompilationUnit *jagp_compilation_unit_new_1() {
	JagPCompilationUnit *instance = g_object_new(JAGP_TYPE_COMPILATION_UNIT, NULL);
	return instance;
}

