package net.natpad.dung.module.model;

import java.util.ArrayList;
import java.util.List;

public class ExportHeadersDescr {

	public List<String> directories = new ArrayList<String>();
	
	
	public void addDir(String dir) {
		directories.add(dir);
	}
}
