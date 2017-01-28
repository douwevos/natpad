package net.natpad.viper;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.viper.model.scanner.VipNodeScanner;

public class VipScanRequest implements VipIRequest {

	ViperService vip_service;
	
	public VipScanRequest(ViperService vip_service) {
		this.vip_service = vip_service;
	}

	
	@Override
	public void run() {
		VipSnapshot snapshot = vip_service.getSnapshot();
		VipNodeScanner nodeScanner = new VipNodeScanner(vip_service.getMapperRegistry());
		CatWritableTreeNode<VipNode> writable_root_node = vip_service.tree.cat_tree_get_writable_root_node();
		nodeScanner.scan(writable_root_node);
		CatReadableTreeNode<VipNode> submit = vip_service.tree.cat_tree_submit(false);
		if (submit==null) {
			submit=vip_service.tree.cat_tree_get_readable_root_node_ref();
		}

		snapshot = snapshot.set_root(submit);
		System.err.println("snapshot="+snapshot+", submit="+submit);
		vip_service.setSnapshot(snapshot);
	}
	
	
}
