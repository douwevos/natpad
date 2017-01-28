package net.natpad.caterpillar.versionedtree;

import net.natpad.caterpillar.CatIMatcher;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntry;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntryList;

public class CatWritableTreeNode<C> extends CatTreeNode<CatWritableTreeNode<C>, C> {

	
	public CatWritableTreeNode(CatITreeEntryListProvider<C> list_provider, int location) {
		super(list_provider, location);
	}
	

	@Override
	public CatWritableTreeNode<C> create_new(CatITreeEntryListProvider<C> list_provider, int location) {
		return new CatWritableTreeNode<C>(list_provider, location);
	}
	

	private CatTreeEntry<C> l_get_writable_entry() {
		CatTreeEntryList<C> entry_list = list_provider.get_writable_entry_list();
		return entry_list.cat_tree_entry_list_get_writable_entry(location);
	}

	private CatTreeEntry<C> l_get_entry() {
		CatTreeEntryList<C> entry_list = list_provider.get_entry_list();
		return entry_list.cat_tree_entry_list_get_writable_entry(location);
	}

	
	public void cat_writable_tree_node_set_child_at(int index, CatWritableTreeNode<C> child) {
		CatTreeEntry<C> node_entry = l_get_writable_entry();
		int child_location = node_entry.cat_tree_entry_get_child(index);
		if (child_location!= -1) {
			CatTreeEntryList<C> entry_list = list_provider.get_writable_entry_list();
			CatTreeEntry<C> entry = entry_list.cat_tree_entry_list_get_writable_entry(location);
			entry.cat_tree_entry_set_child_at(index, child.location);
		} else {
			// TODO error
		}
	}
	
	public void cat_writable_tree_node_remove_child_at(int index) {
		CatTreeEntry<C> node_entry = l_get_writable_entry();
		int child_location = node_entry.cat_tree_entry_get_child(index);
		if (child_location!= -1) {
			CatTreeEntryList<C> entry_list = list_provider.get_writable_entry_list();
			CatTreeEntry<C> entry = entry_list.cat_tree_entry_list_get_writable_entry(location);
			int location = entry.cat_tree_entry_removed_child_at(index);
			if (location!=-1) {
				entry_list.cat_tree_entry_list_remove_entry(location, true);
			}
		} else {
			// TODO error
		}
		
	}
	
	public void cat_writable_tree_node_remove_child(CatWritableTreeNode<C> child_node) {
		CatTreeEntry<C> node_entry = l_get_writable_entry();
		int child_index = node_entry.cat_tree_entry_find_child_index(child_node.location);
		if (child_index!=-1) {
			cat_writable_tree_node_remove_child_at(child_index);
		}
	}

	
	
	public CatWritableTreeNode<C> cat_writable_tree_node_append_child() {
		CatTreeEntry<C> node_entry = l_get_writable_entry();
		CatTreeEntryList<C> entry_list = list_provider.get_writable_entry_list();
		int created_location = entry_list.cat_tree_entry_list_create_node();
		CatTreeEntry<C> created_entry = entry_list.cat_tree_entry_list_get_entry(created_location);
		created_entry.cat_tree_entry_set_parent_location(location);
		node_entry.cat_tree_entry_add_child(created_location);
		return new CatWritableTreeNode<C>(list_provider, created_location);
	}
	
	
	public void cat_writable_tree_node_set_content(C new_content) {
		CatTreeEntry<C> node_entry = l_get_writable_entry();
		node_entry.cat_tree_entry_set_payload(new_content);
	}




	
	
	
	
	
	
	
	


	
	public CatWritableTreeNode<C> cat_tree_node_get_parent_node() {
		CatTreeEntry<C> node_entry = l_get_entry();
		CatWritableTreeNode<C> result = null; 
		int parent_location = node_entry.cat_tree_entry_get_parent_location();
		if (parent_location!= -1) {
			result = new CatWritableTreeNode<C>(list_provider, parent_location);
		}
		return result;
	}

	
	public CatWritableTreeNode<C> cat_tree_node_child_at(int index) {
		CatTreeEntry<C> node_entry = l_get_entry();
		CatWritableTreeNode<C> result = null; 
		int child_location = node_entry.cat_tree_entry_get_child(index);
		if (child_location!= -1) {
			result = new CatWritableTreeNode<C>(list_provider, child_location);
		}
		return result;
	}
	
	public int cat_tree_node_child_count() {
		CatTreeEntry<C> node_entry = l_get_entry();
		return node_entry.cat_tree_entry_count();
	}
	
	
	public int cat_tree_node_find_child_index(CatIMatcher<C> matcher, int guess_index) {
		CatTreeEntry<C> node_entry = l_get_entry();
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
		CatTreeEntry<C> node_entry = l_get_entry();
		return node_entry.cat_tree_entry_get_payload();
	}



	
}
