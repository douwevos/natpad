#include "[%pre%]token.h"

#include <logging/catlogdefs.h>
//define CAT_LOG_LEVEL [%if-set:debug%]CAT_LOG_DETAIL[%end-if:debug%][%if-not-set:debug%]CAT_LOG_WARN[%end-if:debug%]
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "[%Pre%]Token"
#include <logging/catlog.h>

G_DEFINE_TYPE([%Pre%]Token, [%pre%]_token, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void [%pre%]_token_class_init([%Pre%]TokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void [%pre%]_token_init([%Pre%]Token *obj) {
}

static void _dispose(GObject *object) {
	[%Pre%]Token *instance = [%PRE%]_TOKEN(object);
	cat_unref_ptr(instance->value);
}


[%Pre%]Token *[%pre%]_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val) {
	[%Pre%]Token *result = g_object_new([%PRE%]_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	[%if-set:symbol.info%]result->symbol_text = ""; [%end-if:symbol.info%]
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

[%Pre%]Token *[%pre%]_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	[%Pre%]Token *result = g_object_new([%PRE%]_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	[%if-set:symbol.info%]result->symbol_text = ""; [%end-if:symbol.info%]
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

[%Pre%]Token *[%pre%]_token_new(int symbol, int left, int right, int row, GObject *val) {
	return [%pre%]_token_new_full(symbol, FALSE, left, row, right, row, val);
}


[%Pre%]Token *[%pre%]_token_new_terminal(int symbol, int left, int right, int row, GObject *val) {
	return [%pre%]_token_new_full(symbol, TRUE, left, row, right, row, val);
}

[%Pre%]Token *[%pre%]_token_new_symbol_value(int symbol, GObject *val) {
	return [%pre%]_token_new(symbol, -1, -1, -1, val);
}

[%Pre%]Token *[%pre%]_token_new_symbol_pos(int symbol, int left, int right, int row) {
	return [%pre%]_token_new(symbol, left, right, row, NULL);
}

[%Pre%]Token *[%pre%]_token_new_symbol(int symbol) {
	return [%pre%]_token_new(symbol, -1, -1, -1, NULL);
}

[%Pre%]Token *[%pre%]_token_new_symbol_state(int symbol, int state) {
	[%Pre%]Token *result = [%pre%]_token_new(symbol, -1, -1, -1, NULL);
	result->parse_state = state;
	return result;
}

[%if-set:symbol.info%]
void [%pre%]_token_dump([%Pre%]Token *token) {
[%if-not-set:output.non.terminals%]
	cat_log_debug("[%Pre%]Token[ %s, sym=%d, parse_state=%d, is_error=%d]", token->symbol_text, token->sym, token->parse_state, token->is_error);
[%end-if:output.non.terminals%]
 [%if-set:output.non.terminals%]
 	cat_log_debug("[%Pre%]Token[ %s, sym=%d, parse_state=%d, is_error=%d, %s]", token->symbol_text, token->sym, token->parse_state, token->is_error, [%pre%]_token_symbol_as_text(token));
 [%end-if:output.non.terminals%]
}

[%if-set:output.non.terminals%]
const char*[%pre%]_token_symbol_as_text([%Pre%]Token *token) {
	if (token->is_terminal) {
		return [%pre%]_[%symbol.name.lower%]_terminal_as_string(token->sym);
	} else {
		return [%pre%]_[%symbol.name.lower%]_non_terminal_as_string(token->sym);
	}
}
[%end-if:output.non.terminals%]
[%end-if:symbol.info%]

