package net.natpad.dung.hill.task;

import java.nio.file.Path;
import java.nio.file.Paths;

import net.natpad.dung.hill.HillSession;

public class Modulepath extends Task {

	public String path;
	
	
	public Modulepath(String path) {
		this.path = path;
	}

	
	@Override
	public void run(HillSession session) throws Exception {
		
		String rpath = session.resolveProperties(path);
		
		
		Path pathM = Paths.get(rpath);
		if (!pathM.isAbsolute()) {
			Path pathCd = Paths.get(System.getProperty("user.dir"));
			pathM = pathCd.resolve(pathM).normalize();
		}
		
		session.workspace.addModulePath(pathM.toString());
	}
}
