#include "[%pre%]parserbase.h"
#include <stdlib.h>
#include <stdio.h>

[%Pre%]ParserBaseClass *[%pre%]_parser_base_get_class(void *ptr_to_obj) {
	return ([%Pre%]ParserBaseClass *) *((void **) ptr_to_obj);
}



void [%pre%]_parser_base_init([%Pre%]ParserBase *parser_base) {
	parser_base->production_tab = NULL;
	parser_base->action_tab = NULL;
	parser_base->reduce_tab = NULL;
	parser_base->error_sync_size = 5;
	parser_base->scanner = NULL;
	parser_base->scanner_data = NULL;
}


static void _dispose([%Pre%]ParserBase *parser_base) {
//	[%Pre%]ParserBase *instance = [%PRE%]_PARSER_BASE(object);
	// TODO
}


[%Pre%]ParserBase *[%pre%]_parser_base_new() {
	[%Pre%]ParserBase *result = ([%Pre%]ParserBase *) malloc(sizeof([%Pre%]ParserBase));
	result->error_sync_size = 5;
	return result;
}




/**
 * This method provides the main parsing routine. It returns only when
 * done_parsing() has been called (typically because the parser has
 * accepted, or a fatal error has been reported). See the header
 * documentation for the class regarding how shift/reduce parsers operate
 * and how the various tables are used.
 */
