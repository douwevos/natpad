#include "jagsymbol.h"

static const char *jag_symbol_term_strings[] = {
	"EOF",
	"error",
	"ABSTRACT",
	"DO",
	"IMPLEMENTS",
	"PACKAGE",
	"THROW",
	"BOOLEAN",
	"DOUBLE",
	"IMPORT",
	"PRIVATE",
	"THROWS",
	"BREAK",
	"ELSE",
	"PROTECTED",
	"TRANSIENT",
	"BYTE",
	"SHORT",
	"INT",
	"LONG",
	"CHAR",
	"EXTENDS",
	"INSTANCEOF",
	"PUBLIC",
	"TRY",
	"CASE",
	"FINAL",
	"FINALLY",
	"INTERFACE",
	"RETURN",
	"VOID",
	"CATCH",
	"FLOAT",
	"VOLATILE",
	"FOR",
	"NATIVE",
	"STATIC",
	"WHILE",
	"CLASS",
	"NEW",
	"SUPER",
	"CONST",
	"NULL",
	"SWITCH",
	"CONTINUE",
	"GOTO",
	"SYNCHRONIZED",
	"DEFAULT",
	"IF",
	"THIS",
	"LPAREN",
	"RPAREN",
	"LBRACE",
	"RBRACE",
	"LBRACK",
	"RBRACK",
	"IDENTIFIER",
	"DOT",
	"COMMA",
	"COLON",
	"SEMICOLON",
	"STRING_LITERAL",
	"BOOLEAN_LITERAL",
	"QUESTION",
	"OP_MUL",
	"OP_DIV",
	"OP_SUB",
	"OP_ADD",
	"OP_AND",
	"OP_OR",
	"OP_XOR",
	"OP_NOT",
	"OP_NEG",
	"OP_LAND",
	"OP_INC",
	"OP_DEC",
	"OP_MOD",
	"OP_LOR",
	"OP_SHL",
	"OP_SHR",
	"OP_SHRR",
	"OP_EQ",
	"OP_NE",
	"OP_GE",
	"OP_GT",
	"OP_LE",
	"OP_LT",
	"ASS_MUL",
	"ASS_DIV",
	"ASS_MOD",
	"ASS_ADD",
	"ASS_SUB",
	"ASS_SHL",
	"ASS_SHR",
	"ASS_SHRR",
	"ASS_AND",
	"ASS_XOR",
	"ASS_OR",
	"ASS_IS",
	"AT",
	"INTEGER_LITERAL",
	"FLOATING_POINT_LITERAL",
	"CHARACTER_LITERAL",
	"STRICTFP",
	"ASSERT",
	"ELLIPSIS",
	"ENUM"
};

