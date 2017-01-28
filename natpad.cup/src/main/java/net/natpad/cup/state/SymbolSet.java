package net.natpad.cup.state;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Symbol;

/**
 * This class represents a set of symbols and provides a series of set
 * operations to manipulate them.
 * 
 * @see net.natpad.cup.model.bnf.Symbol
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public class SymbolSet implements Iterable<Symbol> {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Constructor for an empty set. */
	public SymbolSet() {
	}

	/**
	 * Constructor for cloning from another set.
	 * 
	 * @param other
	 *            the set we are cloning from.
	 */
	@SuppressWarnings("unchecked")
	public SymbolSet(SymbolSet other) {
		not_null(other);
		_all = (HashMap<String, Symbol>) other._all.clone();
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * A hash table to hold the set. Symbols are keyed using their name string.
	 */
	protected HashMap<String, Symbol> _all = new HashMap<String, Symbol>();


	/** size of the set */
	public int size() {
		return _all.size();
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Helper function to test for a null object and throw an exception if one
	 * is found.
	 * 
	 * @param obj
	 *            the object we are testing.
	 */
	protected void not_null(Object obj) {
		if (obj == null) {
			throw new FatalCupException("Null object used in set operation");
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if the set contains a particular symbol.
	 * 
	 * @param sym
	 *            the symbol we are looking for.
	 */
	public boolean contains(Symbol sym) {
		return _all.containsKey(sym.name());
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if this set is an (improper) subset of another.
	 * 
	 * @param other
	 *            the set we are testing against.
	 */
	public boolean isSubsetOf(SymbolSet other) {
		not_null(other);

		/* walk down our set and make sure every element is in the other */
		for (Symbol sym : _all.values()) {
			if (!other.contains(sym)) {
				return false;
			}
		}

		/* they were all there */
		return true;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Add a single symbol to the set.
	 * 
	 * @param sym   the symbol we are adding.
	 * @return true if this changes the set.
	 */
	public boolean add(Symbol sym) {
		Object previous;

		not_null(sym);

		/* put the object in */
		previous = _all.put(sym.name(), sym);

		/* if we had a previous, this is no change */
		return previous == null;
	}


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality comparison. */
	public boolean equals(SymbolSet other) {
		if (other == null || other.size() != size())
			return false;

		/* once we know they are the same size, then improper subset does test */
		return isSubsetOf(other);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (other == this) {
			return true;
		}
		if (other instanceof SymbolSet) {
			return equals((SymbolSet) other);
		}
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Compute a hash code. */
	public int hashCode() {
		int result = 0;
		for (Symbol sym : _all.values()) {
			result ^= sym.hashCode();
		}
		return result;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		String result;
		boolean comma_flag;

		result = "{";
		comma_flag = false;
		Iterator<Symbol> iterator = iterator();
		while(iterator.hasNext()) {
			Symbol sym = iterator.next();
			if (comma_flag)
				result += ", ";
			else
				comma_flag = true;

			result += sym.name();
		}
		result += "}";

		return result;
	}

	/*-----------------------------------------------------------*/

	@Override
	public Iterator<Symbol> iterator() {
		Collection<Symbol> values = _all.values();
		ArrayList<Symbol> result = new ArrayList<Symbol>(values);
		Collections.sort(result, new Comparator<Symbol>() {
			@Override
			public int compare(Symbol o1, Symbol o2) {
				boolean a_is_nt = o1 instanceof NonTerminal;
				boolean b_is_nt = o2 instanceof NonTerminal;
				if (a_is_nt && !b_is_nt) {
					return -1;
				}
				if (!a_is_nt && b_is_nt) {
					return 1;
				}
				return o1.index()<o2.index() ? -1 : (o1.index()>o2.index() ? 1 : 0);
			}
		});
		return result.iterator();
	}
	
}
