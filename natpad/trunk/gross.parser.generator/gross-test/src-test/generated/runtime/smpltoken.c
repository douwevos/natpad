#include "smpltoken.h"

#include <logging/catlogdefs.h>
//define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SmplToken"
#include <logging/catlog.h>

G_DEFINE_TYPE(SmplToken, smpl_token, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void smpl_token_class_init(SmplTokenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void smpl_token_init(SmplToken *obj) {
}

static void _dispose(GObject *object) {
	SmplToken *instance = SMPL_TOKEN(object);
	cat_unref_ptr(instance->value);
}


SmplToken *smpl_token_new_full(int symbol, gboolean is_terminal, int left, int left_row, int right, int right_row, GObject *val) {
	SmplToken *result = g_object_new(SMPL_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	
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

SmplToken *smpl_token_new_full_ext(int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	SmplToken *result = g_object_new(SMPL_TYPE_TOKEN, NULL);
	cat_ref_anounce(result);
	
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

SmplToken *smpl_token_new(int symbol, int left, int right, int row, GObject *val) {
	return smpl_token_new_full(symbol, FALSE, left, row, right, row, val);
}


SmplToken *smpl_token_new_terminal(int symbol, int left, int right, int row, GObject *val) {
	return smpl_token_new_full(symbol, TRUE, left, row, right, row, val);
}

SmplToken *smpl_token_new_symbol_value(int symbol, GObject *val) {
	return smpl_token_new(symbol, -1, -1, -1, val);
}

SmplToken *smpl_token_new_symbol_pos(int symbol, int left, int right, int row) {
	return smpl_token_new(symbol, left, right, row, NULL);
}

SmplToken *smpl_token_new_symbol(int symbol) {
	return smpl_token_new(symbol, -1, -1, -1, NULL);
}

SmplToken *smpl_token_new_symbol_state(int symbol, int state) {
	SmplToken *result = smpl_token_new(symbol, -1, -1, -1, NULL);
	result->parse_state = state;
	return result;
}