static const char *jag_symbol_non_term_strings[] = {
	"$START",
	"goal",
	"literal",
	"type",
	"primitive_type",
	"numeric_type",
	"integral_type",
	"floating_point_type",
	"reference_type",
	"class_or_interface_type",
	"class_type",
	"interface_type",
	"array_type",
	"simple_name",
	"name",
	"qualified_name",
	"compilation_unit",
	"package_compilation_unit",
	"simple_compilation_unit",
	"importing_compilation_unit",
	"package_declaration",
	"import_declarations_opt",
	"import_declarations",
	"type_declarations_opt",
	"type_declarations",
	"import_declaration",
	"single_type_import_declaration",
	"type_import_on_demand_declaration",
	"static_single_type_import_declaration",
	"static_type_import_on_demand_declaration",
	"type_declaration",
	"modifiers",
	"modifiers_opt",
	"adhoc_modifier",
	"mixed_modifiers",
	"mixed_modifiers_at",
	"modifiers_at",
	"class_declaration",
	"class_header",
	"super_ext",
	"super_opt",
	"interfaces",
	"interfaces_opt",
	"interface_type_list",
	"class_body",
	"class_body_opt",
	"class_body_declarations",
	"class_body_declarations_opt",
	"class_body_declaration",
	"class_member_declaration",
	"enum_declaration",
	"enum_body",
	"enum_constants_opt",
	"enum_constants",
	"enum_constant",
	"enum_arguments_opt",
	"enum_body_declarations_opt",
	"field_declaration",
	"variable_declarators",
	"variable_declarator",
	"variable_declarator_id",
	"variable_initializer",
	"method_declaration",
	"method_header",
	"method_declarator",
	"formal_parameter_list_opt",
	"formal_parameter_list",
	"formal_parameter",
	"formal_parameter_modifiers_opt",
	"formal_parameter_modifiers",
	"formal_parameter_modifier",
	"throws_opt",
	"throws",
	"class_type_list",
	"method_body",
	"static_initializer",
	"constructor_declaration",
	"constructor_declarator",
	"constructor_body",
	"explicit_constructor_invocation",
	"interface_declaration",
	"normal_interface_declaration",
	"annotation_type_declaration",
	"extends_interfaces_opt",
	"extends_interfaces",
	"interface_body",
	"interface_member_declarations_opt",
	"interface_member_declarations",
	"interface_member_declaration",
	"constant_declaration",
	"abstract_method_declaration",
	"array_initializer",
	"variable_initializers",
	"block",
	"block_statements_opt",
	"block_statements",
	"block_statement",
	"local_variable_declaration_statement",
	"local_variable_declaration",
	"statement",
	"statement_no_short_if",
	"statement_without_trailing_substatement",
	"empty_statement",
	"labeled_statement",
	"labeled_statement_no_short_if",
	"expression_statement",
	"statement_expression",
	"if_then_statement",
	"if_then_else_statement",
	"if_then_else_statement_no_short_if",
	"switch_statement",
	"switch_block",
	"switch_block_statement_groups",
	"switch_block_statement_group",
	"switch_labels",
	"switch_label",
	"while_statement",
	"while_statement_no_short_if",
	"do_statement",
	"foreach_statement",
	"foreach_statement_no_short_if",
	"for_statement",
	"for_statement_no_short_if",
	"for_init_opt",
	"for_init",
	"for_update_opt",
	"for_update",
	"statement_expression_list",
	"identifier_opt",
	"break_statement",
	"continue_statement",
	"return_statement",
	"throw_statement",
	"synchronized_statement",
	"try_statement",
	"catches_opt",
	"catches",
	"catch_clause",
	"finally",
	"assert_statement",
	"primary",
	"primary_no_new_array",
	"class_instance_creation_expression",
	"argument_list_opt",
	"argument_list",
	"array_creation_init",
	"array_creation_uninit",
	"dim_exprs",
	"dim_expr",
	"dims_opt",
	"dims",
	"field_access",
	"method_invocation",
	"array_access",
	"postfix_expression",
	"postincrement_expression",
	"postdecrement_expression",
	"unary_expression",
	"unary_expression_not_plus_minus",
	"preincrement_expression",
	"predecrement_expression",
	"cast_expression",
	"multiplicative_expression",
	"additive_expression",
	"shift_expression",
	"relational_expression",
	"equality_expression",
	"and_expression",
	"exclusive_or_expression",
	"inclusive_or_expression",
	"conditional_and_expression",
	"conditional_or_expression",
	"conditional_expression",
	"assignment_expression",
	"assignment",
	"assignment_operator",
	"expression_opt",
	"expression",
	"constant_expression",
	"class_or_interface",
	"type_variable",
	"type_arguments",
	"type_arguments_opt",
	"type_argument_list",
	"type_argument_list_1",
	"reference_type_1",
	"type_argument_list_2",
	"reference_type_2",
	"type_argument_list_3",
	"reference_type_3",
	"type_parameters",
	"type_parameters_opt",
	"type_parameter",
	"type_parameter_list",
	"type_parameter_1",
	"type_parameter_list_1",
	"type_bound",
	"type_bound_opt",
	"type_bound_1",
	"additional_bound_list",
	"additional_bound_list_opt",
	"additional_bound_list_1",
	"additional_bound",
	"additional_bound_1",
	"wildcard",
	"wildcard_1",
	"wildcard_2",
	"wildcard_3",
	"type_argument_2",
	"type_argument_3",
	"type_argument",
	"type_argument_1",
	"instanceof_expression",
	"postfix_expression_nn",
	"unary_expression_nn",
	"unary_expression_not_plus_minus_nn",
	"multiplicative_expression_nn",
	"additive_expression_nn",
	"shift_expression_nn",
	"relational_expression_nn",
	"instanceof_expression_nn",
	"equality_expression_nn",
	"and_expression_nn",
	"exclusive_or_expression_nn",
	"inclusive_or_expression_nn",
	"conditional_and_expression_nn",
	"conditional_or_expression_nn",
	"conditional_expression_nn",
	"assignment_expression_nn",
	"expression_nn",
	"annotations_opt",
	"annotations",
	"annotations_at",
	"annotation",
	"annotation_body",
	"normal_annotation_body",
	"marker_annotation_body",
	"single_element_annotation_body",
	"annotation_type_body",
	"annotation_type_element_declarations",
	"annotation_type_element_declarations_opt",
	"annotation_type_element_declaration",
	"default_value_opt",
	"default_value",
	"element_value_pairs_opt",
	"element_value_pairs",
	"element_value_pair",
	"element_values_opt",
	"element_values",
	"element_value",
	"element_value_array_initializer"
};

const char *jag_symbol_terminal_as_string(int termIndex) {
	if ((termIndex>=0) && (termIndex<JAG_SYMBOL_TERMINAL_COUNT)) {
		return jag_symbol_term_strings[termIndex];
	}
	return "<index-out-of-range>";
}

const char *jag_symbol_non_terminal_as_string(int nonTermIndex) {
	if ((nonTermIndex>=0) && (nonTermIndex<JAG_SYMBOL_NONTERMINAL_COUNT)) {
		return jag_symbol_non_term_strings[nonTermIndex];
	}
	return "<index-out-of-range>";
}

