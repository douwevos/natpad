package net.natpad.viper.model.access;

import net.natpad.viper.model.VipNode;

public class VipNodeIdMatcher implements INodeMatcher {

	long id;
	
	public VipNodeIdMatcher(long id) {
		this.id = id;
	}
	
	public void setId(long id) {
		this.id = id;
	}
	
	@Override
	public boolean matches(VipNode testNode) {
		return testNode.getViperId()==id;
	}
}
