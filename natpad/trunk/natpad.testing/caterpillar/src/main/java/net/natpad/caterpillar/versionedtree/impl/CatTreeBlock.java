package net.natpad.caterpillar.versionedtree.impl;

import net.natpad.caterpillar.CatIMatcher;

public class CatTreeBlock<C> {

	private CatTreeEntry<C> internal[];

	private int used = 0;
	
	private boolean marked_fixed = false;
	
	@SuppressWarnings("unchecked")
	public CatTreeBlock() {
		internal = (CatTreeEntry<C>[]) new CatTreeEntry[1024];
	}

	public void cat_tree_block_mark() {
		marked_fixed = true;
		for(CatTreeEntry<C> entry : internal) {
			if (entry!=null) {
				entry.cat_tree_entry_mark();
			}
		}
	}

	public boolean cat_tree_block_is_marked_fixed() {
		return marked_fixed;
	}

	@SuppressWarnings("unchecked")
	public CatTreeBlock<C> cat_tree_block_ensure_writable() {
		if (!marked_fixed) {
			return this;
		}
		CatTreeBlock<C> result = new CatTreeBlock<C>();
		result.internal = (CatTreeEntry<C>[]) new CatTreeEntry[internal.length];
		System.arraycopy(internal, 0, result.internal, 0, internal.length);
		result.used = used;
		return result;
	}

	
	public void cat_tree_block_set(int index, CatTreeEntry<C> node) {
		if (internal[index]!=null) {
			used--;
		}
		internal[index] = node;
		if (node!=null) {
			used++;
		}
	}
	

	public boolean cat_tree_block_has_available_spot() {
		return used != 1024;
	}
	
	public boolean cat_tree_block_is_empty() {
		return used == 0;
	}

	public int cat_tree_block_count() {
		return used;
	}


	public int cat_tree_block_create_node() {
		if (used == internal.length) {
			return -1;
		}
		for(int idx=0; idx<internal.length; idx++) {
			if (internal[idx]==null) {
				internal[idx] = new CatTreeEntry<C>();
				used++;
				return idx;
			}
		}
		return -1;
	}
	
	public int cat_tree_block_find(CatIMatcher<C> matcher) {
		for(int idx=0; idx<internal.length; idx++) {
			if (internal[idx]!=null) {
				if (matcher.matches(internal[idx].payload)) {
					return idx;
				}
			}
		}
		return -1;
	}


	public CatTreeEntry<C> cat_tree_block_get(int index) {
		return internal[index];
	}

	public CatTreeEntry<C> cat_tree_block_get_writable(int entry_index) {
		CatTreeEntry<C> entry = internal[entry_index];
		if (entry.cat_tree_entry_is_marked_fixed()) {
			entry = entry.cat_tree_entry_ensure_writable();
			internal[entry_index] = entry;
		}
		return entry;
	}


	
}
