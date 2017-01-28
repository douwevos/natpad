package net.natpad.cup.state;

import net.natpad.cup.FatalCupException;

/**
 * This class represents a shift action within the parse table. The action
 * simply stores the state that it shifts to and responds to queries about its
 * type.
 * 
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public class ShiftAction extends ParseAction {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Simple constructor.
	 * 
	 * @param shft_to
	 *            the state that this action shifts to.
	 */
	public ShiftAction(LalrState shft_to) {
		/* sanity check */
		if (shft_to == null) {
			throw new FatalCupException("Attempt to create a shift_action to a null state");
		}

		_shift_to = shft_to;
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** The state we shift to. */
	protected LalrState _shift_to;

	/** The state we shift to. */
	public LalrState shift_to() {
		return _shift_to;
	}

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Quick access to type of action. */
	public int kind() {
		return SHIFT;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality test. */
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof ShiftAction) {
			ShiftAction other = ((ShiftAction) obj);
			return other != null && other.shift_to() == shift_to();
		}
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Compute a hash code. */
	public int hashCode() {
		/* use the hash code of the state we are shifting to */
		return shift_to().hashCode();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		return "SHIFT(to state " + shift_to().index() + ")";
	}

	/*-----------------------------------------------------------*/

}
