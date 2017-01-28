package net.natpad.cup.model.bnf;

import net.natpad.cup.state.Assoc;

/**
 * This class represents a terminal symbol in the grammar. Each terminal has a
 * textual name, an index, and a string which indicates the type of object it
 * will be implemented with at runtime (i.e. the class of object that will be
 * returned by the scanner and pushed on the parse stack to represent it).
 * 
 * @version last updated: 7/3/96
 * @author Frank Flannery
 */
public class Terminal extends Symbol implements Comparable<Terminal> {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	/**
	 * Full constructor.
	 * 
	 * @param nm    the name of the terminal.
	 * @param tp    the type of the terminal.
	 */
	private Terminal(int index, String nm, String tp, int precedence_side, int precedence_num) {
		/* superclass does most of the work */
		super(index, nm, tp);

		/* set the precedence */
		_precedence_num = precedence_num;
		_precedence_side = precedence_side;

	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor for non-precedented terminal
	 */

	public Terminal(int index, String nm, String tp) {
		this(index, nm, tp, Assoc.no_prec, -1);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor with default type.
	 * 
	 * @param nm
	 *            the name of the terminal.
	 */
	public Terminal(int index, String nm) {
		this(index, nm, null);
	}

	/*-----------------------------------------------------------*/
	/*-------------------  Class Variables  ---------------------*/
	/*-----------------------------------------------------------*/

	private int _precedence_num;
	private int _precedence_side;

	/*-----------------------------------------------------------*/
	/*--- (Access to) Static (Class) Variables ------------------*/
	/*-----------------------------------------------------------*/


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


	/*-----------------------------------------------------------*/
	/*--- General Methods ---------------------------------------*/
	/*-----------------------------------------------------------*/

	/** Report this symbol as not being a non-terminal. */
	public boolean is_non_term() {
		return false;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		return super.toString() + "[" + index() + "]";
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** get the precedence of a terminal */
	public int precedenceNum() {
		return _precedence_num;
	}

	public int precedenceSide() {
		return _precedence_side;
	}

	/** set the precedence of a terminal */
	public void setPrecedence(int side, int new_prec) {
		_precedence_side = side;
		_precedence_num = new_prec;
	}

	/*-----------------------------------------------------------*/

	@Override
	public boolean equals(Object obj) {
		if (obj instanceof Terminal) {
			Terminal other = (Terminal) obj;
			return other._name==_name || (other._name!=null && other._name.equals(_name));
		}
		return false;
	}
	
	@Override
	public int hashCode() {
		return _name==null ? 0 : _name.hashCode();
	}

	@Override
	public int compareTo(Terminal o) {
		if (o==null) {
			return 1;
		}
		if (o._index<_index) {
			return 1;
		} else if (o._index>_index) {
			return -1;
		}
		return 0;
	}
	
}
