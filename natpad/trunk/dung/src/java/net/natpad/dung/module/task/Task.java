package net.natpad.dung.module.task;

import net.natpad.dung.StreamHelper;
import net.natpad.dung.run.Context;
import net.natpad.dung.run.Session;

public abstract class Task {

	private final int LOG_NAME_SIZE = 18;
	
	ThreadLocal<Session> currentSession = new ThreadLocal<Session>();
	
	String logDescription;
	
	public void invokeTask(Session session) throws Exception {

		if (System.out instanceof StreamHelper) {
			StreamHelper streamHelper = (StreamHelper) System.out;
			streamHelper.flush();
			streamHelper.setPrefix("  " + (char) 27 +"[1;34m" + getLogDescription()+" " + (char) 27 +"[0m");
			
		}
		currentSession.set(session);
		runTask(session);
		currentSession.remove();
		if (System.out instanceof StreamHelper) {
			StreamHelper streamHelper = (StreamHelper) System.out;
			streamHelper.flush();
			streamHelper.setPrefix("");
		}
	}
	
	public abstract void runTask(Session session) throws Exception;
	
	public String getLogDescription() {
		if (logDescription==null) {
			String rawDesc = getClass().getSimpleName().toLowerCase();
			logDescription = "["+rawDesc+"]";
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
		}
		return logDescription;
	}
	
	public void log(LogLevel level, String msg) {
		if (!isEnabled(level)) {
			return;
		}
//		String rawDesc = getLogDescription();
//		String logDescription = "["+rawDesc+"]";
//		if (logDescription.length()>=LOG_NAME_SIZE) {
//			logDescription = "["+rawDesc.substring(0, LOG_NAME_SIZE-2)+"]";
//		} else {
//			int rest = LOG_NAME_SIZE - logDescription.length();
//			rest = rest/2;
//			StringBuilder buf = new StringBuilder(LOG_NAME_SIZE);
//			for(int idx=0; idx<rest; idx++) {
//				buf.append(' ');
//			}
//			buf.append(logDescription);
//			while(buf.length()<LOG_NAME_SIZE) {
//				buf.append(' ');
//			}
//			logDescription = buf.toString();
//		}
//		System.out.println("  "+logDescription+" "+msg);
		System.out.println(msg);
//		if (level.compareTo(LogLevel.INFO)<0) {
//			
//		}
	}
	
	public boolean isEnabled(LogLevel level) {
		Session session = currentSession.get();
		Context context = session.context;
		return level.ordinal()<=context.getLogLevel().ordinal();
	}
	
	
	protected void logArgs(String args[]) {
		
	}
	
}
