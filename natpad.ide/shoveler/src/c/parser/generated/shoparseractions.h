#ifndef SHOPARSER_ACTIONS_H_
#define SHOPARSER_ACTIONS_H_

#include <caterpillar.h>
#include "runtime/shotoken.h"
#include "runtime/shoparsercontext.h"

G_BEGIN_DECLS

#define SHO_TYPE_PARSER_ACTIONS            (sho_parser_actions_get_type())
#define SHO_PARSER_ACTIONS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_parser_actions_get_type(), ShoParserActions))
#define SHO_PARSER_ACTIONS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_PARSER_ACTIONS, ShoParserActionsClass))
#define SHO_IS_PARSER_ACTIONS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_PARSER_ACTIONS))
#define SHO_IS_PARSER_ACTIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_PARSER_ACTIONS))
#define SHO_PARSER_ACTIONS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_PARSER_ACTIONS, ShoParserActionsClass))

typedef struct _ShoParserActions       ShoParserActions;
typedef struct _ShoParserActionsClass  ShoParserActionsClass;

struct _ShoParserActions {
	GObject parent;
};

struct _ShoParserActionsClass {
	GObjectClass parent_class;
	ShoToken *(*run_action)(ShoParserActions *parser_actions, ShoParserContext *parser_context, int cup_action_id);
};

GType sho_parser_actions_get_type(void);

ShoParserActions *sho_parser_actions_new();

ShoToken *sho_parser_actions_run_action(ShoParserActions *parser_actions, ShoParserContext *parser_context, int cup_action_id);

G_END_DECLS

#endif /* SHOPARSER_ACTIONS_H_ */
