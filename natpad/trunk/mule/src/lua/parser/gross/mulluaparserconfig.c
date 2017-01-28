#include "mulluaparserconfig.h"

const int mullua_parser_config_nr_of_symbols = 83;
static const GroRunSymbolDescription symbols[] = {
		  { FALSE, "⟧", 0} 
		, { FALSE, "⊗", 1} 
		, { TRUE, "◎", 2} 
		, { FALSE, "LBRACK", 3} 
		, { FALSE, "RBRACK", 4} 
		, { FALSE, "LBRACE", 5} 
		, { FALSE, "RBRACE", 6} 
		, { FALSE, "KWAND", 7} 
		, { FALSE, "KWBREAK", 8} 
		, { FALSE, "KWDO", 9} 
		, { FALSE, "KWELSE", 10} 
		, { FALSE, "KWELSEIF", 11} 
		, { FALSE, "KWEND", 12} 
		, { FALSE, "KWFALSE", 13} 
		, { FALSE, "KWFOR", 14} 
		, { FALSE, "KWFUNCTION", 15} 
		, { FALSE, "KWIF", 16} 
		, { FALSE, "KWIN", 17} 
		, { FALSE, "KWLOCAL", 18} 
		, { FALSE, "KWNIL", 19} 
		, { FALSE, "KWNOT", 20} 
		, { FALSE, "KWOR", 21} 
		, { FALSE, "KWREPEAT", 22} 
		, { FALSE, "KWRETURN", 23} 
		, { FALSE, "KWTHEN", 24} 
		, { FALSE, "KWTRUE", 25} 
		, { FALSE, "KWUNTIL", 26} 
		, { FALSE, "KWWHILE", 27} 
		, { FALSE, "KWCONCAT", 28} 
		, { FALSE, "ELLIPSES", 29} 
		, { FALSE, "EQ", 30} 
		, { FALSE, "GE", 31} 
		, { FALSE, "LE", 32} 
		, { FALSE, "NE", 33} 
		, { FALSE, "OP_PLUS", 34} 
		, { FALSE, "OP_MINUS", 35} 
		, { FALSE, "OP_MUL", 36} 
		, { FALSE, "OP_DIV", 37} 
		, { FALSE, "OP_MOD", 38} 
		, { FALSE, "OP_HASH", 39} 
		, { FALSE, "OP_NEG", 40} 
		, { FALSE, "SPECIAL", 41} 
		, { FALSE, "SPECIAL_CONST", 42} 
		, { FALSE, "SPECIAL_NUMBER", 43} 
		, { FALSE, "OP_LT", 44} 
		, { FALSE, "OP_GT", 45} 
		, { FALSE, "NUMBER", 46} 
		, { FALSE, "STRING", 47} 
		, { FALSE, "IDENTIFIER", 48} 
		, { FALSE, "UNARY_OPERATOR", 49} 
		, { FALSE, "SEMICOLON", 50} 
		, { FALSE, "OP_ASSIGN", 51} 
		, { FALSE, "COMMA", 52} 
		, { FALSE, "COLON", 53} 
		, { FALSE, "DOT", 54} 
		, { FALSE, "LPAREN", 55} 
		, { FALSE, "RPAREN", 56} 
		, { TRUE, "block", 57} 
		, { TRUE, "statlist", 58} 
		, { TRUE, "prefixexp", 59} 
		, { TRUE, "stat", 60} 
		, { TRUE, "laststat", 61} 
		, { TRUE, "funcname", 62} 
		, { TRUE, "optelse", 63} 
		, { TRUE, "optforexp", 64} 
		, { TRUE, "varlist", 65} 
		, { TRUE, "explist", 66} 
		, { TRUE, "optasmexplist", 67} 
		, { TRUE, "namelist", 68} 
		, { TRUE, "var", 69} 
		, { TRUE, "exp", 70} 
		, { TRUE, "functioncall", 71} 
		, { TRUE, "args", 72} 
		, { TRUE, "funcbody", 73} 
		, { TRUE, "parlist", 74} 
		, { TRUE, "tableconstructor", 75} 
		, { TRUE, "fieldlist", 76} 
		, { TRUE, "field", 77} 
		, { TRUE, "fieldsep", 78} 
		, { TRUE, "optfieldsep", 79} 
		, { TRUE, "binop", 80} 
		, { TRUE, "unop", 81} 
		, { TRUE, "optsemi", 82} 
};
const GroRunSymbolDescription *mullua_parser_config_symbols = symbols;

