package net.natpad.caterpillar.versionedtree;

import java.util.concurrent.atomic.AtomicReference;

import net.natpad.caterpillar.versionedtree.impl.CatReadableEntryListProvider;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntry;
import net.natpad.caterpillar.versionedtree.impl.CatTreeEntryList;


public class CatTree<C> implements CatITreeEntryListProvider<C> {

	/* writer context */
	private CatTreeEntryList<C> entry_list = new CatTreeEntryList<C>(); 
	
	/* reader/writer context */
	private AtomicReference<CatReadableTreeNode<C>> root_reference = new AtomicReference<CatReadableTreeNode<C>>();
	
	public CatTree() {
		CatTreeEntry<C> root_entry = entry_list.cat_tree_entry_list_get_entry(0);
		root_reference.set(new CatReadableTreeNode<C>(new CatReadableEntryListProvider<C>(entry_list), 0, root_entry));
	}
	
	
	public CatWritableTreeNode<C> cat_tree_get_writable_root_node() {
		return new CatWritableTreeNode<C>(this, 0);
	}
	
	
	public CatReadableTreeNode<C> cat_tree_get_readable_root_node_ref() {
		return root_reference.get();
	}
	
	public CatReadableTreeNode<C> cat_tree_submit(boolean pack) {
		CatReadableTreeNode<C> result = null;
		if (!entry_list.cat_tree_entry_list_is_marked_fixed()) {
			if (pack) {
				entry_list.pack();
			}
			
			entry_list.cat_tree_entry_list_mark();
			CatReadableEntryListProvider<C> readable_list_provider = new CatReadableEntryListProvider<C>(entry_list);
			CatTreeEntry<C> root_entry = entry_list.cat_tree_entry_list_get_entry(0);
			result = new CatReadableTreeNode<C>(readable_list_provider, 0, root_entry);
			root_reference.set(result);
		}
		return result;
	}
	
	@Override
	public CatTreeEntryList<C> get_entry_list() {
		return entry_list;
	}
	
	@Override
	public CatTreeEntryList<C> get_writable_entry_list() {
		if (entry_list.cat_tree_entry_list_is_marked_fixed()) {
			entry_list = entry_list.cat_tree_entry_list_ensure_writable();
		}
		return entry_list;
	}
}
