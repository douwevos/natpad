#ifndef GROTOKEN_H_
#define GROTOKEN_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GRO_TYPE_TOKEN            (gro_token_get_type())
#define GRO_TOKEN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_token_get_type(), GroToken))
#define GRO_TOKEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_TOKEN, GroTokenClass))
#define GRO_IS_TOKEN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_TOKEN))
#define GRO_IS_TOKEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_TOKEN))
#define GRO_TOKEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_TOKEN, GroTokenClass))

typedef struct _GroToken       GroToken;
typedef struct _GroTokenClass  GroTokenClass;


struct _GroToken {
	GObject parent;

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


struct _GroTokenClass {
	GObjectClass parent_class;
};


GType gro_token_get_type(void);

GroToken *gro_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val);

GroToken *gro_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


GroToken *gro_token_new(int symbol, int left, int right, int row, GObject *val);
GroToken *gro_token_new_minimal(int symbol, GObject *val);
GroToken *gro_token_new_terminal(int symbol, int left, int right, int row, GObject *val);
GroToken *gro_token_new_symbol_value(int symbol, GObject *val);
GroToken *gro_token_new_symbol_pos(int symbol, int left, int right, int row);
GroToken *gro_token_new_symbol(int sym_num);
GroToken *gro_token_new_symbol_state(int sym_num, int state);





G_END_DECLS


#endif /* GROTOKEN_H_ */
