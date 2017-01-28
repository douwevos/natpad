/**
 * 
 */
package net.natpad.dung.run;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;


public class ProcessInputStreamer implements Runnable {
	private final InputStream in;
	private final PrintStream out;
	
	public List<String> output = new ArrayList<String>();

	public ProcessInputStreamer(InputStream in) {
		this.in = in;
		out = null;
	}


	public ProcessInputStreamer(InputStream in, PrintStream out) {
		this.in = in;
		this.out = out;
	}

	
	public Thread launch() {
		Thread thread = new Thread(this);
		thread.start();
		return thread;
		
	}

	public void run()  {
		LineNumberReader reader = new LineNumberReader(new InputStreamReader(in));
		while(true) {
			try {
				String line = reader.readLine();
				if (line == null) {
					return;
				}
				output.add(line);
				if (out!=null) {
					out.println(line);
				}
			} catch (IOException e) {
				e.printStackTrace();
				return;
			}
		}
	}
}