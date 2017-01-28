package net.natpad.sht.model;

import java.util.ArrayList;
import java.util.Iterator;

public class Template implements Iterable<TemplateArgument> {

	private String name;
	
	ArrayList<TemplateArgument> arguments = new ArrayList<TemplateArgument>();
	
	public Template(String name) {
		this.name = name;
	}
	
	public void add(TemplateArgument arg) {
		arguments.add(arg);
	}

	
	@Override
	public Iterator<TemplateArgument> iterator() {
		return arguments.iterator();
	}
	
	public String getName() {
		return name;
	}
}
