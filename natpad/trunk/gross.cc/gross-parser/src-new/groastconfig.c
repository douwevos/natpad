#include "groastconfig.h"

const int groast_config_nr_of_symbols = 21;
static const GroRunSymbolDescription symbols[] = {
		  { FALSE, "⟧", 0} 
		, { FALSE, "⊗", 1} 
		, { TRUE, "◎", 2} 
		, { FALSE, "IDENTIFIER", 3} 
		, { FALSE, "LSQBRACK", 4} 
		, { FALSE, "RSQBRACK", 5} 
		, { FALSE, "LCUBRACK", 6} 
		, { FALSE, "RCUBRACK", 7} 
		, { FALSE, "LPAREN", 8} 
		, { FALSE, "RPAREN", 9} 
		, { FALSE, "TYPE", 10} 
		, { FALSE, "STRING", 11} 
		, { FALSE, "EXTENDS", 12} 
		, { FALSE, "SEMI", 13} 
		, { FALSE, "FORWARD", 14} 
		, { TRUE, "spec", 15} 
		, { TRUE, "ast_lines", 16} 
		, { TRUE, "ast_line", 17} 
		, { TRUE, "line_body", 18} 
		, { TRUE, "type_description", 19} 
		, { TRUE, "opt_extends", 20} 
};
const GroRunSymbolDescription *groast_config_symbols = symbols;

const int groast_config_nr_of_states = 18;
const char *groast_config_states_text =
				  "r15,1p1r16,1p3r16,2p2r17,2p5r18,1p6r17,4p4r19,4,2p7r20,2p8r19,4p7A0B17S3B15S4B3S2B16S1A1B17S5B3S2B0R"
				  "0,0A2B14S6B6S7A3B3R1,1B0R1,1A4B0EA5B3R2,2B0R2,2A6B3R3,3B0R3,3A7B10S10B18S8B19S9A8B7S11A9B7R4,4A10B11"
				  "S12A11B3R5,5B0R5,5A12B20S15B12S13B13S14A13B11S16A14B7R6,6A15B13S17A16B13R7,7A17B7R8,6"
			;

