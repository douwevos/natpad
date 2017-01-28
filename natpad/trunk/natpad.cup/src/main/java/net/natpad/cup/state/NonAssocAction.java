package net.natpad.cup.state;


/**
 * This class represents a shift/reduce nonassociative error within the parse
 * table. If action_table element is assign to type nonassoc_action, it cannot
 * be changed, and signifies that there is a conflict between shifting and
 * reducing a production and a terminal that shouldn't be next to each other.
 * 
 * @version last updated: 7/2/96
 * @author Frank Flannery
 */
public class NonAssocAction extends ParseAction {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Simple constructor.
	 */
	public NonAssocAction() {
		/* don't need to set anything, since it signifies error */
	}

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Quick access to type of action. */
	public int kind() {
		return NONASSOC;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality test. */
	public boolean equals(ParseAction other) {
		return other != null && other.kind() == NONASSOC;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality test. */
	public boolean equals(Object other) {
		if (other instanceof ParseAction)
			return equals((ParseAction) other);
		else
			return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Compute a hash code. */
	public int hashCode() {
		/* all objects of this class hash together */
		return 0xCafe321;
	}

	/** Convert to string. */
	public String toString() {
		return "NONASSOC";
	}

	/*-----------------------------------------------------------*/

}
