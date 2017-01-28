package net.natpad.viper.model.scanner;

import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;

public interface VipIMapper<T> {

	public VipIScanWork createWorkForNode(CatWritableTreeNode<T> node, boolean recursiveFromParent);

}
