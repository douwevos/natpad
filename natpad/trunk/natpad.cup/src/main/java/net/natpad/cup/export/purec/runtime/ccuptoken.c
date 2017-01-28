#include "[%pre%]token.h"
#include "[%pre%]vector.h"
#include <stdlib.h>
#include <stdio.h>


void [%pre%]_token_free([%Pre%]Token *token) {
	if (token) {
		free(token);
	}
}



[%Pre%]Token *[%pre%]_token_new(int symbol, int left, int right, int row, void *val) {
	[%Pre%]Token *result = ([%Pre%]Token *) malloc(sizeof([%Pre%]Token));
	[%if-set:debug%]result->symbol_text = ""; [%end-if:debug%]
	result->sym = symbol;
	result->parse_state = -1;
	result->left = left;
	result->right = right;
	result->row = row;
	result->used_by_parser = [%PRE%]FALSE;
	result->value = val;
	result->is_error = [%PRE%]FALSE;
	return result;
}

[%Pre%]Token *[%pre%]_token_new_symbol_value(int symbol, void *val) {
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

[%if-set:debug%]
void [%pre%]_token_dump([%Pre%]Token *token) {
	printf("[%Pre%]Token[ %s, sym=%d, parse_state=%d, is_error=%d]\n", token->symbol_text, token->sym, token->parse_state, token->is_error);
}
[%end-if:debug%]

