package net.natpad.cup;

import java.io.PrintStream;

import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Terminal;
import net.natpad.cup.state.LalrState;
import net.natpad.cup.state.ParseAction;

/**
 * This class represents one row (corresponding to one machine state) of the
 * reduce-goto parse table.
 */
public class ParseReduceRow {
	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	BnfModel model;
	
	/**
	 * Simple constructor. Note: this should not be used until the number of
	 * terminals in the grammar has been established.
	 */
	public ParseReduceRow(BnfModel model) {
		this.model = model;
		/* make sure the size is set */
		if (_size <= 0) {
			_size = model.nonTerminals.count();
		}

		/* allocate the array */
		under_non_term = new LalrState[size()];
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Static (Class) Variables ------------------*/
	/*-----------------------------------------------------------*/

	/** Number of columns (non terminals) in every row. */
	protected int _size = 0;

	/** Number of columns (non terminals) in every row. */
	public int size() {
		return _size;
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** Actual entries for the row. */
	public LalrState under_non_term[];

	public void describe(PrintStream out) {
		for (int col = 0; col < size(); col++) {
			/* if the action is not an error print it */
			NonTerminal terminal = model.nonTerminals.get(col);
			if (under_non_term[col] != null) {
				out.println("    " + terminal.name() + " : shift to state " +under_non_term[col].index() + "]");
			}
		}
		out.println();
	
	}
}
