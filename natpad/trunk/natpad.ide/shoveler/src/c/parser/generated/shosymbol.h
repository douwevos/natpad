#ifndef SHOSYMBOL_H_
#define SHOSYMBOL_H_

#define SHO_SYMBOL_INFO  -1

/* terminals */
#define SHO_SYMBOL_TERM_EOF                0
#define SHO_SYMBOL_TERM_ERROR              1
#define SHO_SYMBOL_TERM_LBRACE             2
#define SHO_SYMBOL_TERM_RBRACE             3
#define SHO_SYMBOL_TERM_COMMA              4
#define SHO_SYMBOL_TERM_NIL                5
#define SHO_SYMBOL_TERM_SEMICOLON          6
#define SHO_SYMBOL_TERM_LPARREN            7
#define SHO_SYMBOL_TERM_RPARREN            8
#define SHO_SYMBOL_TERM_ID                 9
#define SHO_SYMBOL_TERM_ASSIGN_VALUE_EOL   10
#define SHO_SYMBOL_TERM_STRING             11
#define SHO_SYMBOL_TERM_NUMBER             12
#define SHO_SYMBOL_TERMINAL_COUNT 13

  /* non terminals */
#define SHO_SYMBOL_NONTERM_$START               0
#define SHO_SYMBOL_NONTERM_VALUE                1
#define SHO_SYMBOL_NONTERM_VALUES               2
#define SHO_SYMBOL_NONTERM_LIST_VALUE           3
#define SHO_SYMBOL_NONTERM_BLOCK_VALUE          4
#define SHO_SYMBOL_NONTERM_REFERENCE_BLOCK      5
#define SHO_SYMBOL_NONTERM_BLOCK_BODY           6
#define SHO_SYMBOL_NONTERM_SHT_BLOCK            7
#define SHO_SYMBOL_NONTERM_SHT_FILE             8
#define SHO_SYMBOL_NONTERM_TEMPLATE_DEF         9
#define SHO_SYMBOL_NONTERM_TEMPLATE_ARG         10
#define SHO_SYMBOL_NONTERM_TEMPLATE_ARGUMENTS   11
#define SHO_SYMBOL_NONTERM_TEMPLATE_CALL        12
#define SHO_SYMBOL_NONTERM_SIMPLE_REFERENCE     13
#define SHO_SYMBOL_NONTERM_BLOCK_REFERENCE      14
#define SHO_SYMBOL_NONTERM_PROPERTY_REFERENCE   15
#define SHO_SYMBOL_NONTERM_REFERENCE            16
#define SHO_SYMBOL_NONTERM_SIMPLE_VALUE         17
#define SHO_SYMBOL_NONTERMINAL_COUNT 18
const char *sho_symbol_terminal_as_string(int termIndex);
const char *sho_symbol_non_terminal_as_string(int termIndex);
#endif /* SHOSYMBOL_H_ */
