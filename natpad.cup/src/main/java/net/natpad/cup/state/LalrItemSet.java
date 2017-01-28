package net.natpad.cup.state;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.TerminalSet;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;

/**
 * This class represents a set of LALR items. For purposes of building these
 * sets, items are considered unique only if they have unique cores (i.e.,
 * ignoring differences in their lookahead sets).
 * <p>
 * 
 * This class provides fairly conventional set oriented operations (union,
 * sub/super-set tests, etc.), as well as an LALR "closure" operation (see
 * compute_closure()).
 * 
 * @see net.natpad.cup.state.LalrItem
 * @see net.natpad.cup.state.LalrState
 * @version last updated: 3/6/96
 * @author Scott Hudson
 */

public class LalrItemSet implements Iterable<LalrItem> {

	/**
	 * A hash table to implement the set. We store the items using themselves as
	 * keys.
	 */
	protected HashSet<LalrItem> _all = new HashSet<LalrItem>(11);

	/** Cached hashcode for this set. */
	protected Integer hashcode_cache = null;

	/** Constructor for an empty set. */
	public LalrItemSet() {
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor for cloning from another set.
	 * 
	 * @param other
	 *            indicates set we should copy from.
	 */
	@SuppressWarnings("unchecked")
	public LalrItemSet(LalrItemSet other) {
		_all = (HashSet<LalrItem>) other._all.clone();
	}

	/** Access to all elements of the set. */
	@Override
	public Iterator<LalrItem> iterator() {
		List<LalrItem> enlisted = new ArrayList<LalrItem>(_all);
		Collections.sort(enlisted, new LalrItem.ItemComparator());
		return enlisted.iterator();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Size of the set */
	public int size() {
		return _all.size();
	}

	/*-----------------------------------------------------------*/
	/*--- Set Operation Methods ---------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Does the set contain a particular item?
	 * 
	 * @param itm
	 *            the item in question.
	 */
	public boolean contains(LalrItem itm) {
		return _all.contains(itm);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Return the item in the set matching a particular item (or null if not
	 * found)
	 * 
	 * @param itm
	 *            the item we are looking for.
	 */
	public LalrItem find(LalrItem itm) {
		for (LalrItem search : this) {
			if (search.hashCode() == itm.hashCode() && search.equals(itm)) {
				return search;
			}
		}
		return null;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Is this set an (improper) subset of another?
	 * 
	 * @param other
	 *            the other set in question.
	 */
	public boolean is_subset_of(LalrItemSet other) {

		/* walk down our set and make sure every element is in the other */
		for (LalrItem item : _all) {
			if (!other.contains(item)) {
				return false;
			}
		}

		/* they were all there */
		return true;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Is this set an (improper) superset of another?
	 * 
	 * @param other
	 *            the other set in question.
	 */
	public boolean isSupersetOf(LalrItemSet other) {
		return other.is_subset_of(this);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Add a singleton item, merging lookahead sets if the item is already part
	 * of the set. returns the element of the set that was added or merged into.
	 * 
	 * @param itm
	 *            the item being added.
	 */
	public LalrItem add(LalrItem itm) {
		LalrItem other;

		/* see if an item with a matching core is already there */
		other = (LalrItem) find(itm);

		/* if so, merge this lookahead into the original and leave it */
		if (other != null) {
			other.getLookaheadSet().add(itm.getLookaheadSet());
			return other;
		} else {
			/* otherwise we just go in the set */
			/* invalidate cached hashcode */
			hashcode_cache = null;

			_all.add(itm);
			return itm;
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Remove a single item if it is in the set.
	 * 
	 * @param itm
	 *            the item to remove.
	 */
	public void remove(LalrItem itm) {
		/* invalidate cached hashcode */
		hashcode_cache = null;

		/* remove it from hash table implementing set */
		_all.remove(itm);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Remove and return one item from the set (done in hash order). */
	public LalrItem getOne() {
		List<LalrItem> enlisted = new ArrayList<LalrItem>(_all);
		Collections.sort(enlisted, new LalrItem.ItemComparator());
		if (enlisted.isEmpty()) {
			return null;
		}
		LalrItem result = enlisted.get(0);
		remove(result);
		return result;
	}


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Compute the closure of the set using the LALR closure rules. Basically
	 * for every item of the form:
	 * 
	 * <pre>
	 *    [L ::= a *N alpha, l]
	 * </pre>
	 * 
	 * (where N is a a non terminal and alpha is a string of symbols) make sure
	 * there are also items of the form:
	 * 
	 * <pre>
	 *    [N ::= *beta, first(alpha l)]
	 * </pre>
	 * 
	 * corresponding to each production of N. Items with identical cores but
	 * differing lookahead sets are merged by creating a new item with the same
	 * core and the union of the lookahead sets (the LA in LALR stands for
	 * "lookahead merged" and this is where the merger is). This routine assumes
	 * that nullability and first sets have been computed for all productions
	 * before it is called.
	 */
	public void computeClosure() {
		LalrItemSet consider;
		LalrItem itm, new_itm, add_itm;
		NonTerminal nonTerm;
		TerminalSet newLookaheads;
		// Enumeration p;
		boolean needPropagation;

		/* invalidate cached hashcode */
		hashcode_cache = null;

		/* each current element needs to be considered */
		consider = new LalrItemSet(this);

		/* repeat this until there is nothing else to consider */
		while (consider.size() > 0) {
			
			/* get one item to consider */
			itm = consider.getOne();

			/* do we have a dot before a non terminal */
			nonTerm = itm.dotBeforeNonTerminal();
			if (nonTerm != null) {
				/* create the lookahead set based on first after dot */
				newLookaheads = itm.calcLookahead(itm.getLookaheadSet());

				/*
				 * are we going to need to propagate our lookahead to new item
				 */
				needPropagation = itm.lookaheadVisible();

				/* create items for each production of that non term */
				for (Production prod : nonTerm.productions()) {

					/* create new item with dot at start and that lookahead */
					new_itm = new LalrItem(prod, new TerminalSet(newLookaheads));

					/* add/merge item into the set */
					add_itm = add(new_itm);
					/* if propagation is needed link to that item */
					if (needPropagation) {
						itm.addPropagate(add_itm);
					}

					/* was this was a new item */
					if (add_itm == new_itm) {
						/* that may need further closure, consider it also */
						consider.add(new_itm);
					}
				}
			}
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object obj) {
		if (obj == this) {
			return true;
		}
		if ((obj instanceof LalrItemSet)) {
			LalrItemSet other = (LalrItemSet) obj;
			if (other == null || other.size() != size())
				return false;

			/*
			 * once we know they are the same size, then improper subset does
			 * test
			 */
			return is_subset_of(other);

		}
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Return hash code. */
	public int hashCode() {

		/* only compute a new one if we don't have it cached */
		if (hashcode_cache == null) {
			/* hash together codes from at most first 5 elements */
			// CSA fix! we'd *like* to hash just a few elements, but
			// that means equal sets will have inequal hashcodes, which
			// we're not allowed (by contract) to do. So hash them all.
			int result = 0;
			for (LalrItem item : _all) {
				result ^= item.hashCode();
			}
			hashcode_cache = new Integer(result);
		}

		return hashcode_cache.intValue();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to string. */
	public String toString() {
		StringBuilder result = new StringBuilder();

		result.append("{\n");
		for (LalrItem item : _all) {
			result.append("  " + item + "\n");
		}
		result.append("}");

		return result.toString();
	}
	/*-----------------------------------------------------------*/
}
