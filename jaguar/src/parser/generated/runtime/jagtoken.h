#ifndef JAGTOKEN_H_
#define JAGTOKEN_H_

#include <caterpillar.h>

#include "../jagsymbol.h"


G_BEGIN_DECLS

#define JAG_TYPE_TOKEN            (jag_token_get_type())
#define JAG_TOKEN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_token_get_type(), JagToken))
#define JAG_TOKEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_TOKEN, JagTokenClass))
#define JAG_IS_TOKEN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_TOKEN))
#define JAG_IS_TOKEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_TOKEN))
#define JAG_TOKEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_TOKEN, JagTokenClass))

typedef struct _JagToken       JagToken;
typedef struct _JagTokenClass  JagTokenClass;


struct _JagToken {
	GObject parent;

	const char *symbol_text;

	/** The symbol number of the terminal or non terminal being represented */
	int sym;

	/** The parse state to be recorded on the parse stack with this symbol.
	 *  This field is for the convenience of the parser and shouldn't be
	 *  modified except by the parser.
	 */
	int parse_state;

	/** This allows us to catch some errors caused by scanners recycling
	 *  symbols.  For the use of the parser only. [CSA, 23-Jul-1999]
	 */
	gboolean used_by_parser; // = false;

	gboolean is_error;

	gboolean is_terminal;

	int left, left_row;
	int right, right_row;
	
	GObject *value;
};


struct _JagTokenClass {
	GObjectClass parent_class;
};


GType jag_token_get_type(void);

JagToken *jag_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val);

JagToken *jag_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


JagToken *jag_token_new(int symbol, int left, int right, int row, GObject *val);
JagToken *jag_token_new_terminal(int symbol, int left, int right, int row, GObject *val);
JagToken *jag_token_new_symbol_value(int symbol, GObject *val);
JagToken *jag_token_new_symbol_pos(int symbol, int left, int right, int row);
JagToken *jag_token_new_symbol(int sym_num);
JagToken *jag_token_new_symbol_state(int sym_num, int state);


void jag_token_dump(JagToken *token);

const char*jag_token_symbol_as_text(JagToken *token);

G_END_DECLS

#endif /* JAGTOKEN_H_ */
