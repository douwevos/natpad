package net.natpad.gross.build;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.natpad.gross.runtime.NonTerminal;
import net.natpad.gross.runtime.Terminal;

public class NonTerminalExt extends NonTerminal implements Iterable<Production>, SymbolWithType {

	private List<Production> productionList = new ArrayList<Production>();
	private final SymbolSet firstSet = new SymbolSet();
	
	public Boolean nullable;
	private String returnType;
	
	public NonTerminalExt(String name, Integer id) {
		super(name, id);
	}
	
	
	@Override
	public String getType() {
		return returnType;
	}
	
	@Override
	public void setType(String returnType) {
		this.returnType = returnType;
	}
	
	public void addProduction(Production production) {
		productionList.add(production);
		if (production.rhsCount()==0) {
			nullable = Boolean.TRUE;
		}
	}

	public void setNullable(Boolean nullable) {
		this.nullable = nullable;
	}
	
	public Boolean getNullable() {
		return nullable;
	}
	
	public SymbolSet firstSet() {
		return firstSet;
	}

	
	public int productionCount() {
		return productionList.size();
	}
	
	public Production productionAt(int index) {
		return productionList.get(index);
	}

	@Override
	public Iterator<Production> iterator() {
		return productionList.iterator();
	}


	public boolean calculateNullability() {
		if (nullable!=null) {
			return true;
		}
		
		nullable = Boolean.TRUE;
		for(Production prod : productionList) {
			Boolean prodNullability = prod.calculateNullability();
			if (prodNullability==Boolean.TRUE) {
				nullable = Boolean.TRUE;
				return true;
			} else if (prodNullability==null) {
				nullable = null;
			} else if (prodNullability==Boolean.FALSE && nullable!=null) {
				nullable = Boolean.FALSE;
			}
		}
		
		return nullable!=null;
	}

	public boolean computeFirstSet() {
		boolean didChange = false;
		out:
		for(Production prod : productionList) {
			for(ProductionPart part : prod) {
//				if (part.symbol!=this) {
					if (firstSet.add(part.symbol)) {
						didChange = true;
					}
//				}
				if (part.symbol instanceof Terminal) {
					continue out;
				} else if (part.symbol instanceof NonTerminalExt) {
					NonTerminalExt nt = (NonTerminalExt) part.symbol;
					if (nt!=this && firstSet.addAll(nt.firstSet)) {
						didChange = true;
					}
					
					if (nt.getNullable()==Boolean.FALSE) {
						continue out;
					}
				}
			}
		}
		return didChange;
	}


	public void productionEnlist(List<Production> consider) {
		consider.addAll(productionList);
	}



}
