package net.natpad.avaj.ast;

import java.util.ArrayList;

import net.natpad.avaj.util.Util;

public class Name {

	private String parts[];
	private int hashCode;
	
	public Name(String fullName, boolean fromPool) {
		char sep = fromPool ? '/' : '.';
		int idxa = 0;
		ArrayList<String> literals = new ArrayList<String>();
		int hashCode=0;
		while(true) {
			int idxb = fullName.indexOf(sep, idxa);
			if (idxb>=0) {
				String literalId = fullName.substring(idxa, idxb);
				hashCode+=literalId.hashCode();
				literals.add(literalId);
				idxa = idxb+1;
			} else {
				String literalId = fullName.substring(idxa);
				hashCode+=literalId.hashCode();
				literals.add(literalId);
				break;
			}
		}
		this.parts = new String[literals.size()];
		literals.toArray(this.parts);
		this.hashCode = hashCode;		
	}

	private Name(String parts[]) {
		this.parts = parts;
		int hashCode = 0;
		for(String part : parts) {
			hashCode += part.hashCode();
		}
		this.hashCode = hashCode;
	}

	public int count() {
		return parts.length;
	}

	public String get(int index) {
		return index>=0 && index<parts.length ? parts[index] : null;
	}

	
	public String getShortTypeName() {
		return parts.length>0 ? parts[parts.length-1] : null;
	}

	public Name createPackageName() {
		String newParts[] = null;
		if (parts.length>0) {
			newParts = new String[parts.length-1];
			System.arraycopy(parts, 0, newParts, 0, parts.length-1);
		} else {
			newParts = new String[0];
		}
		return new Name(newParts);
	}
	
	public String createFQN() {
		StringBuilder buf = new StringBuilder();
		for(String part : parts) {
			if (buf.length()>0) {
				buf.append('.');
			}
			buf.append(part);
		}
		return buf.toString();
	}


	@Override
	public int hashCode() {
		return hashCode;
	}
	
	
	
	@Override
	public boolean equals(Object other) {
		if (other==this) {
			return true;
		}
		if (other instanceof Name) {
			Name that = (Name) other;
			if (that.parts.length!=parts.length) {
				return false;
			}
			for(int idx=0; idx<parts.length; idx++) {
				if (!Util.nullSafeEquals(parts[idx], that.parts[idx])) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	public boolean startsWith(Name other) {
		if (other.parts.length>parts.length) {
			return false;
		}
		for(int idx=0; idx<other.parts.length; idx++) {
			if (!parts[idx].equals(other.parts[idx])) {
				return false;
			}
		}
		return true;
	}




	
}
