package net.natpad.cup.model.parser.generated.runtime;

/**
 * Defines the LrSymbol class, which is used to represent all terminals
 * and nonterminals while parsing.  The lexer should pass CUP LrSymbols 
 * and CUP returns a LrSymbol.
 *
 * @version last updated: 7/3/96
 * @author  Frank Flannery
 */

/* ****************************************************************
  Class LrSymbol
  what the parser expects to receive from the lexer. 
  the token is identified as follows:
  sym:    the symbol type
  parse_state: the parse state.
  value:  is the lexical value of type Object
  left :  is the left position in the original input file
  right:  is the right position in the original input file
******************************************************************/


public class LrSymbol {

	/** The symbol number of the terminal or non terminal being represented */
	public int sym;

	/** The parse state to be recorded on the parse stack with this symbol.
	 *  This field is for the convenience of the parser and shouldn't be 
	 *  modified except by the parser. 
	 */
	public int parse_state;

	/** This allows us to catch some errors caused by scanners recycling
	 *  symbols.  For the use of the parser only. [CSA, 23-Jul-1999]
	 */
	boolean used_by_parser = false;


	public int left, right;
	public Object value;


	public LrSymbol(int id, int left, int right, Object val) {
		this(id);
		this.left = left;
		this.right = right;
		this.value = val;
	}

	public LrSymbol(int id, Object o) {
		this(id, -1, -1, o);
	}

	public LrSymbol(int id, int l, int r) {
		this(id, l, r, null);
	}

	public LrSymbol(int sym_num) {
		this(sym_num, -1);
		left = -1;
		right = -1;
		value = null;
	}

	LrSymbol(int sym_num, int state) {
		sym = sym_num;
		parse_state = state;
	}



	public String toString() {
		return "#"+sym;
	}
}






