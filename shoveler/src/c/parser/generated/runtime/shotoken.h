#ifndef SHOTOKEN_H_
#define SHOTOKEN_H_

#include <caterpillar.h>

#include "../shosymbol.h"


G_BEGIN_DECLS

#define SHO_TYPE_TOKEN            (sho_token_get_type())
#define SHO_TOKEN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_token_get_type(), ShoToken))
#define SHO_TOKEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_TOKEN, ShoTokenClass))
#define SHO_IS_TOKEN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_TOKEN))
#define SHO_IS_TOKEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_TOKEN))
#define SHO_TOKEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_TOKEN, ShoTokenClass))

typedef struct _ShoToken       ShoToken;
typedef struct _ShoTokenClass  ShoTokenClass;


struct _ShoToken {
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


struct _ShoTokenClass {
	GObjectClass parent_class;
};


GType sho_token_get_type(void);

ShoToken *sho_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val);

ShoToken *sho_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


ShoToken *sho_token_new(int symbol, int left, int right, int row, GObject *val);
ShoToken *sho_token_new_terminal(int symbol, int left, int right, int row, GObject *val);
ShoToken *sho_token_new_symbol_value(int symbol, GObject *val);
ShoToken *sho_token_new_symbol_pos(int symbol, int left, int right, int row);
ShoToken *sho_token_new_symbol(int sym_num);
ShoToken *sho_token_new_symbol_state(int sym_num, int state);


void sho_token_dump(ShoToken *token);

const char*sho_token_symbol_as_text(ShoToken *token);
 




G_END_DECLS


#endif /* SHOTOKEN_H_ */
