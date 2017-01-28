package net.natpad.jaguar.monkey;

import net.natpad.monkey.model.MooIContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.access.MooIdPath;

public class JagClassfileContent implements MooIContent {

	public static final String KEY = "JagClassfileContent";
	
	private MooIdPath classfilePath;
	
	public JagClassfileContent(MooIdPath classfilePath) {
//		if (Math.random()>0.9d) {
//			recursiveErrorCount = recursiveErrorCount.setLocalCount((int) (Math.random()*8));
//		}
	}

	
	public MooIdPath getClassfilePath() {
		return classfilePath;
	}
	
	@Override
	public String getKey() {
		return KEY;
	}
	
	@Override
	public void markFixed() {
	}
	
}
