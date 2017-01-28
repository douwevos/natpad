package net.natpad.cup.model.bnf;

/**
 * This abstract class serves as the base class for grammar symbols (i.e., both terminals and non-terminals). Each
 * symbol has a name string, and a string giving the type of object that the symbol will be represented by on the
 * runtime parse stack. In addition, each symbol maintains a use count in order to detect symbols that are declared but
 * never used, and an index number that indicates where it appears in parse tables (index numbers are unique within
 * terminals or non terminals, but not across both).
 * 
 * @see net.natpad.cup.model.bnf.Terminal
 * @see net.natpad.cup.model.bnf.NonTerminal
 * @version last updated: 7/3/96
 * @author Frank Flannery
 */
public abstract class Symbol {

	/**
	 * Index of this symbol (terminal or non terminal) in the parse tables.
	 * Note: indexes are unique among terminals and unique among non terminals,
	 * however, a terminal may have the same index as a non-terminal, etc.
	 */
	protected final int _index;

	
	/**
	 * Full constructor.
	 * 
	 * @param nm the name of the symbol.
	 * @param tp a string with the type name.
	 */
	public Symbol(int index, String nm, String tp) {
		this._index = index;
		/* sanity check */
		if (nm == null)
			nm = "";

//		/* apply default if no type given */
//		if (tp == null)
//			tp = "Object";

		_name = nm;
		_stack_type = tp;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Constructor with default type.
	 * 
	 * @param nm the name of the symbol.
	 */
	public Symbol(int index, String nm) {
		this(index, nm, null);
	}

	/*-----------------------------------------------------------*/
	/*--- (Access to) Instance Variables ------------------------*/
	/*-----------------------------------------------------------*/

	/** String for the human readable name of the symbol. */
	protected String _name;

	/** String for the human readable name of the symbol. */
	public String name() {
		return _name;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** String for the type of object used for the symbol on the parse stack. */
	protected String _stack_type;

	/** String for the type of object used for the symbol on the parse stack. */
	public String getStackType() {
		return _stack_type;
	}
	
	public void setStackType(String stackType) {
		_stack_type = stackType;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Count of how many times the symbol appears in productions. */
	protected int _use_count = 0;

	/** Count of how many times the symbol appears in productions. */
	public int use_count() {
		return _use_count;
	}

	/** Increment the use count. */
	public void note_use() {
		_use_count++;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Index of this symbol (terminal or non terminal) in the parse tables. Note: indexes are unique among terminals and
	 * unique among non terminals, however, a terminal may have the same index as a non-terminal, etc.
	 */
	public int index() {
		return _index;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Indicate if this is a non-terminal. Here in the base class we don't know, so this is abstract.
	 */
	public abstract boolean is_non_term();

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Convert to a string. */
	public String toString() {
		return name();
	}

	/*-----------------------------------------------------------*/

}
