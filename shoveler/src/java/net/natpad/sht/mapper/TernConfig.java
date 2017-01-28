package net.natpad.sht.mapper;

import java.util.HashMap;

public class TernConfig {

	HashMap<String, DocumentClass> classes = new HashMap<String, DocumentClass>();
	
	
	
	public void addClass(DocumentClass clazz) {
		classes.put(clazz.getClazzName(), clazz);
	}
}
