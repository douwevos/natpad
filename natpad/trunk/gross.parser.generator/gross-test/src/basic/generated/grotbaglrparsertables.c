#include "grotbaglrparsertables.h"

/** Production table. */
const int grotba_glr_parser_tables_production_table[] = {
	/* the number of rows, default_value */
	12, -1,
	/* the number of columns for each row */
	2,2,2,2,2,2,2,2,2,2,2,2,
	/* the raw table data */
/* 0 */	0,2
/* 1 */	,1,5
/* 2 */	,1,5
/* 3 */	,1,6
/* 4 */	,1,6
/* 5 */	,2,2
/* 6 */	,2,1
/* 7 */	,2,2
/* 8 */	,3,3
/* 9 */	,6,3
/* 10 */	,4,4
/* 11 */	,5,3
};

/** action table. */
const int grotba_glr_parser_tables_action_table[] = {
	/* the number of rows, default_value */
	24, 1,
	/* the number of columns for each row */
	2,2,2,2,2,2,2,2,4,2,2,2,4,2,2,2,2,2,2,2,4,2,2,2,
	/* the raw table data */
/* 0 */	2,3
/* 1 */	,0,24
/* 2 */	,2,4
/* 3 */	,2,9
/* 4 */	,7,21
/* 5 */	,6,20
/* 6 */	,7,-7
/* 7 */	,2,19
/* 8 */	,1,11,3,12
/* 9 */	,7,17
/* 10 */	,7,15
/* 11 */	,4,13
/* 12 */	,2,-12,6,10001
/* 13 */	,7,10000
/* 14 */	,7,16
/* 15 */	,0,-5
/* 16 */	,7,18
/* 17 */	,0,-4
/* 18 */	,7,-8
/* 19 */	,7,-6
/* 20 */	,5,22,7,23
/* 21 */	,0,-3
/* 22 */	,0,-2
/* 23 */	,0,-1
};

/** fork table. */
const int grotba_glr_parser_tables_fork_table[] = {
	/* the number of rows, default_value */
	2, 0,
	/* the number of columns for each row */
	2,2,
	/* the raw table data */
/* 0 */	-10,-11
/* 1 */	,-9,14
};

/** reduce table. */
const int grotba_glr_parser_tables_reduce_table[] = {
	/* the number of rows, default_value */
	24, 1,
	/* the number of columns for each row */
	2,0,0,8,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	/* the raw table data */
/* 0 */	1,1
/* 1 */	
/* 2 */	
/* 3 */	,2,4,3,5,4,6,5,7
/* 4 */	
/* 5 */	
/* 6 */	
/* 7 */	
/* 8 */	,6,9
/* 9 */	
/* 10 */	
/* 11 */	
/* 12 */	
/* 13 */	
/* 14 */	
/* 15 */	
/* 16 */	
/* 17 */	
/* 18 */	
/* 19 */	
/* 20 */	
/* 21 */	
/* 22 */	
/* 23 */	
};


GroParserConfig grotba_glr_parser_tables_definitions = {
	grotba_glr_parser_tables_production_table,
	grotba_glr_parser_tables_action_table,
	grotba_glr_parser_tables_reduce_table,
	grotba_glr_parser_tables_fork_table,
	0,	/* start-state */
	0,	/* start-production */
	0,	/* EOF-terminal */
	1	/* error-terminal */
};
