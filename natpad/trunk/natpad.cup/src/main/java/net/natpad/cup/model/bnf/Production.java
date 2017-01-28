package net.natpad.cup.model.bnf;

import java.util.ArrayList;
import java.util.List;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.TerminalSet;

/**
 * This class represents a production in the grammar. It contains a LHS non
 * terminal, and an array of RHS symbols. As various transformations are done on
 * the RHS of the production, it may shrink. As a result a separate length is
 * always maintained to indicate how much of the RHS array is still valid.
 * <p>
 * 
 * I addition to construction and manipulation operations, productions provide
 * methods for factoring out actions (see remove_embedded_actions()), for
 * computing the nullability of the production (i.e., can it derive the empty
 * string, see check_nullable()), and operations for computing its first set
 * (i.e., the set of terminals that could appear at the beginning of some string
 * derived from the production, see check_first_set()).
 * 
 * @see net.natpad.cup.model.bnf.ProductionPart
 * @see net.natpad.cup.model.bnf.SymbolPart
 * @see net.natpad.cup.model.bnf.ActionPart
 * @version last updated: 7/3/96
 * @author Frank Flannery
 */

public class Production {

	/** Index number of the production. */
	protected final int _index;
	
	protected String fullActionString;
	List<ActionDeclaration> declarations;

	
	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	
	/**
	 * Full constructor. This constructor accepts a LHS non terminal, an array
	 * of RHS parts (including terminals, non terminals, and actions), and a
	 * string for a final reduce action. It does several manipulations in the
	 * process of creating a production object. After some validity checking it
	 * translates labels that appear in actions into code for accessing objects
	 * on the runtime parse stack. It them merges adjacent actions if they
	 * appear and moves any trailing action into the final reduce actions
	 * string. Next it removes any embedded actions by factoring them out with
	 * new action productions. Finally it assigns a unique index to the
	 * production.
	 * <p>
	 * 
	 * Factoring out of actions is accomplished by creating new "hidden" non
	 * terminals. For example if the production was originally:
	 * 
	 * <pre>
	 *    A ::= B {action} C D
	 * </pre>
	 * 
	 * then it is factored into two productions:
	 * 
	 * <pre>
	 *    A ::= B X C D
	 *    X ::= {action}
	 * </pre>
	 * 
	 * (where X is a unique new non terminal). This has the effect of placing
	 * all actions at the end where they can be handled as part of a reduce by
	 * the parser.
	 */
	public Production(int index, NonTerminal lhs_sym, ProductionPart rhs_parts[], int rhs_l, String actionString) {
		this._index = index;
		_action_is_user = false;
		fullActionString = actionString;
		int i;
		ActionPart tail_action;
		int rightlen = rhs_l;

		/* remember the length */
		if (rhs_l >= 0) {
			_rhs_length = rhs_l;
		} else if (rhs_parts != null) {
			_rhs_length = rhs_parts.length;
		} else {
			_rhs_length = 0;
		}

		/* make sure we have a valid left-hand-side */
		if (lhs_sym == null) {
			throw new FatalCupException("Attempt to construct a production with a null LHS");
		}

		/*
		 * I'm not translating labels anymore, I'm adding code to declare labels
		 * as valid variables. This way, the users code string is untouched 6/96
		 * frankf
		 */


		/* count use of lhs */
		lhs_sym.note_use();

		/* create the part for left-hand-side */
		_lhs = new SymbolPart(lhs_sym);

		/* merge adjacent actions (if any) */
		_rhs_length = mergeAdjacentActions(rhs_parts, _rhs_length);

		
		
		/*
		 * check if the last part of the right hand side is an action. If it is,
		 * it won't be on the stack, so we don't want to count it in the
		 * rightlen. Then when we search down the stack for a Symbol, we don't
		 * try to search past action
		 */

		if (_rhs_length > 0) {
			if (rhs_parts[_rhs_length - 1].isAction()) {
				rightlen = _rhs_length - 1;
			} else {
				rightlen = _rhs_length;
			}
		}

		/* get the generated declaration code for the necessary labels. */
		declarations = declareLabels(rhs_parts, rightlen);

		
		
		/* strip off any trailing action */
		tail_action = stripTrailingAction(rhs_parts, _rhs_length);
		if (tail_action != null)
			_rhs_length--;

		/*
		 * Why does this run through the right hand side happen over and over?
		 * here a quick combination of two prior runs plus one I wanted of my
		 * own frankf 6/25/96
		 */
		/* allocate and copy over the right-hand-side */
		/* count use of each rhs symbol */
		_rhs = new ProductionPart[_rhs_length];
		for (i = 0; i < _rhs_length; i++) {
			_rhs[i] = rhs_parts[i];
			if (!_rhs[i].isAction()) {
				((SymbolPart) _rhs[i]).getSymbol().note_use();
				if (((SymbolPart) _rhs[i]).getSymbol() instanceof Terminal) {
					_rhs_prec = ((Terminal) ((SymbolPart) _rhs[i]).getSymbol()).precedenceNum();
					_rhs_assoc = ((Terminal) ((SymbolPart) _rhs[i]).getSymbol()).precedenceSide();
				}
			}
		}

		/*
		 * now action string is really declaration string, so put it in front!
		 * 6/14/96 frankf
		 */
		if (fullActionString == null) {
			fullActionString = "";
		}

		if (tail_action != null && tail_action.code_string() != null) {
			if (fullActionString.length()>0) {
				fullActionString += "\n";
			}
			
			fullActionString += tail_action.code_string();
		}

		
		
		/* stash the action */
//		_action = new ActionPart(fullActionString);

		/* put us in the production list of the lhs non terminal */
		lhs_sym.add_production(this);
	}

	
	/**
	 * An action_part containing code for the action to be performed when we
	 * reduce with this production.
	 */
	protected ActionPart _action;
	boolean _action_is_user;

