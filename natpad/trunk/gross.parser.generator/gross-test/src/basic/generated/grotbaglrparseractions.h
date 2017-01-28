#ifndef GROTBAGLR_PARSER_ACTIONS_H_
#define GROTBAGLR_PARSER_ACTIONS_H_

#include <grossglibcruntime.h>

GroContent *grotba_glr_parser_actions_run_action(GroGlrParserContext *context, int action_id, GroContent *content, int lhs, int rhs_count);

#endif /* GROTBAGLR_PARSER_ACTIONS_H_ */
