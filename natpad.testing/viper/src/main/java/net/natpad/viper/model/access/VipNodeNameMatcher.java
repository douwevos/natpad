package net.natpad.viper.model.access;

import net.natpad.viper.model.VipNode;

public class VipNodeNameMatcher implements INodeMatcher {

	String name;
	
	public VipNodeNameMatcher(String name) {
		this.name = name;
	}
	
	
	@Override
	public boolean matches(VipNode testNode) {
		return testNode.getName().equals(name);
	}
	
}
