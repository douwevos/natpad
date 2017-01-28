package net.natpad.gross.build;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;

import net.natpad.gross.definition.bnf.AstDescriptionList;
import net.natpad.gross.definition.bnf.Specification.ProductionPartList;
import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.SymbolProvider;
import net.natpad.gross.runtime.Terminal;

public class CCModel implements SymbolProvider {

	
	int sub_prod_seq;
	int sequence;
	private final HashMap<Integer, Symbol> symbolMap = new HashMap<Integer, Symbol>();
	private final List<NonTerminalExt> nonTermList = new ArrayList<NonTerminalExt>();
	private final List<Terminal> termList = new ArrayList<Terminal>();
	private final List<Production> productionList = new ArrayList<Production>();
	private final TerminalExt eofTerminal, errorTerminal;
	
	private AstDescriptionList astDescriptionList;

	private String headerCodeText;
	
	public void setAstDescriptionList(AstDescriptionList astDescriptionList) {
		this.astDescriptionList = astDescriptionList;
	}
	
	public AstDescriptionList getAstDescriptionList() {
		return astDescriptionList;
	}
	
	public void setHeaderCodeText(String codeText) {
		headerCodeText = codeText;
	}
	
	public String getHeaderCodeText() {
		return headerCodeText;
	}

	
	public CCModel() {
		eofTerminal = addTerminal("⟧");
		errorTerminal = addTerminal("⊗");
		addNonTerminal("◎");
	}

	public TerminalExt getEofTerminal() {
		return eofTerminal;
	}

	
	public TerminalExt getErrorTerminal() {
		return errorTerminal;
	}

	@Override
	public Terminal getTerminalByName(String name) {
		for(Terminal term : termList) {
			if (name.equals(term.name)) {
				return term;
			}
		}
		return null;
	}

	public NonTerminalExt getNonTerminalByName(String name) {
		for(NonTerminalExt nterm : nonTermList) {
			if (name.equals(nterm.name)) {
				return nterm;
			}
		}
		return null;
	}

	public Symbol getSymbolByName(String symId) {
		for(Symbol sym : symbolMap.values()) {
			if (sym.name.equals(symId)) {
				return sym;
			}
		}
		return null;
	}

	
	public NonTerminalExt addNonTerminal(String name) {
		Integer newId = new Integer(sequence++);
		NonTerminalExt nonTerminal = new NonTerminalExt(name, newId);
		symbolMap.put(newId, nonTerminal);
		nonTermList.add(nonTerminal);
		if (nonTermList.size()==2) {
			NonTerminalExt ntStart = nonTermList.get(0);
			ArrayList<ProductionPart> parts = new ArrayList<ProductionPart>();
			parts.add(new ProductionPart(nonTerminal, null));
			parts.add(new ProductionPart(termList.get(0), null));
			addProduction(-1, null, ntStart, parts, null);
		}
		return nonTerminal;
	}

	public TerminalExt addTerminal(String name) {
		Integer newId = new Integer(sequence++);
		TerminalExt terminal = new TerminalExt(name, newId);
		symbolMap.put(newId, terminal);
		termList.add(terminal);
		return terminal;
	}


	public Production addProduction(NonTerminalExt lhs, List<ProductionPart> rhs_parts) {
		Integer prodId = new Integer(productionList.size());
		Production production = new Production(prodId, -1, null, lhs, rhs_parts, null);
		productionList.add(production);
		lhs.addProduction(production);
		return production;
	}

	
	public Production addProduction(int offset, String label, NonTerminalExt lhs, List<ProductionPart> rhs_parts, List<ProductionPart> excluded_rhs_parts) {
		Integer prodId = new Integer(productionList.size());
		Production production = new Production(prodId, offset, label, lhs, rhs_parts, excluded_rhs_parts);
		productionList.add(production);
		lhs.addProduction(production);
		return production;
	}
	
	public NonTerminalExt createSubNonTerminal() {
		String subNonTermName ="sub_list$"+sub_prod_seq++;
		return addNonTerminal(subNonTermName);
	}


	public NonTerminalExt addSubProductions(List<ProductionPartList> sub_pp_arrays) {
		NonTerminalExt lhs = createSubNonTerminal();
		for (ProductionPartList rhsParts : sub_pp_arrays) {
			addProduction(-1, null, lhs, rhsParts.list, rhsParts.excluded);
		}
		return lhs;
	}

	
	public Production getProduction(int index) {
		return productionList.get(index);
	}

	
	public void calculateNullabilty() {
		HashSet<NonTerminalExt> finished = new HashSet<NonTerminalExt>();
		while(finished.size()!=nonTermList.size()) {
			int lastFinished = finished.size();
			for(NonTerminalExt nonTerm : nonTermList) {
				if (!finished.contains(nonTerm)) {
					if (nonTerm.calculateNullability()) {
						finished.add(nonTerm);
					}
				}
			}
			if (lastFinished==finished.size()) {
				StringBuilder buf = new StringBuilder();
				for(NonTerminalExt nonTerm : nonTermList) {
					if (!finished.contains(nonTerm)) {
						buf.append(",").append(nonTerm);
					}
				}
				if (buf.length()>0) {
					buf.replace(0, 1, "");
				}
				throw new RuntimeException("Not able to calculate nullability for the next non-terminals:"+buf);
			}
		}
	}
	
	public void calculateFirstSets() {
		boolean didChange = true;
		while(didChange) {
			didChange = false;
			for(NonTerminalExt nonTerm : nonTermList) {
				if (nonTerm.computeFirstSet()) {
					didChange = true;
				}
			}
		}
	}
	
	
	public Iterator<Terminal> terminalIterator() {
		return termList.iterator();
	}
	
	public Iterator<NonTerminalExt> nonTerminalIterator() {
		return nonTermList.iterator();
	}

	public Collection<Symbol> allSymbols() {
		return symbolMap.values();
	}

	public Iterable<Production> productionsIterable() {
		return productionList;
	}



	
	
}
