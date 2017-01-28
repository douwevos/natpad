package net.natpad.cup.state;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.model.bnf.Production;

/**
 * This class represents a reduce action within the parse table. The action simply stores the production that it reduces
 * with and responds to queries about its type.
 * 
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public class ReduceAction extends ParseAction {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Simple constructor.
	 * 
	 * @param prod the production this action reduces with.
	 */
	public ReduceAction(Production prod) {
		/* sanity check */
		if (prod == null) {
			throw new FatalCupException("Attempt to create a reduce_action with a null production");
		}

		_reduce_with = prod;
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** The production we reduce with. */
	protected Production _reduce_with;

	/** The production we reduce with. */
	public Production reduce_with() {
		return _reduce_with;
	}

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Quick access to type of action. */
	public int kind() {
		return REDUCE;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality test. */
	public boolean equals(ReduceAction other) {
		return other != null && other.reduce_with() == reduce_with();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality test. */
	public boolean equals(Object other) {
		if (other instanceof ReduceAction)
			return equals((ReduceAction) other);
		else
			return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Compute a hash code. */
	public int hashCode() {
		/* use the hash code of the production we are reducing with */
		return reduce_with().hashCode();
	}

	/** Convert to string. */
	public String toString() {
		return "REDUCE(with prod " + reduce_with().index() + ", "+reduce_with()+")";
	}

	/*-----------------------------------------------------------*/

}
