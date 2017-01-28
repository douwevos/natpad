package net.natpad.caterpillar.versionedtree.impl;

import net.natpad.caterpillar.versionedtree.CatITreeEntryListProvider;

public class CatReadableEntryListProvider<C> implements CatITreeEntryListProvider<C> {

	protected final CatTreeEntryList<C> entry_list;
	
	public CatReadableEntryListProvider(CatTreeEntryList<C> entry_list) {
		this.entry_list = entry_list;
	}
	
	
	@Override
	public CatTreeEntryList<C> get_entry_list() {
		return entry_list;
	}

	@Override
	public CatTreeEntryList<C> get_writable_entry_list() {
		return null;
	}
}
