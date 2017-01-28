package net.natpad.dung.module.model;

import java.util.ArrayList;
import java.util.List;

public class ExportLibraryDescr {

	public List<String> libnames = new ArrayList<String>();
	public List<String> paths = new ArrayList<String>();
	
	
	public void addLibName(String libName) {
		libnames.add(libName);
	}

	public void addPath(String path) {
		paths.add(path);
	}

	@Override
	public String toString() {
		return "ExportLibraryDescr [libnames=" + libnames + ", paths=" + paths + "]";
	}

	
	
}
