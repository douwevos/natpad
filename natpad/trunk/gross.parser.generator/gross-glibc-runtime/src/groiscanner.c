#include "groiscanner.h"

G_DEFINE_INTERFACE(GroIScanner, gro_iscanner, G_TYPE_OBJECT)

static GroToken *l_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	return gro_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
}

static void gro_iscanner_default_init(GroIScannerInterface *iface) {
	iface->createToken = l_create_token;
}

GroToken *gro_iscanner_next_token(GroIScanner *scanner) {
	  g_return_val_if_fail(GRO_IS_ISCANNER(scanner), NULL);
	  return GRO_ISCANNER_GET_INTERFACE(scanner)->next_token(scanner);
}


GroToken *gro_iscanner_create_token(GroIScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	  g_return_val_if_fail(GRO_IS_ISCANNER(scanner), NULL);
	  return GRO_ISCANNER_GET_INTERFACE(scanner)->createToken(scanner, symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
}
