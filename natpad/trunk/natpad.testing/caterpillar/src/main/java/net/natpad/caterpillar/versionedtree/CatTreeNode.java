package net.natpad.caterpillar.versionedtree;

import net.natpad.caterpillar.CatIMatcher;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntryList;

public abstract class CatTreeNode<T extends CatTreeNode<T,C>, C> {

	protected CatITreeEntryListProvider<C> list_provider;
	protected int location;
	
	Object cache_entry_list;

	public CatTreeNode(CatITreeEntryListProvider<C> list_provider, int location) {
		this.list_provider = list_provider;
		this.location = location;
		cache_entry_list = list_provider.get_entry_list();
	}
	
	
	public abstract T create_new(CatITreeEntryListProvider<C> list_provider, int location);

	@SuppressWarnings("unchecked")
	public T cat_tree_node_refresh(CatITreeEntryListProvider<C> new_list_provider, CatIMatcher<C> matcher) {
		if (new_list_provider==list_provider) {
			return (T) this;
		}
		CatTreeEntryList<C> new_entry_list = new_list_provider.get_entry_list();
		CatTreeEntryList<C> old_entry_list = list_provider.get_entry_list();
		if (old_entry_list==new_entry_list) {
			return (T) this;
		}
		
		int new_location = new_entry_list.find_location(matcher, location);
		return create_new(new_list_provider, new_location);
	}
	

	public abstract T cat_tree_node_get_parent_node();
	
	public abstract T cat_tree_node_child_at(int index);
	public abstract int cat_tree_node_child_count();
	
	public abstract int cat_tree_node_find_child_index(CatIMatcher<C> matcher, int guess_index);

	public abstract C cat_tree_node_get_content();


	
	public int cat_tree_node_get_location() {
		return location;
	}
	

	/****** entry-list based methods ******/ 
	
	public int cat_tree_node_find_location(CatIMatcher<C> matcher, int guess_location) {
		CatTreeEntryList<C> entry_list = list_provider.get_entry_list();
		return entry_list.find_location(matcher, guess_location);
	}

	public T cat_tree_node_get_node_for_location(int location) {
		T result = null;
		if (location!= -1) {
			result = create_new(list_provider, location);
		}
		return result;
	}



	
}
