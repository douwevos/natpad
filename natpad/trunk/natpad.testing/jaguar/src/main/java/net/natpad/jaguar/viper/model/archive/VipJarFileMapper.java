package net.natpad.jaguar.viper.model.archive;

import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.scanner.VipIScanWork;
import net.natpad.viper.model.scanner.VipIMapper;

public class VipJarFileMapper implements VipIMapper<VipNode> {

	VipISequence idProvider;
	
	public VipJarFileMapper(VipISequence idProvider) {
		this.idProvider = idProvider;
	}
	
	
	@Override
	public VipIScanWork createWorkForNode(CatWritableTreeNode<VipNode> node, boolean recursiveFromParent) {
		VipNode vipNode = node.cat_tree_node_get_content();
		Object content = vipNode.getResource();
		if (content instanceof VipJarRoot) {
			return new ScanJarMapWork(idProvider, node, true);
		} else if (content instanceof VipJarMap) {
			return new ScanJarMapWork(idProvider, node, recursiveFromParent);
		} else if (content instanceof VipIFile) {
			VipIFile vipFile = (VipIFile) content;
			String name = vipFile.getName();
			if (name.endsWith(".jar") || name.endsWith(".ear") || name.endsWith(".war")) {
				VipJarRoot vipJarRoot = new VipJarRoot(vipFile);
				VipNode replacement_vip_node = vipNode.setResource(vipJarRoot);
				node.cat_writable_tree_node_set_content(replacement_vip_node);
				return new ScanJarMapWork(idProvider, node, true);
//				return new ScanJarWork(idProvider, new VipNode(idProvider.next(), vipFile, true));
			}
			if (content instanceof VipJarFile) {
				return new ScanJarFileWork(vipNode);
			}
		}
		return null;
	}
	
	
	
	
}
