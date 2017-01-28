package net.natpad.dung.module.task.cc;

import java.io.File;
import java.util.ArrayList;

public class CcLinkerSettings {

	ArrayList<String> libSearchPaths = new ArrayList<String>();
	ArrayList<String> arguments = new ArrayList<String>();
	ArrayList<String> objectFiles = new ArrayList<String>();
	File path = null;

	
	public void setLinkerPath(File path) {
		this.path = path;
	}

	public void addArgument(String value) {
		arguments.add(value);
	}

	public void addObjectFile(String objectFile) {
		if (!objectFiles.contains(objectFile)) {
			objectFiles.add(objectFile);
		}
	}

	
	public void addLibSearchPath(String path) {
		path = "-L"+path;
		if (!libSearchPaths.contains(path)) {
			libSearchPaths.add(path);
		}
	}

	public void addLibName(String libName) {
		libName = "-l"+libName;
		arguments.remove(libName);
//		if (!arguments.contains(libName)) {
			arguments.add(libName);
//		}
	}


	public ArrayList<String> getExecAsArgList() {
		ArrayList<String> result = new ArrayList<String>();
		String pathtxt = "";
		if (path!=null) {
			pathtxt = path.getAbsolutePath();
		} else {
			pathtxt = "/usr/bin/gcc";
		}
		result.add(pathtxt);
		result.addAll(objectFiles);
		result.addAll(libSearchPaths);
		result.addAll(arguments);
		return result;
	}
		
}
