package net.natpad.cup;

import java.io.PrintStream;

import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.Terminal;
import net.natpad.cup.state.ParseAction;
import net.natpad.cup.state.ReduceAction;

/**
 * This class represents one row (corresponding to one machine state) of the
 * parse action table.
 */
public class ParseActionRow {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	public final BnfModel model;
	
	/**
	 * Simple constructor. Note: this should not be used until the number of
	 * terminals in the grammar has been established.
	 */
	public ParseActionRow(BnfModel model) {
		this.model = model;
		/* make sure the size is set */
		if (_size <= 0)
			_size = model.terminals.count();

		/* allocate the array */
		under_term = new ParseAction[size()];

		/* set each element to an error action */
		for (int i = 0; i < _size; i++)
			under_term[i] = new ParseAction();
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Static (Class) Variables ------------------*/
	/*-----------------------------------------------------------*/

	/** Number of columns (terminals) in every row. */
	protected int _size = 0;

	/** Number of columns (terminals) in every row. */
	public int size() {
		return _size;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Table of reduction counts (reused by compute_default()). */
	protected int reduction_count[] = null;

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** Actual action entries for the row. */
	public ParseAction under_term[];

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Default (reduce) action for this row. -1 will represent default of error.
	 */
	public int default_reduce;

	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Compute the default (reduce) action for this row and store it in
	 * default_reduce. In the case of non-zero default we will have the effect
	 * of replacing all errors by that reduction. This may cause us to do
	 * erroneous reduces, but will never cause us to shift past the point of the
	 * error and never cause an incorrect parse. -1 will be used to encode the
	 * fact that no reduction can be used as a default (in which case error will
	 * be used).
	 */
	public void compute_default() {
		int i, prod, max_prod, max_red;

		/* if we haven't allocated the count table, do so now */
		if (reduction_count == null)
			reduction_count = new int[model.productions.count()];

		/* clear the reduction count table and maximums */
		for (i = 0; i < model.productions.count(); i++)
			reduction_count[i] = 0;
		max_prod = -1;
		max_red = 0;

		/* walk down the row and look at the reduces */
		for (i = 0; i < size(); i++)
			if (under_term[i].kind() == ParseAction.REDUCE) {
				/*
				 * count the reduce in the proper production slot and keep the
				 * max up to date
				 */
				prod = ((ReduceAction) under_term[i]).reduce_with().index();
				reduction_count[prod]++;
				if (reduction_count[prod] > max_red) {
					max_red = reduction_count[prod];
					max_prod = prod;
				}
			}

		/* record the max as the default (or -1 for not found) */
		default_reduce = max_prod;
	}

	public void describe(PrintStream out) {
		out.println("  Expect:");
		for (int col = 0; col < size(); col++) {
			/* if the action is not an error print it */
			Terminal terminal = model.terminals.get(col);
			if (under_term[col].kind() != ParseAction.ERROR) {
				out.println("    " + terminal.name() + " : " +under_term[col] + "]");
			}
		}
		out.println();
	}

	/*-----------------------------------------------------------*/

}
