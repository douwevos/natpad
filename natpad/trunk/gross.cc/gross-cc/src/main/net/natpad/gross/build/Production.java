package net.natpad.gross.build;

import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import net.natpad.gross.runtime.Terminal;

public class Production implements Iterable<ProductionPart> {

	public final Integer id;
	public final NonTerminalExt lhs;
	public final int offset;
	public final String label;
	
	
	ProductionPart parts[];
	ProductionPart excluded_parts[];
	
	
	String actionCode;
	
	public Production(Integer prodId, int offset, String label, NonTerminalExt lhs, List<ProductionPart> rhs_parts, List<ProductionPart> excluded_rhs_parts) {
		id = prodId;
		this.lhs = lhs;
		this.offset = offset;
		this.label = label;
		parts = rhs_parts.toArray(new ProductionPart[rhs_parts.size()]);
		if (excluded_rhs_parts==null) {
			excluded_parts = null;
		} else {
			excluded_parts = excluded_rhs_parts.toArray(new ProductionPart[excluded_rhs_parts.size()]);
		}
	}

	public Boolean calculateNullability() {
		Boolean result = Boolean.TRUE;
		for(ProductionPart pp : parts) {
			if (pp.symbol instanceof Terminal) {
				return Boolean.FALSE;
			} else if (pp.symbol instanceof NonTerminalExt) {
				NonTerminalExt nte = (NonTerminalExt) pp.symbol;
				Boolean nteNullable = nte.getNullable();
				if (nteNullable==null) {
					result = null;
				} else if (nteNullable == Boolean.FALSE) {
					return Boolean.FALSE;
				}
			}
		}
		return result;
	}
	
	public void setActionCode(String actionCode) {
		this.actionCode = actionCode;
	}
	
	public String getActionCode() {
		return actionCode;
	}
	
	public int rhsCount() {
		return parts.length;
	}
	
	public ProductionPart rhsAt(int idx) {
		return parts[idx];
	}
	
	public int rhsIndexWithLabel(String label) {
		for(int idx=0; idx<parts.length; idx++) {
			if (label.equals(parts[idx].label)) {
				return idx;
			}
		}
		return -1;
	}
	
	
	@Override
	public Iterator<ProductionPart> iterator() {
		return Arrays.asList(parts).iterator();
	}
	
	public List<ProductionPart> getExcluded() {
		return excluded_parts ==null ? null : Arrays.asList(excluded_parts);
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		for(ProductionPart part : parts) {
			buf.append(' ').append(part.symbol.name);
		}
		buf.append('⇒').append(lhs.name);

		if (excluded_parts!=null) {
			for(ProductionPart part : parts) {
				buf.append(" -").append(part.symbol.name);
			}
		}
		
		return buf.toString().substring(1);
	}
}
