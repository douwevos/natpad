package net.natpad.cup;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import net.natpad.cup.model.bnf.Terminal;

/**
 * A set of terminals implemented as a bitset.
 * 
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public class TerminalSet implements Iterable<Terminal> {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/
	
	protected final HashSet<Terminal> internalSet = new HashSet<Terminal>();

	
	/** Constructor for an empty set. */
	public TerminalSet() {
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor for cloning from another set.
	 * 
	 * @param other
	 *            the set we are cloning from.
	 */
	public TerminalSet(TerminalSet other) {
		notNull(other);
		internalSet.addAll(other.internalSet);
	}


	/*-----------------------------------------------------------*/
	/*--- General Methods ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Helper function to test for a null object and throw an exception if one
	 * is found.
	 * 
	 * @param obj   the object we are testing.
	 */
	protected void notNull(Object obj) {
		if (obj == null) {
			throw new FatalCupException("Null object used in set operation");
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Determine if the set is empty. */
	public boolean empty() {
		return internalSet.isEmpty();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if the set contains a particular terminal.
	 * 
	 * @param sym      the terminal symbol we are looking for.
	 */
	public boolean contains(Terminal sym) {
		notNull(sym);
		return internalSet.contains(sym);
	}


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if this set is an (improper) subset of another.
	 * 
	 * @param other    the set we are testing against.
	 */
	public boolean isSubSetOf(TerminalSet other) {
		notNull(other);
		if (other==this) {
			return true;
		}
		for(Terminal term : internalSet) {
			if (!other.internalSet.contains(term)) {
				return false;
			}
		}
		return true;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if this set is an (improper) superset of another.
	 * 
	 * @param other
	 *            the set we are testing against.
	 */
	public boolean isSuperSetOf(TerminalSet other) {
		notNull(other);
		return other.isSubSetOf(this);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Add a single terminal to the set.
	 * 
	 * @param sym
	 *            the terminal being added.
	 * @return true if this changes the set.
	 */
	public boolean add(Terminal sym) {
		notNull(sym);
		return internalSet.add(sym);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Remove a terminal if it is in the set.
	 * 
	 * @param sym
	 *            the terminal being removed.
	 */
	public void remove(Terminal sym) {
		notNull(sym);
		internalSet.remove(sym);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Add (union) in a complete set.
	 * 
	 * @param other   the set being added.
	 * @return true if this changes the set.
	 */
	public boolean add(TerminalSet other) {
		notNull(other);
		return internalSet.addAll(other.internalSet);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if this set intersects another.
	 * 
	 * @param other   the other set in question.
	 */
	public boolean intersects(TerminalSet other) {
		notNull(other);

		for(Terminal term : other.internalSet) {
			if (internalSet.contains(term)) {
				return false;
			}
		}
		return true;
	}

	
	@Override
	public Iterator<Terminal> iterator() {
		return internalSet.iterator();
	}
	
	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality comparison. */
	public boolean equals(TerminalSet other) {
		if (other == null) {
			return false;
		} else {
			return internalSet.equals(other.internalSet);
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (!(other instanceof TerminalSet)) {
			return false;
		} else {
			return equals((TerminalSet) other);
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to string. */
	public String toString() {
		StringBuilder buf = new StringBuilder();

		List<Terminal> orderedTerminals = new ArrayList<Terminal>(internalSet);
		Collections.sort(orderedTerminals);
		for(Terminal term : orderedTerminals) {
			if (buf.length()>0) {
				buf.append(", ");
			}

			buf.append(term.name());
		}
		return "{"+buf+"}";
	}

	/*-----------------------------------------------------------*/

}
