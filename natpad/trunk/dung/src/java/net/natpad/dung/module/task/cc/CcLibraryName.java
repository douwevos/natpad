package net.natpad.dung.module.task.cc;

import net.natpad.dung.run.Session;

public class CcLibraryName implements ICcConfigItem {

	String name;
	
	
	public CcLibraryName() {
	}

	public CcLibraryName(String name) {
		this.name = name;
	}
	
	
	
	public void setName(String name) {
		this.name = name;
	}
	
	
	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
	}
	
	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
		linkerSettings.addLibName(name);
	}
}
