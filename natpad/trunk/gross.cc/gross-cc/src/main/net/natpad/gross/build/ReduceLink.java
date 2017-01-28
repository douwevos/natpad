package net.natpad.gross.build;

import java.util.Objects;

import net.natpad.gross.runtime.Symbol;

public class ReduceLink {

	
	public final Kernel dotSet;
	public final Symbol symbol;
	
	public ReduceLink(Kernel dotSet, Symbol symbol) {
		this.dotSet = dotSet;
		this.symbol = symbol;
	}
	
	
	
	@Override
	public int hashCode() {
		return dotSet.hashCode()*13+symbol.hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof ReduceLink) {
			ReduceLink other = (ReduceLink) obj;
			return other.symbol==symbol && Objects.equals(other.dotSet,dotSet);
		}
		return false;
	}
	
	
	@Override
	public String toString() {
		return "ReduceLink[dotset:"+dotSet+" symbol:"+symbol+"]";
	}
	
}
