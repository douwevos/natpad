package net.natpad.dung.module.task.cc;

import java.io.File;

import net.natpad.dung.run.Session;

public class CcLinkerPath implements ICcConfigItem {

	
	protected File path;
	
	public void setPath(File file) {
		path = file;
	}
	
	public File getPath() {
		return path;
	}

	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
	}

	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
		linkerSettings.setLinkerPath(path);
	}
}
