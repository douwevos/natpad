package net.natpad.monkey.model;

import java.util.ArrayList;

import net.natpad.monkey.model.access.MooINodeMatcher;

public interface MooINode {


	MooNode getOriginal();


	long getUniqueId();
	
	String getName();
	
	int count();

	MooINode childAt(int idx);

	ArrayList<MooINode> enlistChildren(ArrayList<MooINode> children);

	int findIndex(MooINodeMatcher nodeMatcher, int try_index);

	MooEditableNode ensureEditable();

	<T extends MooIContent> T getContent(String key);

	
	
	
}
