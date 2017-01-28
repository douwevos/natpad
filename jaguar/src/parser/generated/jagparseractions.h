#ifndef JAGPARSER_ACTIONS_H_
#define JAGPARSER_ACTIONS_H_

#include <caterpillar.h>
#include "runtime/jagtoken.h"
#include "runtime/jagparsercontext.h"

G_BEGIN_DECLS

#define JAG_TYPE_PARSER_ACTIONS            (jag_parser_actions_get_type())
#define JAG_PARSER_ACTIONS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_parser_actions_get_type(), JagParserActions))
#define JAG_PARSER_ACTIONS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PARSER_ACTIONS, JagParserActionsClass))
#define JAG_IS_PARSER_ACTIONS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PARSER_ACTIONS))
#define JAG_IS_PARSER_ACTIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PARSER_ACTIONS))
#define JAG_PARSER_ACTIONS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PARSER_ACTIONS, JagParserActionsClass))

typedef struct _JagParserActions       JagParserActions;
typedef struct _JagParserActionsClass  JagParserActionsClass;

struct _JagParserActions {
	GObject parent;
};

struct _JagParserActionsClass {
	GObjectClass parent_class;
	JagToken *(*run_action)(JagParserActions *parser_actions, JagParserContext *parser_context, int cup_action_id);
};

GType jag_parser_actions_get_type(void);

JagParserActions *jag_parser_actions_new();

JagToken *jag_parser_actions_run_action(JagParserActions *parser_actions, JagParserContext *parser_context, int cup_action_id);

G_END_DECLS

#endif /* JAGPARSER_ACTIONS_H_ */
