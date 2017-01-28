package net.natpad.cup.model.parser;

import java.util.HashSet;
import java.util.Hashtable;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.ProductionPart;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;
import net.natpad.cup.model.bnf.Terminal;
import net.natpad.cup.state.Assoc;

public class ParserHelper {

	/** helper routine to clone a new production part adding a given label */
	public ProductionPart add_lab(ProductionPart part, String lab) throws FatalCupException {
		/* if there is no label, or this is an action, just return the original */
		if (lab == null || part.isAction())
			return part;

		/* otherwise build a new one with the given label attached */
		return new SymbolPart(((SymbolPart) part).getSymbol(), lab);
	}

	/** max size of right hand side we will support */
	protected final int MAX_RHS = 200;

	/** array for accumulating right hand side parts */
	public ProductionPart[] rhs_parts = new ProductionPart[MAX_RHS];

	/** where we are currently in building a right hand side */
	public int rhs_pos = 0;

	/** start a new right hand side */
	public void new_rhs() {
		rhs_pos = 0;
	}

	/** add a new right hand side part */
	public void add_rhs_part(ProductionPart part) throws java.lang.Exception {
		if (rhs_pos >= MAX_RHS)
			throw new Exception("Internal Error: Productions limited to "
					+ MAX_RHS + " symbols and actions");

		rhs_parts[rhs_pos] = part;
		rhs_pos++;
	}

	/** string to build up multiple part names */
	public String multipart_name = new String();

	/** append a new name segment to the accumulated multipart name */
	public void append_multipart(String name) {
		String dot = "";

		/* if we aren't just starting out, put on a dot */
		if (multipart_name.length() != 0)
			dot = ".";

		multipart_name = multipart_name.concat(dot + name);
	}

	/** table of declared symbols -- contains production parts indexed by name */
	public Hashtable symbols = new Hashtable();

	/** table of just non terminals -- contains non_terminals indexed by name */
	public Hashtable non_terms = new Hashtable();

	public HashSet undefinedSymbols = new HashSet();
	
	/** declared start NonTerminal */
	public NonTerminal start_nt = null;

	/** left hand side non terminal of the current production */
	public NonTerminal lhs_nt;

	/** Current precedence number */
	int _cur_prec = 0;

	/** Current precedence side */
	int _cur_side = Assoc.no_prec;

	/** update the precedences we are declaring */
	public void update_precedence(int p) {
		_cur_side = p;
		_cur_prec++;
	}

	/** add relevant data to terminals */
	public void add_precedence(String term) {
		if (term == null) {
			System.err.println("Unable to add precedence to nonexistent terminal");
		} else {
			SymbolPart sp = (SymbolPart) symbols.get(term);
			if (sp == null) {
				System.err.println("Could find terminal " + term + " while declaring precedence");
			} else {
				Symbol sym = sp.getSymbol();
				if (sym instanceof Terminal) {
					((Terminal) sym).setPrecedence(_cur_side, _cur_prec);
				} else {
					System.err.println("Precedence declaration: Can't find terminal "+ term);
				}
			}
		}
	}

	
	public boolean addUndefinedSymbol(String symbol) {
		return undefinedSymbols.add(symbol);
	}
	
}
