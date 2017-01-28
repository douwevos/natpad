package net.natpad.cup.state;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.model.bnf.Symbol;

/**
 * This class represents a transition in an LALR viable prefix recognition machine. Transitions can be under terminals
 * for non-terminals. They are internally linked together into singly linked lists containing all the transitions out of
 * a single state via the _next field.
 * 
 * @see net.natpad.cup.state.LalrState
 * @version last updated: 11/25/95
 * @author Scott Hudson
 * 
 */
public class LalrTransition {

	
	/** The symbol we make the transition on. */
	protected final Symbol keySymbol;
	
	/** The state we transition to. */
	protected final LalrState toState;

	/** Next transition in linked list of transitions out of a state */
	protected final LalrTransition next;

	
	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Full constructor.
	 * 
	 * @param on_sym symbol we are transitioning on.
	 * @param to_st state we transition to.
	 * @param nxt next transition in linked list.
	 */
	public LalrTransition(Symbol on_sym, LalrState to_st, LalrTransition nxt) {
		/* sanity checks */
		if (on_sym == null)
			throw new FatalCupException("Attempt to create transition on null symbol");
		if (to_st == null)
			throw new FatalCupException("Attempt to create transition to null state");

		/* initialize */
		keySymbol = on_sym;
		toState = to_st;
		next = nxt;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor with null next.
	 * 
	 * @param on_sym symbol we are transitioning on.
	 * @param to_st state we transition to.
	 */
	public LalrTransition(Symbol on_sym, LalrState to_st) {
		this(on_sym, to_st, null);
	}




	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Convert to a string. */
	public String toString() {
		String result;

		result = "transition on " + keySymbol.name() + " to state [";
		result += toState.index();
		result += "]";

		return result;
	}

	/*-----------------------------------------------------------*/
}
