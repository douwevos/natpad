package net.natpad.dung.hill.task;

import net.natpad.dung.hill.HillSession;

public class Export extends Task {

	String key;
	String value;
	
	
	
	@Override
	public void run(HillSession session) throws Exception {
		String rkey = session.resolveProperties(key);
		String rvalue = session.resolveProperties(value);
		session.workspace.putProperty(rkey, rvalue);
	}
	
	
}
