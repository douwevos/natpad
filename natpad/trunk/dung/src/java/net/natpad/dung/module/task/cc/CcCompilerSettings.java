package net.natpad.dung.module.task.cc;

import java.io.File;
import java.util.ArrayList;


public class CcCompilerSettings {

//	private final Project project;
	ArrayList<String> arguments = new ArrayList<String>();
	File path = null;

//	public CcCompilerSettings(Project project) {
//		this.project = project;
//	}
//	
	
	public void setCompilerPath(File path) {
		this.path = path;
	}

	public void addArgument(String value) {
		arguments.add(value);
	}
	
	public void addArgumentOnce(String value, boolean replace) {
		if (arguments.contains(value)) {
			if (replace) {
				int idx = arguments.indexOf(value);
				if (idx>=0) {
					arguments.remove(idx);
				}
				arguments.add(value);
			}
		} else {
			arguments.add(value);
		}
	}

	public void addIncludePath(String path) {
		arguments.add("-I"+path);
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
		result.addAll(arguments);
		result.add("-c");

		return result;
	}
	
	
	public void dump() {
		for(String arg : arguments) {
			System.out.println("arg="+arg);
		}
	}





}
