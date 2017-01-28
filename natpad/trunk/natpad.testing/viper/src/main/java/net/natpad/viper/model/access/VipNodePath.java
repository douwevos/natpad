package net.natpad.viper.model.access;

import java.io.PrintStream;
import java.util.List;

import net.natpad.caterpillar.versionedtree.CatITreeEntryListProvider;
import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.viper.model.VipNode;

public class VipNodePath {

	
	private VipNode nodes[];
	protected List<CatReadableTreeNode<VipNode>> node_list;
	 
	public VipNodePath(VipNode nodes[], List<CatReadableTreeNode<VipNode>> node_list) {
		this.nodes = nodes;
		this.node_list = node_list;
		if (nodes.length>0) {
			nodes[nodes.length-1].getObserverInfo().reference();
		}
	}
	
	@Override
	protected void finalize() throws Throwable {
		if (nodes.length>0) {
			nodes[nodes.length-1].getObserverInfo().unreference();
		}
		super.finalize();
	}

	
//	public boolean update(CatITreeEntryListProvider<VipNode> new_entry_list_provider) {
//		boolean result = true;
//		VipNodeIdMatcher id_matcher = new VipNodeIdMatcher(0);
//		for(int idx=0; idx<node_list.size(); idx++) {
//			CatReadableTreeNode<VipNode> node = node_list.get(idx);
//			
//			id_matcher.setId(node.cat_tree_node_get_content().getViperId());
//			CatReadableTreeNode<VipNode> refreshed = node.cat_tree_node_refresh(new_entry_list_provider, id_matcher);
//			if (refreshed!=node) {
//				if (refreshed.cat_tree_node_get_content()!=null) {
//					node_list.set(idx, refreshed);
//				} else {
//					result = false;
//				}
//			}
//		}
//		return result;
//	}
	
	public void print(PrintStream out) {
		for(VipNode node : nodes) {
			out.print("::"+node.toString());
		}
		out.println();
	}

	public VipNode getTail() {
		return nodes!=null && nodes.length>0 ? nodes[nodes.length-1] : null;
	}

	public CatReadableTreeNode<VipNode> getTailNode() {
		return node_list!=null && node_list.size()>0 ? node_list.get(node_list.size()-1) : null;
	}
	
	
	
}
