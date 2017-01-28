package net.natpad.dung.module.task.cc;

import net.natpad.dung.run.Session;


public class CcLibraryPath implements ICcConfigItem {

	String path;
	
	public CcLibraryPath() {
	}
	
	public CcLibraryPath(String path) {
		this.path = path;
	}
	
	
	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
	}

	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
		linkerSettings.addLibSearchPath(path);
	}

}