const int mullua_parser_config_nr_of_states = 188;
const char *mullua_parser_config_states_text =
				  "r61,2,1p23r60,1p11r59,1p8r59,1p7r65,1p32r57,1p3r58,2,1p5r61,2,1p21r69,1p40r57,1p2r72,1p59r71,2p54r72"
				  ",1p58r68,1p38r62,1p24r82,1p95r61,2p23r70,1p49r70,1p45r70,1p50r70,1p44r70,1p46r81,1p93r70,1p47r81,1p9"
				  "2r81,1p94r70,1p48r70,1p43r58,2p5r58,3,1p6r61,3,2p22r66,1p34r61,2p21r60,3,1p12r57,2p1r60,3,2p20r76,1p"
				  "67r77,1p71r75,2p66r72,2p57r69,3p42r60,3p18r70,2p53r80,1p90r80,1p77r80,1p91r80,1p82r80,1p89r80,1p80r8"
				  "0,1p81r80,1p79r80,1p87r80,1p85r80,1p78r80,1p83r80,1p88r59,3p9r80,1p84r80,1p86r70,2p52r65,3p33r58,3p6"
				  "r61,3p22r60,4,1p14r60,3p10r60,3p12r60,3p20r78,1p72r78,1p73r75,3p65r71,4p55r72,3p56r69,4p41r68,3p39r7"
				  "4,1p63r74,1p61r62,3p26r62,3p25r70,3p51r60,5,3p13r66,3p35r60,6,3,4p15r63,2,1p27r60,4p14r60,4p19r67,2p"
				  "36r76,3p68r77,3p70r73,5,1,3p60r60,5p13r60,6,4p15r60,6,3p15r63,2p27r60,7,5p17r73,5,3p60r74,3p62r73,5,"
				  "1p60r60,6p15r63,5,3,4p28r77,5p69r60,7p17r73,5p60r63,5,4p28r63,5,3p28r64,2p30r60,10,6,8p16r63,5p28r60"
				  ",10,8p16r60,10,6p16r60,10p16A0B65S16B61S10B9S17B58S18B60S11B57S2B14S3B15S4B16S14B71S8B55S6B69S9B23S1"
				  "2B22S15B0EB48S13B18S19B59S1B8S5B27S7A1B72S24B75S25B54S27B5S26B3S20B53S21B47S22B55S23A2B0EA3B68S28B48"
				  "S29A4B48S31B62S30A5B11R0,-1B50S32B0R0,-1B12R0,-1B82S33B26R0,-1B10R0,-1A6B29S47B81S48B75S40B25S35B15S"
				  "36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S39B19S49B46S42B48S13B13S41B59S34B39S46A7B29S47B81S48B7"
				  "5S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S50B19S49B46S42B48S13B13S41B59S34B39S46A8"
				  "B11R1,-1R2,-1B50R1,-1R2,-1B40R2,-1B12R1,-1R2,-1B30R2,-1B36R2,-1B14R1,-1R2,-1B53R2,-1B47R2,-1B71R1,-1"
				  "R2,-1B69R1,-1R2,-1B55R1,-1R2,-1B22R1,-1R2,-1B6R2,-1B33R2,-1B38R2,-1B18R1,-1R2,-1B48R1,-1R2,-1B37R2,-"
				  "1B52R2,-1B21R2,-1B8R1,-1R2,-1B31R2,-1B32R2,-1B65R1,-1R2,-1B9R1,-1R2,-1B75R2,-1B60R1,-1R2,-1B56R2,-1B"
				  "54R2,-1B15R1,-1R2,-1B4R2,-1B3R2,-1B34R2,-1B24R2,-1B16R1,-1R2,-1B35R2,-1B10R1,-1R2,-1B23R1,-1R2,-1B0R"
				  "1,-1R2,-1B5R2,-1B59R1,-1R2,-1B7R2,-1B28R2,-1B26R1,-1R2,-1B45R2,-1B27R1,-1R2,-1A9B11R3,-1B50R3,-1B40R"
				  "3,-1B12R3,-1B30R3,-1B36R3,-1B14R3,-1B53R3,-1B47R3,-1B71R3,-1B69R3,-1B55R3,-1B22R3,-1B6R3,-1B33R3,-1B"
				  "38R3,-1B18R3,-1B48R3,-1B37R3,-1B52S51R3,-1B21R3,-1B8R3,-1B31R3,-1B32R3,-1B65R3,-1B9R3,-1B75R3,-1B60R"
				  "3,-1B56R3,-1B54R3,-1B15R3,-1B4R3,-1B3R3,-1B34R3,-1B24R3,-1B16R3,-1B35R3,-1B10R3,-1B23R3,-1B0R3,-1B51"
				  "R4,-1B5R3,-1B59R3,-1B7R3,-1B28R3,-1B26R3,-1B45R3,-1B27R3,-1A10B11R5,-1B0R5,-1B12R5,-1B26R5,-1B10R5,-"
				  "1A11B65S16B11R6,-1B50S32B9S17B58S53B12R6,-1B60S11B82S52B14S3B15S4B16S14B71S8B10R6,-1B55S6B69S9B22S15"
				  "B23R6,-1B0R6,-1B48S13B18S19B59S1B8R6,-1B26R6,-1B27S7A12B11R7,-1B50S32B12R7,-1B25S35B47S44B71S37B55S6"
				  "B69S38B70S55B48S13B29S47B81S48B75S40B82S56B15S36B3S20B20S43B35S45B66S54B10R7,-1B0R7,-1B19S49B46S42B1"
				  "3S41B59S34B39S46B26R7,-1A13B11R8,-1B50R8,-1B40R8,-1B12R8,-1B30R8,-1B36R8,-1B14R8,-1B53R8,-1B47R8,-1B"
				  "71R8,-1B69R8,-1B55R8,-1B22R8,-1B6R8,-1B33R8,-1B38R8,-1B18R8,-1B48R8,-1B37R8,-1B52R8,-1B21R8,-1B8R8,-"
				  "1B31R8,-1B32R8,-1B65R8,-1B9R8,-1B75R8,-1B60R8,-1B56R8,-1B54R8,-1B15R8,-1B4R8,-1B3R8,-1B34R8,-1B24R8,"
				  "-1B16R8,-1B35R8,-1B10R8,-1B23R8,-1B0R8,-1B51R8,-1B5R8,-1B59R8,-1B7R8,-1B28R8,-1B26R8,-1B45R8,-1B27R8"
				  ",-1A14B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S57B19S49B46S42B48S13"
				  "B13S41B59S34B39S46A15B65S16B61S10B9S17B58S18B60S11B57S58B14S3B15S4B16S14B71S8B55S6B69S9B23S12B22S15B"
				  "48S13B18S19B59S1B8S5B26S59B27S7A16B51S60A17B65S16B61S10B9S17B58S18B12S62B60S11B57S61B14S3B15S4B16S14"
				  "B71S8B55S6B69S9B23S12B22S15B48S13B18S19B59S1B8S5B27S7A18B23S12B11R9,-1B0R9,-1B61S63B12R9,-1B8S5B26R9"
				  ",-1B10R9,-1A19B68S65B48S66B15S64A20B29S47B81S48B75S40B77S67B25S35B15S36B4S71B3S20B20S43B76S68B47S44B"
				  "35S45B71S37B55S6B69S38B70S69B19S49B46S42B5S70B48S72B13S41B59S34B39S46A21B48S73A22B11R10,-1B50R10,-1B"
				  "40R10,-1B12R10,-1B30R10,-1B36R10,-1B14R10,-1B53R10,-1B47R10,-1B71R10,-1B55R10,-1B69R10,-1B22R10,-1B6"
				  "R10,-1B33R10,-1B38R10,-1B18R10,-1B48R10,-1B37R10,-1B52R10,-1B21R10,-1B8R10,-1B31R10,-1B32R10,-1B65R1"
				  "0,-1B75R10,-1B9R10,-1B60R10,-1B56R10,-1B54R10,-1B3R10,-1B15R10,-1B4R10,-1B34R10,-1B24R10,-1B16R10,-1"
				  "B35R10,-1B10R10,-1B23R10,-1B0R10,-1B5R10,-1B59R10,-1B7R10,-1B28R10,-1B26R10,-1B45R10,-1B27R10,-1A23B"
				  "29S47B81S48B75S40B56S75B25S35B15S36B3S20B20S43B47S44B35S45B66S74B71S37B55S6B69S38B70S55B19S49B46S42B"
				  "48S13B13S41B59S34B39S46A24B11R11,-1B50R11,-1B40R11,-1B12R11,-1B30R11,-1B36R11,-1B14R11,-1B53R11,-1B4"
				  "7R11,-1B71R11,-1B55R11,-1B69R11,-1B22R11,-1B6R11,-1B33R11,-1B38R11,-1B18R11,-1B48R11,-1B37R11,-1B52R"
				  "11,-1B21R11,-1B8R11,-1B31R11,-1B32R11,-1B65R11,-1B75R11,-1B9R11,-1B60R11,-1B56R11,-1B54R11,-1B3R11,-"
				  "1B15R11,-1B4R11,-1B34R11,-1B24R11,-1B16R11,-1B35R11,-1B10R11,-1B23R11,-1B0R11,-1B5R11,-1B59R11,-1B7R"
				  "11,-1B28R11,-1B26R11,-1B45R11,-1B27R11,-1A25B11R12,-1B50R12,-1B40R12,-1B12R12,-1B30R12,-1B36R12,-1B1"
				  "4R12,-1B53R12,-1B47R12,-1B71R12,-1B55R12,-1B69R12,-1B22R12,-1B6R12,-1B33R12,-1B38R12,-1B18R12,-1B48R"
				  "12,-1B37R12,-1B52R12,-1B21R12,-1B8R12,-1B31R12,-1B32R12,-1B65R12,-1B75R12,-1B9R12,-1B60R12,-1B56R12,"
				  "-1B54R12,-1B3R12,-1B15R12,-1B4R12,-1B34R12,-1B24R12,-1B16R12,-1B35R12,-1B10R12,-1B23R12,-1B0R12,-1B5"
				  "R12,-1B59R12,-1B7R12,-1B28R12,-1B26R12,-1B45R12,-1B27R12,-1A26B29S47B81S48B75S40B25S35B15S36B3S20B20"
				  "S43B47S44B35S45B71S37B55S6B69S38B70S76B19S49B46S42B48S13B13S41B59S34B39S46A27B48S77A28B17S79B52S78A2"
				  "9B11R13,-1B17R13,-1B50R13,-1B12R13,-1B14R13,-1B71R13,-1B69R13,-1B55R13,-1B22R13,-1B18R13,-1B48R13,-1"
				  "B52R13,-1B8R13,-1B65R13,-1B9R13,-1B60R13,-1B56R13,-1B15R13,-1B16R13,-1B10R13,-1B23R13,-1B51S80R13,-1"
				  "B0R13,-1B59R13,-1B26R13,-1B27R13,-1A30B73S81B55S82A31B54S84B53S83B55R14,-1A32B65R15,-1B11R15,-1B9R15"
				  ",-1B12R15,-1B60R15,-1B14R15,-1B15R15,-1B16R15,-1B10R15,-1B71R15,-1B69R15,-1B55R15,-1B22R15,-1B23R15,"
				  "-1B0R15,-1B18R15,-1B48R15,-1B59R15,-1B26R15,-1B8R15,-1B27R15,-1A33B11R16,-1B0R16,-1B12R16,-1B26R16,-"
				  "1B10R16,-1A34B72S24B11R17,-1B50R17,-1B40R17,-1B12R17,-1B30R17,-1B36R17,-1B14R17,-1B53S21B47S22B71R17"
				  ",-1B55S23R17,-1B69R17,-1B22R17,-1B6R17,-1B33R17,-1B38R17,-1B18R17,-1B48R17,-1B37R17,-1B52R17,-1B21R1"
				  "7,-1B8R17,-1B31R17,-1B32R17,-1B65R17,-1B75S25B9R17,-1B54S27B60R17,-1B56R17,-1B3S20B15R17,-1B4R17,-1B"
				  "34R17,-1B24R17,-1B16R17,-1B35R17,-1B10R17,-1B23R17,-1B0R17,-1B5S26B59R17,-1B7R17,-1B28R17,-1B26R17,-"
				  "1B45R17,-1B27R17,-1A35B11R18,-1B50R18,-1B40R18,-1B12R18,-1B30R18,-1B36R18,-1B14R18,-1B71R18,-1B69R18"
				  ",-1B55R18,-1B22R18,-1B6R18,-1B33R18,-1B38R18,-1B18R18,-1B48R18,-1B37R18,-1B52R18,-1B21R18,-1B8R18,-1"
				  "B31R18,-1B32R18,-1B65R18,-1B9R18,-1B60R18,-1B56R18,-1B15R18,-1B4R18,-1B34R18,-1B24R18,-1B16R18,-1B35"
				  "R18,-1B10R18,-1B23R18,-1B0R18,-1B59R18,-1B7R18,-1B28R18,-1B26R18,-1B45R18,-1B27R18,-1A36B73S85B55S82"
				  "A37B11R2,-1B50R2,-1B40R2,-1B12R2,-1B30R2,-1B36R2,-1B14R2,-1B53R2,-1B47R2,-1B71R2,-1B69R2,-1B55R2,-1B"
				  "22R2,-1B6R2,-1B33R2,-1B38R2,-1B18R2,-1B48R2,-1B37R2,-1B52R2,-1B21R2,-1B8R2,-1B31R2,-1B32R2,-1B65R2,-"
				  "1B9R2,-1B75R2,-1B60R2,-1B56R2,-1B54R2,-1B15R2,-1B4R2,-1B3R2,-1B34R2,-1B24R2,-1B16R2,-1B35R2,-1B10R2,"
				  "-1B23R2,-1B0R2,-1B5R2,-1B59R2,-1B7R2,-1B28R2,-1B26R2,-1B45R2,-1B27R2,-1A38B11R3,-1B50R3,-1B40R3,-1B1"
				  "2R3,-1B30R3,-1B36R3,-1B14R3,-1B53R3,-1B47R3,-1B71R3,-1B69R3,-1B55R3,-1B22R3,-1B6R3,-1B33R3,-1B38R3,-"
				  "1B18R3,-1B48R3,-1B37R3,-1B52R3,-1B21R3,-1B8R3,-1B31R3,-1B32R3,-1B65R3,-1B9R3,-1B75R3,-1B60R3,-1B56R3"
				  ",-1B54R3,-1B15R3,-1B4R3,-1B3R3,-1B34R3,-1B24R3,-1B16R3,-1B35R3,-1B10R3,-1B23R3,-1B0R3,-1B5R3,-1B59R3"
				  ",-1B7R3,-1B28R3,-1B26R3,-1B45R3,-1B27R3,-1A39B40S90B30S100B56S99B36S94B80S86B34S88B35S96B33S101B38S9"
				  "3B37S92B7S87B21S89B28S97B31S91B45S98B32S95A40B11R19,-1B50R19,-1B40R19,-1B12R19,-1B30R19,-1B36R19,-1B"
				  "14R19,-1B71R19,-1B69R19,-1B55R19,-1B22R19,-1B6R19,-1B33R19,-1B38R19,-1B18R19,-1B48R19,-1B37R19,-1B52"
				  "R19,-1B21R19,-1B8R19,-1B31R19,-1B32R19,-1B65R19,-1B9R19,-1B60R19,-1B56R19,-1B15R19,-1B4R19,-1B34R19,"
				  "-1B24R19,-1B16R19,-1B35R19,-1B10R19,-1B23R19,-1B0R19,-1B59R19,-1B7R19,-1B28R19,-1B26R19,-1B45R19,-1B"
				  "27R19,-1A41B11R20,-1B50R20,-1B40R20,-1B12R20,-1B30R20,-1B36R20,-1B14R20,-1B71R20,-1B69R20,-1B55R20,-"
				  "1B22R20,-1B6R20,-1B33R20,-1B38R20,-1B18R20,-1B48R20,-1B37R20,-1B52R20,-1B21R20,-1B8R20,-1B31R20,-1B3"
				  "2R20,-1B65R20,-1B9R20,-1B60R20,-1B56R20,-1B15R20,-1B4R20,-1B34R20,-1B24R20,-1B16R20,-1B35R20,-1B10R2"
				  "0,-1B23R20,-1B0R20,-1B59R20,-1B7R20,-1B28R20,-1B26R20,-1B45R20,-1B27R20,-1A42B11R21,-1B50R21,-1B40R2"
				  "1,-1B12R21,-1B30R21,-1B36R21,-1B14R21,-1B71R21,-1B69R21,-1B55R21,-1B22R21,-1B6R21,-1B33R21,-1B38R21,"
				  "-1B18R21,-1B48R21,-1B37R21,-1B52R21,-1B21R21,-1B8R21,-1B31R21,-1B32R21,-1B65R21,-1B9R21,-1B60R21,-1B"
				  "56R21,-1B15R21,-1B4R21,-1B34R21,-1B24R21,-1B16R21,-1B35R21,-1B10R21,-1B23R21,-1B0R21,-1B59R21,-1B7R2"
				  "1,-1B28R21,-1B26R21,-1B45R21,-1B27R21,-1A43B29R22,-1B81R22,-1B75R22,-1B25R22,-1B15R22,-1B3R22,-1B20R"
				  "22,-1B47R22,-1B35R22,-1B71R22,-1B69R22,-1B55R22,-1B70R22,-1B19R22,-1B46R22,-1B13R22,-1B48R22,-1B59R2"
				  "2,-1B39R22,-1A44B11R23,-1B50R23,-1B40R23,-1B12R23,-1B30R23,-1B36R23,-1B14R23,-1B71R23,-1B69R23,-1B55"
				  "R23,-1B22R23,-1B6R23,-1B33R23,-1B38R23,-1B18R23,-1B48R23,-1B37R23,-1B52R23,-1B21R23,-1B8R23,-1B31R23"
				  ",-1B32R23,-1B65R23,-1B9R23,-1B60R23,-1B56R23,-1B15R23,-1B4R23,-1B34R23,-1B24R23,-1B16R23,-1B35R23,-1"
				  "B10R23,-1B23R23,-1B0R23,-1B59R23,-1B7R23,-1B28R23,-1B26R23,-1B45R23,-1B27R23,-1A45B29R24,-1B81R24,-1"
				  "B75R24,-1B25R24,-1B15R24,-1B3R24,-1B20R24,-1B47R24,-1B35R24,-1B71R24,-1B69R24,-1B55R24,-1B70R24,-1B1"
				  "9R24,-1B46R24,-1B13R24,-1B48R24,-1B59R24,-1B39R24,-1A46B29R25,-1B81R25,-1B75R25,-1B25R25,-1B15R25,-1"
				  "B3R25,-1B20R25,-1B47R25,-1B35R25,-1B71R25,-1B69R25,-1B55R25,-1B70R25,-1B19R25,-1B46R25,-1B13R25,-1B4"
				  "8R25,-1B59R25,-1B39R25,-1A47B11R26,-1B50R26,-1B40R26,-1B12R26,-1B30R26,-1B36R26,-1B14R26,-1B71R26,-1"
				  "B69R26,-1B55R26,-1B22R26,-1B6R26,-1B33R26,-1B38R26,-1B18R26,-1B48R26,-1B37R26,-1B52R26,-1B21R26,-1B8"
				  "R26,-1B31R26,-1B32R26,-1B65R26,-1B9R26,-1B60R26,-1B56R26,-1B15R26,-1B4R26,-1B34R26,-1B24R26,-1B16R26"
				  ",-1B35R26,-1B10R26,-1B23R26,-1B0R26,-1B59R26,-1B7R26,-1B28R26,-1B26R26,-1B45R26,-1B27R26,-1A48B29S47"
				  "B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S102B19S49B46S42B48S13B13S41B59S3"
				  "4B39S46A49B11R27,-1B50R27,-1B40R27,-1B12R27,-1B30R27,-1B36R27,-1B14R27,-1B71R27,-1B69R27,-1B55R27,-1"
				  "B22R27,-1B6R27,-1B33R27,-1B38R27,-1B18R27,-1B48R27,-1B37R27,-1B52R27,-1B21R27,-1B8R27,-1B31R27,-1B32"
				  "R27,-1B65R27,-1B9R27,-1B60R27,-1B56R27,-1B15R27,-1B4R27,-1B34R27,-1B24R27,-1B16R27,-1B35R27,-1B10R27"
				  ",-1B23R27,-1B0R27,-1B59R27,-1B7R27,-1B28R27,-1B26R27,-1B45R27,-1B27R27,-1A50B40S90B9S103B30S100B36S9"
				  "4B80S86B34S88B35S96B33S101B38S93B37S92B7S87B21S89B28S97B31S91B45S98B32S95A51B65S104B48S13B59S1B71S37"
				  "B55S6B69S9A52B65S16B11R28,-1B9S17B58S105B12R28,-1B60S11B14S3B15S4B16S14B71S8B10R28,-1B55S6B69S9B22S1"
				  "5B23R28,-1B0R28,-1B48S13B18S19B59S1B8R28,-1B26R28,-1B27S7A53B23R29,-1B11R29,-1B0R29,-1B12R29,-1B8R29"
				  ",-1B26R29,-1B10R29,-1A54B11R30,-1B50S32B0R30,-1B12R30,-1B82S106B26R30,-1B10R30,-1A55B11R31,-1B40S90B"
				  "50R31,-1B30S100B12R31,-1B36S94B14R31,-1B71R31,-1B69R31,-1B55R31,-1B22R31,-1B33S101B38S93B18R31,-1B48"
				  "R31,-1B37S92B21S89B52S107B8R31,-1B31S91B32S95B65R31,-1B9R31,-1B60R31,-1B56R31,-1B15R31,-1B80S86B34S8"
				  "8B35S96B16R31,-1B10R31,-1B23R31,-1B0R31,-1B59R31,-1B7S87B28S97B26R31,-1B45S98B27R31,-1A56B11R32,-1B0"
				  "R32,-1B12R32,-1B26R32,-1B10R32,-1A57B40S90B30S100B36S94B80S86B34S88B24S108B35S96B33S101B38S93B37S92B"
				  "7S87B21S89B28S97B31S91B45S98B32S95A58B26S109A59B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45"
				  "B71S37B55S6B69S38B70S110B19S49B46S42B48S13B13S41B59S34B39S46A60B29S47B81S48B75S40B25S35B15S36B3S20B2"
				  "0S43B47S44B35S45B66S111B71S37B55S6B69S38B70S55B19S49B46S42B48S13B13S41B59S34B39S46A61B12S112A62B65R3"
				  "3,-1B11R33,-1B50R33,-1B9R33,-1B12R33,-1B60R33,-1B14R33,-1B15R33,-1B16R33,-1B71R33,-1B10R33,-1B69R33,"
				  "-1B55R33,-1B22R33,-1B23R33,-1B0R33,-1B18R33,-1B48R33,-1B59R33,-1B26R33,-1B8R33,-1B27R33,-1A63B11R34,"
				  "-1B0R34,-1B12R34,-1B26R34,-1B10R34,-1A64B48S113A65B11R35,-1B50R35,-1B12R35,-1B14R35,-1B71R35,-1B69R3"
				  "5,-1B55R35,-1B22R35,-1B18R35,-1B48R35,-1B52S78B8R35,-1B65R35,-1B9R35,-1B60R35,-1B15R35,-1B16R35,-1B1"
				  "0R35,-1B23R35,-1B51S115B0R35,-1B67S114B59R35,-1B26R35,-1B27R35,-1A66B11R13,-1B17R13,-1B50R13,-1B12R1"
				  "3,-1B14R13,-1B71R13,-1B69R13,-1B55R13,-1B22R13,-1B18R13,-1B48R13,-1B52R13,-1B8R13,-1B65R13,-1B9R13,-"
				  "1B60R13,-1B56R13,-1B15R13,-1B16R13,-1B10R13,-1B23R13,-1B51R13,-1B0R13,-1B59R13,-1B26R13,-1B27R13,-1A"
				  "67B50S118B52S116B4R36,-1B78S117A68B4S119A69B40S90B50R37,-1B30S100B36S94B4R37,-1B80S86B34S88B35S96B33"
				  "S101B38S93B37S92B7S87B21S89B52R37,-1B28S97B31S91B45S98B32S95A70B29S47B81S48B75S40B25S35B15S36B3S20B2"
				  "0S43B47S44B35S45B71S37B55S6B69S38B70S120B19S49B46S42B48S13B13S41B59S34B39S46A71B11R38,-1B50R38,-1B40"
				  "R38,-1B12R38,-1B30R38,-1B36R38,-1B14R38,-1B53R38,-1B47R38,-1B71R38,-1B55R38,-1B69R38,-1B22R38,-1B6R3"
				  "8,-1B33R38,-1B38R38,-1B18R38,-1B48R38,-1B37R38,-1B52R38,-1B21R38,-1B8R38,-1B31R38,-1B32R38,-1B65R38,"
				  "-1B75R38,-1B9R38,-1B60R38,-1B56R38,-1B54R38,-1B3R38,-1B15R38,-1B4R38,-1B34R38,-1B24R38,-1B16R38,-1B3"
				  "5R38,-1B10R38,-1B23R38,-1B0R38,-1B5R38,-1B59R38,-1B7R38,-1B28R38,-1B26R38,-1B45R38,-1B27R38,-1A72B11"
				  "R8,-1B50R8,-1B40R8,-1B12R8,-1B30R8,-1B36R8,-1B14R8,-1B53R8,-1B47R8,-1B71R8,-1B69R8,-1B55R8,-1B22R8,-"
				  "1B6R8,-1B33R8,-1B38R8,-1B18R8,-1B48R8,-1B37R8,-1B52R8,-1B21R8,-1B8R8,-1B31R8,-1B32R8,-1B65R8,-1B9R8,"
				  "-1B75R8,-1B60R8,-1B56R8,-1B54R8,-1B15R8,-1B4R8,-1B3R8,-1B34R8,-1B24R8,-1B16R8,-1B35R8,-1B10R8,-1B23R"
				  "8,-1B51S121R8,-1B0R8,-1B5R8,-1B59R8,-1B7R8,-1B28R8,-1B26R8,-1B45R8,-1B27R8,-1A73B72S122B75S25B3S20B4"
				  "7S22B55S23A74B56S123A75B11R39,-1B50R39,-1B40R39,-1B12R39,-1B30R39,-1B36R39,-1B14R39,-1B53R39,-1B47R3"
				  "9,-1B71R39,-1B55R39,-1B69R39,-1B22R39,-1B6R39,-1B33R39,-1B38R39,-1B18R39,-1B48R39,-1B37R39,-1B52R39,"
				  "-1B21R39,-1B8R39,-1B31R39,-1B32R39,-1B65R39,-1B75R39,-1B9R39,-1B60R39,-1B56R39,-1B54R39,-1B3R39,-1B1"
				  "5R39,-1B4R39,-1B34R39,-1B24R39,-1B16R39,-1B35R39,-1B10R39,-1B23R39,-1B0R39,-1B5R39,-1B59R39,-1B7R39,"
				  "-1B28R39,-1B26R39,-1B45R39,-1B27R39,-1A76B40S90B30S100B36S94B80S86B34S88B35S96B6S124B33S101B38S93B37"
				  "S92B7S87B21S89B28S97B31S91B45S98B32S95A77B11R40,-1B50R40,-1B40R40,-1B12R40,-1B30R40,-1B36R40,-1B14R4"
				  "0,-1B53R40,-1B47R40,-1B71R40,-1B69R40,-1B55R40,-1B22R40,-1B6R40,-1B33R40,-1B38R40,-1B18R40,-1B48R40,"
				  "-1B37R40,-1B52R40,-1B21R40,-1B8R40,-1B31R40,-1B32R40,-1B65R40,-1B9R40,-1B75R40,-1B60R40,-1B56R40,-1B"
				  "54R40,-1B15R40,-1B4R40,-1B3R40,-1B34R40,-1B24R40,-1B16R40,-1B35R40,-1B10R40,-1B23R40,-1B0R40,-1B51R4"
				  "0,-1B5R40,-1B59R40,-1B7R40,-1B28R40,-1B26R40,-1B45R40,-1B27R40,-1A78B48S125A79B29S47B81S48B75S40B25S"
				  "35B15S36B3S20B20S43B47S44B35S45B66S126B71S37B55S6B69S38B70S55B19S49B46S42B48S13B13S41B59S34B39S46A80"
				  "B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S127B19S49B46S42B48S13B13S4"
				  "1B59S34B39S46A81B65R41,-1B11R41,-1B50R41,-1B9R41,-1B12R41,-1B60R41,-1B14R41,-1B15R41,-1B16R41,-1B71R"
				  "41,-1B10R41,-1B69R41,-1B55R41,-1B22R41,-1B23R41,-1B0R41,-1B18R41,-1B48R41,-1B59R41,-1B26R41,-1B8R41,"
				  "-1B27R41,-1A82B29S129B68S130B56S131B48S66B74S128A83B48S132A84B48S31B62S133A85B11R42,-1B50R42,-1B40R4"
				  "2,-1B12R42,-1B30R42,-1B36R42,-1B14R42,-1B71R42,-1B69R42,-1B55R42,-1B22R42,-1B6R42,-1B33R42,-1B38R42,"
				  "-1B18R42,-1B48R42,-1B37R42,-1B52R42,-1B21R42,-1B8R42,-1B31R42,-1B32R42,-1B65R42,-1B9R42,-1B60R42,-1B"
				  "56R42,-1B15R42,-1B4R42,-1B34R42,-1B24R42,-1B16R42,-1B35R42,-1B10R42,-1B23R42,-1B0R42,-1B59R42,-1B7R4"
				  "2,-1B28R42,-1B26R42,-1B45R42,-1B27R42,-1A86B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S"
				  "37B55S6B69S38B70S134B19S49B46S42B48S13B13S41B59S34B39S46A87B29R43,-1B81R43,-1B75R43,-1B25R43,-1B15R4"
				  "3,-1B3R43,-1B20R43,-1B47R43,-1B35R43,-1B71R43,-1B69R43,-1B55R43,-1B70R43,-1B19R43,-1B46R43,-1B13R43,"
				  "-1B48R43,-1B59R43,-1B39R43,-1A88B29R44,-1B81R44,-1B75R44,-1B25R44,-1B15R44,-1B3R44,-1B20R44,-1B47R44"
				  ",-1B35R44,-1B71R44,-1B69R44,-1B55R44,-1B70R44,-1B19R44,-1B46R44,-1B13R44,-1B48R44,-1B59R44,-1B39R44,"
				  "-1A89B29R45,-1B81R45,-1B75R45,-1B25R45,-1B15R45,-1B3R45,-1B20R45,-1B47R45,-1B35R45,-1B71R45,-1B69R45"
				  ",-1B55R45,-1B70R45,-1B19R45,-1B46R45,-1B13R45,-1B48R45,-1B59R45,-1B39R45,-1A90B29R46,-1B81R46,-1B75R"
				  "46,-1B25R46,-1B15R46,-1B3R46,-1B20R46,-1B47R46,-1B35R46,-1B71R46,-1B69R46,-1B55R46,-1B70R46,-1B19R46"
				  ",-1B46R46,-1B13R46,-1B48R46,-1B59R46,-1B39R46,-1A91B29R47,-1B81R47,-1B75R47,-1B25R47,-1B15R47,-1B3R4"
				  "7,-1B20R47,-1B47R47,-1B35R47,-1B71R47,-1B69R47,-1B55R47,-1B70R47,-1B19R47,-1B46R47,-1B13R47,-1B48R47"
				  ",-1B59R47,-1B39R47,-1A92B29R48,-1B81R48,-1B75R48,-1B25R48,-1B15R48,-1B3R48,-1B20R48,-1B47R48,-1B35R4"
				  "8,-1B71R48,-1B69R48,-1B55R48,-1B70R48,-1B19R48,-1B46R48,-1B13R48,-1B48R48,-1B59R48,-1B39R48,-1A93B29"
				  "R49,-1B81R49,-1B75R49,-1B25R49,-1B15R49,-1B3R49,-1B20R49,-1B47R49,-1B35R49,-1B71R49,-1B69R49,-1B55R4"
				  "9,-1B70R49,-1B19R49,-1B46R49,-1B13R49,-1B48R49,-1B59R49,-1B39R49,-1A94B29R50,-1B81R50,-1B75R50,-1B25"
				  "R50,-1B15R50,-1B3R50,-1B20R50,-1B47R50,-1B35R50,-1B71R50,-1B69R50,-1B55R50,-1B70R50,-1B19R50,-1B46R5"
				  "0,-1B13R50,-1B48R50,-1B59R50,-1B39R50,-1A95B29R51,-1R52,-1B81R51,-1R52,-1B75R51,-1R52,-1B25R51,-1R52"
				  ",-1B15R51,-1R52,-1B3R51,-1R52,-1B20R51,-1R52,-1B47R51,-1R52,-1B35R51,-1R52,-1B71R51,-1R52,-1B69R51,-"
				  "1R52,-1B55R51,-1R52,-1B70R51,-1R52,-1B19R51,-1R52,-1B46R51,-1R52,-1B13R51,-1R52,-1B48R51,-1R52,-1B59"
				  "R51,-1R52,-1B39R51,-1R52,-1A96B29R53,-1B81R53,-1B75R53,-1B25R53,-1B15R53,-1B3R53,-1B20R53,-1B47R53,-"
				  "1B35R53,-1B71R53,-1B69R53,-1B55R53,-1B70R53,-1B19R53,-1B46R53,-1B13R53,-1B48R53,-1B59R53,-1B39R53,-1"
				  "A97B29R54,-1B81R54,-1B75R54,-1B25R54,-1B15R54,-1B3R54,-1B20R54,-1B47R54,-1B35R54,-1B71R54,-1B69R54,-"
				  "1B55R54,-1B70R54,-1B19R54,-1B46R54,-1B13R54,-1B48R54,-1B59R54,-1B39R54,-1A98B29R55,-1B81R55,-1B75R55"
				  ",-1B25R55,-1B15R55,-1B3R55,-1B20R55,-1B47R55,-1B35R55,-1B71R55,-1B69R55,-1B55R55,-1B70R55,-1B19R55,-"
				  "1B46R55,-1B13R55,-1B48R55,-1B59R55,-1B39R55,-1A99B11R56,-1B50R56,-1B40R56,-1B12R56,-1B30R56,-1B36R56"
				  ",-1B14R56,-1B53R56,-1B47R56,-1B71R56,-1B69R56,-1B55R56,-1B22R56,-1B6R56,-1B33R56,-1B38R56,-1B18R56,-"
				  "1B48R56,-1B37R56,-1B52R56,-1B21R56,-1B8R56,-1B31R56,-1B32R56,-1B65R56,-1B9R56,-1B75R56,-1B60R56,-1B5"
				  "6R56,-1B54R56,-1B15R56,-1B4R56,-1B3R56,-1B34R56,-1B24R56,-1B16R56,-1B35R56,-1B10R56,-1B23R56,-1B0R56"
				  ",-1B5R56,-1B59R56,-1B7R56,-1B28R56,-1B26R56,-1B45R56,-1B27R56,-1A100B29R57,-1B81R57,-1B75R57,-1B25R5"
				  "7,-1B15R57,-1B3R57,-1B20R57,-1B47R57,-1B35R57,-1B71R57,-1B69R57,-1B55R57,-1B70R57,-1B19R57,-1B46R57,"
				  "-1B13R57,-1B48R57,-1B59R57,-1B39R57,-1A101B29R58,-1B81R58,-1B75R58,-1B25R58,-1B15R58,-1B3R58,-1B20R5"
				  "8,-1B47R58,-1B35R58,-1B71R58,-1B69R58,-1B55R58,-1B70R58,-1B19R58,-1B46R58,-1B13R58,-1B48R58,-1B59R58"
				  ",-1B39R58,-1A102B11R59,-1B40S90R59,-1B50R59,-1B30S100R59,-1B12R59,-1B36S94R59,-1B14R59,-1B71R59,-1B6"
				  "9R59,-1B55R59,-1B22R59,-1B6R59,-1B33S101R59,-1B38S93R59,-1B18R59,-1B48R59,-1B37S92R59,-1B21S89R59,-1"
				  "B52R59,-1B8R59,-1B31S91R59,-1B32S95R59,-1B65R59,-1B9R59,-1B60R59,-1B56R59,-1B15R59,-1B4R59,-1B80S86B"
				  "34S88R59,-1B24R59,-1B35S96R59,-1B16R59,-1B10R59,-1B23R59,-1B0R59,-1B59R59,-1B7S87R59,-1B28S97R59,-1B"
				  "26R59,-1B45S98R59,-1B27R59,-1A103B65S16B61S10B9S17B58S18B12S136B60S11B57S135B14S3B15S4B16S14B71S8B55"
				  "S6B69S9B23S12B22S15B48S13B18S19B59S1B8S5B27S7A104B51R60,-1A105B23R61,-1B11R61,-1B0R61,-1B12R61,-1B8R"
				  "61,-1B26R61,-1B10R61,-1A106B11R62,-1B0R62,-1B12R62,-1B26R62,-1B10R62,-1A107B29S47B81S48B75S40B25S35B"
				  "15S36B3S20B20S43B47S44B35S45B66S137B71S37B55S6B69S38B70S55B19S49B46S42B48S13B13S41B59S34B39S46A108B6"
				  "5S16B11S142B61S10B9S17B58S18B12S139B60S11B57S138B14S3B15S4B63S140B16S14B71S8B10S141B55S6B69S9B23S12B"
				  "22S15B48S13B18S19B59S1B8S5B27S7A109B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B"
				  "69S38B70S143B19S49B46S42B48S13B13S41B59S34B39S46A110B11R63,-1B40S90B50R63,-1B30S100B12R63,-1B36S94B1"
				  "4R63,-1B71R63,-1B69R63,-1B55R63,-1B22R63,-1B33S101B38S93B18R63,-1B48R63,-1B37S92B21S89B8R63,-1B31S91"
				  "B32S95B65R63,-1B9R63,-1B60R63,-1B15R63,-1B80S86B34S88B35S96B16R63,-1B10R63,-1B23R63,-1B0R63,-1B59R63"
				  ",-1B7S87B28S97B26R63,-1B45S98B27R63,-1A111B65R64,-1B11R64,-1B50R64,-1B9R64,-1B12R64,-1B60R64,-1B14R6"
				  "4,-1B15R64,-1B16R64,-1B71R64,-1B10R64,-1B69R64,-1B55R64,-1B22R64,-1B23R64,-1B0R64,-1B18R64,-1B48R64,"
				  "-1B59R64,-1B26R64,-1B8R64,-1B27R64,-1A112B65R65,-1B11R65,-1B50R65,-1B9R65,-1B12R65,-1B60R65,-1B14R65"
				  ",-1B15R65,-1B16R65,-1B71R65,-1B10R65,-1B69R65,-1B55R65,-1B22R65,-1B23R65,-1B0R65,-1B18R65,-1B48R65,-"
				  "1B59R65,-1B26R65,-1B8R65,-1B27R65,-1A113B73S144B55S82A114B65R66,-1B11R66,-1B50R66,-1B9R66,-1B12R66,-"
				  "1B60R66,-1B14R66,-1B15R66,-1B16R66,-1B71R66,-1B10R66,-1B69R66,-1B55R66,-1B22R66,-1B23R66,-1B0R66,-1B"
				  "18R66,-1B48R66,-1B59R66,-1B26R66,-1B8R66,-1B27R66,-1A115B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47"
				  "S44B35S45B66S145B71S37B55S6B69S38B70S55B19S49B46S42B48S13B13S41B59S34B39S46A116B29R67,-1B81R67,-1B75"
				  "R67,-1B77R67,-1B25R67,-1B15R67,-1B3R67,-1B20R67,-1B47R67,-1B35R67,-1B71R67,-1B69R67,-1B55R67,-1B70R6"
				  "7,-1B19R67,-1B46R67,-1B5R67,-1B48R67,-1B13R67,-1B59R67,-1B39R67,-1A117B29S47B81S48B75S40B77S67B25S35"
				  "B15S36B3S20B20S43B76S146B47S44B35S45B71S37B55S6B69S38B70S69B19S49B46S42B5S70B48S72B13S41B59S34B39S46"
				  "A118B29R68,-1B81R68,-1B75R68,-1B77R68,-1B25R68,-1B15R68,-1B3R68,-1B20R68,-1B47R68,-1B35R68,-1B71R68,"
				  "-1B69R68,-1B55R68,-1B70R68,-1B19R68,-1B46R68,-1B5R68,-1B48R68,-1B13R68,-1B59R68,-1B39R68,-1A119B11R6"
				  "9,-1B50R69,-1B40R69,-1B12R69,-1B30R69,-1B36R69,-1B14R69,-1B53R69,-1B47R69,-1B71R69,-1B55R69,-1B69R69"
				  ",-1B22R69,-1B6R69,-1B33R69,-1B38R69,-1B18R69,-1B48R69,-1B37R69,-1B52R69,-1B21R69,-1B8R69,-1B31R69,-1"
				  "B32R69,-1B65R69,-1B75R69,-1B9R69,-1B60R69,-1B56R69,-1B54R69,-1B3R69,-1B15R69,-1B4R69,-1B34R69,-1B24R"
				  "69,-1B16R69,-1B35R69,-1B10R69,-1B23R69,-1B0R69,-1B5R69,-1B59R69,-1B7R69,-1B28R69,-1B26R69,-1B45R69,-"
				  "1B27R69,-1A120B40S90B30S100B36S94B80S86B34S88B35S96B6S147B33S101B38S93B37S92B7S87B21S89B28S97B31S91B"
				  "45S98B32S95A121B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S148B19S49B4"
				  "6S42B48S13B13S41B59S34B39S46A122B11R70,-1B50R70,-1B40R70,-1B12R70,-1B30R70,-1B36R70,-1B14R70,-1B53R7"
				  "0,-1B47R70,-1B71R70,-1B55R70,-1B69R70,-1B22R70,-1B6R70,-1B33R70,-1B38R70,-1B18R70,-1B48R70,-1B37R70,"
				  "-1B52R70,-1B21R70,-1B8R70,-1B31R70,-1B32R70,-1B65R70,-1B75R70,-1B9R70,-1B60R70,-1B56R70,-1B54R70,-1B"
				  "3R70,-1B15R70,-1B4R70,-1B34R70,-1B24R70,-1B16R70,-1B35R70,-1B10R70,-1B23R70,-1B0R70,-1B5R70,-1B59R70"
				  ",-1B7R70,-1B28R70,-1B26R70,-1B45R70,-1B27R70,-1A123B11R71,-1B50R71,-1B40R71,-1B12R71,-1B30R71,-1B36R"
				  "71,-1B14R71,-1B53R71,-1B47R71,-1B71R71,-1B55R71,-1B69R71,-1B22R71,-1B6R71,-1B33R71,-1B38R71,-1B18R71"
				  ",-1B48R71,-1B37R71,-1B52R71,-1B21R71,-1B8R71,-1B31R71,-1B32R71,-1B65R71,-1B75R71,-1B9R71,-1B60R71,-1"
				  "B56R71,-1B54R71,-1B3R71,-1B15R71,-1B4R71,-1B34R71,-1B24R71,-1B16R71,-1B35R71,-1B10R71,-1B23R71,-1B0R"
				  "71,-1B5R71,-1B59R71,-1B7R71,-1B28R71,-1B26R71,-1B45R71,-1B27R71,-1A124B11R72,-1B50R72,-1B40R72,-1B12"
				  "R72,-1B30R72,-1B36R72,-1B14R72,-1B53R72,-1B47R72,-1B71R72,-1B69R72,-1B55R72,-1B22R72,-1B6R72,-1B33R7"
				  "2,-1B38R72,-1B18R72,-1B48R72,-1B37R72,-1B52R72,-1B21R72,-1B8R72,-1B31R72,-1B32R72,-1B65R72,-1B9R72,-"
				  "1B75R72,-1B60R72,-1B56R72,-1B54R72,-1B15R72,-1B4R72,-1B3R72,-1B34R72,-1B24R72,-1B16R72,-1B35R72,-1B1"
				  "0R72,-1B23R72,-1B0R72,-1B51R72,-1B5R72,-1B59R72,-1B7R72,-1B28R72,-1B26R72,-1B45R72,-1B27R72,-1A125B1"
				  "1R73,-1B17R73,-1B50R73,-1B12R73,-1B14R73,-1B71R73,-1B69R73,-1B55R73,-1B22R73,-1B18R73,-1B48R73,-1B52"
				  "R73,-1B8R73,-1B65R73,-1B9R73,-1B60R73,-1B56R73,-1B15R73,-1B16R73,-1B10R73,-1B23R73,-1B51R73,-1B0R73,"
				  "-1B59R73,-1B26R73,-1B27R73,-1A126B9S149A127B40S90B30S100B36S94B80S86B34S88B35S96B33S101B38S93B37S92B"
				  "7S87B21S89B52S150B28S97B31S91B45S98B32S95A128B56S151A129B56R74,-1A130B56R75,-1B52S152A131B65S16B61S1"
				  "0B9S17B58S18B12S154B60S11B57S153B14S3B15S4B16S14B71S8B55S6B69S9B23S12B22S15B48S13B18S19B59S1B8S5B27S"
				  "7A132B55R76,-1A133B55R77,-1A134B11R78,-1B40S90R78,-1B50R78,-1B30S100R78,-1B12R78,-1B36S94R78,-1B14R7"
				  "8,-1B71R78,-1B69R78,-1B55R78,-1B22R78,-1B6R78,-1B33S101R78,-1B38S93R78,-1B18R78,-1B48R78,-1B37S92R78"
				  ",-1B21S89R78,-1B52R78,-1B8R78,-1B31S91R78,-1B32S95R78,-1B65R78,-1B9R78,-1B60R78,-1B56R78,-1B15R78,-1"
				  "B4R78,-1B80S86B34S88R78,-1B24R78,-1B35S96R78,-1B16R78,-1B10R78,-1B23R78,-1B0R78,-1B59R78,-1B7S87R78,"
				  "-1B28S97R78,-1B26R78,-1B45S98R78,-1B27R78,-1A135B12S155A136B65R79,-1B11R79,-1B50R79,-1B9R79,-1B12R79"
				  ",-1B60R79,-1B14R79,-1B15R79,-1B16R79,-1B71R79,-1B10R79,-1B69R79,-1B55R79,-1B22R79,-1B23R79,-1B0R79,-"
				  "1B18R79,-1B48R79,-1B59R79,-1B26R79,-1B8R79,-1B27R79,-1A137B65R80,-1B11R80,-1B50R80,-1B9R80,-1B12R80,"
				  "-1B60R80,-1B56R80,-1B14R80,-1B15R80,-1B16R80,-1B71R80,-1B10R80,-1B69R80,-1B55R80,-1B22R80,-1B23R80,-"
				  "1B0R80,-1B18R80,-1B48R80,-1B59R80,-1B26R80,-1B8R80,-1B27R80,-1A138B11S142B12S157B63S156B10S141A139B6"
				  "5R81,-1B11R81,-1B50R81,-1B9R81,-1B12R81,-1B60R81,-1B14R81,-1B15R81,-1B16R81,-1B71R81,-1B10R81,-1B69R"
				  "81,-1B55R81,-1B22R81,-1B23R81,-1B0R81,-1B18R81,-1B48R81,-1B59R81,-1B26R81,-1B8R81,-1B27R81,-1A140B12"
				  "S158A141B65S16B61S10B9S17B58S18B12R82,-1B60S11B57S159B14S3B15S4B16S14B71S8B55S6B69S9B23S12B22S15B48S"
				  "13B18S19B59S1B8S5B27S7A142B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S"
				  "160B19S49B46S42B48S13B13S41B59S34B39S46A143B11R83,-1B40S90B50R83,-1B30S100B12R83,-1B36S94B14R83,-1B7"
				  "1R83,-1B69R83,-1B55R83,-1B22R83,-1B33S101B38S93B18R83,-1B48R83,-1B37S92B21S89B8R83,-1B31S91B32S95B65"
				  "R83,-1B9R83,-1B60R83,-1B15R83,-1B80S86B34S88B35S96B16R83,-1B10R83,-1B23R83,-1B0R83,-1B59R83,-1B7S87B"
				  "28S97B26R83,-1B45S98B27R83,-1A144B65R84,-1B11R84,-1B50R84,-1B9R84,-1B12R84,-1B60R84,-1B14R84,-1B15R8"
				  "4,-1B16R84,-1B71R84,-1B10R84,-1B69R84,-1B55R84,-1B22R84,-1B23R84,-1B0R84,-1B18R84,-1B48R84,-1B59R84,"
				  "-1B26R84,-1B8R84,-1B27R84,-1A145B65R85,-1B11R85,-1B50R85,-1B9R85,-1B12R85,-1B60R85,-1B14R85,-1B15R85"
				  ",-1B16R85,-1B71R85,-1B10R85,-1B69R85,-1B55R85,-1B22R85,-1B23R85,-1B0R85,-1B18R85,-1B48R85,-1B59R85,-"
				  "1B26R85,-1B8R85,-1B27R85,-1A146B4R86,-1A147B51S161A148B40S90B50R87,-1B30S100B36S94B4R87,-1B80S86B34S"
				  "88B35S96B33S101B38S93B37S92B7S87B21S89B52R87,-1B28S97B31S91B45S98B32S95A149B65S16B61S10B9S17B58S18B1"
				  "2S163B60S11B57S162B14S3B15S4B16S14B71S8B55S6B69S9B23S12B22S15B48S13B18S19B59S1B8S5B27S7A150B29S47B81"
				  "S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69S38B70S164B19S49B46S42B48S13B13S41B59S34B3"
				  "9S46A151B65S16B61S10B9S17B58S18B12S166B60S11B57S165B14S3B15S4B16S14B71S8B55S6B69S9B23S12B22S15B48S13"
				  "B18S19B59S1B8S5B27S7A152B29S167B48S125A153B12S168A154B11R88,-1B50R88,-1B40R88,-1B12R88,-1B30R88,-1B3"
				  "6R88,-1B14R88,-1B71R88,-1B69R88,-1B55R88,-1B22R88,-1B6R88,-1B33R88,-1B38R88,-1B18R88,-1B48R88,-1B37R"
				  "88,-1B52R88,-1B21R88,-1B8R88,-1B31R88,-1B32R88,-1B65R88,-1B9R88,-1B60R88,-1B56R88,-1B15R88,-1B4R88,-"
				  "1B34R88,-1B24R88,-1B16R88,-1B35R88,-1B10R88,-1B23R88,-1B0R88,-1B59R88,-1B7R88,-1B28R88,-1B26R88,-1B4"
				  "5R88,-1B27R88,-1A155B65R89,-1B11R89,-1B50R89,-1B9R89,-1B12R89,-1B60R89,-1B14R89,-1B15R89,-1B16R89,-1"
				  "B71R89,-1B10R89,-1B69R89,-1B55R89,-1B22R89,-1B23R89,-1B0R89,-1B18R89,-1B48R89,-1B59R89,-1B26R89,-1B8"
				  "R89,-1B27R89,-1A156B12S169A157B65R90,-1B11R90,-1B50R90,-1B9R90,-1B12R90,-1B60R90,-1B14R90,-1B15R90,-"
				  "1B16R90,-1B71R90,-1B10R90,-1B69R90,-1B55R90,-1B22R90,-1B23R90,-1B0R90,-1B18R90,-1B48R90,-1B59R90,-1B"
				  "26R90,-1B8R90,-1B27R90,-1A158B65R91,-1B11R91,-1B50R91,-1B9R91,-1B12R91,-1B60R91,-1B14R91,-1B15R91,-1"
				  "B16R91,-1B71R91,-1B10R91,-1B69R91,-1B55R91,-1B22R91,-1B23R91,-1B0R91,-1B18R91,-1B48R91,-1B59R91,-1B2"
				  "6R91,-1B8R91,-1B27R91,-1A159B12R92,-1A160B40S90B30S100B36S94B80S86B34S88B24S170B35S96B33S101B38S93B3"
				  "7S92B7S87B21S89B28S97B31S91B45S98B32S95A161B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S"
				  "37B55S6B69S38B70S171B19S49B46S42B48S13B13S41B59S34B39S46A162B12S172A163B65R93,-1B11R93,-1B50R93,-1B9"
				  "R93,-1B12R93,-1B60R93,-1B14R93,-1B15R93,-1B16R93,-1B71R93,-1B10R93,-1B69R93,-1B55R93,-1B22R93,-1B23R"
				  "93,-1B0R93,-1B18R93,-1B48R93,-1B59R93,-1B26R93,-1B8R93,-1B27R93,-1A164B40S90B9S175B30S100B36S94B80S8"
				  "6B34S88B64S173B35S96B33S101B38S93B37S92B7S87B21S89B52S174B28S97B31S91B45S98B32S95A165B12S176A166B11R"
				  "94,-1B50R94,-1B40R94,-1B12R94,-1B30R94,-1B36R94,-1B14R94,-1B71R94,-1B69R94,-1B55R94,-1B22R94,-1B6R94"
				  ",-1B33R94,-1B38R94,-1B18R94,-1B48R94,-1B37R94,-1B52R94,-1B21R94,-1B8R94,-1B31R94,-1B32R94,-1B65R94,-"
				  "1B9R94,-1B60R94,-1B56R94,-1B15R94,-1B4R94,-1B34R94,-1B24R94,-1B16R94,-1B35R94,-1B10R94,-1B23R94,-1B0"
				  "R94,-1B59R94,-1B7R94,-1B28R94,-1B26R94,-1B45R94,-1B27R94,-1A167B56R95,-1A168B11R96,-1B50R96,-1B40R96"
				  ",-1B12R96,-1B30R96,-1B36R96,-1B14R96,-1B71R96,-1B69R96,-1B55R96,-1B22R96,-1B6R96,-1B33R96,-1B38R96,-"
				  "1B18R96,-1B48R96,-1B37R96,-1B52R96,-1B21R96,-1B8R96,-1B31R96,-1B32R96,-1B65R96,-1B9R96,-1B60R96,-1B5"
				  "6R96,-1B15R96,-1B4R96,-1B34R96,-1B24R96,-1B16R96,-1B35R96,-1B10R96,-1B23R96,-1B0R96,-1B59R96,-1B7R96"
				  ",-1B28R96,-1B26R96,-1B45R96,-1B27R96,-1A169B65R97,-1B11R97,-1B50R97,-1B9R97,-1B12R97,-1B60R97,-1B14R"
				  "97,-1B15R97,-1B16R97,-1B71R97,-1B10R97,-1B69R97,-1B55R97,-1B22R97,-1B23R97,-1B0R97,-1B18R97,-1B48R97"
				  ",-1B59R97,-1B26R97,-1B8R97,-1B27R97,-1A170B65S16B11S142B61S10B9S17B58S18B12R98,-1B60S11B57S177B14S3B"
				  "15S4B63S178B16S14B71S8B10S141B55S6B69S9B23S12B22S15B48S13B18S19B59S1B8S5B27S7A171B40S90B50R99,-1B30S"
				  "100B36S94B4R99,-1B80S86B34S88B35S96B33S101B38S93B37S92B7S87B21S89B52R99,-1B28S97B31S91B45S98B32S95A1"
				  "72B65R100,-1B11R100,-1B50R100,-1B9R100,-1B12R100,-1B60R100,-1B14R100,-1B15R100,-1B16R100,-1B71R100,-"
				  "1B10R100,-1B69R100,-1B55R100,-1B22R100,-1B23R100,-1B0R100,-1B18R100,-1B48R100,-1B59R100,-1B26R100,-1"
				  "B8R100,-1B27R100,-1A173B9S179A174B29S47B81S48B75S40B25S35B15S36B3S20B20S43B47S44B35S45B71S37B55S6B69"
				  "S38B70S180B19S49B46S42B48S13B13S41B59S34B39S46A175B65S16B61S10B9S17B58S18B12S182B60S11B57S181B14S3B1"
				  "5S4B16S14B71S8B55S6B69S9B23S12B22S15B48S13B18S19B59S1B8S5B27S7A176B11R101,-1B50R101,-1B40R101,-1B12R"
				  "101,-1B30R101,-1B36R101,-1B14R101,-1B71R101,-1B69R101,-1B55R101,-1B22R101,-1B6R101,-1B33R101,-1B38R1"
				  "01,-1B18R101,-1B48R101,-1B37R101,-1B52R101,-1B21R101,-1B8R101,-1B31R101,-1B32R101,-1B65R101,-1B9R101"
				  ",-1B60R101,-1B56R101,-1B15R101,-1B4R101,-1B34R101,-1B24R101,-1B16R101,-1B35R101,-1B10R101,-1B23R101,"
				  "-1B0R101,-1B59R101,-1B7R101,-1B28R101,-1B26R101,-1B45R101,-1B27R101,-1A177B11S142B12R102,-1B63S183B1"
				  "0S141A178B12R103,-1A179B65S16B61S10B9S17B58S18B12S185B60S11B57S184B14S3B15S4B16S14B71S8B55S6B69S9B23"
				  "S12B22S15B48S13B18S19B59S1B8S5B27S7A180B40S90B9R104,-1B30S100B36S94B80S86B34S88B35S96B33S101B38S93B3"
				  "7S92B7S87B21S89B28S97B31S91B45S98B32S95A181B12S186A182B65R105,-1B11R105,-1B50R105,-1B9R105,-1B12R105"
				  ",-1B60R105,-1B14R105,-1B15R105,-1B16R105,-1B71R105,-1B10R105,-1B69R105,-1B55R105,-1B22R105,-1B23R105"
				  ",-1B0R105,-1B18R105,-1B48R105,-1B59R105,-1B26R105,-1B8R105,-1B27R105,-1A183B12R106,-1A184B12S187A185"
				  "B65R107,-1B11R107,-1B50R107,-1B9R107,-1B12R107,-1B60R107,-1B14R107,-1B15R107,-1B16R107,-1B71R107,-1B"
				  "10R107,-1B69R107,-1B55R107,-1B22R107,-1B23R107,-1B0R107,-1B18R107,-1B48R107,-1B59R107,-1B26R107,-1B8"
				  "R107,-1B27R107,-1A186B65R108,-1B11R108,-1B50R108,-1B9R108,-1B12R108,-1B60R108,-1B14R108,-1B15R108,-1"
				  "B16R108,-1B71R108,-1B10R108,-1B69R108,-1B55R108,-1B22R108,-1B23R108,-1B0R108,-1B18R108,-1B48R108,-1B"
				  "59R108,-1B26R108,-1B8R108,-1B27R108,-1A187B65R109,-1B11R109,-1B50R109,-1B9R109,-1B12R109,-1B60R109,-"
				  "1B14R109,-1B15R109,-1B16R109,-1B71R109,-1B10R109,-1B69R109,-1B55R109,-1B22R109,-1B23R109,-1B0R109,-1"
				  "B18R109,-1B48R109,-1B59R109,-1B26R109,-1B8R109,-1B27R109,-1"
			;

