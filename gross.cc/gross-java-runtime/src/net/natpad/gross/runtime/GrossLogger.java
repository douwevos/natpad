package net.natpad.gross.runtime;


public interface GrossLogger {

	public boolean isEnabled(Level l);
	public void log(Level level, String msg, Object ... args);
	
	enum Level {
		ERROR,
		WARN,
		INFO,
		VERBOSE,
		DEBUG,
		DETAIL,
		ALL
	}
}
