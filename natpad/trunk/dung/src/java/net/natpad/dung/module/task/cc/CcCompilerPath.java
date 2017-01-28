package net.natpad.dung.module.task.cc;

import java.io.File;

import net.natpad.dung.run.Session;



public class CcCompilerPath implements ICcConfigItem {

	protected File path;
	
	public void setPath(File file) {
		path = file;
	}
	
	public File getPath() {
		return path;
	}
	
	
	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
		compilerSettings.setCompilerPath(path);
	}
	
	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
	}
	
}
