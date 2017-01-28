package net.natpad.cup.state;

import java.io.PrintStream;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.ParseActionRow;
import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceRow;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.TerminalSet;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.Terminal;

/**
 * This class represents a state in the LALR viable prefix recognition machine.
 * A state consists of an LALR item set and a set of transitions to other states
 * under terminal and non-terminal symbols. Each state represents a potential
 * configuration of the parser. If the item set of a state includes an item such
 * as:
 * 
 * <pre>
 *    [A ::= B * C d E , {a,b,c}]
 * </pre>
 * 
 * this indicates that when the parser is in this state it is currently looking
 * for an A of the given form, has already seen the B, and would expect to see
 * an a, b, or c after this sequence is complete. Note that the parser is
 * normally looking for several things at once (represented by several items).
 * In our example above, the state would also include items such as:
 * 
 * <pre>
 *    [C ::= * X e Z, {d}]
 *    [X ::= * f, {e}]
 * </pre>
 * 
 * to indicate that it was currently looking for a C followed by a d (which
 * would be reduced into a C, matching the first symbol in our production
 * above), and the terminal f followed by e.
 * <p>
 * 
 * At runtime, the parser uses a viable prefix recognition machine made up of
 * these states to parse. The parser has two operations, shift and reduce. In a
 * shift, it consumes one Symbol and makes a transition to a new state. This
 * corresponds to "moving the dot past" a terminal in one or more items in the
 * state (these new shifted items will then be found in the state at the end of
 * the transition). For a reduce operation, the parser is signifying that it is
 * recognizing the RHS of some production. To do this it first "backs up" by
 * popping a stack of previously saved states. It pops off the same number of
 * states as are found in the RHS of the production. This leaves the machine in
 * the same state is was in when the parser first attempted to find the RHS.
 * From this state it makes a transition based on the non-terminal on the LHS of
 * the production. This corresponds to placing the parse in a configuration
 * equivalent to having replaced all the symbols from the the input
 * corresponding to the RHS with the symbol on the LHS.
 * 
 * @see net.natpad.cup.state.LalrItem
 * @see net.natpad.cup.state.LalrItemSet
 * @see net.natpad.cup.state.LalrTransition
 * @version last updated: 7/3/96
 * @author Frank Flannery
 * 
 */

public class LalrState {

	int warningCount;
	
	/** Index of this state in the parse tables */
	public final int _index;

	/** The item set for this state. */
	protected final LalrItemSet _items;

	
	/**
	 * Constructor for building a state from a set of items.
	 * 
	 * @param itms    the set of items that makes up this state.
	 */
	public LalrState(LalrItemSet itms, int index) {
		/* don't allow null or duplicate item sets */
		if (itms == null) {
			throw new FatalCupException("Attempt to construct an LALR state from a null item set");
		}

		/* assign a unique index */
		_index = index;

		/* store the items */
		_items = itms;

	}


