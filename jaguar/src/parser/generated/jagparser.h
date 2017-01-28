#ifndef JAGPARSER_H_
#define JAGPARSER_H_

#include <caterpillar.h>
#include "jagparseractions.h"
#include "runtime/jag2darray.h"
#include "runtime/jagparserbase.h"

G_BEGIN_DECLS

#define JAG_TYPE_PARSER            (jag_parser_get_type())
#define JAG_PARSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_parser_get_type(), JagParser))
#define JAG_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PARSER, JagParserClass))
#define JAG_IS_PARSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PARSER))
#define JAG_IS_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PARSER))
#define JAG_PARSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PARSER, JagParserClass))

typedef struct _JagParser       JagParser;
typedef struct _JagParserClass  JagParserClass;


struct _JagParser {
	JagParserBase parent;
	JagParserActions *parser_actions;
};

struct _JagParserClass {
	JagParserBaseClass parent_class;
};

GType jag_parser_get_type(void);

JagParser *jag_parser_new(JagIScanner *scanner);

G_END_DECLS

#endif /* JAGPARSER_H_ */
