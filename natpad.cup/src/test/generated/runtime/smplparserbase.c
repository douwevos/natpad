#include "smplparserbase.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SmplParserBase"
#include <logging/catlog.h>

G_DEFINE_TYPE(SmplParserBase, smpl_parser_base, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void smpl_parser_base_class_init(SmplParserBaseClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void smpl_parser_base_init(SmplParserBase *obj) {
}

static void _dispose(GObject *object) {
//	SmplParserBase *instance = SMPL_PARSER_BASE(object);
	// TODO
}


SmplParserBase *smpl_parser_base_new() {
	SmplParserBase *result = g_object_new(SMPL_TYPE_PARSER_BASE, NULL);
	cat_ref_anounce(result);
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
SmplToken *smpl_parser_base_parse(SmplParserBase *parser_base, SmplParserContext *parserContext) {
	/* the current action code */
	int act;

	/* the Symbol/stack element returned by a reduce */
	SmplToken *lhs_sym = NULL;

	/* information about production being reduced with */
	short handle_size, lhs_sym_num;


	const SmplParserBaseClass *base_class = SMPL_PARSER_BASE_GET_CLASS(parser_base);
	const SmplParserContextClass *parser_context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parserContext);


	/* get the first token */
	parser_context_class->scanNext(parserContext, base_class->eof_symbol(parser_base));

	/* push dummy Symbol with start state to get us underway */
	cat_array_clear(parserContext->stack);
	
	
	SmplIScannerInterface *scanner_iface = SMPL_ISCANNER_GET_INTERFACE(parserContext->scanner);
	
	SmplToken *start_token = scanner_iface->createToken(parserContext->scanner, 0, base_class->start_state(parser_base), FALSE, FALSE, FALSE, 0, 0, 0, 0, NULL);
	parser_context_class->push(parserContext, start_token);	
	cat_unref_ptr(start_token);
//	parser_context_class->push(parserContext, smpl_token_new_symbol_state(0, base_class->start_state(parser_base)));
	parserContext->tos = 0;

	/* continue until we are told to stop */
	parserContext->done_parsing = FALSE;
	while(!parserContext->done_parsing) {
		/* Check current token for freshness. */
//		if (parserContext->cur_token.used_by_parser)
//			throw new Error("Symbol recycling detected (fix your scanner).");

		/* current state is always on the top of the stack */

		

		/* look up action out of the current state with the current input */
		
		
		act = smpl_parser_base_get_action(parser_base, parser_context_class->peek(parserContext)->parse_state, parserContext->current_token->sym);
		cat_log_debug("act=%d", act);

		/* decode the action -- > 0 encodes shift */
		if (act > 0) {
			/* shift to the encoded state by pushing it on the stack */
			parserContext->current_token->parse_state = act - 1;
			parserContext->current_token->used_by_parser = TRUE;
			parser_context_class->shift(parserContext, parserContext->current_token);
			cat_unref_ptr(parserContext->current_token);

			/* advance to the next Symbol */
			parser_context_class->scanNext(parserContext, base_class->eof_symbol(parser_base));
		} else if (act < 0) {
			/* if its less than zero, then it encodes a reduce action */
			/* perform the action for the reduce */

			cat_log_debug("running action %d", ((-act) - 1));

			lhs_sym = base_class->run_action(parser_base, parserContext, (-act) - 1);

			cat_log_debug("lhs_sym %d", lhs_sym->sym);


			/* look up information about the production */
			lhs_sym_num = smpl_2d_array_get(parser_base->production_tab, (-act) - 1, 0);
			handle_size = smpl_2d_array_get(parser_base->production_tab, (-act) - 1, 1);

			act = smpl_parser_base_get_reduce(parser_base, parser_context_class->peek_at(parserContext, handle_size)->parse_state, lhs_sym_num);
			lhs_sym->parse_state = act;
			lhs_sym->used_by_parser = TRUE;

			parser_context_class->reduce(parserContext, handle_size, lhs_sym);


//			/* pop the handle off the stack */
//			int i;
//			for(i = 0; i < handle_size; i++) {
//				parser_context_class->pop(parserContext);
//			}
//
//			/* look up the state to go to from the one popped back to */
//			act = smpl_parser_base_get_reduce(parser_base, parser_context_class->peek(parserContext)->parse_state, lhs_sym_num);
//
//			/* shift to that state */
//			lhs_sym->parse_state = act;
//			lhs_sym->used_by_parser = TRUE;
//			parser_context_class->shift(parserContext, lhs_sym);
		} else if (act == 0) {
		/* finally if the entry is zero, we have an error */

			/* call user syntax error reporting routine */
//			smpl_parser_report_syntax_error(parserContext->current_token);

			/* try to error recover */
			if (!smpl_parser_base_error_recovery(parser_base, parserContext, FALSE)) {
				/* if that fails give up with a fatal syntax error */
//				smpl_parser_unrecovered_syntax_error(parser_base, parserContext, parserContext->current_token);

				/* just in case that wasn't fatal enough, end parse */
				smpl_parser_context_done_parsing(parserContext);
				lhs_sym = NULL;
			} else {
				lhs_sym = parser_context_class->peek(parserContext);
			}
		}
	}
	return lhs_sym;
}




short smpl_parser_base_get_action(SmplParserBase *parser_base, int state, int sym) {
	short tag;
	int first, last, probe;
	int row_length = 0;
	short *row = smpl_2d_array_get_row(parser_base->action_tab, state, &row_length);
	cat_log_detail("row[ %d ]=%p", state, row);

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
gboolean smpl_parser_base_error_recovery(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug) {
	SmplParserBaseClass *base_class = SMPL_PARSER_BASE_GET_CLASS(parser_base);

	cat_log_debug("# Attempting error recovery");

	if (parserContext->current_token) {
		parserContext->current_token->is_error = TRUE;		// TODO when to mark a token as error
	}

	/*
	 * first pop the stack back into a state that can shift on error and do
	 * that shift (if that fails, we fail)
	 */
	if (!smpl_parser_base_find_recovery_config(parser_base, parserContext, debug)) {
//		if (debug) cat_log_debug("# Error recovery fails");
		return FALSE;
	}

	/* read ahead to create lookahead we can parse multiple times */
	smpl_parser_base_read_lookahead(parser_base, parserContext);

	/* repeatedly try to parse forward until we make it the required dist */
	while(TRUE) {
		/* try to parse forward, if it makes it, bail out of loop */
		cat_log_debug("# Trying to parse ahead");
		if (smpl_parser_base_try_parse_ahead(parser_base, parserContext, debug)) {
			break;
		}

		/* if we are now at EOF, we have failed */
		SmplToken *bottomToken = (SmplToken *) cat_array_get(parserContext->lookahead, 0);
		if (bottomToken==NULL) {
			cat_log_debug("# Error recovery fails at NULL");
			return FALSE;
		}

		if (bottomToken->sym == base_class->eof_symbol(parser_base)) {
			cat_log_debug("# Error recovery fails at EOF");
			return FALSE;
		}

		/* otherwise, we consume another Symbol and try again */
		// BUG FIX by Bruce Hutton
		// Computer Science Department, University of Auckland,
		// Auckland, New Zealand.
		// It is the first token that is being consumed, not the one
		// we were up to parsing
//		cat_log_debug("# Consuming Symbol #" + parserContext->lookahead[0].sym);
		bottomToken->is_error = TRUE;
		smpl_parser_base_restart_lookahead(parser_base, parserContext);
	}

	/* we have consumed to a point where we can parse forward */
	cat_log_detail("# Parse-ahead ok, going back to normal parse");

	/* do the real parse (including actions) across the lookahead */
	smpl_parser_base_parse_lookahead(parser_base, parserContext, debug);

	/* we have success */
	return TRUE;
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
short smpl_parser_base_get_reduce(SmplParserBase *parser_base, int state, int sym) {
	short tag;
	int row_length;
	cat_log_detail("get reduce:sym=%d, state=%d", sym, state);
	short *row = smpl_2d_array_get_row(parser_base->reduce_tab, state, &row_length);
	cat_log_detail("get reduce_tab[ %d ]=%p", state, row);

	/* if we have a null row we go with the default */
	if (row == NULL)
		return -1;

	int probe;
	for(probe = 0; probe < row_length; probe++) {
		/* is this entry labeled with our Symbol or the default? */
		tag = row[probe++];
		cat_log_trace("tag=%d", tag);
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
gboolean smpl_parser_base_shift_under_error(SmplParserBase *parser_base, SmplParserContext *parserContext) {
	/* is there a shift under error Symbol */
	int errorSym = SMPL_PARSER_BASE_GET_CLASS(parser_base)->error_symbol(parser_base);
	SmplParserContextClass *parser_context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parserContext);
	return smpl_parser_base_get_action(parser_base, parser_context_class->peek(parserContext)->parse_state, errorSym) > 0;
}


/**
 * Put the (real) parse stack into error recovery configuration by popping
 * the stack down to a state that can shift on the special error Symbol,
 * then doing the shift. If no suitable state exists on the stack we return
 * false
 *
 * @param debug  should we produce debugging messages as we parse.
 */
gboolean smpl_parser_base_find_recovery_config(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug) {
	SmplToken *error_token = NULL;
	int act;
	SmplParserBaseClass *base_class = SMPL_PARSER_BASE_GET_CLASS(parser_base);
	SmplParserContextClass *parser_context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parserContext);

	cat_log_debug("# Finding recovery state on stack");

	/* Remember the right-position of the top symbol on the stack */
	int right_pos = parser_context_class->peek(parserContext)->right;
	int right_row = parser_context_class->peek(parserContext)->right_row;
	int left_pos = parser_context_class->peek(parserContext)->left;
	int left_row = parser_context_class->peek(parserContext)->left_row;

	/* pop down until we can shift under error Symbol */
	while (!smpl_parser_base_shift_under_error(parser_base, parserContext)) {
		/* pop the stack */
//		if (debug) cat_log_debug("# Pop stack by one, state was # " + parser_context_class->peek(parserContext).parse_state);
		error_token = parser_context_class->pop(parserContext);
		if (error_token!=NULL) {
			left_pos = error_token->left;
			left_row = error_token->left_row;
		}
		cat_unref_ptr(error_token);

		/* if we have hit bottom, we fail */
		if (cat_array_size(parserContext->stack)==0) {
			cat_log_debug("# No recovery state found on stack");
			return FALSE;
		}
	}

	/* state on top of the stack can shift under error, find the shift */
	act = smpl_parser_base_get_action(parser_base, parser_context_class->peek(parserContext)->parse_state, base_class->error_symbol(parser_base));
//	if (debug) {
//		cat_log_debug("# Recover state found (#" + parser_context_class->peek(parserContext).parse_state + ")");
//		cat_log_debug("# Shifting on error to state #" + (act - 1));
//	}

	/* build and shift a special error Symbol */
	SmplIScannerInterface *scanner_iface = SMPL_ISCANNER_GET_INTERFACE(parserContext->scanner);
	error_token = scanner_iface->createToken(parserContext->scanner, base_class->error_symbol(parser_base), act-1, FALSE, FALSE, TRUE, left_pos, left_row, right_pos, right_row, NULL);
//	error_token = smpl_token_new_full(base_class->error_symbol(parser_base), FALSE, left_pos, left_row, right_pos, right_row, NULL);
//	error_token->parse_state = act - 1;
//	error_token->used_by_parser = TRUE;
	parser_context_class->shift(parserContext, error_token);
	cat_unref_ptr(error_token);
	return TRUE;
}



/**
 * Read from input to establish our buffer of "parse ahead" lookahead
 * Symbols.
 */
void smpl_parser_base_read_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext) {
	/* create the lookahead array */
	if (parserContext->lookahead==NULL) {
		parserContext->lookahead = cat_array_new();
//		parserContext.lookahead = new SmplToken[smpl_parser_error_sync_size()];
	} else {
		cat_array_clear(parserContext->lookahead);
	}

	SmplParserBaseClass *base_class = SMPL_PARSER_BASE_GET_CLASS(parser_base);
	SmplParserContextClass *parser_context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parserContext);
	int eofSymbol = base_class->eof_symbol(parser_base);
	
	/* fill in the array */
	int i;
	for(i = 0; i < parser_base->error_sync_size; i++) {
		cat_array_append(parserContext->lookahead, (GObject *) parserContext->current_token);
		parser_context_class->scanNext(parserContext, eofSymbol);
	}

	/* start at the beginning */
	parserContext->lookahead_pos = 0;
}

/*
 * Advance to next "parse ahead" input Symbol. Return true if we have input
 * to advance to, false otherwise.
 */
gboolean smpl_parser_base_advance_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext) {
	/* advance the input location */
	parserContext->lookahead_pos++;
	/* return true if we didn't go off the end */
	return parserContext->lookahead_pos < parser_base->error_sync_size;
}



/**
 * Reset the parse ahead input to one Symbol past where we started error
 * recovery (this consumes one new Symbol from the real input).
 */
void smpl_parser_base_restart_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext) {
	/* move all the existing input over */
	
	int i;
	for(i = 1; i < parser_base->error_sync_size; i++) {
		cat_array_set_at(parserContext->lookahead, cat_array_get(parserContext->lookahead, i), i-1, NULL);
	}

	SmplParserContextClass *parser_context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parserContext);

	/* read a new Symbol into the last spot */
	// BUG Fix by Bruce Hutton
	// Computer Science Department, University of Auckland,
	// Auckland, New Zealand. [applied 5-sep-1999 by csa]
	// The following two lines were out of order!!
	cat_array_set_at(parserContext->lookahead, (GObject *) (parserContext->current_token), parser_base->error_sync_size-1, NULL);
	int eofSymbol = SMPL_PARSER_BASE_GET_CLASS(parser_base)->eof_symbol(parser_base);
	parser_context_class->scanNext(parserContext, eofSymbol);

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
gboolean smpl_parser_base_try_parse_ahead(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug) {
	SmplParserBaseClass *base_class = SMPL_PARSER_BASE_GET_CLASS(parser_base);

	gboolean result = TRUE;
	/* create a virtual stack from the real parse stack */
	SmplVirtualParseStack *vstack = smpl_virtual_parse_stack_new(parserContext->stack);


	SmplIScannerInterface *scanner_iface = SMPL_ISCANNER_GET_INTERFACE(parserContext->scanner);

	/* parse until we fail or get past the lookahead input */
	while(TRUE) {
		SmplToken *error_token = smpl_parser_context_current_error_token(parserContext);

		if (error_token==NULL) {
			result = FALSE;
			break;
		}

		/* look up the action from the current state (on top of stack) */
		int act = smpl_parser_base_get_action(parser_base, smpl_virtual_parser_stack_top(vstack), error_token->sym);

		/* if its an error, we fail */
		if (act == 0) {
			result = FALSE;
			break;
		}

		/* > 0 encodes a shift */
		if (act > 0) {
			/* push the new state on the stack */
			SmplToken *token2push = scanner_iface->createToken(parserContext->scanner, 0, act-1, FALSE, FALSE, FALSE, 0, 0, 0, 0, NULL);
			smpl_virtual_parser_stack_push(vstack, token2push);
			cat_unref_ptr(token2push);
			
//			smpl_virtual_parser_stack_push(vstack, smpl_token_new_symbol_state(0, act-1));

//			if (debug) cat_log_debug("# Parse-ahead shifts Symbol #" + smpl_parser_context_current_error_token(parserContext).sym + " into state #" + (act - 1));

			/* advance simulated input, if we run off the end, we are done */
			if (!smpl_parser_base_advance_lookahead(parser_base, parserContext)) {
				break;
			}
		} else {
		/* < 0 encodes a reduce */
			/* if this is a reduce with the start production we are done */
			if ((-act) - 1 == base_class->start_production(parser_base)) {
//				if (debug) cat_log_debug("# Parse-ahead accepts");
				break;
			}

			/* get the lhs Symbol and the rhs size */
			short lhs = smpl_2d_array_get(parser_base->production_tab, (-act) - 1, 0);
			short rhs_size = smpl_2d_array_get(parser_base->production_tab, (-act) - 1, 1);

			/* pop handle off the stack */
			int i;
			for(i = 0; i < rhs_size; i++) {
				smpl_virtual_parser_stack_pop(vstack);
			}

//			if (debug) cat_log_debug("# Parse-ahead reduces: handle size = " + rhs_size + " lhs = #" + lhs + " from state #" + vstack.smpl_parser_top());

			/* look up goto and push it onto the stack */

			int reduced_state = smpl_parser_base_get_reduce(parser_base, smpl_virtual_parser_stack_top(vstack), lhs);
			SmplToken *token2push = scanner_iface->createToken(parserContext->scanner, 0, reduced_state, FALSE, FALSE, FALSE, 0, 0, 0, 0, NULL);
			smpl_virtual_parser_stack_push(vstack, token2push);
			cat_unref_ptr(token2push);
//			smpl_virtual_parser_stack_push(vstack, smpl_token_new_symbol_state(0, reduced_state));
//			if (debug) cat_log_debug("# Goto state #" + vstack.smpl_parser_top());
		}
	}
	cat_unref_ptr(vstack);
	return result;
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
void smpl_parser_base_parse_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug) {
	SmplParserBaseClass *base_class = SMPL_PARSER_BASE_GET_CLASS(parser_base);
	SmplParserContextClass *parser_context_class = SMPL_PARSER_CONTEXT_GET_CLASS(parserContext);

	/* the current action code */
	int act;

	/* the Symbol/stack element returned by a reduce */
	SmplToken *lhs_sym = NULL;

	/* information about production being reduced with */
	short handle_size, lhs_sym_num;

	/* restart the saved input at the beginning */
	parserContext->lookahead_pos = 0;



//	if (debug) {
//		cat_log_debug("# Reparsing saved input with actions");
//		cat_log_debug("# Current Symbol is #" + smpl_parser_context_current_error_token(parserContext).sym);
//		cat_log_debug("# Current state is #" + parser_context_class->peek(parserContext)->parse_state);
//	}

	/* continue until we accept or have read all lookahead input */
	while (!parserContext->done_parsing) {
		/* current state is always on the top of the stack */

		/* look up action out of the current state with the current input */


		SmplToken *error_token = smpl_parser_context_current_error_token(parserContext);

		act = smpl_parser_base_get_action(parser_base, parser_context_class->peek(parserContext)->parse_state, error_token->sym);

		/* decode the action -- > 0 encodes shift */
		if (act > 0) {
			/* shift to the encoded state by pushing it on the stack */
			error_token->parse_state = act - 1;
			error_token->used_by_parser = TRUE;
//			if (debug) smpl_parser_debug_shift(error_token);
			parser_context_class->shift(parserContext, error_token);
//			cat_unref_ptr(parserContext->current_token);

			/* advance to the next Symbol, if there is none, we are done */
			if (!smpl_parser_base_advance_lookahead(parser_base, parserContext)) {
//				if (debug) cat_log_debug("# Completed reparse");

				/* scan next Symbol so we can continue parse */
				// BUGFIX by Chris Harris <ckharris@ucsd.edu>:
				// correct a one-off error by commenting out
				// this next line.
				/* cur_token = scan(); */

				/* go back to normal parser */
				return;
			}

//			if (debug) cat_log_debug("# Current Symbol is #" + smpl_parser_context_current_error_token(parserContext).sym);
		}
		/* if its less than zero, then it encodes a reduce action */
		else if (act < 0) {
			/* perform the action for the reduce */
			lhs_sym = base_class->run_action(parser_base, parserContext, (-act) - 1);

			/* look up information about the production */
			lhs_sym_num = smpl_2d_array_get(parser_base->production_tab, (-act) - 1, 0);
			handle_size = smpl_2d_array_get(parser_base->production_tab, (-act) - 1, 1);


//			if (debug) smpl_parser_debug_reduce((-act) - 1, lhs_sym_num, handle_size);

			act = smpl_parser_base_get_reduce(parser_base, parser_context_class->peek_at(parserContext, handle_size)->parse_state, lhs_sym_num);
			lhs_sym->parse_state = act;
			lhs_sym->used_by_parser = TRUE;

			parser_context_class->reduce(parserContext, handle_size, lhs_sym);




//			/* pop the handle off the stack */
//			int i;
//			for(i = 0; i < handle_size; i++) {
//				parser_context_class->pop(parserContext);
//			}
//
//			/* look up the state to go to from the one popped back to */
//			act = smpl_parser_base_get_reduce(parser_base, parser_context_class->peek(parserContext)->parse_state, lhs_sym_num);
//
//			/* shift to that state */
//			lhs_sym->parse_state = act;
//			lhs_sym->used_by_parser = TRUE;
//			parser_context_class->shift(parserContext, lhs_sym);
//
////			if (debug) cat_log_debug("# Goto state #" + act);
		}
		/*
		 * finally if the entry is zero, we have an error (shouldn't happen
		 * here, but...)
		 */
		else if (act == 0) {
//			smpl_parser_base_report_fatal_error(parserContext, "Syntax error", lhs_sym);
			return;
		}
	}

}

