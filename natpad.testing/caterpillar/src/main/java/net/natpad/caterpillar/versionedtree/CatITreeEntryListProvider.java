package net.natpad.caterpillar.versionedtree;

import net.natpad.caterpillar.versionedtree.impl.CatTreeEntryList;

public interface CatITreeEntryListProvider<C> {

	CatTreeEntryList<C> get_entry_list();

	CatTreeEntryList<C> get_writable_entry_list();
	
	
}
