package net.natpad.viper.model;

import java.io.PrintStream;
import java.util.concurrent.atomic.AtomicLong;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.caterpillar.versionedtree.CatTreeNode;
import net.natpad.viper.model.access.VipNodeIdMatcher;

public class VipSnapshot {

	final int adres = (int) (Math.random()*0x7fffff);

	final AtomicLong versionGenerator;
	
	public final CatReadableTreeNode<VipNode> root;
	public final long version;
	
	
	public VipSnapshot(CatReadableTreeNode<VipNode> root) {
		this(root, new AtomicLong());
	}
	
	protected VipSnapshot(CatReadableTreeNode<VipNode> root, AtomicLong versionGenerator) {
		this.root = root;
		this.versionGenerator = versionGenerator;
		version = versionGenerator.incrementAndGet();
	}
	
	@Override
	protected void finalize() throws Throwable {
		super.finalize();
	}
	
	
	public VipSnapshot set_root(CatReadableTreeNode<VipNode> newRoot) {
		if (newRoot!=root) {
			return new VipSnapshot(newRoot, versionGenerator);
		}
		return this;
	}

	@SuppressWarnings("unchecked")
	public <T extends CatTreeNode<?, VipNode>> T refresh(T node) {
		if (node==null) {
			return null;
		}
		VipNodeIdMatcher id_matcher = new VipNodeIdMatcher(node.cat_tree_node_get_content().getViperId());
		return (T) node.cat_tree_node_refresh(root.get_list_provider(), id_matcher);
	}
	

	
//	public VipNode find_node_by_viper_id(long viperId) {
//		VipNode result = null;
//		ArrayList<VipNode> stack = new ArrayList<VipNode>();
//		stack.add(root);
//		while(!stack.isEmpty()) {
//			int index = stack.size()-1;
//			VipNode workNode = stack.get(index);
//			if (workNode.viperId==viperId) {
//				result = workNode;
//				break;
//			} else {
//				stack.remove(index);
//				workNode.enlistChildren(stack);
//			}
//		}
//		return result;
//	}
	
	
	public void print(PrintStream out) {
		out.println("snapshot:"+hashCode());
		print(root, 0);
//		root.print("", out);
	}
	
	private void print(CatReadableTreeNode<VipNode> submit, int level) {
		VipNode content = submit.cat_tree_node_get_content();
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
			CatReadableTreeNode<VipNode> child_at = submit.cat_tree_node_child_at(idx);
			print(child_at, level);
		}
	}
	
	@Override
	public String toString() {
		String cname = getClass().getName();
		int lastIndexOf = cname.lastIndexOf(".");
		if (lastIndexOf>0) {
			cname = cname.substring(lastIndexOf+1);
		}
		return cname+"["+Integer.toHexString(adres)+", version="+version+", entry-cnt="+ (root==null ? -1 : root.get_list_provider().get_entry_list().count()) +"]";
	}

	
	
}
