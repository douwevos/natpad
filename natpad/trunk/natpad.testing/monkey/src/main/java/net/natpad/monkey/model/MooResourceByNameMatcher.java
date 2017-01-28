package net.natpad.monkey.model;

import net.natpad.monkey.model.access.MooINodeMatcher;

public class MooResourceByNameMatcher implements MooINodeMatcher {

	private String name;
	
	public MooResourceByNameMatcher(String name) {
		this.name = name;
	}

	
	public void setSearchName(String name) {
		this.name = name;
	}
	
	
	@Override
	public boolean matches(MooINode node) {
		boolean result = false;
		MooResourceContent resourceContent = node.getContent(MooResourceContent.KEY);
		if (resourceContent!=null) {
			
			result = name.equals(resourceContent.getViperNode().getName());
		}
		return result;
	}
}
