package net.natpad.viper.model.file;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipIResource;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.access.VipNodeNameMatcher;
import net.natpad.viper.model.scanner.VipIScanWork;
import net.natpad.viper.model.scanner.VipMapperRegistry;

public class VipFSScanWork implements VipIScanWork {


	VipISequence sequence;
	
	CatWritableTreeNode<VipNode> node;
	
	VipNode mainNode;
	boolean recursiveFromParent;
	
	VipIScanWork childWork[];
	
	
	public VipFSScanWork(VipISequence sequence, CatWritableTreeNode<VipNode> node, boolean recursiveFromParent) {
		this.sequence = sequence;
		this.node = node;
		this.mainNode = (VipNode) node.cat_tree_node_get_content();
		this.recursiveFromParent = recursiveFromParent;
	}
	
	public VipIScanWork[] initChildWork(VipMapperRegistry creator) {
		if (childWork==null) {
			VipIResource content = mainNode.getResource();
			if (content instanceof VipIMap) {
				initializeWorkForMap(creator, (VipIMap) content);
			} else if (node.cat_tree_node_child_count()>0) {
				initializeWorkForMap(creator, null);
			}
			return childWork;
		}
		return null;
	}
	

	public void initializeWorkForMap(VipMapperRegistry creator, VipIMap vipMap) {
		
		ArrayList<VipIScanWork> workList = new ArrayList<VipIScanWork>();
		HashSet<String> namesAdded = new HashSet<String>();
		
		boolean parentRecursive = recursiveFromParent || mainNode.shouldScanRecursive();
		
		if (vipMap!=null && parentRecursive) {
			CatArray<VipIResource> enlisted = null;
			try {
				enlisted = vipMap.enlist(true);
			} catch (IOException e) {
				e.printStackTrace();
				enlisted = new CatArray<VipIResource>();
			}
			int guessIndex = -1;
			for (VipIResource childFile : enlisted) {
				String resourceName = childFile.getName();
				namesAdded.add(resourceName);
				int foundIndex = node.cat_tree_node_find_child_index(new VipNodeNameMatcher(resourceName), guessIndex);
				if (foundIndex>=0) {
					CatWritableTreeNode<VipNode> child_at = node.cat_tree_node_child_at(foundIndex);
					VipNode oldChild = child_at.cat_tree_node_get_content();
					guessIndex = foundIndex+1;
					
					/* TODO if content is a map then the enlist info should be updated
					 */
					
					VipIResource oldContent = oldChild.getResource();
					if (oldContent.getClass()==childFile.getClass()) {
						workList.add(creator.createWorkForNode(child_at, true));
						if (oldContent.equals(childFile)) {
							continue;
						}
						VipNode vipNode = new VipNode(oldChild.getViperId(), childFile, oldChild.getHoldCounter(), oldChild.getRecursiveCounter());
						child_at.cat_writable_tree_node_set_content(vipNode);
						continue;
					}
				} 
				CatWritableTreeNode<VipNode> appended_node = node.cat_writable_tree_node_append_child();
				VipNode vipNode = new VipNode(sequence.next(), childFile);
				appended_node.cat_writable_tree_node_set_content(vipNode);
				workList.add(creator.createWorkForNode(appended_node, true));
			}
		}
		
		for(int idx=0; idx<node.cat_tree_node_child_count(); idx++) {
			CatWritableTreeNode<VipNode> child_at = node.cat_tree_node_child_at(idx);
			VipNode childAt = child_at.cat_tree_node_get_content();
			if (!namesAdded.contains(childAt.getName())) {
				workList.add(creator.createWorkForNode(child_at, parentRecursive));
			}
		}
		cat_log_debug("workList.size=%d", workList.size());
		childWork = new VipIScanWork[workList.size()];
		workList.toArray(childWork);
	}


	public void runScan() {

		cat_log_debug("run-scan on:"+mainNode+", recursiveFromParent="+recursiveFromParent);
//		node.cleanCache();

		
		VipNode vip_node = (VipNode) node.cat_tree_node_get_content();
		cat_log_debug("run-scan on:vip_node="+vip_node+", node.cat_tree_node_child_count()="+node.cat_tree_node_child_count());

		cat_log_debug("run-scan on:vip_node.shouldHold()="+vip_node.shouldHold());
		cat_log_debug("run-scan on:vip_node.shouldScanRecursive()="+vip_node.shouldScanRecursive());

		
		boolean hold = recursiveFromParent
				|| vip_node.shouldHold() || vip_node.shouldScanRecursive()
				|| node.cat_tree_node_child_count()>0;
		cat_log_debug("hold="+hold);
		if (!hold) {
			CatWritableTreeNode<VipNode> parent_node = node.cat_tree_node_get_parent_node();
			if (parent_node!=null) {
				parent_node.cat_writable_tree_node_remove_child(node);
			} else {
				cat_log_debug("error not parent");
			}
		}
		
	}

}