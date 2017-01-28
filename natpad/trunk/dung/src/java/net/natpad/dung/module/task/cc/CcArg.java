package net.natpad.dung.module.task.cc;

import net.natpad.dung.run.Session;


public class CcArg implements ICcConfigItem {

	protected String value;
	
	public CcArg(String value) {
		this.value = value;
	}
	
	public void setValue(String value) {
		this.value = value;
	}
	

	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
		compilerSettings.addArgument(value);
	}

	
	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
		linkerSettings.addArgument(value);
	}

}
