package net.natpad.cup.state;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.ProductionPart;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;

/**
 * The "core" of an LR item. This includes a production and the position of a
 * marker (the "dot") within the production. Typically item cores are written
 * using a production with an embedded "dot" to indicate their position. For
 * example:
 * 
 * <pre>
 *     A ::= B * C d E
 * </pre>
 * 
 * This represents a point in a parse where the parser is trying to match the
 * given production, and has succeeded in matching everything before the "dot"
 * (and hence is expecting to see the symbols after the dot next). See
 * lalr_item, lalr_item_set, and lalr_start for full details on the meaning and
 * use of items.
 * 
 * @see net.natpad.cup.state.LalrItem
 * @see net.natpad.cup.state.LalrItemSet
 * @see net.natpad.cup.state.LalrState
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */

public class LrItemCore {


	/** The production for the item. */
	public Production production;

	/**
	 * The position of the "dot" -- this indicates the part of the production
	 * that the marker is before, so 0 indicates a dot at the beginning of the
	 * RHS.
	 */
	public final int dotIndex;


	/** Cache of the hash code. */
	public final int hashCode;

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Cache of symbol after the dot. */
	public final Symbol symbolAfterDot;

	
	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Full constructor.
	 * 
	 * @param prod     production this item uses.
	 * @param pos      position of the "dot" within the item.
	 */
	public LrItemCore(Production prod, int pos) {
		ProductionPart part;

		if (prod == null) {
			throw new FatalCupException("Attempt to create an lr_item_core with a null production");
		}

		production = prod;

		if (pos < 0 || pos > production.rhsLength()) {
			throw new FatalCupException("Attempt to create an lr_item_core with a bad dot position");
		}

		dotIndex = pos;

		/* compute and cache hash code now */
		hashCode = 13 * production.hashCode() + pos;

		Symbol symbol = null;
		/* cache the symbol after the dot */
		if (dotIndex < production.rhsLength()) {
			part = production.rhs(dotIndex);
			if (!part.isAction()) {
				symbol = ((SymbolPart) part).getSymbol();
			}
		}
		symbolAfterDot = symbol;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor for dot at start of right hand side.
	 * 
	 * @param prod    production this item uses.
	 */
	public LrItemCore(Production prod) {
		this(prod, 0);
	}



	/** Is the dot at the end of the production? */
	public boolean dotAtEnd() {
		return dotIndex >= production.rhsLength();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Determine if we have a dot before a non terminal, and if so which one
	 * (return null or the non terminal).
	 */
	public NonTerminal dotBeforeNonTerminal() {
		/* get the symbol after the dot */
		Symbol sym = symbolAfterDot;

		/* if it exists and is a non terminal, return it */
		return  (sym != null && sym.is_non_term()) ? (NonTerminal) sym : null;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Equality comparison for the core only. This is separate out because we
	 * need separate access in a super class.
	 */
	public boolean coreEquals(LrItemCore other) {
		return other != null && production.equals(other.production)
				&& dotIndex == other.dotIndex;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality comparison. */
	public boolean equals(LrItemCore other) {
		return coreEquals(other);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (other==this) {
			return true;
		} else if (!(other instanceof LrItemCore)) {
			return false;
		} else {
			return equals((LrItemCore) other);
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Hash code for the item. */
	public int hashCode() {
		return hashCode;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Convert to a string (separated out from toString() so we can call it from
	 * subclass that overrides toString()).
	 */
	public String toSimpleString() {
		StringBuilder buf = new StringBuilder();
		ProductionPart part;

		if (production.lhs() != null && production.lhs().getSymbol() != null
				&& production.lhs().getSymbol().name() != null) {
			buf.append(production.lhs().getSymbol().name());
		} else {
			buf.append("$$NULL$$");
		}

		buf.append(" ::= ");

		for (int i = 0; i < production.rhsLength(); i++) {
			/* do we need the dot before this one? */
			if (i == dotIndex) {
				buf.append("(*) ");
			}

			/* print the name of the part */
			if (production.rhs(i) == null) {
				buf.append("$$NULL$$ ");
			} else {
				part = production.rhs(i);
				if (part == null) {
					buf.append("$$NULL$$ ");
				} else if (part.isAction()) {
					buf.append("{ACTION} ");
				} else if (((SymbolPart) part).getSymbol() != null
						&& ((SymbolPart) part).getSymbol().name() != null) {
					buf.append(((SymbolPart) part).getSymbol().name()).append(" ");
				} else {
					buf.append("$$NULL$$ ");
				}
			}
		}

		/* put the dot after if needed */
		if (dotIndex == production.rhsLength()) {
			buf.append("(*) ");
		}

		return buf.toString();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string */
	public String toString() {
		return toSimpleString();
	}

	/*-----------------------------------------------------------*/

}
