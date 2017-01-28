package net.natpad.gross.build;

import java.util.Arrays;

import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.Terminal;

public class DotState {
	
	public final Production production;
	public final int dotPos;
	public int nullified[];
	private boolean isTerminated;
	private SymbolSet firstSet;
	
	
	public DotState(Production production, int dotPos) {
		this(production, dotPos, null);
	}
	

	public DotState(Production production, int dotPos, int nullified[]) {
		this.production = production;
		this.dotPos = dotPos;
		this.nullified = nullified;
	}

	
	public SymbolSet getLocalFirstSet() {
		if (firstSet==null) {
			firstSet = new SymbolSet();

			isTerminated = false;
			for(int idx=dotPos; idx<production.rhsCount(); idx++) {
				ProductionPart rhsAt = production.rhsAt(idx);
				Symbol symbol = rhsAt.symbol;
				if (symbol instanceof Terminal) {
					firstSet.add(symbol);
					isTerminated = true;
					break;
				} else if (symbol instanceof NonTerminalExt) {
					NonTerminalExt nte = (NonTerminalExt) symbol;
					firstSet.addAll(nte.firstSet());
					if (nte.getNullable()!=Boolean.TRUE) {
						isTerminated = true;
						break;
					}
				}
			}
		}
		return firstSet;
	}



	
	public boolean isAtEnd() {
		return dotPos>=production.rhsCount();
	}

	public boolean isAtLast() {
		return dotPos>=production.rhsCount()-1;
	}

	public boolean isTerminated() {
		getLocalFirstSet();
		return isTerminated;
	}
	
	public boolean isLeftRecursive() {
		if (production.rhsCount()==0) {
			return false;
		}
		if (nullified!=null) {
			return false;
		}
//		for(int idx=0; idx<production.rhsCount(); idx++) {
		int idx = 0;
			Symbol leftSymbol = production.rhsAt(idx).symbol;
			if (leftSymbol==production.lhs) {
				return true;
			} else if (leftSymbol instanceof NonTerminalExt) {
				NonTerminalExt nte = (NonTerminalExt) leftSymbol;
				if (nte.firstSet().contains(production.lhs)) {
					return true;
				}
				if (nte.getNullable()!=Boolean.TRUE) {
					return false;
				}
			} else {
				return false;
			}
//		}
		return false;
	}
	
	
	public DotState shift() {
		return shift(false);
	}
	
	public DotState shift(boolean doNullify) {
		DotState dotState = null;
		if (!isAtEnd()) {

			int nextNullified[] = nullified;
			if (doNullify && dotPos<production.rhsCount()) {
				if (nextNullified==null) {
					nextNullified = new int[1];
					nextNullified[0] = dotPos;
				} else {
					nextNullified = new int[nextNullified.length+1];
					System.arraycopy(nullified, 0, nextNullified, 0, nullified.length);
					nextNullified[nullified.length] = dotPos;
				}
			}
			
			dotState = new DotState(production, dotPos+1, nextNullified);
		}
		return dotState;
	}
	

	public Symbol getSymbolAtDot() {
		if (isAtEnd()) {
			return null;
		}
		ProductionPart partAt = production.rhsAt(dotPos);
		return partAt.symbol;
	}

	
	public boolean isNullified(int dotPos) {
		if (nullified==null) {
			return false;
		}
		for(int k : nullified) {
			if (k==dotPos) {
				return true;
			}
		}
		return false;
	}

	
	public void prefix(StringBuilder buf) {
		buf.append("[");
		for(int idx=0; idx<dotPos; idx++) {
			ProductionPart partAt = production.rhsAt(idx);
			if (idx!=0){
				buf.append(' ');
			}
			boolean isNullified = isNullified(idx);
			if (isNullified) {
				buf.append('~');	
			}
			buf.append(partAt);
		}
	}

	public void postfix(StringBuilder buf,int level) {
		if (production.rhsCount()==0) {
			buf.append("∅");
		}
		for(int idx=dotPos + (level>0 ? 1 : 0); idx<production.rhsCount(); idx++) {
			ProductionPart partAt = production.rhsAt(idx);
			if (idx!=dotPos){
				buf.append(' ');
			}
			boolean isNullified = isNullified(idx);
			if (isNullified) {
				buf.append('~');	
			}
			buf.append(partAt);
		}
		buf.append(" ⇒ "+production.lhs.name+"");
		if (isLeftRecursive()) {
			buf.append(" (left-rec)");
		}
		buf.append(']');
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		prefix(buf);
		buf.append("⚫");
		postfix(buf, 0);
		
//		if (nullified!=null) {
//			for(int s : nullified) {
//				buf.append(" ").append(s);
//			}
//		}
		
		return buf.toString();
	}


	@Override
	public int hashCode() {
		return production.id*13 + dotPos*117;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof DotState) {
			DotState other = (DotState) obj;
			return other.dotPos == dotPos
					&& other.production == production && Arrays.equals(other.nullified, nullified);
		}
		return false;
	}


	public DotState createTerminated() {
		DotState ds = this;
		while(!ds.isAtEnd()) {
			ds = ds.shift(true);
		}
		return ds;
	}



}
