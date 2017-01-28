package net.natpad.jaguar.monkey;

import net.natpad.monkey.model.MooIContent;

public class JagSrcFolderContent implements MooIContent {

	public static final String KEY = "JagNodeSrcFolder";
	
	public JagSrcFolderContent() {
	}

	
	@Override
	public String getKey() {
		return KEY;
	}
	
	@Override
	public void markFixed() {
	}

	
}
