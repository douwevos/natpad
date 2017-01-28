package net.natpad.caterpillar.versionedtree.impl;


public class CatTreeEntry<C> {

	int parent_location = -1;
	int children[];
	C payload;
	private boolean marked_fixed = false; 
	

	public void cat_tree_entry_mark() {
		marked_fixed = true;
	}

	public boolean cat_tree_entry_is_marked_fixed() {
		return marked_fixed;
	}

	public CatTreeEntry<C> cat_tree_entry_ensure_writable() {
		if (!marked_fixed) {
			return this;
		}
		CatTreeEntry<C> result = new CatTreeEntry<C>();
		result.parent_location = parent_location;
		if (children!=null) {
			result.children = new int[children.length];
			System.arraycopy(children, 0, result.children, 0, children.length);
		} else {
			result.children = null;
		}
		result.payload = payload;
		return result;
	}
	
	public void cat_tree_entry_set_parent_location(int location) {
		this.parent_location = location;
	}
	
	public int cat_tree_entry_get_parent_location() {
		return parent_location;
	}
	
	public int cat_tree_entry_find_child_index(int location) {
		if (children==null) {
			return -1;
		}
		for(int idx=0 ; idx<children.length; idx++) {
			if (children[idx]==location) {
				return idx;
			}
		}
		return -1;
	}
	
	
	public void cat_tree_entry_add_child(int location) {
		if (children==null) {
			children = new int[1];
			children[0] = location;
		} else {
			int new_children[] = new int[children.length+1];
			System.arraycopy(children, 0, new_children, 0, children.length);
			new_children[children.length] = location;
			children = new_children;
		}
	}

	public void cat_tree_entry_set_child_at(int index, int location) {
		children[index] = location;
	}

	public int cat_tree_entry_removed_child_at(int index) {
		int result = children[index];
		if (children.length==1) {
			// TODO what if index not 0
			children = null;
		} else {
			int new_children[] = new int[children.length-1];
			System.arraycopy(children, 0, new_children, 0, index);
			System.arraycopy(children, index+1, new_children, index, new_children.length-index);
			children = new_children;
		}
		return result;
	}

	
	public int cat_tree_entry_get_child(int index) {
		return children[index];
	}
	
	public int cat_tree_entry_count() {
		return children==null ? 0 : children.length;
	}


	
	public void cat_tree_entry_set_payload(C content) {
		this.payload = content;
	}
	
	public C cat_tree_entry_get_payload() {
		return payload;
	}

	public boolean cat_tree_entry_references_location(int location) {
		if (parent_location == location) {
			return true;
		}
		if (children!=null) {
			for(int i : children) {
				if (i==location) {
					return true;
				}
			}
		}
		return false;
	}

	public void cat_tree_entry_writable_move_location(int src_location, int dest_location) {
		if (parent_location==src_location) {
			parent_location = dest_location;
		}
		
		if (children!=null) {
			for(int idx=0 ; idx<children.length; idx++) {
				if (children[idx]==src_location) {
					children[idx] = dest_location;
				}
			}
		}
	}


	
}
