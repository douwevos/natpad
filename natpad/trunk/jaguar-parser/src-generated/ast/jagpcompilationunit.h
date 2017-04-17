/* This file was generated by Gross-CC on 2017.04.13 n. Chr. at 15:51:33 MESZ */
#ifndef JAGPCOMPILATION_UNIT_H_
#define JAGPCOMPILATION_UNIT_H_

#include "grorunfork.h"

G_BEGIN_DECLS

#define JAGP_TYPE_COMPILATION_UNIT              (jagp_compilation_unit_get_type())
#define JAGP_COMPILATION_UNIT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_compilation_unit_get_type(), JagPCompilationUnit))
#define JAGP_COMPILATION_UNIT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_COMPILATION_UNIT, JagPCompilationUnitClass))
#define JAGP_IS_COMPILATION_UNIT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_COMPILATION_UNIT))
#define JAGP_IS_COMPILATION_UNIT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_COMPILATION_UNIT))
#define JAGP_COMPILATION_UNIT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_COMPILATION_UNIT, JagPCompilationUnitClass))


typedef struct _JagPCompilationUnit               JagPCompilationUnit;
typedef struct _JagPCompilationUnitClass          JagPCompilationUnitClass;


struct _JagPCompilationUnit {
	GObject parent;
};

struct _JagPCompilationUnitClass {
	GObjectClass parent_class;
};


GType jagp_compilation_unit_get_type();

JagPCompilationUnit *jagp_compilation_unit_new_1();

G_END_DECLS

#endif /* JAGPCOMPILATION_UNIT_H_ */