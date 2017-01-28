package net.natpad.cup.export.java.ast;

import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

public class AstQualifiedName implements Iterable<String> {

	public final String fqname;
	
	public AstQualifiedName(String fqname) {
		this.fqname = fqname;
	}
	
	public String getFullQualifiedName() {
		return fqname;
	}
	
	@Override
	public String toString() {
		return "AstQualifiedName["+fqname+"]";
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof AstQualifiedName) {
			AstQualifiedName other = (AstQualifiedName) obj;
			return (other.fqname==fqname || (fqname!=null && fqname.equals(other.fqname)));
		}
		return false;
	}
	
	@Override
	public Iterator<String> iterator() {
		String[] split = fqname.split("\\.");
		List<String> asList = Arrays.asList(split);
		return asList.iterator();
	}
	
}
