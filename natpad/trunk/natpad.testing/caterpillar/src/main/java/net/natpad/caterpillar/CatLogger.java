package net.natpad.caterpillar;

import java.io.PrintStream;

public class CatLogger {

	
	
	private final static long start_time = System.currentTimeMillis();
	
	
	private boolean ignoreAnnotations = false;
	
	static PrintStream out = System.out;
	
	private CatLogLevels log_level = CatLogLevels.CAT_LOG_WARN;
	
	public static CatLogger default_instance = new CatLogger();
	
	public static CatLoggerPrintStream logout = new CatLoggerPrintStream(default_instance, CatLogLevels.CAT_LOG_INFO);
	
	
	public CatLogger() {
	}

	public CatLogger(CatLogLevels logLevel) {
		this.log_level = logLevel;
	}
	
	
	
	public void cat_log_print(CatLogLevels level, String txt, Object ... args)  {
		synchronized (CatLogger.class) {
			CatLogLevels testlevel = log_level;
			if (ignoreAnnotations && level.ordinal()>log_level.ordinal()) {
				return;
			}
			long diftime = System.currentTimeMillis() - start_time;
			
			String className = "???";
			String methodName = "???";
			int lineNumber = 0;
			
			
			StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
			for(int idx=1; idx<stackTrace.length; idx++) {
				String classNameTst = stackTrace[idx].getClassName();
				if (classNameTst.startsWith("net.natpad.caterpillar.CatLog") 
						|| (classNameTst.startsWith("java.io"))
						|| (classNameTst.startsWith("sun."))) {
					continue;
				}
				className = classNameTst;
				lineNumber = stackTrace[idx].getLineNumber();
				methodName = stackTrace[idx].getMethodName();
				
				if (!ignoreAnnotations) {
					try {
						Class<?> clazz = Class.forName(className);
						CatLogLevel annotation = clazz.getAnnotation(CatLogLevel.class);
						if (annotation!=null) {
							testlevel = annotation.value();
						}
						
					} catch (Throwable ignore) {
					}
				}
				
				break;
			}

			if (!ignoreAnnotations && level.ordinal()>testlevel.ordinal()) {
				return;
			}

			out.print(diftime + " "+level+":"+className+"."+methodName+"("+lineNumber+") ");
			
			CatString t = CatString.format(txt, args);
			
			out.println(t.toString());
		}
	}

	public void trace(String txt, Object ... args) {
		cat_log_print(CatLogLevels.CAT_LOG_TRACE, txt, args);
	}

	public static void cat_log_trace(String txt, Object ... args) {
		default_instance.trace(txt, args);
	}


	public void info(String txt, Object ... args) {
		cat_log_print(CatLogLevels.CAT_LOG_INFO, txt, args);
	}

	public static void cat_log_info(String txt, Object ... args) {
		default_instance.info(txt, args);
	}

	
	public void debug(String txt, Object ... args) {
		cat_log_print(CatLogLevels.CAT_LOG_DEBUG, txt, args);
	}

	public static void cat_log_debug(String txt, Object ... args) {
		default_instance.debug(txt, args);
	}

	public void error(String txt, Object ... args) {
		cat_log_print(CatLogLevels.CAT_LOG_ERROR, txt, args);
	}

	public static void cat_log_error(String txt, Object ... args) {
		default_instance.error(txt, args);
	}

	
	
	
	public void setLevel(CatLogLevels level) {
		this.log_level = level;
	}

	
	public static <T> boolean safeEquals(T a, T b) {
		if (a==b) {
			return true;
		}
		
		if ((a==null) || (b==null)) {
			return false;
		}
		return a.equals(b);
	}
	
}
