package net.natpad.jaguar.monkey;

import net.natpad.monkey.model.MooIContent;

public class JagNodeCompiler implements MooIContent {

	public static final String KEY = "jaguar-compiler";
	
	
	public JagNodeCompiler() {
		super();
	}

	public JagNodeCompiler(long uniqueId) {
	}


	@Override
	public String getKey() {
		return KEY;
	}

	@Override
	public void markFixed() {
	}
	
	
}
