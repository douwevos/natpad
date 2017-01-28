package net.natpad.viper.model.file;

import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.scanner.VipIMapper;
import net.natpad.viper.model.scanner.VipIScanWork;


public class VipSimpleFileMapper implements VipIMapper<VipNode> {

	private final VipISequence sequence;
	
	public VipSimpleFileMapper(VipISequence sequence) {
		this.sequence = sequence;
	}
	
	
	@Override
	public VipIScanWork createWorkForNode(CatWritableTreeNode<VipNode> node, boolean recursiveFromParent) {
		return new VipFSScanWork(sequence, node, recursiveFromParent);
	}
}
