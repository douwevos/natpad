#ifndef SMPLPARSER_H_
#define SMPLPARSER_H_

#include <caterpillar.h>
#include "smplparseractions.h"
#include "runtime/smpl2darray.h"
#include "runtime/smplparserbase.h"

G_BEGIN_DECLS

#define SMPL_TYPE_PARSER            (smpl_parser_get_type())
#define SMPL_PARSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_parser_get_type(), SmplParser))
#define SMPL_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_PARSER, SmplParserClass))
#define SMPL_IS_PARSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_PARSER))
#define SMPL_IS_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_PARSER))
#define SMPL_PARSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_PARSER, SmplParserClass))

typedef struct _SmplParser       SmplParser;
typedef struct _SmplParserClass  SmplParserClass;


struct _SmplParser {
	SmplParserBase parent;
	SmplParserActions *parser_actions;
};

struct _SmplParserClass {
	SmplParserBaseClass parent_class;
};

GType smpl_parser_get_type(void);

SmplParser *smpl_parser_new(SmplIScanner *scanner);

G_END_DECLS

#endif /* SMPLPARSER_H_ */
