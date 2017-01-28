package net.natpad.sht.mapper;

import java.util.ArrayList;
import java.util.List;

public class DocumentClass {

	String name;
	String grammar;
	List<String> patterns;
	
	public DocumentClass(String name, String grammar, List<String> patterns) {
		this.name = name;
		this.grammar = grammar;
		this.patterns = new ArrayList<String>(patterns);
	}

	
	
	
	public String getClazzName() {
		return name;
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("DocumentClass[name=");
		buf.append(name);
		buf.append(",grammar=").append(grammar);
		buf.append(",patterns={");
		if (patterns!=null) {
			boolean isFirst = true;
			for(String pat : patterns) {
				if (isFirst) {
					isFirst = false;
				} else {
					buf.append(",");
				}
				buf.append(pat);
			}
		}
		buf.append("}");
		return buf.append("]").toString();
	}
	
}
