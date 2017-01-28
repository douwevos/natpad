package net.natpad.dung.module.task.cc;

import net.natpad.dung.run.Session;

public interface ICcConfigItem {

	public void setup(Session session, CcCompilerSettings compilerSettings);

	public void setup(Session session, CcLinkerSettings linkerSettings);

}
