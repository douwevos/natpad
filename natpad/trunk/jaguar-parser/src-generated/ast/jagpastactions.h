/* This file was generated by Gross-CC on 2017.04.13 n. Chr. at 15:51:33 MESZ */
#ifndef JAGPAST_ACTIONS_H_
#define JAGPAST_ACTIONS_H_

#include <grossruntime.h>
#include "jagpcompilationunit.h"

G_BEGIN_DECLS

#define JAGP_TYPE_AST_ACTIONS              (jagp_ast_actions_get_type())
#define JAGP_AST_ACTIONS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_ast_actions_get_type(), JagPAstActions))
#define JAGP_AST_ACTIONS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_AST_ACTIONS, JagPAstActionsClass))
#define JAGP_IS_AST_ACTIONS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_AST_ACTIONS))
#define JAGP_IS_AST_ACTIONS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_AST_ACTIONS))
#define JAGP_AST_ACTIONS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_AST_ACTIONS, JagPAstActionsClass))


typedef struct _JagPAstActions               JagPAstActions;
typedef struct _JagPAstActionsClass          JagPAstActionsClass;


struct _JagPAstActions {
	GObject parent;
};

struct _JagPAstActionsClass {
	GObjectClass parent_class;
};


GType jagp_ast_actions_get_type();

JagPAstActions *jagp_ast_actions_new();

G_END_DECLS

#endif /* JAGPAST_ACTIONS_H_ */
