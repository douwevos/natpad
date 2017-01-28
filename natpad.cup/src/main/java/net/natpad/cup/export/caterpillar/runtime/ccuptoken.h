#ifndef [%PRE%]TOKEN_H_
#define [%PRE%]TOKEN_H_

#include <caterpillar.h>
[%if-set:symbol.info%]
#include "../[%pre%]symbol.h"
[%end-if:symbol.info%]

G_BEGIN_DECLS

#define [%PRE%]_TYPE_TOKEN            ([%pre%]_token_get_type())
#define [%PRE%]_TOKEN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), [%pre%]_token_get_type(), [%Pre%]Token))
#define [%PRE%]_TOKEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), [%PRE%]_TYPE_TOKEN, [%Pre%]TokenClass))
#define [%PRE%]_IS_TOKEN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), [%PRE%]_TYPE_TOKEN))
#define [%PRE%]_IS_TOKEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), [%PRE%]_TYPE_TOKEN))
#define [%PRE%]_TOKEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), [%PRE%]_TYPE_TOKEN, [%Pre%]TokenClass))

typedef struct _[%Pre%]Token       [%Pre%]Token;
typedef struct _[%Pre%]TokenClass  [%Pre%]TokenClass;


struct _[%Pre%]Token {
	GObject parent;
[%if-set:symbol.info%]
	const char *symbol_text;
[%end-if:symbol.info%]
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


struct _[%Pre%]TokenClass {
	GObjectClass parent_class;
};


GType [%pre%]_token_get_type(void);

[%Pre%]Token *[%pre%]_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val);

[%Pre%]Token *[%pre%]_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


[%Pre%]Token *[%pre%]_token_new(int symbol, int left, int right, int row, GObject *val);
[%Pre%]Token *[%pre%]_token_new_terminal(int symbol, int left, int right, int row, GObject *val);
[%Pre%]Token *[%pre%]_token_new_symbol_value(int symbol, GObject *val);
[%Pre%]Token *[%pre%]_token_new_symbol_pos(int symbol, int left, int right, int row);
[%Pre%]Token *[%pre%]_token_new_symbol(int sym_num);
[%Pre%]Token *[%pre%]_token_new_symbol_state(int sym_num, int state);

[%if-set:symbol.info%]
void [%pre%]_token_dump([%Pre%]Token *token);
[%if-set:output.non.terminals%]
const char*[%pre%]_token_symbol_as_text([%Pre%]Token *token);
 [%end-if:output.non.terminals%]
[%end-if:symbol.info%]



G_END_DECLS


#endif /* [%PRE%]TOKEN_H_ */
