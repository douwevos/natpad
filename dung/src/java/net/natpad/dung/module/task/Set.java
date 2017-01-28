package net.natpad.dung.module.task;

import net.natpad.dung.run.Session;

public class Set extends Task {

	String key;
	String value;
	String location;
	
	@Override
	public void runTask(Session session) throws Exception {

		String rkey = session.resolveProperties(key);
		String rvalue = null;
		if (value!=null) {
			rvalue = session.resolveProperties(value);
		} else if (location!=null) {
			rvalue = session.createModulePath(location).toString();
		}
		log(LogLevel.INFO, "setting "+rkey+" to "+rvalue);
		session.properties.put(rkey, rvalue);
	}
}
