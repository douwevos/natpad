package net.natpad.dung.hill.task;

import net.natpad.dung.hill.HillSession;
import net.natpad.dung.module.task.LogLevel;

public abstract class Task {

	private final int LOG_NAME_SIZE = 18;
	
	public abstract void run(HillSession session) throws Exception;
	
	public String getLogDescription() {
		return getClass().getSimpleName().toLowerCase();
	}
	
	public void log(LogLevel level, String msg) {
		String rawDesc = getLogDescription();
		String logDescription = "["+rawDesc+"]";
		if (logDescription.length()>=LOG_NAME_SIZE) {
			logDescription = "["+rawDesc.substring(0, LOG_NAME_SIZE-2)+"]";
		} else {
			int rest = LOG_NAME_SIZE - logDescription.length();
			rest = rest/2;
			StringBuilder buf = new StringBuilder(LOG_NAME_SIZE);
			for(int idx=0; idx<rest; idx++) {
				buf.append(' ');
			}
			buf.append(logDescription);
			while(buf.length()<LOG_NAME_SIZE) {
				buf.append(' ');
			}
			logDescription = buf.toString();
		}
		System.out.println("  "+logDescription+" "+msg);
//		if (level.compareTo(LogLevel.INFO)<0) {
//			
//		}
	}
	
	
}