	/**
	 * An action_part containing code for the action to be performed when we
	 * reduce with this production.
	 */
	public ActionPart action() {
		String local = "";
		
		if (fullActionString == null) {
			local = "";
		} else {
			local = fullActionString;
		}
		
		
		_action = new ActionPart(local);

		
		return _action;
	}

	public List<ActionDeclaration> getDeclarations() {
		return declarations;
	}
	
	
	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Constructor with no action string. */
	public Production(int index, NonTerminal lhs_sym, ProductionPart rhs_parts[], int rhs_l) {
		this(index, lhs_sym, rhs_parts, rhs_l, null);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/*
	 * Constructor with precedence and associativity of production contextually
	 * define
	 */
	public Production(int index, NonTerminal lhs_sym, ProductionPart rhs_parts[], int rhs_l, String action_str, int prec_num,
					int prec_side) {
		this(index, lhs_sym, rhs_parts, rhs_l, action_str);

		/* set the precedence */
		set_precedence_num(prec_num);
		set_precedence_side(prec_side);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/*
	 * Constructor w/ no action string and contextual precedence defined
	 */
	public Production(int index, NonTerminal lhs_sym, ProductionPart rhs_parts[], int rhs_l, int prec_num, int prec_side) {
		this(index, lhs_sym, rhs_parts, rhs_l, null);
		/* set the precedence */
		set_precedence_num(prec_num);
		set_precedence_side(prec_side);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/*-----------------------------------------------------------*/
	/*--- (Access to) Static (Class) Variables ------------------*/
	/*-----------------------------------------------------------*/


	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** The left hand side non-terminal. */
	protected SymbolPart _lhs;

	/** The left hand side non-terminal. */
	public SymbolPart lhs() {
		return _lhs;
	}

	/** The precedence of the rule */
	protected int _rhs_prec = -1;
	protected int _rhs_assoc = -1;

	/** Access to the precedence of the rule */
	public int precedence_num() {
		return _rhs_prec;
	}

	public int precedence_side() {
		return _rhs_assoc;
	}

	/** Setting the precedence of a rule */
	public void set_precedence_num(int prec_num) {
		_rhs_prec = prec_num;
	}

	public void set_precedence_side(int prec_side) {
		_rhs_assoc = prec_side;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** A collection of parts for the right hand side. */
	protected ProductionPart _rhs[];

	/** Access to the collection of parts for the right hand side. */
	public ProductionPart rhs(int indx) {
		if (indx >= 0 && indx < _rhs_length)
			return _rhs[indx];
		else
			throw new FatalCupException("Index out of range for right hand side of production");
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** How much of the right hand side array we are presently using. */
	protected int _rhs_length;

	/** How much of the right hand side array we are presently using. */
	public int rhsLength() {
		return _rhs_length;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/** Index number of the production. */
	public int index() {
		return _index;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Count of number of reductions using this production. */
	protected int _num_reductions = 0;

	/** Count of number of reductions using this production. */
	public int num_reductions() {
		return _num_reductions;
	}

	/** Increment the count of reductions with this non-terminal */
	public void note_reduction_use() {
		_num_reductions++;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Is the nullability of the production known or unknown? */
	protected boolean _nullable_known = false;

	/** Is the nullability of the production known or unknown? */
	public boolean nullable_known() {
		return _nullable_known;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Nullability of the production (can it derive the empty string). */
	protected boolean _nullable = false;

	/** Nullability of the production (can it derive the empty string). */
	public boolean nullable() {
		return _nullable;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * First set of the production. This is the set of terminals that could
	 * appear at the front of some string derived from this production.
	 */
	protected TerminalSet _first_set = new TerminalSet();

	/**
	 * First set of the production. This is the set of terminals that could
	 * appear at the front of some string derived from this production.
	 */
	public TerminalSet firstSet() {
		return _first_set;
	}

	/*-----------------------------------------------------------*/
	/*--- Static Methods ----------------------------------------*/
	/*-----------------------------------------------------------*/


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Declare label names as valid variables within the action string
	 * 
	 * @param rhs            array of RHS parts.
	 * @param rhs_len        how much of rhs to consider valid.
	 */
	protected List<ActionDeclaration> declareLabels(ProductionPart rhs[], int rhs_len) {
		List<ActionDeclaration> declarations = new ArrayList<ActionDeclaration>();
		
		SymbolPart part;
		int pos;

		/* walk down the parts and extract the labels */
		for (pos = 0; pos < rhs_len; pos++) {
			if (!rhs[pos].isAction()) {
				part = (SymbolPart) rhs[pos];

				/* if it has a label, make declaration! */
				if (part.label() != null) {
					declarations.add(new ActionDeclaration(part.label(), part.getSymbol().getStackType(), rhs_len - pos - 1));
				}
			}
		}
		return declarations;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Helper routine to merge adjacent actions in a set of RHS parts
	 * 
	 * @param rhs_parts   array of RHS parts.
	 * @param len         amount of that array that is valid.
	 * @return remaining valid length.
	 */
	protected int mergeAdjacentActions(ProductionPart rhs_parts[], int len) {
		int from_loc, to_loc, merge_cnt;

		/* bail out early if we have no work to do */
		if (rhs_parts == null || len == 0)
			return 0;

		merge_cnt = 0;
		to_loc = -1;
		for (from_loc = 0; from_loc < len; from_loc++) {
			/* do we go in the current position or one further */
			if (to_loc < 0 || !rhs_parts[to_loc].isAction() || !rhs_parts[from_loc].isAction()) {
				/* next one */
				to_loc++;

				/* clear the way for it */
				if (to_loc != from_loc) {
					rhs_parts[to_loc] = null;
				}
			}

			/* if this is not trivial? */
			if (to_loc != from_loc) {
				/* do we merge or copy? */
				if (rhs_parts[to_loc] != null && rhs_parts[to_loc].isAction() && rhs_parts[from_loc].isAction()) {
					/* merge */
					String baseCodeString = ( (ActionPart) rhs_parts[to_loc]).code_string();
					String nextCodeString = ((ActionPart) rhs_parts[from_loc]).code_string();
					StringBuilder buf = new StringBuilder();
					if (!baseCodeString.trim().isEmpty()) {
						buf.append(baseCodeString);
					}
					if (!nextCodeString.trim().isEmpty()) {
						if (buf.length()>0) {
							buf.append('\n');
						}
						buf.append(nextCodeString);
					}
					rhs_parts[to_loc] = new ActionPart(buf.toString());
					merge_cnt++;
				} else {
					/* copy */
					rhs_parts[to_loc] = rhs_parts[from_loc];
				}
			}
		}

		/* return the used length */
		return len - merge_cnt;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Helper routine to strip a trailing action off rhs and return it
	 * 
	 * @param rhs_parts   array of RHS parts.
	 * @param len         how many of those are valid.
	 * @return the removed action part.
	 */
	protected ActionPart stripTrailingAction(ProductionPart rhs_parts[], int len) {
		ActionPart result;

		/* bail out early if we have nothing to do */
		if (rhs_parts == null || len == 0)
			return null;

		/* see if we have a trailing action */
		if (rhs_parts[len - 1].isAction()) {
			/* snip it out and return it */
			result = (ActionPart) rhs_parts[len - 1];
			rhs_parts[len - 1] = null;
			return result;
		} else
			return null;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Check to see if the production (now) appears to be nullable. A production
	 * is nullable if its RHS could derive the empty string. This results when
	 * the RHS is empty or contains only non terminals which themselves are
	 * nullable.
	 */
	public boolean checkNullable() {
		ProductionPart part;
		Symbol sym;
		int pos;

		/* if we already know bail out early */
		if (nullable_known())
			return nullable();

		/* if we have a zero size RHS we are directly nullable */
		if (rhsLength() == 0) {
			/* stash and return the result */
			return setNullable(true);
		}

		/* otherwise we need to test all of our parts */
		for (pos = 0; pos < rhsLength(); pos++) {
			part = rhs(pos);

			/* only look at non-actions */
			if (!part.isAction()) {
				sym = ((SymbolPart) part).getSymbol();

				/* if its a terminal we are definitely not nullable */
				if (!sym.is_non_term())
					return setNullable(false);
				/* its a non-term, is it marked nullable */
				else if (!((NonTerminal) sym).nullable())
					/* this one not (yet) nullable, so we aren't */
					return false;
			}
		}

		/* if we make it here all parts are nullable */
		return setNullable(true);
	}

	/** set (and return) nullability */
	boolean setNullable(boolean v) {
		_nullable_known = true;
		_nullable = v;
		return v;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Update (and return) the first set based on current NT firsts. This
	 * assumes that nullability has already been computed for all non terminals
	 * and productions.
	 */
	public TerminalSet checkFirstSet() {
		int part;
		Symbol sym;

		/* walk down the right hand side till we get past all nullables */
		for (part = 0; part < rhsLength(); part++) {
			/* only look at non-actions */
			if (!rhs(part).isAction()) {
				sym = ((SymbolPart) rhs(part)).getSymbol();

				/* is it a non-terminal? */
				if (sym.is_non_term()) {
					/* add in current firsts from that NT */
					_first_set.add(((NonTerminal) sym).first_set());

					/* if its not nullable, we are done */
					if (!((NonTerminal) sym).nullable())
						break;
				} else {
					/* its a terminal -- add that to the set */
					_first_set.add((Terminal) sym);

					/* we are done */
					break;
				}
			}
		}

		/* return our updated first set */
		return firstSet();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Equality comparison. */
	public boolean equals(Production other) {
		if (other == null)
			return false;
		return other._index == _index;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Generic equality comparison. */
	public boolean equals(Object other) {
		if (!(other instanceof Production))
			return false;
		else
			return equals((Production) other);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Produce a hash code. */
	public int hashCode() {
		/* just use a simple function of the index */
		return _index * 13;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		String result;

		/* catch any internal errors */
		result = "production [" + index() + "]: ";
		result += ((lhs() != null) ? lhs().toString() : "$$NULL-LHS$$");
		result += " :: = ";
		for (int i = 0; i < rhsLength(); i++) {
			result += rhs(i) + " ";
		}
		result += ";";
		if (action() != null && action().code_string() != null) {
			result += " {" + action().code_string() + "}";
		}

		if (nullable_known()) {
			if (nullable()) {
				result += "[NULLABLE]";
			} else {
				result += "[NOT NULLABLE]";
			}
		}

		return result;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a simpler string. */
	public String toSimpleString() {
		String result;

		result = ((lhs() != null) ? lhs().getSymbol().name() : "NULL_LHS");
		result += " ::= ";
		for (int i = 0; i < rhsLength(); i++)
			if (!rhs(i).isAction())
				result += ((SymbolPart) rhs(i)).getSymbol().name() + " ";

		return result;
	}

	/*-----------------------------------------------------------*/

}
