#include "shotoken.h"

#include <logging/catlogdefs.h>
//define CAT_LOG_LEVEL CAT_LOG_DETAIL
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ShoToken"
#include <logging/catlog.h>

G_DEFINE_TYPE(ShoToken, sho_token, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void sho_token_class_init(ShoTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void sho_token_init(ShoToken *obj) {
}

static void _dispose(GObject *object) {
	ShoToken *instance = SHO_TOKEN(object);
	cat_unref_ptr(instance->value);
}


ShoToken *sho_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val) {
	ShoToken *result = g_object_new(SHO_TYPE_TOKEN, NULL);
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

ShoToken *sho_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	ShoToken *result = g_object_new(SHO_TYPE_TOKEN, NULL);
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

ShoToken *sho_token_new(int symbol, int left, int right, int row, GObject *val) {
	return sho_token_new_full(symbol, FALSE, left, row, right, row, val);
}


ShoToken *sho_token_new_terminal(int symbol, int left, int right, int row, GObject *val) {
	return sho_token_new_full(symbol, TRUE, left, row, right, row, val);
}

ShoToken *sho_token_new_symbol_value(int symbol, GObject *val) {
	return sho_token_new(symbol, -1, -1, -1, val);
}

ShoToken *sho_token_new_symbol_pos(int symbol, int left, int right, int row) {
	return sho_token_new(symbol, left, right, row, NULL);
}

ShoToken *sho_token_new_symbol(int symbol) {
	return sho_token_new(symbol, -1, -1, -1, NULL);
}

ShoToken *sho_token_new_symbol_state(int symbol, int state) {
	ShoToken *result = sho_token_new(symbol, -1, -1, -1, NULL);
	result->parse_state = state;
	return result;
}


void sho_token_dump(ShoToken *token) {

 
 	cat_log_debug("ShoToken[ %s, sym=%d, parse_state=%d, is_error=%d, %s]", token->symbol_text, token->sym, token->parse_state, token->is_error, sho_token_symbol_as_text(token));
 
}


const char*sho_token_symbol_as_text(ShoToken *token) {
	if (token->is_terminal) {
		return sho_symbol_terminal_as_string(token->sym);
	} else {
		return sho_symbol_non_terminal_as_string(token->sym);
	}
}



