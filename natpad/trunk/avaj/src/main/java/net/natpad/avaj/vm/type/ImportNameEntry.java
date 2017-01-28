package net.natpad.avaj.vm.type;

import java.util.HashSet;

import net.natpad.avaj.ast.Name;

public class ImportNameEntry {

	public final String shortTypeName;
	
	private HashSet<Name> names = new HashSet<Name>();
	
	public ImportNameEntry(String shortTypeName) {
		this.shortTypeName = shortTypeName;
	}
	
	public Name getOrAdd(Name name) {
		Name result = null;
		if (!names.isEmpty()) {
			for(Name testName : names) {
				if (testName.equals(name)) {
					result = testName;
					break;
				}
			}
		}
		if (result==null) {
			names.add(name);
			result = name;
		}
		return result;
	}
	
	public boolean allowShortVersion() {
		return names.size()==1;
	}

	public Name getFirstName() {
		if (names.size()==0) {
			return null;
		}
		return names.iterator().next();
	}
	
}
