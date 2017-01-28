package net.natpad.dung;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

public class StreamHelper extends PrintStream {

	private final DelegateOut delegateOut;
	
	public StreamHelper(DelegateOut delegateOut) {
		super(delegateOut);
		System.setOut(this);
		System.setErr(this);
		this.delegateOut = delegateOut;
	}
	
	
	public void setPrefix(String prefix) {
		delegateOut.get().setPrefix(prefix);
	}

	public String getPrefix() {
		return delegateOut.get().prefix;
	}

	
	
	public static void setup() {
		DelegateOut deo = new DelegateOut(System.out);
		new StreamHelper(deo);
	}
	
	public static class DelegateOut extends OutputStream {

		ThreadLocal<Forwarder> tls = new ThreadLocal<Forwarder>();
		private final PrintStream delegate;
		
		public DelegateOut(PrintStream delegate) {
			this.delegate = delegate;
		}
		
		
		@Override
		public void write(byte[] b, int off, int len) throws IOException {
			Forwarder fw = get();
			fw.write(b, off, len);
		}
		
		@Override
		public void write(int b) throws IOException {
			Forwarder fw = get();
			fw.write(b);
		}

		@Override
		public void flush() throws IOException {
			Forwarder fw = get();
			fw.flush();
		}
		
		public Forwarder get() {
			Forwarder forwarder = tls.get();
			if (forwarder==null) {
				forwarder = new Forwarder(delegate);
				tls.set(forwarder);
			}
			return forwarder;
		}
	}
	
	
	static class Forwarder {

		final PrintStream delegate;
		ByteArrayOutputStream baos;
		ByteArrayOutputStream baosOut;
		boolean didPrefix;
		String prefix = "";
		
		public Forwarder(PrintStream delegate) {
			this.delegate = delegate;
			baos = new ByteArrayOutputStream();
			baosOut = new ByteArrayOutputStream();
		}

		public void setPrefix(String prefix2) {
			this.prefix = prefix2;
		}

		public void write(byte[] b, int off, int len) throws IOException {
			baos.write(b, off, len);
			flush();
		}

		public void write(int b) {
			baos.write(b);
		}

		public void flush() throws IOException {
			byte[] byteArray = baos.toByteArray();
			int sidx=0;
			for(int idx=0; idx<byteArray.length; idx++) {
				byte b = byteArray[idx];
//				delegate.print("."+(int) b); 
				if (b==0xa || b==0xd) {
					if (idx+1<byteArray.length) {
						byte b2 = byteArray[idx+1];
						if ((b2==0xa || b2==0xd) && (b2!=b)) {
							b = b2;
							idx++;
						}
					}
					baosOut.reset();
					if (!didPrefix) {
//						if (idx!=sidx) {
							baosOut.write(prefix.getBytes());
//						}
					}
					didPrefix = false;
					baosOut.write(byteArray, sidx, 1+idx-sidx);
					delegate.write(baosOut.toByteArray());
					sidx=idx+1;
				}
			}
			if (sidx<byteArray.length) {
				int idx=byteArray.length-1;
				baosOut.reset();
				if (!didPrefix) {
					didPrefix = true;
//					if (idx!=sidx) {
						baosOut.write(prefix.getBytes());
//					}
				}
				baosOut.write(byteArray, sidx, 1+idx-sidx);
				delegate.write(baosOut.toByteArray());
				sidx=idx+1;
			}
			baos.reset();
		}
		
	}


	
}
