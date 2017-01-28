#ifndef SHOLR_PARSER_H_
#define SHOLR_PARSER_H_

#include <caterpillar.h>
#include "shoparseractions.h"
#include "runtime/sho2darray.h"
#include "runtime/shoparserbase.h"

G_BEGIN_DECLS

#define SHO_TYPE_LR_PARSER            (sho_lr_parser_get_type())
#define SHO_LR_PARSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_lr_parser_get_type(), ShoLrParser))
#define SHO_LR_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_LR_PARSER, ShoLrParserClass))
#define SHO_IS_LR_PARSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_LR_PARSER))
#define SHO_IS_LR_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_LR_PARSER))
#define SHO_LR_PARSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_LR_PARSER, ShoLrParserClass))

typedef struct _ShoLrParser       ShoLrParser;
typedef struct _ShoLrParserClass  ShoLrParserClass;


struct _ShoLrParser {
	ShoParserBase parent;
	ShoParserActions *parser_actions;
};

struct _ShoLrParserClass {
	ShoParserBaseClass parent_class;
};

GType sho_lr_parser_get_type(void);

ShoLrParser *sho_lr_parser_new(ShoIScanner *scanner);

G_END_DECLS

#endif /* SHOLR_PARSER_H_ */
