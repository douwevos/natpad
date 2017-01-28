#include "grotoken.h"

G_DEFINE_TYPE(GroToken, gro_token, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void gro_token_class_init(GroTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void gro_token_init(GroToken *obj) {
}

static void _dispose(GObject *object) {
	GroToken *instance = GRO_TOKEN(object);
	if (instance->value) {
		g_object_unref(instance->value);
		instance->value = NULL;
	}
}


GroToken *gro_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val) {
	GroToken *result = g_object_new(GRO_TYPE_TOKEN, NULL);
	
//	gro_log("result=%p, sym=%d", result, symbol);

	result->is_terminal = is_terminal;
	result->sym = symbol;
	result->parse_state = -1;
	result->left = left;
	result->left_row = left_row;
	result->right = right;
	result->right_row = right_row;
	result->used_by_parser = FALSE;
	result->value = val==NULL ? NULL : g_object_ref(val);
	result->is_error = FALSE;
	return result;
}

GroToken *gro_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	GroToken *result = g_object_new(GRO_TYPE_TOKEN, NULL);
	
	result->is_terminal = is_terminal;
	result->sym = symbol;
	result->parse_state = parse_state;
	result->left = left;
	result->left_row = left_row;
	result->right = right;
	result->right_row = right_row;
	result->used_by_parser = used_by_parser;
	result->value = val==NULL ? NULL : g_object_ref(val);
	result->is_error = is_error;
	return result;
}

GroToken *gro_token_new(int symbol, int left, int right, int row, GObject *val) {
	return gro_token_new_full(symbol, FALSE, left, row, right, row, val);
}

GroToken *gro_token_new_minimal(int symbol, GObject *val) {
	return gro_token_new_full(symbol, FALSE, -1, -1, -1, -1, val);
}



GroToken *gro_token_new_terminal(int symbol, int left, int right, int row, GObject *val) {
	return gro_token_new_full(symbol, TRUE, left, row, right, row, val);
}

GroToken *gro_token_new_symbol_value(int symbol, GObject *val) {
	return gro_token_new(symbol, -1, -1, -1, val);
}

GroToken *gro_token_new_symbol_pos(int symbol, int left, int right, int row) {
	return gro_token_new(symbol, left, right, row, NULL);
}

GroToken *gro_token_new_symbol(int symbol) {
	return gro_token_new(symbol, -1, -1, -1, NULL);
}

GroToken *gro_token_new_symbol_state(int symbol, int state) {
	GroToken *result = gro_token_new(symbol, -1, -1, -1, NULL);
	result->parse_state = state;
	return result;
}



