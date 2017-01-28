package net.natpad.caterpillar.versionedtree;

import net.natpad.caterpillar.CatIMatcher;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntryList;

public class CatTreeNodeReference<T> implements CatIMatcher<T>{
	
	protected T payload;
	protected int location;
	
	
	public CatTreeNodeReference(T payload, int location) {
		this.payload = payload;
		this.location = location;
	}
	
	
	@Override
	public boolean matches(T t) {
		return t.equals(payload);
	}

	
	public boolean update(CatTreeEntryList<T> t) {
		boolean result = false;
		int new_location = t.find_location(this, location);
		if (new_location!=-1) {
			location = new_location;
			result = true;
		}
		return result;
	}
	
}
