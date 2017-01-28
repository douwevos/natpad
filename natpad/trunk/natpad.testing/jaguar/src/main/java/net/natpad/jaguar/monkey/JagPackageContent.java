package net.natpad.jaguar.monkey;

import net.natpad.monkey.model.MooIContent;

public class JagPackageContent implements MooIContent {

	public final static String KEY = "JagPackageContent";
	
	public JagPackageContent() {
	}

	
	@Override
	public String getKey() {
		return KEY;
	}
	
	@Override
	public void markFixed() {
	}

}
