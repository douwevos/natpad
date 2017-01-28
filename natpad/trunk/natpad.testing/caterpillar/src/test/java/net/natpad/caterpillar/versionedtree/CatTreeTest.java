package net.natpad.caterpillar.versionedtree;

import java.io.File;

public class CatTreeTest {

	
	
	public static void main(String[] args) {
		CatTreeTest test = new CatTreeTest();
		
		
		test.testWrite();
	}

	private void testWrite() {
		CatTree<String> tree = new CatTree<String>();
		
		CatWritableTreeNode<String> writable_root_node = tree.cat_tree_get_writable_root_node();
		list_files(writable_root_node, new File("../.."));
		CatReadableTreeNode<String> submit = tree.cat_tree_submit(false);
		
		print(submit, 0);
		
		
		System.out.println("--------------------------------------------------------------------");
		
		writable_root_node = tree.cat_tree_get_writable_root_node();
		CatWritableTreeNode<String> child = writable_root_node.cat_tree_node_child_at(0);
		System.out.println(child.cat_tree_node_get_content());
		child.cat_writable_tree_node_remove_child_at(0);

		submit = tree.cat_tree_submit(false);
		
		print(submit, 0);

		
		
		writable_root_node = tree.cat_tree_get_writable_root_node();

		submit = tree.cat_tree_submit(true);
		
		print(submit, 0);
		
		
		
	}

	private void print(CatReadableTreeNode<String> submit, int level) {
		Object content = submit.cat_tree_node_get_content();
		if (content!=null) {
			StringBuilder buf = new StringBuilder();
			for(int s=level; s>0; s--) {
				buf.append("  ");
			}
			buf.append(content);
			System.out.println(buf.toString());
		}
		int child_count = submit.cat_tree_node_child_count();
		level++;
		for(int idx=0; idx<child_count; idx++) {
			try {
				CatReadableTreeNode<String> child_at = submit.cat_tree_node_child_at(idx);
				print(child_at, level);
			} catch(NullPointerException e) {
				int loc = submit.node_entry.cat_tree_entry_get_child(idx);
				System.out.println("level:"+level+", idx="+idx+", submit.content="+submit.cat_tree_node_get_content()+", loc="+loc);
				throw e;
			} 
		}
		
	}

	private void list_files(CatWritableTreeNode<String> writable_root_node, File file) {
		CatWritableTreeNode<String> appended_child = writable_root_node.cat_writable_tree_node_append_child();
		appended_child.cat_writable_tree_node_set_content(file.getName());
		File[] files = file.listFiles();
		if (files!=null) {
			for(File nfile : files) {
				list_files(appended_child, nfile);
			}
		}
	}
	
}
