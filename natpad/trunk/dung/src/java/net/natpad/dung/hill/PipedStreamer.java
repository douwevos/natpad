package net.natpad.dung.hill;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class PipedStreamer implements Runnable {

	InputStream inStream;
	OutputStream outStream;
	
	public PipedStreamer(InputStream inStream, OutputStream outStream) {
		this.inStream = inStream;
		this.outStream = outStream;
		new Thread(this).start();
	}
	
	
	@Override
	public void run() {
		byte buffer[] = new byte[16384];
		while(true) {
			try {
				int cnt = inStream.read(buffer, 0, buffer.length);
//				System.out.println("cnt="+cnt);
				if (cnt>0) {
					outStream.write(buffer, 0, cnt);
					outStream.flush();
				} else {
					break;
				}
			} catch (IOException e) {
				e.printStackTrace();
				break;
			}
		}
		try { inStream.close(); } catch(Exception ignore) {}
		try { outStream.close(); } catch(Exception ignore) {}
		
	}
}
