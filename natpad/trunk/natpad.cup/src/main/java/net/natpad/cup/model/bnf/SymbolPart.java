package net.natpad.cup.model.bnf;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.util.TextUtil;

/**
 * This class represents a part of a production which is a symbol (terminal or non terminal). This simply maintains a
 * reference to the symbol in question.
 * 
 * @see net.natpad.cup.model.bnf.Production
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public class SymbolPart implements ProductionPart {

	
	/**
	 * Optional label for referring to the part within an action (null for no label).
	 */
	protected String label;

	/**
	 * Optional label for referring to the part within an action (null for no label).
	 */
	public String label() {
		return label;
	}

	
	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Full constructor.
	 * 
	 * @param sym the symbol that this part is made up of.
	 * @param lab an optional label string for the part.
	 */
	public SymbolPart(Symbol sym, String lab) {
		super();
		label = lab;

		if (sym == null) {
			throw new FatalCupException("Attempt to construct a symbol_part with a null symbol");
		}
		_the_symbol = sym;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor with no label.
	 * 
	 * @param sym the symbol that this part is made up of.
	 */
	public SymbolPart(Symbol sym) {
		this(sym, null);
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** The symbol that this part is made up of. */
	protected Symbol _the_symbol;

	/** The symbol that this part is made up of. */
	public Symbol getSymbol() {
		return _the_symbol;
	}

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Respond that we are not an action part. */
	public boolean isAction() {
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality comparison. */
	public boolean equals(SymbolPart other) {
		if (other == this) {
			return true;
		}
		if (other instanceof SymbolPart) {
			SymbolPart that = (SymbolPart) other;
			return TextUtil.nullSafeEquals(label, that.label())
					&& getSymbol().equals(that.getSymbol());
		}
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (this == other) {
			return true;
		}
		if (other instanceof SymbolPart) {
			return equals((SymbolPart) other);
		}
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Produce a hash code. */
	public int hashCode() {
		return super.hashCode() ^ (getSymbol() == null ? 0 : getSymbol().hashCode());
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		if (getSymbol() != null)
			return getSymbol()._name;
		else
			return "$$MISSING-SYMBOL$$";
	}

	/*-----------------------------------------------------------*/

}
