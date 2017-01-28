package net.natpad.caterpillar;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;

public class CatLoggerPrintStream extends PrintStream {

	CatLogger logger;
	CatLogLevels level;
	
	public CatLoggerPrintStream(CatLogger logger, CatLogLevels level) {
		super(new MB(), true);
		((MB) out).sss = this;
		this.logger = logger;
		this.level = level;
		
	}
	
	@Override
	protected void finalize() throws Throwable {
		flush();
	}
	
	@Override
	public void flush() {
		ByteArrayOutputStream baos = (ByteArrayOutputStream) out; 
		String txt = baos.toString();
		int idx=0;
		while(idx<txt.length()) {
			int b = txt.indexOf('\n', idx);
			if (b<0) {
				if (idx==0) {
					return;
				}
				b = txt.length();
				String sline = txt.substring(idx);
				baos.reset();
				try {
					baos.write(sline.getBytes());
				} catch (IOException e) {
					e.printStackTrace();
				}
				return;
			}
			String sline = txt.substring(idx, b);
			logger.cat_log_print(level, sline);
			idx = b+1;
		}
		baos.reset();
		
	}
	
	
	static class MB extends ByteArrayOutputStream {
		
		public CatLoggerPrintStream sss;
		
		@Override
		public void flush() throws IOException {
			super.flush();
			sss.flush();
		}
		
		
		
	}
	
}
