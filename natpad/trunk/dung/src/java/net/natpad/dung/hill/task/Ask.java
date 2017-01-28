package net.natpad.dung.hill.task;

import java.io.InputStreamReader;
import java.io.LineNumberReader;

import net.natpad.dung.hill.HillSession;

public class Ask extends Task {

	public String property;
	public String question;
	public String defaultValue;
	
	
	
	@Override
	public void run(HillSession session) throws Exception {
		String rquestion = session.resolveProperties(question);
		StringBuilder buf = new StringBuilder(rquestion);
		if (defaultValue!=null) {
			buf.append(" [").append(session.resolveProperties(defaultValue)).append("]");
		}
		buf.append(':');
		System.out.print(buf);
		LineNumberReader lr = new LineNumberReader(new InputStreamReader(System.in));
		String readLine = lr.readLine();
		if (readLine==null || readLine.isEmpty()) {
			readLine = defaultValue;
		}
		
		if (readLine!=null && !readLine.isEmpty()) {
			session.properties.put(property, readLine);
		}
	}
	
}
