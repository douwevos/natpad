#ifndef SMPLTOKEN_H_
#define SMPLTOKEN_H_

#include <caterpillar.h>


G_BEGIN_DECLS

#define SMPL_TYPE_TOKEN            (smpl_token_get_type())
#define SMPL_TOKEN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_token_get_type(), SmplToken))
#define SMPL_TOKEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_TOKEN, SmplTokenClass))
#define SMPL_IS_TOKEN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_TOKEN))
#define SMPL_IS_TOKEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_TOKEN))
#define SMPL_TOKEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_TOKEN, SmplTokenClass))

typedef struct _SmplToken       SmplToken;
typedef struct _SmplTokenClass  SmplTokenClass;


struct _SmplToken {
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


struct _SmplTokenClass {
	GObjectClass parent_class;
};


GType smpl_token_get_type(void);

SmplToken *smpl_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val);

SmplToken *smpl_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


SmplToken *smpl_token_new(int symbol, int left, int right, int row, GObject *val);
SmplToken *smpl_token_new_terminal(int symbol, int left, int right, int row, GObject *val);
SmplToken *smpl_token_new_symbol_value(int symbol, GObject *val);
SmplToken *smpl_token_new_symbol_pos(int symbol, int left, int right, int row);
SmplToken *smpl_token_new_symbol(int sym_num);
SmplToken *smpl_token_new_symbol_state(int sym_num, int state);





G_END_DECLS


#endif /* SMPLTOKEN_H_ */
