#include "smplsymbol.h"

static const char *smpl_term_strings[] = {
	"EOF",
	"error",
	"DOT",
	"IDENTIFIER",
	"OPMUL",
	"OPDIV",
	"OPPLUS",
	"OPMINUS",
	"OPEQ",
	"OPNE",
	"ASSIGN",
	"LEFTPAREN",
	"RIGHTPAREN",
	"SEMICOLON"
};

static const char *smpl_non_term_strings[] = {
	"$START",
	"program",
	"statements",
	"statement",
	"assign_statement",
	"expr",
	"term",
	"fact",
	"prim",
	"qualified_name"
};

char *smpl_terminal_as_string(int termIndex) {
	return smpl_term_strings[termIndex];
}

char *smpl_non_terminal_as_string(int termIndex) {
	return smpl_non_term_strings[termIndex];
}

