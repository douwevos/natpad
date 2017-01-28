package net.natpad.jaguar.viper.model.archive;

import java.io.IOException;
import java.io.InputStream;

public class LimitInputStream extends InputStream {

	InputStream delegate;
	long mark;
	long offset;
	long maxLength;
	
	public LimitInputStream(InputStream delegate, long maxLength) {
		this.delegate = delegate;
		this.maxLength = maxLength;
		this.offset = 0L;
	}
	
	
	
	@Override
	public int read() throws IOException {
		int result = -1;
		if (offset<maxLength) {
			result = delegate.read();
			if (result>=0) {
				offset++;
			}
		}
		return result;
	}
	
	@Override
	public int read(byte[] b, int off, int len) throws IOException {
		int rest = (int) (maxLength-offset);
		if (len>rest) {
			len = rest;
		}
		int readCnt = delegate.read(b, off, len);
		if (readCnt>=0) {
			offset += readCnt;
		}
		return readCnt;
	}
	
	
	@Override
	public int available() throws IOException {
		int available = delegate.available();
		int rest = (int) (maxLength-offset);
		if (available>rest) {
			available = rest;
		}
		return available;
	}
	
	@Override
	public long skip(long toSkip) throws IOException {
		long rest = maxLength-offset;
		if (toSkip>rest) {
			toSkip = rest;
		}
		long skipped = delegate.skip(toSkip);
		offset += skipped;
		return skipped;
	}
	
	
	@Override
	public synchronized void mark(int readlimit) {
		mark = offset;
		delegate.mark(readlimit);
	}
	
	@Override
	public boolean markSupported() {
		return delegate.markSupported();
	}
	
	@Override
	public synchronized void reset() throws IOException {
		delegate.reset();
		offset = mark;
	}
	
}
