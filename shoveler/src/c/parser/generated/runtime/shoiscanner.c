#include "shoiscanner.h"

static ShoToken *l_create_token(ShoIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	return sho_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
}


static void sho_iscanner_interface_init (gpointer clazz) {
	ShoIScannerInterface *iface = (ShoIScannerInterface*) clazz;
	iface->createToken = l_create_token;
}

GType sho_iscanner_get_type (void) {
  static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof(ShoIScannerInterface),
				sho_iscanner_interface_init, /* base_init */
				NULL, /* base_finalize */
		};

		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE, g_intern_static_string("ShoIScanner"), &info, 0);
		g_once_init_leave(&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}



ShoToken *sho_iscanner_next_token(ShoIScanner *scanner) {
	  g_return_val_if_fail(SHO_IS_ISCANNER(scanner), NULL);
	  return SHO_ISCANNER_GET_INTERFACE(scanner)->next_token(scanner);
}


ShoToken *sho_iscanner_create_token(ShoIScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	  g_return_val_if_fail(SHO_IS_ISCANNER(scanner), NULL);
	  return SHO_ISCANNER_GET_INTERFACE(scanner)->createToken(scanner, symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
}