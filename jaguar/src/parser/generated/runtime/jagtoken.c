#include "jagtoken.h"

#include <logging/catlogdefs.h>
//define CAT_LOG_LEVEL CAT_LOG_DETAIL
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagToken"
#include <logging/catlog.h>

G_DEFINE_TYPE(JagToken, jag_token, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void jag_token_class_init(JagTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void jag_token_init(JagToken *obj) {
}

static void _dispose(GObject *object) {
	JagToken *instance = JAG_TOKEN(object);
	cat_unref_ptr(instance->value);
}


JagToken *jag_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val) {
	JagToken *result = g_object_new(JAG_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	result->symbol_text = ""; 
	result->is_terminal = is_terminal;
	result->sym = symbol;
	result->parse_state = -1;
	result->left = left;
	result->left_row = left_row;
	result->right = right;
	result->right_row = right_row;
	result->used_by_parser = FALSE;
	result->value = cat_ref_ptr(val);
	result->is_error = FALSE;
	return result;
}

JagToken *jag_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	JagToken *result = g_object_new(JAG_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	result->symbol_text = ""; 
	result->is_terminal = is_terminal;
	result->sym = symbol;
	result->parse_state = parse_state;
	result->left = left;
	result->left_row = left_row;
	result->right = right;
	result->right_row = right_row;
	result->used_by_parser = used_by_parser;
	result->value = cat_ref_ptr(val);
	result->is_error = is_error;
	return result;
}

JagToken *jag_token_new(int symbol, int left, int right, int row, GObject *val) {
	return jag_token_new_full(symbol, FALSE, left, row, right, row, val);
}


JagToken *jag_token_new_terminal(int symbol, int left, int right, int row, GObject *val) {
	return jag_token_new_full(symbol, TRUE, left, row, right, row, val);
}

JagToken *jag_token_new_symbol_value(int symbol, GObject *val) {
	return jag_token_new(symbol, -1, -1, -1, val);
}

JagToken *jag_token_new_symbol_pos(int symbol, int left, int right, int row) {
	return jag_token_new(symbol, left, right, row, NULL);
}

JagToken *jag_token_new_symbol(int symbol) {
	return jag_token_new(symbol, -1, -1, -1, NULL);
}

JagToken *jag_token_new_symbol_state(int symbol, int state) {
	JagToken *result = jag_token_new(symbol, -1, -1, -1, NULL);
	result->parse_state = state;
	return result;
}


void jag_token_dump(JagToken *token) {

 
 	cat_log_debug("JagToken[ %s, sym=%d, parse_state=%d, is_error=%d, %s]", token->symbol_text, token->sym, token->parse_state, token->is_error, jag_token_symbol_as_text(token));
 
}


const char*jag_token_symbol_as_text(JagToken *token) {
	if (token->is_terminal) {
		return jag_symbol_terminal_as_string(token->sym);
	} else {
		return jag_symbol_non_terminal_as_string(token->sym);
	}
}



