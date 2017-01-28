#ifndef [%PRE%]TOKEN_H_
#define [%PRE%]TOKEN_H_

#include "[%pre%]vector.h"

typedef struct s_[%Pre%]Token       [%Pre%]Token;
typedef struct s_[%Pre%]TokenClass  [%Pre%]TokenClass;


struct s_[%Pre%]Token {
[%if-set:debug%]
	const char *symbol_text;
[%end-if:debug%]
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
	[%pre%]boolean used_by_parser; // = false;

	[%pre%]boolean is_error;


	int left, right, row;
	void *value;
};


void [%pre%]_token_free([%Pre%]Token *token);

[%Pre%]Token *[%pre%]_token_new(int symbol, int left, int right, int row, void *val);
[%Pre%]Token *[%pre%]_token_new_symbol_value(int symbol, void *val);
[%Pre%]Token *[%pre%]_token_new_symbol_pos(int symbol, int left, int right, int row);
[%Pre%]Token *[%pre%]_token_new_symbol(int sym_num);
[%Pre%]Token *[%pre%]_token_new_symbol_state(int sym_num, int state);

[%if-set:debug%]
void [%pre%]_token_dump([%Pre%]Token *token);
[%end-if:debug%]

#endif /* [%PRE%]TOKEN_H_ */
