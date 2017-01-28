package net.natpad.caterpillar.versionedtree.impl;

import net.natpad.caterpillar.CatIMatcher;

public class CatTreeEntryList<C> {

	public CatTreeBlock<C>[] blocks;
	
	public boolean marked_fixed = false;
	
	public CatTreeEntryList() {
		blocks = new CatTreeBlock[1];
		blocks[0] = new CatTreeBlock<C>();
		blocks[0].cat_tree_block_create_node();
	}
	
	public void cat_tree_entry_list_mark() {
		marked_fixed = true;
		for(CatTreeBlock<C> block : blocks) {
			block.cat_tree_block_mark();
		}
	}

	public boolean cat_tree_entry_list_is_marked_fixed() {
		return marked_fixed;
	}

	
	public CatTreeEntryList<C> cat_tree_entry_list_ensure_writable() {
		if (!marked_fixed) {
			return this;
		}

		CatTreeEntryList<C> result = new CatTreeEntryList<C>();	// TODO should be an empty constructor
		result.blocks = new CatTreeBlock[blocks.length];
		System.arraycopy(blocks, 0, result.blocks, 0, blocks.length);
		return result;
	}
	
	public int cat_tree_entry_list_create_node() {
		int found = -1;
		for(int idx=0; idx<blocks.length; idx++) {
			CatTreeBlock<C> block = blocks[idx];
			if (block.cat_tree_block_has_available_spot()) {
				if (block.cat_tree_block_is_marked_fixed()) {
					block = block.cat_tree_block_ensure_writable();
					blocks[idx] = block;
				}
				found = block.cat_tree_block_create_node();
				if (found!=-1) {
					found = (idx<<10) + found;
					break;
				}
			}
		}
		if (found==-1) {
			CatTreeBlock<C>[] new_arrays = new CatTreeBlock[blocks.length+1];
			System.arraycopy(blocks, 0, new_arrays, 0, blocks.length);
			int block_idx = blocks.length;
			new_arrays[block_idx] = new CatTreeBlock<C>();
			blocks = new_arrays;
			found = new_arrays[block_idx].cat_tree_block_create_node();
			found = (block_idx<<10) + found;
		}
		return found;
	}

	public void cat_tree_entry_list_remove_entry(int entry_location, boolean recursive) {
		if (entry_location == -1 || entry_location == 0) {
			return;
		}
		int block_index = entry_location>>>10;
		int entry_index = entry_location % 1024;
		CatTreeBlock<C> block = blocks[block_index];
		if (block.cat_tree_block_is_marked_fixed()) {
			block = block.cat_tree_block_ensure_writable();
			blocks[block_index] = block;
		}
		if (recursive) {
			CatTreeEntry<C> entry = block.cat_tree_block_get(entry_index);
			if (entry != null && entry.children!=null) {
				for(int location : entry.children) {
					cat_tree_entry_list_remove_entry(location, true);
				}
			}
		}
		block.cat_tree_block_set(entry_index, null);
	}

	public CatTreeEntry<C> cat_tree_entry_list_get_entry(int entry_location) {
		if (entry_location == -1) {
			return null;
		}
		int block_index = entry_location>>>10;
		int entry_index = entry_location % 1024;
		return blocks[block_index].cat_tree_block_get(entry_index);
	}

	public CatTreeEntry<C> cat_tree_entry_list_get_writable_entry(int entry_location) {
		if (entry_location == -1) {
			return null;
		}
		int block_index = entry_location>>>10;
		int entry_index = entry_location % 1024;
		CatTreeBlock<C> block = blocks[block_index];
		if (block.cat_tree_block_is_marked_fixed()) {
			block = block.cat_tree_block_ensure_writable();
			blocks[block_index] = block;
		}
		return block.cat_tree_block_get_writable(entry_index);
	}

