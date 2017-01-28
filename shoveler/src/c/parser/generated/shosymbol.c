#include "shosymbol.h"

static const char *sho_symbol_term_strings[] = {
	"EOF",
	"error",
	"LBRACE",
	"RBRACE",
	"COMMA",
	"NIL",
	"SEMICOLON",
	"LPARREN",
	"RPARREN",
	"ID",
	"ASSIGN_VALUE_EOL",
	"STRING",
	"NUMBER"
};

static const char *sho_symbol_non_term_strings[] = {
	"$START",
	"value",
	"values",
	"list_value",
	"block_value",
	"reference_block",
	"block_body",
	"sht_block",
	"sht_file",
	"template_def",
	"template_arg",
	"template_arguments",
	"template_call",
	"simple_reference",
	"block_reference",
	"property_reference",
	"reference",
	"simple_value"
};

const char *sho_symbol_terminal_as_string(int termIndex) {
	if ((termIndex>=0) && (termIndex<SHO_SYMBOL_TERMINAL_COUNT)) {
		return sho_symbol_term_strings[termIndex];
	}
	return "<index-out-of-range>";
}

const char *sho_symbol_non_terminal_as_string(int nonTermIndex) {
	if ((nonTermIndex>=0) && (nonTermIndex<SHO_SYMBOL_NONTERMINAL_COUNT)) {
		return sho_symbol_non_term_strings[nonTermIndex];
	}
	return "<index-out-of-range>";
}

