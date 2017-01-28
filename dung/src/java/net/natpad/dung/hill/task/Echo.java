package net.natpad.dung.hill.task;

import net.natpad.dung.hill.HillSession;

public class Echo extends Task {

	private final String text;

	public Echo(String text) {
		this.text = text;
	}

	
	@Override
	public void run(HillSession session) throws Exception {
		String otext = session.resolveProperties(text);
		System.out.println(""+otext);
	}
}
