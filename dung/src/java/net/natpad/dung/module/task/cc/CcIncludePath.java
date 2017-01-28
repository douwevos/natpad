package net.natpad.dung.module.task.cc;

import java.util.ArrayList;

import net.natpad.dung.run.Session;


public class CcIncludePath implements ICcConfigItem {

	
	ArrayList<String> paths = new ArrayList<String>();
	
	public CcIncludePath() {
		super();
	}
	
	
	public void addPath(String path) {
		paths.add(path);
	}
	
	
	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
		for(String i : paths) {
			compilerSettings.addArgumentOnce("-I"+i, false);
		}
		
	}

	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
	}
}
