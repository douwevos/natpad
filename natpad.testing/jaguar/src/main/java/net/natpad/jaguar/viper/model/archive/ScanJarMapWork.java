package net.natpad.jaguar.viper.model.archive;

import java.io.IOException;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipIResource;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.access.VipNodeNameMatcher;
import net.natpad.viper.model.scanner.VipIScanWork;
import net.natpad.viper.model.scanner.VipMapperRegistry;

public class ScanJarMapWork implements VipIScanWork {

	VipISequence idProvider;
	CatWritableTreeNode<VipNode> mainNode;
	VipIScanWork childWork[];
	boolean recursiveFromParent;

	
	public ScanJarMapWork(VipISequence idProvider, CatWritableTreeNode<VipNode> jarMapNode, boolean recursiveFromParent) {
		this.idProvider = idProvider;
		this.mainNode = jarMapNode;
		this.recursiveFromParent = recursiveFromParent;
	}

	@Override
	public VipIScanWork[] initChildWork(VipMapperRegistry creator) {
		if (childWork==null) {
			initializeWorkForMap(creator);
			return childWork;
		}
		return null;
	}

	
	public void initializeWorkForMap(VipMapperRegistry creator) {
		
		VipNode main_vip_node = mainNode.cat_tree_node_get_content();
		
		VipIMap vipMap = (VipIMap) main_vip_node.getResource();
		
		boolean parentRecursive = recursiveFromParent || mainNode.cat_tree_node_get_content().shouldScanRecursive();


		if (parentRecursive) {
			CatArray<VipIResource> enlisted = null;
			try {
				enlisted = vipMap.enlist(true);
			} catch (IOException e) {
				e.printStackTrace();
				enlisted = new CatArray<VipIResource>();
			}
			childWork = new VipIScanWork[enlisted.size()];
			int guessIndex = -1;
			for(int fidx = 0; fidx<enlisted.size(); fidx++) {
				VipIResource childFile = enlisted.get(fidx);
				int foundIndex = mainNode.cat_tree_node_find_child_index(new VipNodeNameMatcher(childFile.getName()), guessIndex);
				CatWritableTreeNode<VipNode> child = null;
				if (foundIndex>=0) {
					child = mainNode.cat_tree_node_child_at(foundIndex);
					guessIndex = foundIndex+1;
				} else {
					VipNode vipNode = new VipNode(idProvider.next(), childFile);
					child = mainNode.cat_writable_tree_node_append_child();
					child.cat_writable_tree_node_set_content(vipNode);
				}
				childWork[fidx] = creator.createWorkForNode(child, true);
			}
		}
	
	}
	
	
	public void runScan() {
	}



}
