package net.natpad.monkey.model.access;

import net.natpad.monkey.model.MooINode;

public class MooIdNodeMatcher implements MooINodeMatcher {

	long id;
	
	
	public MooIdNodeMatcher(long uniqueId) {
		this.id = uniqueId;
	}


	public void setId(int id) {
		this.id = id;
	}
	
	
	@Override
	public boolean matches(MooINode node) {
		return (node.getUniqueId()==id);
	}
}
