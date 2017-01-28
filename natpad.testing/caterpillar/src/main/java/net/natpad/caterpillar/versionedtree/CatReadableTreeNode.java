package net.natpad.caterpillar.versionedtree;

import net.natpad.caterpillar.CatIMatcher;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntry;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntryList;

public class CatReadableTreeNode<C> extends CatTreeNode<CatReadableTreeNode<C>, C> {

	CatTreeEntry<C> node_entry;
	
	public CatReadableTreeNode(CatITreeEntryListProvider<C> list_provider, int location, CatTreeEntry<C> node_entry) {
		super(list_provider, location);
		this.node_entry = node_entry;
	}
	
	@Override
	public CatReadableTreeNode<C> create_new(CatITreeEntryListProvider<C> list_provider, int location) {
		return new CatReadableTreeNode<C>(list_provider, location, list_provider.get_entry_list().cat_tree_entry_list_get_entry(location));
	}
	
	public CatITreeEntryListProvider<C> get_list_provider() {
		return list_provider;
	}
	

	private void l_check_cached_entry() {
		CatTreeEntryList<C> new_entry_list = list_provider.get_entry_list();
		if (new_entry_list!=cache_entry_list) {
			node_entry = new_entry_list.cat_tree_entry_list_get_entry(location);
			cache_entry_list = new_entry_list;
		}
	}

	
	public CatReadableTreeNode<C> cat_tree_node_get_parent_node() {
		l_check_cached_entry();
		CatReadableTreeNode<C> result = null; 
		int parent_location = node_entry.cat_tree_entry_get_parent_location();
		if (parent_location!= -1) {
			CatTreeEntryList<C> entry_list = list_provider.get_entry_list();
			CatTreeEntry<C> parent_entry = entry_list.cat_tree_entry_list_get_entry(parent_location);
			result = new CatReadableTreeNode<C>(list_provider, parent_location, parent_entry);
		}
		return result;
	}

	
	public CatReadableTreeNode<C> cat_tree_node_child_at(int index) {
		l_check_cached_entry();
		CatReadableTreeNode<C> result = null; 
		int child_location = node_entry.cat_tree_entry_get_child(index);
		if (child_location!= -1) {
			CatTreeEntryList<C> entry_list = list_provider.get_entry_list();
			CatTreeEntry<C> child_entry = entry_list.cat_tree_entry_list_get_entry(child_location);
			result = new CatReadableTreeNode<C>(list_provider, child_location, child_entry);
		}
		return result;
	}
	
	public int cat_tree_node_child_count() {
		l_check_cached_entry();
		return node_entry.cat_tree_entry_count();
	}
	
	
	public int cat_tree_node_find_child_index(CatIMatcher<C> matcher, int guess_index) {
		l_check_cached_entry();
		int entry_count = node_entry.cat_tree_entry_count();
		if (entry_count<=0) {
			return -1;
		}
		int result = -1;
		CatTreeEntryList<C> entry_list = list_provider.get_entry_list();
		if (guess_index>=0 && guess_index<entry_count) {
			int child_location = node_entry.cat_tree_entry_get_child(guess_index);
			CatTreeEntry<C> child_entry = entry_list.cat_tree_entry_list_get_entry(child_location);
			if (matcher.matches((C) child_entry.cat_tree_entry_get_payload())) {
				return guess_index;
			}
		}
		for(int idx=entry_count-1; idx>=0; idx--) {
			int child_location = node_entry.cat_tree_entry_get_child(idx);
			CatTreeEntry<C> child_entry = entry_list.cat_tree_entry_list_get_entry(child_location);
			if (matcher.matches((C) child_entry.cat_tree_entry_get_payload())) {
				result = idx;
				break;
			}
		}
		return result;
	}

	
	public C cat_tree_node_get_content() {
		l_check_cached_entry();
		return node_entry.cat_tree_entry_get_payload();
	}

	
}
