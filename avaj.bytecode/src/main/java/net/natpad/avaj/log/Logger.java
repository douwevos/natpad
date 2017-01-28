package net.natpad.avaj.log;

import java.io.PrintStream;
import java.util.HashMap;

import net.natpad.avaj.util.TextUtil;


public class Logger {

	public enum LogFormat {
		LONG,
		SEMILONG,
		SHORT
	}
	
	public static String ln = "\n";
	public static LogLevel defaultLogLevel = LogLevel.TRACE;
	protected static HashMap<String, Logger> instanceMap = new HashMap<String, Logger>();

	protected LogLevel level = defaultLogLevel;
	protected LogFormat logFormat = LogFormat.LONG;
	protected PrintStream out = System.out;
	protected long startTime = System.currentTimeMillis();
	protected static int maxLineLength = 15;
	protected static int indent = 0;
	
	public final String loggerName;


	static {
		try { 
			ln = System.getenv("line.separator");
		} catch(RuntimeException ignore) {}
		if (ln==null) {
			ln="\n";
		}
	}

	private Logger(String loggerName) {
		this.loggerName = loggerName;
	}
	
	public static Logger getInstance(String name) {
		Logger logger = instanceMap.get(name);
		if (logger==null) {
			logger = new Logger(name);
			instanceMap.put(name, logger);
		}
		return logger;
	}

	public static Logger getInstance(Class<?> clazz) {
		String name = clazz.getName();
		int indexOf = name.lastIndexOf('.');
		if (indexOf>0) {
			name = name.substring(indexOf+1);
		}
		return getInstance(name);
	}

	
	
	public void indent() {
		indent++;
	}

	public void dedent() {
		indent--;
	}

	
	public void setLevel(LogLevel logLevel) {
		level = logLevel;
	}

	public boolean logEnabbledFor(LogLevel logLevel) {
		return (level.level >= logLevel.level);
	}
	
	public void setFormat(LogFormat newFormat) {
		logFormat = newFormat;
	}

	
	public void trace(String txt) {
		log(LogLevel.TRACE, txt);
	}


	public void detail(String txt) {
		log(LogLevel.DETAIL, txt);
	}

	
	public void debug(String txt) {
		log(LogLevel.DEBUG, txt);
	}

	public void info(String txt) {
		log(LogLevel.INFO, txt);
	}

	public void message(String txt) {
		log(LogLevel.MESSAGE, txt);
	}

	public void warn(String txt) {
		log(LogLevel.WARN, txt);
	}

	
	public void error(String txt) {
		log(LogLevel.ERROR, txt);
	}

	public void fail(String txt) {
		log(LogLevel.FAIL, txt);
	}

	public void log(LogLevel logLevel, String txt) {
		if (txt==null || out==null) {
			return;
		}
		if (level.level >= logLevel.level) {
			StringBuilder fullMessage = createMessage(txt, logLevel);
			out.print(fullMessage);
			if (fullMessage.length()>200) {
				out.flush();
			}
		}
	}
	
	public void exception(Throwable ignore) {
		if (out!=null) {
			ignore.printStackTrace(out);
		}
	}

	
	private StringBuilder createMessage(String message, LogLevel logLevel) {
		StringBuilder result = new StringBuilder();
		String prefixMessage = "";
		
		if (logFormat == LogFormat.LONG) {
			
			StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
			
			StackTraceElement ste = null;
			int idx;
			for(idx=2; idx<stackTrace.length; idx++) {
				if (!stackTrace[idx].getClassName().startsWith(getClass().getName())) {
					ste = stackTrace[idx];
					break;
				}
			}
			if (ste!=null) {
				result.append(logLevel.shortName).append(" # ");
	
				String clsname = ste.getClassName();
				idx = clsname.lastIndexOf('.');
				if (idx>=0) {
					clsname = clsname.substring(idx+1);
				}
				
				result.append(clsname);
				result.append('.');
				result.append(ste.getMethodName());
				result.append('(');
				result.append(""+ste.getLineNumber());
				result.append(')');
			}
			if (maxLineLength<result.length()+1) {
				maxLineLength = result.length()+1;
			}
			while(result.length()<maxLineLength) {
				result.append(' ');
			}
			
		} else if (logFormat == LogFormat.SEMILONG) {
			result.append(logLevel.shortName).append(" # ");
			result.append(loggerName).append(": ");
			if (maxLineLength<result.length()+1) {
				maxLineLength = result.length()+1;
			}
			while(result.length()<maxLineLength) {
				result.append(' ');
			}

		}
		
		for(int idx=indent; idx>0; idx--) {
			result.append("   ");
		}

		prefixMessage = result.toString();
		result.setLength(0);

		
		int startIdx = 0;
		String sub;
		while(startIdx>=0) {
			int endIdx = message.indexOf(ln, startIdx);
			if (endIdx>=0) {
				sub = message.substring(startIdx, endIdx);
				startIdx = endIdx+ln.length();
			} else {
				sub = message.substring(startIdx);
				startIdx = -1;
			}
			if (startIdx!=-1 || sub.length()>0) {
				result.append(prefixMessage).append(sub).append(ln);
			}
		}
		
		return result;
	}
	
	

	
	public void logByteArray(LogLevel level, byte data[], long base, int baseDigits, int width, boolean withAscii) {
		if (!logEnabbledFor(level)) {
			return;
		}
		
		
		StringBuilder outBuf = dumpByteArray("", data, base, baseDigits, width, withAscii);
		log(level, outBuf.toString());
	}

	
	
	public static StringBuilder dumpByteArray(String prefix, byte[] data, long base, int baseDigits, int width, boolean withAscii) {
		StringBuilder outBuf = new StringBuilder();
		
		StringBuilder hexBuf = new StringBuilder();
		StringBuilder asciiBuf = new StringBuilder();
		
		
		int idx=0; 
		while(idx<data.length) {
			outBuf.append(prefix);
			outBuf.append(TextUtil.hex(base+idx, baseDigits));
			outBuf.append(" : ");
			for(int subIdx=0; subIdx<width; subIdx++, idx++) {
				if (idx<data.length) {
					int byteVal = 0xFF & data[idx];
					hexBuf.append(TextUtil.hex(byteVal, 2));
					hexBuf.append(' ');
					if (withAscii) {
						if (byteVal>=32 && byteVal<128) {
							asciiBuf.append((char) byteVal);
						} else {
							asciiBuf.append('.');
						}
					}
				} else {
					hexBuf.append("   ");
				}
			}
			outBuf.append(hexBuf);
			if (withAscii) {
				outBuf.append("  '").append(asciiBuf).append('\'');
			}
			outBuf.append(ln);
			hexBuf.setLength(0);
			asciiBuf.setLength(0);
		}
		return outBuf;
	}
	
	
	
}