[%Pre%]Token *[%pre%]_parser_base_parse([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext) {
	/* the current action code */
	int act;

	/* the Symbol/stack element returned by a reduce */
	[%Pre%]Token *lhs_sym = NULL;

	/* information about production being reduced with */
	short handle_size, lhs_sym_num;

	/* set up direct reference to tables to drive the parser */

	[%Pre%]ParserBaseClass *base_class = [%pre%]_parser_base_get_class(parser_base);


//	/* initialize the action encapsulation object */
//	[%pre%]_parser_init_actions();
//
//	/* do user initialization */
//	[%pre%]_parser_user_init();

	/* get the first token */
	parserContext->current_token = [%pre%]_parser_base_scan(parser_base, parserContext);

	/* push dummy Symbol with start state to get us underway */
	[%pre%]_vector_clear(parserContext->stack);
	[%pre%]_parser_context_push(parserContext, [%pre%]_token_new_symbol_state(0, base_class->start_state(parser_base)));
	parserContext->tos = 0;

	/* continue until we are told to stop */
	parserContext->done_parsing = [%PRE%]FALSE;
	while(!parserContext->done_parsing) {
		/* Check current token for freshness. */
//		if (parserContext->cur_token.used_by_parser)
//			throw new Error("Symbol recycling detected (fix your scanner).");

		/* current state is always on the top of the stack */

		[%pre%]_parser_context_debug(parserContext);
		[%if-set:debug%]
		 [%pre%]_token_dump([%pre%]_parser_context_peek(parserContext));

		if (parserContext->current_token!=NULL) {
			[%pre%]_token_dump(parserContext->current_token);
		}
		[%end-if:debug%]

		/* look up action out of the current state with the current input */
		act = [%pre%]_parser_base_get_action(parser_base, [%pre%]_parser_context_peek(parserContext)->parse_state, parserContext->current_token->sym);
		printf("act=%d\n", act);

		/* decode the action -- > 0 encodes shift */
		if (act > 0) {
			/* shift to the encoded state by pushing it on the stack */
			parserContext->current_token->parse_state = act - 1;
			parserContext->current_token->used_by_parser = [%PRE%]TRUE;
			[%pre%]_parser_context_shift(parserContext, parserContext->current_token);

			/* advance to the next Symbol */
			parserContext->current_token = [%pre%]_parser_base_scan(parser_base, parserContext);
		} else if (act < 0) {
			/* if its less than zero, then it encodes a reduce action */
			/* perform the action for the reduce */

			printf("running action %d\n", ((-act) - 1));

			lhs_sym = base_class->action_cb(parserContext, (-act) - 1);

			printf("lhs_sym %d[%if-set:debug%], %s[%end-if:debug%]\n", lhs_sym->sym, [%if-set:debug%]lhs_sym->symbol_text[%end-if:debug%]);


			/* look up information about the production */
			lhs_sym_num = [%pre%]_2d_array_get(parser_base->production_tab, (-act) - 1, 0);
			handle_size = [%pre%]_2d_array_get(parser_base->production_tab, (-act) - 1, 1);

			/* pop the handle off the stack */
			int i;
			for(i = 0; i < handle_size; i++) {
				[%pre%]_parser_context_pop(parserContext);
			}

			/* look up the state to go to from the one popped back to */
			act = [%pre%]_parser_base_get_reduce(parser_base, [%pre%]_parser_context_peek(parserContext)->parse_state, lhs_sym_num);

			/* shift to that state */
			lhs_sym->parse_state = act;
			lhs_sym->used_by_parser = [%PRE%]TRUE;
			[%pre%]_parser_context_shift(parserContext, lhs_sym);
		} else if (act == 0) {
		/* finally if the entry is zero, we have an error */

			/* call user syntax error reporting routine */
//			[%pre%]_parser_report_syntax_error(parserContext->current_token);

			/* try to error recover */
			if (![%pre%]_parser_base_error_recovery(parser_base, parserContext, [%PRE%]FALSE)) {
				/* if that fails give up with a fatal syntax error */
//				[%pre%]_parser_unrecovered_syntax_error(parser_base, parserContext, parserContext->current_token);

				/* just in case that wasn't fatal enough, end parse */
				[%pre%]_parser_context_done_parsing(parserContext);
			} else {
				lhs_sym = [%pre%]_parser_context_peek(parserContext);
			}
		}
	}
	return lhs_sym;
}




/**
 * Fetch an action from the action table. The table is broken up into rows,
 * one per state (rows are indexed directly by state number). Within each
 * row, a list of index, value pairs are given (as sequential entries in the
 * table), and the list is terminated by a default entry (denoted with a
 * Symbol index of -1). To find the proper entry in a row we do a linear or
 * binary search (depending on the size of the row).
 *
 * @param state   the state index of the action being accessed.
 * @param sym     the Symbol index of the action being accessed.
 */
short [%pre%]_parser_base_get_action([%Pre%]ParserBase *parser_base, int state, int sym) {
	short tag;
	int first, last, probe;
	int row_length = 0;
	short *row = [%pre%]_2d_array_get_row(parser_base->action_tab, state, &row_length);
	printf("row[ %d ]=%lx\n", state, (unsigned long) row);

	/* linear search if we are < 10 entries */
	if (row_length < 20) {
		for (probe = 0; probe < row_length; probe++) {
			/* is this entry labeled with our Symbol or the default? */
			tag = row[probe++];
			if (tag == sym || tag == -1) {
				/* return the next entry */
				return row[probe];
			}
		}
	/* otherwise binary search */
	} else {
		first = 0;
		last = (row_length - 1) / 2 - 1; /* leave out trailing default entry */
		while (first <= last) {
			probe = (first + last) / 2;
			if (sym == row[probe * 2]) {
				return row[probe * 2 + 1];
			} else if (sym > row[probe * 2]) {
				first = probe + 1;
			} else {
				last = probe - 1;
			}
		}

		/* not found, use the default at the end */
		return row[row_length - 1];
	}

	/*
	 * shouldn't happened, but if we run off the end we return the default
	 * (error == 0)
	 */
	return 0;
}



/**
 * Get the next Symbol from the input (supplied by generated subclass). Once
 * end of file has been reached, all subsequent calls to scan should return
 * an EOF Symbol (which is Symbol number 0). By default this method returns
 * getScanner().next_token(); this implementation can be overriden by the
 * generated parser using the code declared in the "scan with" clause. Do
 * not recycle objects; every call to scan() should return a fresh object.
 */
[%Pre%]Token *[%pre%]_parser_base_scan([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext) {
	[%Pre%]Token *sym = parser_base->scanner(parser_base->scanner_data);
	if (sym==NULL) {
		int eofSymbol = [%pre%]_parser_base_get_class(parser_base)->eof_symbol(parser_base);
		sym = [%pre%]_token_new_symbol(eofSymbol);[%if-set:debug%]
		sym->symbol_text= "eof"; [%end-if:debug%]
	}
	return sym;
}





/**
 * Attempt to recover from a syntax error. This returns false if recovery
 * fails, true if it succeeds. Recovery happens in 4 steps. First we pop the
 * parse stack down to a point at which we have a shift out of the top-most
 * state on the error Symbol. This represents the initial error recovery
 * configuration. If no such configuration is found, then we fail. Next a
 * small number of "lookahead" or "parse ahead" Symbols are read into a
 * buffer. The size of this buffer is determined by error_sync_size() and
 * determines how many Symbols beyond the error must be matched to consider
 * the recovery a success. Next, we begin to discard Symbols in attempt to
 * get past the point of error to a point where we can continue parsing.
 * After each Symbol, we attempt to "parse ahead" though the buffered
 * lookahead Symbols. The "parse ahead" process simulates that actual parse,
 * but does not modify the real parser's configuration, nor execute any
 * actions. If we can parse all the stored Symbols without error, then the
 * recovery is considered a success. Once a successful recovery point is
 * determined, we do an actual parse over the stored input -- modifying the
 * real parse configuration and executing all actions. Finally, we return
 * the the normal parser to continue with the overall parse.
 *
 * @param debug  should we produce debugging messages as we parse.
 */
[%pre%]boolean [%pre%]_parser_base_error_recovery([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug) {
	[%Pre%]ParserBaseClass *base_class = [%pre%]_parser_base_get_class(parser_base);

	printf("# Attempting error recovery\n");

	/*
	 * first pop the stack back into a state that can shift on error and do
	 * that shift (if that fails, we fail)
	 */
	if (![%pre%]_parser_base_find_recovery_config(parser_base, parserContext, debug)) {
//		if (debug) printf("# Error recovery fails");
		return [%PRE%]FALSE;
	}

	if (parserContext->current_token) {
		parserContext->current_token->is_error = [%PRE%]TRUE;
	}
	/* read ahead to create lookahead we can parse multiple times */
	[%pre%]_parser_base_read_lookahead(parser_base, parserContext);

	/* repeatedly try to parse forward until we make it the required dist */
	while([%PRE%]TRUE) {
		/* try to parse forward, if it makes it, bail out of loop */
		printf("# Trying to parse ahead\n");
		if ([%pre%]_parser_base_try_parse_ahead(parser_base, parserContext, debug)) {
			break;
		}

		/* if we are now at EOF, we have failed */
		[%Pre%]Token *bottomToken = ([%Pre%]Token *) [%pre%]_vector_get_at(parserContext->lookahead, 0);
		if (bottomToken==NULL) {
			printf("# Error recovery fails at NULL\n");
			return [%PRE%]FALSE;
		}

		if (bottomToken->sym == base_class->eof_symbol(parser_base)) {
			printf("# Error recovery fails at EOF\n");
			return [%PRE%]FALSE;
		}

		/* otherwise, we consume another Symbol and try again */
		// BUG FIX by Bruce Hutton
		// Computer Science Department, University of Auckland,
		// Auckland, New Zealand.
		// It is the first token that is being consumed, not the one
		// we were up to parsing
//		printf("# Consuming Symbol #" + parserContext->lookahead[0].sym);
		bottomToken->is_error = [%PRE%]TRUE;
		[%pre%]_parser_base_restart_lookahead(parser_base, parserContext);
	}

	/* we have consumed to a point where we can parse forward */
	printf("# Parse-ahead ok, going back to normal parse\n");

	/* do the real parse (including actions) across the lookahead */
	[%pre%]_parser_base_parse_lookahead(parser_base, parserContext, debug);

	/* we have success */
	return [%PRE%]TRUE;
}




/**
 * Fetch a state from the reduce-goto table. The table is broken up into
 * rows, one per state (rows are indexed directly by state number). Within
 * each row, a list of index, value pairs are given (as sequential entries
 * in the table), and the list is terminated by a default entry (denoted
 * with a Symbol index of -1). To find the proper entry in a row we do a
 * linear search.
 *
 * @param state the state index of the entry being accessed.
 * @param sym   the Symbol index of the entry being accessed.
 */
short [%pre%]_parser_base_get_reduce([%Pre%]ParserBase *parser_base, int state, int sym) {
	short tag;
	int row_length;
	printf("get reduce:sym=%d, state=%d\n", sym, state);
	short *row = [%pre%]_2d_array_get_row(parser_base->reduce_tab, state, &row_length);
	printf("get reduce_tab[ %d ]=%lx\n", state, (unsigned long) row);

	/* if we have a null row we go with the default */
	if (row == NULL)
		return -1;

	int probe;
	for(probe = 0; probe < row_length; probe++) {
		/* is this entry labeled with our Symbol or the default? */
		tag = row[probe++];
		printf("tag=%d\n", tag);
		if (tag == sym || tag == -1) {
			/* return the next entry */
			return row[probe];
		}
	}
	/* if we run off the end we return the default (error == -1) */
	return -1;
}


/**
 * Determine if we can shift under the special error Symbol out of the state
 * currently on the top of the (real) parse stack.
 */
[%pre%]boolean [%pre%]_parser_base_shift_under_error([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext) {
	/* is there a shift under error Symbol */
	int errorSym = [%pre%]_parser_base_get_class(parser_base)->error_symbol(parser_base);
	return [%pre%]_parser_base_get_action(parser_base, [%pre%]_parser_context_peek(parserContext)->parse_state, errorSym) > 0;
}


/**
 * Put the (real) parse stack into error recovery configuration by popping
 * the stack down to a state that can shift on the special error Symbol,
 * then doing the shift. If no suitable state exists on the stack we return
 * false
 *
 * @param debug  should we produce debugging messages as we parse.
 */
[%pre%]boolean [%pre%]_parser_base_find_recovery_config([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug) {
	[%Pre%]Token *error_token;
	int act;
	[%Pre%]ParserBaseClass *base_class = [%pre%]_parser_base_get_class(parser_base);

	printf("# Finding recovery state on stack\n");

	/* Remember the right-position of the top symbol on the stack */
	int right_pos = [%pre%]_parser_context_peek(parserContext)->right;
	int left_pos = [%pre%]_parser_context_peek(parserContext)->left;
	int row = [%pre%]_parser_context_peek(parserContext)->row;

	/* pop down until we can shift under error Symbol */
	while (![%pre%]_parser_base_shift_under_error(parser_base, parserContext)) {
		/* pop the stack */
//		if (debug) printf("# Pop stack by one, state was # " + [%pre%]_parser_context_peek(parserContext).parse_state);
		error_token = [%pre%]_parser_context_pop(parserContext);
		if (error_token!=NULL) {
			left_pos = error_token->left;
			row = error_token->row;
		}

		/* if we have hit bottom, we fail */
		if ([%pre%]_vector_count(parserContext->stack)==0) {
			printf("# No recovery state found on stack\n");
			return [%PRE%]FALSE;
		}
	}

	/* state on top of the stack can shift under error, find the shift */
	act = [%pre%]_parser_base_get_action(parser_base, [%pre%]_parser_context_peek(parserContext)->parse_state, base_class->error_symbol(parser_base));
//	if (debug) {
//		printf("# Recover state found (#" + [%pre%]_parser_context_peek(parserContext).parse_state + ")");
//		printf("# Shifting on error to state #" + (act - 1));
//	}

	/* build and shift a special error Symbol */
	error_token = [%pre%]_token_new_symbol_pos(base_class->error_symbol(parser_base), left_pos, right_pos, row);[%if-set:debug%]
	error_token->symbol_text= "error"; [%end-if:debug%]
	error_token->parse_state = act - 1;
	error_token->used_by_parser = [%PRE%]TRUE;
	[%pre%]_parser_context_shift(parserContext, error_token);
	return [%PRE%]TRUE;
}



/**
 * Read from input to establish our buffer of "parse ahead" lookahead
 * Symbols.
 */
void [%pre%]_parser_base_read_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext) {
	/* create the lookahead array */
	if (parserContext->lookahead==NULL) {
		parserContext->lookahead = [%pre%]_vector_new(NULL);
//		parserContext.lookahead = new [%Pre%]Token[[%pre%]_parser_error_sync_size()];
	} else {
		[%pre%]_vector_clear(parserContext->lookahead);
	}



	/* fill in the array */
	int i;
	for(i = 0; i < parser_base->error_sync_size; i++) {
		[%pre%]_vector_add(parserContext->lookahead, parserContext->current_token);
		parserContext->current_token = [%pre%]_parser_base_scan(parser_base, parserContext);
	}

	/* start at the beginning */
	parserContext->lookahead_pos = 0;
}

/*
 * Advance to next "parse ahead" input Symbol. Return true if we have input
 * to advance to, false otherwise.
 */
[%pre%]boolean [%pre%]_parser_base_advance_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext) {
	/* advance the input location */
	parserContext->lookahead_pos++;
	/* return true if we didn't go off the end */
	return parserContext->lookahead_pos < parser_base->error_sync_size;
}



/**
 * Reset the parse ahead input to one Symbol past where we started error
 * recovery (this consumes one new Symbol from the real input).
 */
void [%pre%]_parser_base_restart_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext) {
	/* move all the existing input over */
	int i;
	// TODO shifting of lookahead buffer ... first element needs freeeing
	for(i = 1; i < parser_base->error_sync_size; i++) {
		[%pre%]_vector_set_at(parserContext->lookahead, [%pre%]_vector_get_at(parserContext->lookahead, i), i-1);
	}

	/* read a new Symbol into the last spot */
	// BUG Fix by Bruce Hutton
	// Computer Science Department, University of Auckland,
	// Auckland, New Zealand. [applied 5-sep-1999 by csa]
	// The following two lines were out of order!!
	[%pre%]_vector_set_at(parserContext->lookahead, (parserContext->current_token), parser_base->error_sync_size-1);
	parserContext->current_token = [%pre%]_parser_base_scan(parser_base, parserContext);

	/* reset our internal position marker */
	parserContext->lookahead_pos = 0;
}



/**
 * Do a simulated parse forward (a "parse ahead") from the current stack
 * configuration using stored lookahead input and a virtual parse stack.
 * Return true if we make it all the way through the stored lookahead input
 * without error. This basically simulates the action of parse() using only
 * our saved "parse ahead" input, and not executing any actions.
 *
 * @param debug   should we produce debugging messages as we parse.
 */
[%pre%]boolean [%pre%]_parser_base_try_parse_ahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug) {
	[%Pre%]ParserBaseClass *base_class = [%pre%]_parser_base_get_class(parser_base);

	/* create a virtual stack from the real parse stack */
	[%Pre%]VirtualParseStack *vstack = [%pre%]_virtual_parse_stack_new(parserContext->stack);


	/* parse until we fail or get past the lookahead input */
	while([%PRE%]TRUE) {
		[%Pre%]Token *error_token = [%pre%]_parser_context_current_error_token(parserContext);

		if (error_token==NULL) {
			return [%PRE%]FALSE;
		}

		/* look up the action from the current state (on top of stack) */
		int act = [%pre%]_parser_base_get_action(parser_base, [%pre%]_virtual_parser_stack_top(vstack), error_token->sym);

		/* if its an error, we fail */
		if (act == 0) {
			return [%PRE%]FALSE;
		}

		/* > 0 encodes a shift */
		if (act > 0) {
			/* push the new state on the stack */
			[%pre%]_virtual_parser_stack_push(vstack, [%pre%]_token_new_symbol_state(0, act-1));

//			if (debug) printf("# Parse-ahead shifts Symbol #" + [%pre%]_parser_context_current_error_token(parserContext).sym + " into state #" + (act - 1));

			/* advance simulated input, if we run off the end, we are done */
			if (![%pre%]_parser_base_advance_lookahead(parser_base, parserContext))
				return [%PRE%]TRUE;
		} else {
		/* < 0 encodes a reduce */
			/* if this is a reduce with the start production we are done */
			if ((-act) - 1 == base_class->start_production(parser_base)) {
//				if (debug) printf("# Parse-ahead accepts");
				return [%PRE%]TRUE;
			}

			/* get the lhs Symbol and the rhs size */
			short lhs = [%pre%]_2d_array_get(parser_base->production_tab, (-act) - 1, 0);
			short rhs_size = [%pre%]_2d_array_get(parser_base->production_tab, (-act) - 1, 1);

			/* pop handle off the stack */
			int i;
			for(i = 0; i < rhs_size; i++) {
				[%pre%]_virtual_parser_stack_pop(vstack);
			}

//			if (debug) printf("# Parse-ahead reduces: handle size = " + rhs_size + " lhs = #" + lhs + " from state #" + vstack.[%pre%]_parser_top());

			/* look up goto and push it onto the stack */

			int reduced_state = [%pre%]_parser_base_get_reduce(parser_base, [%pre%]_virtual_parser_stack_top(vstack), lhs);
			[%pre%]_virtual_parser_stack_push(vstack, [%pre%]_token_new_symbol_state(0, reduced_state));
//			if (debug) printf("# Goto state #" + vstack.[%pre%]_parser_top());
		}
	}
}


/**
 * Parse forward using stored lookahead Symbols. In this case we have
 * already verified that parsing will make it through the stored lookahead
 * Symbols and we are now getting back to the point at which we can hand
 * control back to the normal parser. Consequently, this version of the
 * parser performs all actions and modifies the real parse configuration.
 * This returns once we have consumed all the stored input or we accept.
 *
 * @param debug should we produce debugging messages as we parse.
 */
void [%pre%]_parser_base_parse_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug) {
	[%Pre%]ParserBaseClass *base_class = [%pre%]_parser_base_get_class(parser_base);

	/* the current action code */
	int act;

	/* the Symbol/stack element returned by a reduce */
	[%Pre%]Token *lhs_sym = NULL;

	/* information about production being reduced with */
	short handle_size, lhs_sym_num;

	/* restart the saved input at the beginning */
	parserContext->lookahead_pos = 0;

//	if (debug) {
//		printf("# Reparsing saved input with actions");
//		printf("# Current Symbol is #" + [%pre%]_parser_context_current_error_token(parserContext).sym);
//		printf("# Current state is #" + [%pre%]_parser_context_peek(parserContext)->parse_state);
//	}

	/* continue until we accept or have read all lookahead input */
	while (!parserContext->done_parsing) {
		/* current state is always on the top of the stack */

		/* look up action out of the current state with the current input */


		[%Pre%]Token *error_token = [%pre%]_parser_context_current_error_token(parserContext);

		act = [%pre%]_parser_base_get_action(parser_base, [%pre%]_parser_context_peek(parserContext)->parse_state, error_token->sym);

		/* decode the action -- > 0 encodes shift */
		if (act > 0) {
			/* shift to the encoded state by pushing it on the stack */
			error_token->parse_state = act - 1;
			error_token->used_by_parser = [%PRE%]TRUE;
//			if (debug) [%pre%]_parser_debug_shift(error_token);
			[%pre%]_parser_context_shift(parserContext, error_token);

			/* advance to the next Symbol, if there is none, we are done */
			if (![%pre%]_parser_base_advance_lookahead(parser_base, parserContext)) {
//				if (debug) printf("# Completed reparse");

				/* scan next Symbol so we can continue parse */
				// BUGFIX by Chris Harris <ckharris@ucsd.edu>:
				// correct a one-off error by commenting out
				// this next line.
				/* cur_token = scan(); */

				/* go back to normal parser */
				return;
			}

//			if (debug) printf("# Current Symbol is #" + [%pre%]_parser_context_current_error_token(parserContext).sym);
		}
		/* if its less than zero, then it encodes a reduce action */
		else if (act < 0) {
			/* perform the action for the reduce */
			lhs_sym = base_class->action_cb(parserContext, (-act) - 1);

			/* look up information about the production */
			lhs_sym_num = [%pre%]_2d_array_get(parser_base->production_tab, (-act) - 1, 0);
			handle_size = [%pre%]_2d_array_get(parser_base->production_tab, (-act) - 1, 1);


//			if (debug) [%pre%]_parser_debug_reduce((-act) - 1, lhs_sym_num, handle_size);

			/* pop the handle off the stack */
			int i;
			for(i = 0; i < handle_size; i++) {
				[%pre%]_parser_context_pop(parserContext);
			}

			/* look up the state to go to from the one popped back to */
			act = [%pre%]_parser_base_get_reduce(parser_base, [%pre%]_parser_context_peek(parserContext)->parse_state, lhs_sym_num);

			/* shift to that state */
			lhs_sym->parse_state = act;
			lhs_sym->used_by_parser = [%PRE%]TRUE;
			[%pre%]_parser_context_shift(parserContext, lhs_sym);

//			if (debug) printf("# Goto state #" + act);
		}
		/*
		 * finally if the entry is zero, we have an error (shouldn't happen
		 * here, but...)
		 */
		else if (act == 0) {
//			[%pre%]_parser_base_report_fatal_error(parserContext, "Syntax error", lhs_sym);
			return;
		}
	}

}

