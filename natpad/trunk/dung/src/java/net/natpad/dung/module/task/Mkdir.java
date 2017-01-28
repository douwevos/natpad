package net.natpad.dung.module.task;

import java.nio.file.Path;

import net.natpad.dung.run.Session;

public class Mkdir extends Task {
	
	String path;
	
	public Mkdir(String path) {
		this.path = path;
	}
	
	@Override
	public void runTask(Session session) throws Exception {
		String pathRes = session.resolveProperties(path);
		Path modulePath = session.createModulePath(pathRes);
		log(LogLevel.INFO, ""+modulePath.toString());
		modulePath.toFile().mkdirs();
	}
}
