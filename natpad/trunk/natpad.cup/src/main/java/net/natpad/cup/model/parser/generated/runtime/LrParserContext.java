package net.natpad.cup.model.parser.generated.runtime;

import java.util.Stack;

public class LrParserContext {

	public final LrScanner scanner;

	/** Internal flag to indicate when parser should quit. */
	protected boolean _done_parsing = false;

	/** Indication of the index for top of stack (for use by actions). */
	public int tos;

	/** The current lookahead Symbol. */
	protected LrSymbol cur_token;

	/** The parse stack itself. */
	protected Stack stack = new Stack();


	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	protected LrSymbol lookahead[];

	/** Position in lookahead input buffer used for "parse ahead". */
	protected int lookahead_pos;

	
	public LrParserContext(LrScanner scanner) {
		this.scanner = scanner;
	}
	
	

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * This method is called to indicate that the parser should quit. This is
	 * normally called by an accept action, but can be used to cancel parsing
	 * early in other circumstances if desired.
	 */
	public void doneParsing() {
		_done_parsing = true;
	}

	

	
	
	public LrSymbol peek() {
		return (LrSymbol) stack.peek();
	}

	public LrSymbol pop() {
		tos--;
		return (LrSymbol) stack.pop();
	}

	
	public void push(LrSymbol symbol) {
		stack.push(symbol);
	}



	public void shift(LrSymbol symbol) {
		stack.push(symbol);
		tos++;
	}

	public LrSymbol getFromTop(int reverseIndex) {
		return (LrSymbol) stack.elementAt(tos-reverseIndex);
	}



	public void dump() {
		for(int idx=stack.size()-1; idx>=0; idx--) {
			LrSymbol object = (LrSymbol) stack.get(idx);
			System.out.println("stack["+idx+"] "+object.sym+", parse_state="+object.parse_state);
		}
		if (cur_token!=null) {
			System.out.println(cur_token.sym+", parse_state="+cur_token.parse_state);
		}
	}
	
}
