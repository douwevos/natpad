package net.natpad.cup.model.bnf;

import java.util.HashSet;
import java.util.Set;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.TerminalSet;

/**
 * This class represents a non-terminal symbol in the grammar. Each non terminal
 * has a textual name, an index, and a string which indicates the type of object
 * it will be implemented with at runtime (i.e. the class of object that will be
 * pushed on the parse stack to represent it).
 * 
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */

public class NonTerminal extends Symbol {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Full constructor.
	 * 
	 * @param nm
	 *            the name of the non terminal.
	 * @param tp
	 *            the type string for the non terminal.
	 */
	public NonTerminal(int index, String nm, String tp) {
		/* super class does most of the work */
		super(index, nm, tp);


//		/* assign a unique index */
//		_index = next_index++;
//
//		/* add to by_index set */
//		_all_by_index.put(new Integer(_index), this);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor with default type.
	 * 
	 * @param nm
	 *            the name of the non terminal.
	 */
	public NonTerminal(int index, String nm) {
		this(index, nm, null);
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Static (Class) Variables ------------------*/
	/*-----------------------------------------------------------*/


	/** Static counter for creating unique non-terminal names */
	protected static int next_nt = 0;

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** flag non-terminals created to embed action productions */
	public boolean is_embedded_action = false; /* added 24-Mar-1998, CSA */

	/*-----------------------------------------------------------*/
	/*--- Static Methods ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Method for creating a new uniquely named hidden non-terminal using the
	 * given string as a base for the name (or "NT$" if null is passed).
	 * 
	 * @param prefix
	 *            base name to construct unique name from.
	 */
	static NonTerminal createNew(int index, String prefix) {
		if (prefix == null) {
			prefix = "NT$";
		}
		return new NonTerminal(index, prefix + next_nt++);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** static routine for creating a new uniquely named hidden non-terminal */
	static NonTerminal createNew(int index) {
		return createNew(index, null);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** Table of all productions with this non terminal on the LHS. */
	protected HashSet<Production> _productions = new HashSet<Production>();

	/** Access to productions with this non terminal on the LHS. */
	public Set<Production> productions() {
		return _productions;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Total number of productions with this non terminal on the LHS. */
	public int num_productions() {
		return _productions.size();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Add a production to our set of productions. */
	public void add_production(Production prod) {
		/* catch improper productions */
		if (prod == null || prod.lhs() == null || prod.lhs().getSymbol() != this)
			throw new FatalCupException("Attempt to add invalid production to non terminal production table");

		/* add it to the table, keyed with itself */
		_productions.add(prod);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Nullability of this non terminal. */
	protected boolean _nullable;

	/** Nullability of this non terminal. */
	public boolean nullable() {
		return _nullable;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** First set for this non-terminal. */
	protected TerminalSet _first_set = new TerminalSet();

	/** First set for this non-terminal. */
	public TerminalSet first_set() {
		return _first_set;
	}

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Indicate that this symbol is a non-terminal. */
	public boolean is_non_term() {
		return true;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Test to see if this non terminal currently looks nullable. */
	protected boolean looks_nullable() {
		/* look and see if any of the productions now look nullable */
		for (Production prod : productions()) {
			/* if the production can go to empty, we are nullable */
			if (prod.checkNullable()) {
				return true;
			}
		}

		/* none of the productions can go to empty, so we are not nullable */
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** convert to string */
	public String toString() {
		return super.toString() + "[" + index() + "]" + (nullable() ? "*" : "");
	}

	/*-----------------------------------------------------------*/
}
