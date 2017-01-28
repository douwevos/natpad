package net.natpad.viper.model;

import net.natpad.caterpillar.versionedtree.CatTreeNodeReference;

public class VipNodeReference extends CatTreeNodeReference<VipNode> {

	public VipNodeReference(VipNode payload, int location) {
		super(payload, location);
	}
	
	@Override
	public boolean matches(VipNode test) {
		boolean result = payload.getViperId() == test.getViperId();
		if (result) {
			payload = test;
		}
		return result;
	}

	public VipNode get_vip_node() {
		return payload;
	}
}
