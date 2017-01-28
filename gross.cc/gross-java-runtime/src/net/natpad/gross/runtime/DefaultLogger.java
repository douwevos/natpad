package net.natpad.gross.runtime;

import java.io.PrintStream;
import java.io.PrintWriter;

public class DefaultLogger implements GrossLogger {

	Level level = Level.WARN;
	PrintWriter delegate;

	public DefaultLogger(PrintStream delegate) {
		this.delegate = new PrintWriter(delegate, true);
	}

	public DefaultLogger(PrintStream out, Level all) {
		this.delegate = new PrintWriter(out, true);
		level = all;
	}

	
	public DefaultLogger(PrintWriter delegate) {
		this.delegate = delegate;
	}
	


	@Override
	public boolean isEnabled(Level l) {
		return l.ordinal()<=level.ordinal();
	}
	
	@Override
	public void log(Level l, String msg, Object... args) {
		if (l.ordinal()<=level.ordinal()) {
//			delegate.println(level+" : "+ String.format(msg, args));
			delegate.println(String.format(msg, args));
			delegate.flush();
		}
	}

	public void increaseLogLevel() {
		switch(level) {
			case ALL : break;
			case ERROR : level = Level.WARN; break;
			case WARN : level = Level.INFO; break;
			case INFO : level = Level.VERBOSE; break;
			case VERBOSE : level = Level.DEBUG; break;
			case DEBUG : level = Level.DETAIL; break;
			case DETAIL : level = Level.ALL; break;
		}
		
	}
	
}
