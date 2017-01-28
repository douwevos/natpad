package net.natpad.dung.module.task.cc;

import net.natpad.dung.run.Session;

public class CcStripArgument implements ICcConfigItem {

	public String name;
	
	
	public void setArgument(String name) {
		this.name = name;
	}
	
	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
		while(true) {
			int idx = compilerSettings.arguments.indexOf(name);
			if (idx>=0) {
				compilerSettings.arguments.remove(idx);
			} else {
				break;
			}
		}
	}

	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
		while(true) {
			int idx = linkerSettings.arguments.indexOf(name);
			if (idx>=0) {
				linkerSettings.arguments.remove(idx);
			} else {
				break;
			}
		}
	}
	
}
