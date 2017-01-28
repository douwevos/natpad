package net.natpad.cup.model.bnf;


/**
 * A specialized version of a production used when we split an existing production in order to remove an embedded
 * action. Here we keep a bit of extra bookkeeping so that we know where we came from.
 * 
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */

public class ActionProduction extends Production {

	/**
	 * Constructor.
	 * 
	 * @param base the production we are being factored out of.
	 * @param lhs_sym the LHS symbol for this production.
	 * @param rhs_parts array of production parts for the RHS.
	 * @param rhs_len how much of the rhs_parts array is valid.
	 * @param action_str the trailing reduce action for this production.
	 */
	public ActionProduction(int index, Production base, NonTerminal lhs_sym, String action_str) {
		super(index, lhs_sym, null, 0, action_str);
//		_base_production = base;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
//
//	/** The production we were taken out of. */
//	protected Production _base_production;
//
//	/** The production we were taken out of. */
//	public Production base_production() {
//		return _base_production;
//	}
}