	public int find_location(CatIMatcher<C> matcher, int guess_location) {
		if (guess_location!=-1) {
			int block_index = guess_location>>>10;
			int entry_index = guess_location % 1024;
			if (block_index>=0 && block_index<blocks.length) {
				CatTreeBlock<C> block = blocks[block_index];
				CatTreeEntry<C> entry = block.cat_tree_block_get(entry_index);
				if (entry!=null) {
					C content = entry.cat_tree_entry_get_payload();
					if (matcher.matches(content)) {
						return guess_location;
					}
				}
			}
		}

		for(int block_index=0; block_index<blocks.length-1; block_index++) {
			CatTreeBlock<C> block = blocks[block_index];
			int location = block.cat_tree_block_find(matcher);
			if (location!=-1) {
				location = (block_index<<10) + location;
				return location;
			}
		}

		return -1;
	}

	public void pack() {
		if (blocks.length<=1) {
			return;
		}
		for(int block_index=0; block_index<blocks.length-1; block_index++) {
			CatTreeBlock<C> block = blocks[block_index];
			for(int entry_index=0; entry_index<1024; entry_index++) {
				CatTreeEntry<C> entry = block.cat_tree_block_get(entry_index);
				if (entry==null) {
					int location = find_entry_to_move(block_index+1);
					if (location==-1) {
						block_index = blocks.length;
						break;
					}
					if (block.cat_tree_block_is_marked_fixed()) {
						block = block.cat_tree_block_ensure_writable();
						blocks[block_index] = block;
					}
					
					move_entry(location, block, block_index, entry_index);
				}
			}
			
			while(true) {
				CatTreeBlock<C> last_block = blocks[blocks.length-1];
				if (last_block.cat_tree_block_is_empty()) {
					CatTreeBlock<C>[] new_arrays = new CatTreeBlock[blocks.length-1];
					System.arraycopy(blocks, 0, new_arrays, 0, blocks.length-1);
					blocks = new_arrays;
				} else {
					break;
				}
			}
		}
		
	}

	private void move_entry(int src_location, CatTreeBlock<C> dest_block, int dest_block_index, int dest_entry_index) {
		CatTreeEntry<C> src_entry = cat_tree_entry_list_get_entry(src_location);
		dest_block.cat_tree_block_set(dest_entry_index, src_entry);
		cat_tree_entry_list_remove_entry(src_location, false);
		
		int dest_location = (dest_block_index<<10) + dest_entry_index;
		
		
		System.out.println("moving from:"+src_location+" to "+dest_location);
		/* rewire all references to the node */
		for(int block_index=0; block_index<blocks.length; block_index++) {
			CatTreeBlock<C> block = blocks[block_index];
			for(int entry_index=0; entry_index<1024; entry_index++) {
				CatTreeEntry<C> entry = block.cat_tree_block_get(entry_index);
				if (entry!=null) {
					if (entry.cat_tree_entry_references_location(src_location)) {
						
						System.out.println("updating:"+ (block_index<<10)+entry_index);
						
						if (block.cat_tree_block_is_marked_fixed()) {
							block = block.cat_tree_block_ensure_writable();
							blocks[block_index] = block;
						}
						
						CatTreeEntry<C> writable_entry = block.cat_tree_block_get_writable(entry_index);
						writable_entry.cat_tree_entry_writable_move_location(src_location, dest_location);
					}
				}
			}
		}
		
	}

	private int find_entry_to_move(int lowest_block_index) {
		for(int block_index=blocks.length-1; block_index>=lowest_block_index; block_index--) {
			CatTreeBlock<C> block = blocks[block_index];
			for(int entry_index=1024-1; entry_index>=0; entry_index--) {
				CatTreeEntry<C> entry = block.cat_tree_block_get(entry_index);
				if (entry != null) {
					return (block_index<<10) + entry_index;
				}
			}
		}
		return -1;
	}

	public int count() {
		int total = 0;
		for(int block_index=0; block_index<blocks.length; block_index++) {
			CatTreeBlock<C> block = blocks[block_index];
			total += block.cat_tree_block_count();
		}
		return total;
	}
	
	
}
