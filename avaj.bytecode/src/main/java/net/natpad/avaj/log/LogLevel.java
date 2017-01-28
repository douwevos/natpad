package net.natpad.avaj.log;

public enum LogLevel {

	CRITICAL("CRIT", 0),
	ERROR("ERRO", 1),
	FAIL("FAIL", 2),
	WARN("WARN", 3),
	MESSAGE("MESG", 4),
	INFO("INFO", 5),
	DEBUG("DEBG", 6),
	DETAIL("DETL", 7),
	TRACE("TRCE", 8);
	
	public final String shortName;
	public final int level;
	
	LogLevel(String shortName, int level) {
		this.shortName = shortName;
		this.level = level;
	}
	
}
