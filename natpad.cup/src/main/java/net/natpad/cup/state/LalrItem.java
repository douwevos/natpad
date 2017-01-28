package net.natpad.cup.state;

import java.util.Comparator;
import java.util.Stack;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.TerminalSet;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.ProductionPart;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;
import net.natpad.cup.model.bnf.Terminal;

/**
 * This class represents an LALR item. Each LALR item consists of a production,
 * a "dot" at a position within that production, and a set of lookahead symbols
 * (terminal). (The first two of these parts are provide by the super class). An
 * item is designed to represent a configuration that the parser may be in. For
 * example, an item of the form:
 * 
 * <pre>
 *    [A ::= B * C d E  , {a,b,c}]
 * </pre>
 * 
 * indicates that the parser is in the middle of parsing the production
 * 
 * <pre>
 *    A ::= B C d E
 * </pre>
 * 
 * that B has already been parsed, and that we will expect to see a lookahead of
 * either a, b, or c once the complete RHS of this production has been found.
 * <p>
 * 
 * Items may initially be missing some items from their lookahead sets. Links
 * are maintained from each item to the set of items that would need to be
 * updated if symbols are added to its lookahead set. During
 * "lookahead propagation", we add symbols to various lookahead sets and
 * propagate these changes across these dependency links as needed.
 * 
 * @see net.natpad.cup.state.LalrItemSet
 * @see net.natpad.cup.state.LalrState
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public class LalrItem extends LrItemCore {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Full constructor.
	 * 
	 * @param prod   the production for the item.
	 * @param pos    the position of the "dot" within the production.
	 * @param look   the set of lookahead symbols.
	 */
	public LalrItem(Production prod, int pos, TerminalSet look) {
		super(prod, pos);
		_lookahead = look;
		_propagate_items = new Stack<LalrItem>();
		needs_propagation = true;
	}

	/**
	 * Constructor with default position (dot at start).
	 * 
	 * @param prod   the production for the item.
	 * @param look   the set of lookahead symbols.
	 */
	public LalrItem(Production prod, TerminalSet look) {
		this(prod, 0, look);
	}

	/**
	 * Constructor with default position and empty lookahead set.
	 * 
	 * @param prod    the production for the item.
	 */
	public LalrItem(Production prod) {
		this(prod, 0, new TerminalSet());
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** The lookahead symbols of the item. */
	protected TerminalSet _lookahead;

	/** The lookahead symbols of the item. */
	public TerminalSet getLookaheadSet() {
		return _lookahead;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Links to items that the lookahead needs to be propagated to. */
	protected Stack<LalrItem> _propagate_items;

	/** Links to items that the lookahead needs to be propagated to */
	public Stack<LalrItem> propagate_items() {
		return _propagate_items;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Flag to indicate that this item needs to propagate its lookahead (whether
	 * it has changed or not).
	 */
	protected boolean needs_propagation;

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Add a new item to the set of items we propagate to. */
	public void addPropagate(LalrItem prop_to) {
		_propagate_items.push(prop_to);
		needs_propagation = true;
	}

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Propagate incoming lookaheads through this item to others need to be
	 * changed.
	 * 
	 * @params incoming symbols to potentially be added to lookahead of this
	 *         item.
	 */
	public void propagate_lookaheads(TerminalSet incoming) {
		boolean change = false;

		/* if we don't need to propagate, then bail out now */
		if (!needs_propagation && (incoming == null || incoming.empty()))
			return;

		/* if we have null incoming, treat as an empty set */
		if (incoming != null) {
			/* add the incoming to the lookahead of this item */
			change = getLookaheadSet().add(incoming);
		}

		/* if we changed or need it anyway, propagate across our links */
		if (change || needs_propagation) {
			/* don't need to propagate again */
			needs_propagation = false;

			/* propagate our lookahead into each item we are linked to */
			for(LalrItem item : propagate_items()) {
				item.propagate_lookaheads(getLookaheadSet());
			}
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Produce the new lalr_item that results from shifting the dot one position
	 * to the right.
	 */
	public LalrItem shift() {
		LalrItem result;

		/* can't shift if we have dot already at the end */
		if (dotAtEnd()) {
			throw new FatalCupException("Attempt to shift past end of an lalr_item");
		}

		/* create the new item w/ the dot shifted by one */
		result = new LalrItem(production, dotIndex + 1, new TerminalSet(getLookaheadSet()));

		/* change in our lookahead needs to be propagated to this item */
		addPropagate(result);

		return result;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Calculate lookahead representing symbols that could appear after the
	 * symbol that the dot is currently in front of. Note: this routine must not
	 * be invoked before first sets and nullability has been calculated for all
	 * non terminals.
	 */
	public TerminalSet calcLookahead(TerminalSet lookahead_after) {
		TerminalSet result;
		int pos;
		ProductionPart part;
		Symbol sym;

		/* sanity check */
		if (dotAtEnd()) {
			throw new FatalCupException("Attempt to calculate a lookahead set with a completed item");
		}

		/* start with an empty result */
		result = new TerminalSet();

		/* consider all nullable symbols after the one to the right of the dot */
		for (pos = dotIndex + 1; pos < production.rhsLength(); pos++) {
			part = production.rhs(pos);

			/* consider what kind of production part it is -- skip actions */
			if (!part.isAction()) {
				sym = ((SymbolPart) part).getSymbol();

				/* if its a terminal add it in and we are done */
				if (!sym.is_non_term()) {
					result.add((Terminal) sym);
					return result;
				} else {
					/* otherwise add in first set of the non terminal */
					TerminalSet nt_first_set = ((NonTerminal) sym).first_set();
					result.add(nt_first_set);

					/* if its nullable we continue adding, if not, we are done */
					if (!((NonTerminal) sym).nullable())
						return result;
				}
			}
		}

		/*
		 * if we get here everything past the dot was nullable we add in the
		 * lookahead for after the production and we are done
		 */
		result.add(lookahead_after);
		return result;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if everything from the symbol one beyond the dot all the way to
	 * the end of the right hand side is nullable. This would indicate that the
	 * lookahead of this item must be included in the lookaheads of all items
	 * produced as a closure of this item. Note: this routine should not be
	 * invoked until after first sets and nullability have been calculated for
	 * all non terminals.
	 */
	public boolean lookaheadVisible() {
		ProductionPart part;
		Symbol sym;

		/*
		 * if the dot is at the end, we have a problem, but the cleanest thing
		 * to do is just return true.
		 */
		if (dotAtEnd())
			return true;

		/* walk down the rhs and bail if we get a non-nullable symbol */
		for (int pos = dotIndex + 1; pos < production.rhsLength(); pos++) {
			part = production.rhs(pos);

			/* skip actions */
			if (!part.isAction()) {
				sym = ((SymbolPart) part).getSymbol();

				/* if its a terminal we fail */
				if (!sym.is_non_term())
					return false;

				/* if its not nullable we fail */
				if (!((NonTerminal) sym).nullable())
					return false;
			}
		}

		/* if we get here its all nullable */
		return true;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Equality comparison -- here we only require the cores to be equal since
	 * we need to do sets of items based only on core equality (ignoring
	 * lookahead sets).
	 */
	public boolean equals(LalrItem other) {
		if (other == null) {
			return false;
		}
		return super.equals(other);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (!(other instanceof LalrItem))
			return false;
		else
			return equals((LalrItem) other);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Return a hash code -- here we only hash the core since we only test core
	 * matching in LALR items.
	 */
	public int hashCode() {
		return super.hashCode();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to string. */
	public String toString() {
		String result = "";

		// additional output for debugging:
		// result += "(" + obj_hash() + ")";
		result += "[";
		result += super.toString();
		result += ", ";
		if (getLookaheadSet() != null) {
			result += "{";
			for(Terminal term : getLookaheadSet()) {
					result += term.name() + " ";
			}
			result += "}";
		} else
			result += "NULL LOOKAHEAD!!";
		result += "]";

		// additional output for debugging:
		// result += " -> ";
		// for (int i = 0; i<propagate_items().size(); i++)
		// result+=((lalr_item)(propagate_items().elementAt(i))).obj_hash()+" ";
		//
		// if (needs_propagation) result += " NP";

		return result;
	}
	/*-----------------------------------------------------------*/
	
	
	static class ItemComparator implements Comparator<LalrItem> {
		public ItemComparator() {
		}
		
		@Override
		public int compare(LalrItem o1, LalrItem o2) {
			int val1 = o1.production.index()*1024+o1.dotIndex;
			int val2 = o2.production.index()*1024+o2.dotIndex;
			return val1<val2 ? -1 : (val1>val2 ? 1 : 0);
		}
	}
}
