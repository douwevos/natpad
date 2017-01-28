#include "groconfig.h"

const int gro_config_nr_of_symbols = 6;
static const GroRunSymbolDescription symbols[] = {
		  { FALSE, "⟧", 0} 
		, { FALSE, "⊗", 1} 
		, { TRUE, "◎", 2} 
		, { FALSE, "IDENTIFIER", 3} 
		, { FALSE, "CLASS", 4} 
		, { TRUE, "id_list", 5} 
};
const GroRunSymbolDescription *gro_config_symbols = symbols;

const int gro_config_nr_of_states = 4;
const char *gro_config_states_text =
				  "r5,2,0p1r5,2p1A0B5S1B3S2B0EA1B3S3B0EA2B3R0,-1B0R0,-1A3B3R1,-1B0R1,-1"
			;

