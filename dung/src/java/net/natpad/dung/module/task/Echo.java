package net.natpad.dung.module.task;

import net.natpad.dung.run.Session;

public class Echo extends Task {

	private String ftext;

	public Echo() {
	}
	
	public Echo(String text) {
		this.ftext = text;
	}
	
	public void addText(String atext) {
		if (ftext==null) {
			ftext = atext;
		} else {
			ftext += atext;
		}
	}

	@Override
	public void runTask(Session session) throws Exception {
		String otext = session.resolveProperties(ftext);
		log(LogLevel.INFO, otext);
	}
}
