#ifndef SMPLPARSER_ACTIONS_H_
#define SMPLPARSER_ACTIONS_H_

#include <caterpillar.h>
#include "runtime/smpltoken.h"
#include "runtime/smplparsercontext.h"

G_BEGIN_DECLS

#define SMPL_TYPE_PARSER_ACTIONS            (smpl_parser_actions_get_type())
#define SMPL_PARSER_ACTIONS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_parser_actions_get_type(), SmplParserActions))
#define SMPL_PARSER_ACTIONS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_PARSER_ACTIONS, SmplParserActionsClass))
#define SMPL_IS_PARSER_ACTIONS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_PARSER_ACTIONS))
#define SMPL_IS_PARSER_ACTIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_PARSER_ACTIONS))
#define SMPL_PARSER_ACTIONS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_PARSER_ACTIONS, SmplParserActionsClass))

typedef struct _SmplParserActions       SmplParserActions;
typedef struct _SmplParserActionsClass  SmplParserActionsClass;

struct _SmplParserActions {
	GObject parent;
};

struct _SmplParserActionsClass {
	GObjectClass parent_class;
	SmplToken *(*run_action)(SmplParserActions *parser_actions, SmplParserContext *parser_context, int cup_action_id);
};

GType smpl_parser_actions_get_type(void);

SmplParserActions *smpl_parser_actions_new();

SmplToken *smpl_parser_actions_run_action(SmplParserActions *parser_actions, SmplParserContext *parser_context, int cup_action_id);

G_END_DECLS

#endif /* SMPLPARSER_ACTIONS_H_ */
