package net.natpad.viper.model.access;

import net.natpad.caterpillar.CatIMatcher;
import net.natpad.viper.model.VipNode;

public interface INodeMatcher extends CatIMatcher<VipNode> {

	boolean matches(VipNode testNode);
}