	/** The item set for this state. */
	public LalrItemSet items() {
		return _items;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** List of transitions out of this state. */
	protected LalrTransition _transitions = null;

	/** List of transitions out of this state. */
	public LalrTransition transitions() {
		return _transitions;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/** Index of this state in the parse tables */
	public int index() {
		return _index;
	}

	/*-----------------------------------------------------------*/
	/*--- Static Methods ----------------------------------------*/
	/*-----------------------------------------------------------*/

//	/**
//	 * Helper routine for debugging -- produces a dump of the given state onto
//	 * System.out.
//	 */
//	public static void dump_state(LalrState st) {
//		LalrItemSet itms;
//		ProductionPart part;
//
//		if (st == null) {
//			System.out.println("NULL lalr_state");
//			return;
//		}
//
//		System.out.println("lalr_state [" + st.index() + "] {");
//		itms = st.items();
//		for (LalrItem itm : itms) {
//			System.out.print("  [");
//			System.out.print(itm.production.lhs().getSymbol().name());
//			System.out.print(" ::= ");
//			for (int i = 0; i < itm.production.rhsLength(); i++) {
//				if (i == itm.dot_pos())
//					System.out.print("(*) ");
//				part = itm.production.rhs(i);
//				if (part.is_action()) {
//					System.out.print("{action} ");
//				} else {
//					System.out.print(((SymbolPart) part).getSymbol().name() + " ");
//				}
//			}
//			if (itm.dotAtEnd()) {
//				System.out.print("(*) ");
//			}
//			System.out.println("]");
//		}
//		System.out.println("}");
//	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Add a transition out of this state to another.
	 * 
	 * @param on_sym  the symbol the transition is under.
	 * @param to_st   the state the transition goes to.
	 */
	public void add_transition(Symbol on_sym, LalrState to_st) {
		LalrTransition trans;

		/* create a new transition object and put it in our list */
		trans = new LalrTransition(on_sym, to_st, _transitions);
		_transitions = trans;
	}


	/**
	 * Propagate lookahead sets out of this state. This recursively propagates
	 * to all items that have propagation links from some item in this state.
	 */
	protected void propagateLookaheads() {
		/* recursively propagate out from each item in the state */
		for (LalrItem item : items()) {
			item.propagate_lookaheads(null);
		}
	}


	/**
	 * Fill in the parse table entries for this state. There are two parse
	 * tables that encode the viable prefix recognition machine, an action table
	 * and a reduce-goto table. The rows in each table correspond to states of
	 * the machine. The columns of the action table are indexed by terminal
	 * symbols and correspond to either transitions out of the state (shift
	 * entries) or reductions from the state to some previous state saved on the
	 * stack (reduce entries). All entries in the action table that are not
	 * shifts or reduces, represent errors. The reduce-goto table is indexed by
	 * non terminals and represents transitions out of a state on that
	 * non-terminal.
	 * <p>
	 * Conflicts occur if more than one action needs to go in one entry of the
	 * action table (this cannot happen with the reduce-goto table). Conflicts
	 * are resolved by always shifting for shift/reduce conflicts and choosing
	 * the lowest numbered production (hence the one that appeared first in the
	 * specification) in reduce/reduce conflicts. All conflicts are reported and
	 * if more conflicts are detected than were declared by the user, code
	 * generation is aborted.
	 * 
	 * @param lexer          lexical analyzer used for parsing the grammar
	 * @param model          model of the grammar
	 * @param act_table      the action table to put entries in.
	 * @param reduce_table   the reduce-goto table to put entries in.
	 */
	public void buildTableEntries(BnfModel model, ParseActionTable act_table, ParseReduceTable reduce_table) {
		ParseActionRow our_act_row;
		ParseReduceRow our_red_row;
		ParseAction act, other_act;
		Symbol sym;
		TerminalSet conflict_set = new TerminalSet();

		/* pull out our rows from the tables */
		our_act_row = act_table.under_state[index()];
		our_red_row = reduce_table.under_state[index()];

		/* consider each item in our state */
		for (LalrItem itm : items()) {

			/* if its completed (dot at end) then reduce under the lookahead */
			if (itm.dotAtEnd()) {
				act = new ReduceAction(itm.production);

				for(int t=0; t<model.terminals.count(); t++) {
//				/* consider each lookahead symbol */
//				for(Terminal term : model.terminals) {
//					int t = term.index();
					Terminal term = model.terminals.get(t);
					/* skip over the ones not in the lookahead */
					if (!itm.getLookaheadSet().contains(term)) {
						continue;
					}

					/* if we don't already have an action put this one in */
					if (our_act_row.under_term[t].kind() == ParseAction.ERROR) {
						our_act_row.under_term[t] = act;
					} else {
						/* we now have at least one conflict */
						other_act = our_act_row.under_term[t];

						/* if the other act was not a shift */
						if ((other_act.kind() != ParseAction.SHIFT) && (other_act.kind() != ParseAction.NONASSOC)) {
							/* if we have lower index hence priority, replace it */
							if (itm.production.index() < ((ReduceAction) other_act).reduce_with().index()) {
								/* replace the action */
								our_act_row.under_term[t] = act;
							}
						} else {
							/* Check precedences,see if problem is correctable */
							if (fix_with_precedence(model, itm.production, t, our_act_row, act)) {
								term = null;
							}
						}
						if (term != null) {
							conflict_set.add(term);
						}
					}
				}
			}
		}

		/* consider each outgoing transition */
		for (LalrTransition trans = transitions(); trans != null; trans = trans.next) {
			/* if its on an terminal add a shift entry */
			sym = trans.keySymbol;
			if (!sym.is_non_term()) {
				act = new ShiftAction(trans.toState);

				/* if we don't already have an action put this one in */
				if (our_act_row.under_term[sym.index()].kind() == ParseAction.ERROR) {
					our_act_row.under_term[sym.index()] = act;
				} else {
					/* we now have at least one conflict */
					Production p = ((ReduceAction) our_act_row.under_term[sym.index()]).reduce_with();

					/* shift always wins */
					if (!fix_with_precedence(model, p, sym.index(), our_act_row, act)) {
						
						our_act_row.under_term[sym.index()] = act;
						conflict_set.add(model.terminals.get(sym.index()));
					}
				}
			} else {
				/* for non terminals add an entry to the reduce-goto table */
				our_red_row.under_non_term[sym.index()] = trans.toState;
			}
		}

		/* if we end up with conflict(s), report them */
		if (!conflict_set.empty()) {
			report_conflicts(model, conflict_set);
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Procedure that attempts to fix a shift/reduce error by using precedences.
	 * --frankf 6/26/96
	 * 
	 * if a production (also called rule) or the lookahead terminal has a
	 * precedence, then the table can be fixed. if the rule has greater
	 * precedence than the terminal, a reduce by that rule in inserted in the
	 * table. If the terminal has a higher precedence, it is shifted. if they
	 * have equal precedence, then the associativity of the precedence is used
	 * to determine what to put in the table: if the precedence is left
	 * associative, the action is to reduce. if the precedence is right
	 * associative, the action is to shift. if the precedence is non
	 * associative, then it is a syntax error.
	 * 
	 * @param p               the production
	 * @param term_index      the index of the lokahead terminal
	 * @param ParseActionRow  a row of the action table
	 * @param act             the rule in conflict with the table entry
	 */

	protected boolean fix_with_precedence(BnfModel model, Production p, int term_index, ParseActionRow table_row, ParseAction act) {

		Terminal term = model.terminals.get(term_index);

		/* if the production has a precedence number, it can be fixed */
		if (p.precedence_num() > Assoc.no_prec) {

			/* if production precedes terminal, put reduce in table */
			if (p.precedence_num() > term.precedenceNum()) {
				table_row.under_term[term_index] = insert_reduce(table_row.under_term[term_index], act);
				return true;
			} else if (p.precedence_num() < term.precedenceNum()) {
				/* if terminal precedes rule, put shift in table */
				table_row.under_term[term_index] = insert_shift(table_row.under_term[term_index], act);
				return true;
			} else { /* they are == precedence */
				/*
				 * equal precedences have equal sides, so only need to look at
				 * one: if it is right, put shift in table
				 */
				if (term.precedenceSide() == Assoc.right) {
					table_row.under_term[term_index] = insert_shift(table_row.under_term[term_index], act);
					return true;
				} else if (term.precedenceSide() == Assoc.left) {
					/* if it is left, put reduce in table */
					table_row.under_term[term_index] = insert_reduce(table_row.under_term[term_index], act);
					return true;
				} else if (term.precedenceSide() == Assoc.nonassoc) {
					/*
					 * if it is nonassoc, we're not allowed to have two nonassocs of
					 * equal precedence in a row, so put in NONASSOC
					 */
					table_row.under_term[term_index] = new NonAssocAction();
					return true;
				} else {
					/* something really went wrong */
					throw new FatalCupException("Unable to resolve conflict correctly");
				}
			}
		} else if (term.precedenceNum() > Assoc.no_prec) {
			/*
			 * check if terminal has precedence, if so, shift, since rule does not
			 * have precedence
			 */
			table_row.under_term[term_index] = insert_shift(table_row.under_term[term_index], act);
			return true;
		}

		/*
		 * otherwise, neither the rule nor the terminal has a precedence, so it
		 * can't be fixed.
		 */
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/*
	 * given two actions, and an action type, return the action of that action
	 * type. give an error if they are of the same action, because that should
	 * never have tried to be fixed
	 */
	protected ParseAction insert_action(ParseAction a1, ParseAction a2, int act_type) {
		if ((a1.kind() == act_type) && (a2.kind() == act_type)) {
			throw new FatalCupException("Conflict resolution of bogus actions");
		} else if (a1.kind() == act_type) {
			return a1;
		} else if (a2.kind() == act_type) {
			return a2;
		} else {
			throw new FatalCupException("Conflict resolution of bogus actions");
		}
	}

	/* find the shift in the two actions */
	protected ParseAction insert_shift(ParseAction a1, ParseAction a2) {
		return insert_action(a1, a2, ParseAction.SHIFT);
	}

	/* find the reduce in the two actions */
	protected ParseAction insert_reduce(ParseAction a1, ParseAction a2) {
		return insert_action(a1, a2, ParseAction.REDUCE);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Produce warning messages for all conflicts found in this state.
	 * 
	 * @param lexer   lexical analyzer used for parsing the grammar
	 * @param model   model of the grammar
	 */
	protected void report_conflicts(BnfModel model, TerminalSet conflict_set) {

		boolean after_itm;

		/* consider each element */
		for (LalrItem itm : items()) {

			/* clear the S/R conflict set for this item */

			/* if it results in a reduce, it could be a conflict */
			if (itm.dotAtEnd()) {
				/* not yet after itm */
				after_itm = false;

				/* compare this item against all others looking for conflicts */
				for (LalrItem compItem : items()) {

					/* if this is the item, next one is after it */
					if (itm == compItem)
						after_itm = true;

					/* only look at it if its not the same item */
					if (itm != compItem) {
						/* is it a reduce */
						if (compItem.dotAtEnd()) {
							/* only look at reduces after itm */
							if (after_itm) {
								/* does the comparison item conflict? */
								if (compItem.getLookaheadSet().intersects(itm.getLookaheadSet())) {
									/* report a reduce/reduce conflict */
									report_reduce_reduce(model, itm, compItem);
								}
							}
						}
					}
				}
				/* report S/R conflicts under all the symbols we conflict under */
				for(Terminal term : model.terminals) {
					if (conflict_set.contains(term)) {
						report_shift_reduce(model, itm, term.index());
					}
				}
			}
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Produce a warning message for one reduce/reduce conflict.
	 * 
	 * @param lexer   lexical analyzer used for parsing the grammar
	 * @param model   model of the grammar
	 * @param itm1    first item in conflict.
	 * @param itm2    second item in conflict.
	 */
	protected void report_reduce_reduce(BnfModel model, LalrItem itm1, LalrItem itm2) {
		boolean comma_flag = false;

		System.err.println("*** Reduce/Reduce conflict found in state #" + index());
		System.err.print("  between ");
		System.err.println(itm1.toSimpleString());
		System.err.print("  and     ");
		System.err.println(itm2.toSimpleString());
		System.err.print("  under symbols: {");
		for (int t = 0; t < model.terminals.count(); t++) {
			Terminal term = model.terminals.get(t);
			if (itm1.getLookaheadSet().contains(term) && itm2.getLookaheadSet().contains(term)) {
				if (comma_flag)
					System.err.print(", ");
				else
					comma_flag = true;
				System.err.print(term.name());
			}
		}
		System.err.println("}");
		System.err.print("  Resolved in favor of ");
		if (itm1.production.index() < itm2.production.index())
			System.err.println("the first production.\n");
		else
			System.err.println("the second production.\n");

		/* count the conflict */
		model.num_conflicts++;
		warningCount++;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Produce a warning message for one shift/reduce conflict.
	 * 
	 * @param lexer   lexical analyzer used for parsing the grammar
	 * @param model   model of the grammar
	 * @param red_itm        the item with the reduce.
	 * @param conflict_sym   the index of the symbol conflict occurs under.
	 */
	protected void report_shift_reduce(BnfModel model, LalrItem red_itm, int conflict_sym) {
		Symbol shift_sym;

		/* emit top part of message including the reduce item */
		System.err.println("*** Shift/Reduce conflict found in state #" + index());
		System.err.print("  between ");
		System.err.println(red_itm.toSimpleString());

		/* find and report on all items that shift under our conflict symbol */
		for (LalrItem itm : items()) {

			/* only look if its not the same item and not a reduce */
			if (itm != red_itm && !itm.dotAtEnd()) {
				/* is it a shift on our conflicting terminal */
				shift_sym = itm.symbolAfterDot;
				if (!shift_sym.is_non_term() && shift_sym.index() == conflict_sym) {
					/* yes, report on it */
					System.err.println("  and     " + itm.toSimpleString());
				}
			}
		}
		System.err.println("  under symbol " + model.terminals.get(conflict_sym).name());
		System.err.println("  Resolved in favor of shifting.\n");

		/* count the conflict */
		model.num_conflicts++;
		warningCount++;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality comparison. */
	public boolean equals(LalrState other) {
		/* we are equal if our item sets are equal */
		return other != null && items().equals(other.items());
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (!(other instanceof LalrState))
			return false;
		else
			return equals((LalrState) other);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Produce a hash code. */
	public int hashCode() {
		/* just use the item set hash code */
		return items().hashCode();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		String result;
		LalrTransition tr;

		/* dump the item set */
		result = "lalr_state [" + index() + "]: " + _items + "\n";

		/* do the transitions */
		for (tr = transitions(); tr != null; tr = tr.next) {
			result += tr;
			result += "\n";
		}

		return result;
	}


	public void describe(PrintStream out) {
		out.println("  Kernel");
		LalrTransition tr = transitions();
		for(LalrItem item : _items) {
			out.println("    "+item);
		}
//		while(tr != null) {
//			out.println("    "+tr);
//			tr = tr.next;
//		}
		out.println();
	}

	/*-----------------------------------------------------------*/
}
