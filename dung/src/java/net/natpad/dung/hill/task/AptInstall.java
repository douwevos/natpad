package net.natpad.dung.hill.task;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.OutputStream;
import java.lang.ProcessBuilder.Redirect;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import net.natpad.dung.hill.HillSession;
import net.natpad.dung.module.task.LogLevel;
import net.natpad.dung.run.BashHelper;
import net.natpad.dung.run.ProcessInputStreamer;

public class AptInstall extends Task {

	public String name;
	
	@Override
	public void run(HillSession session) throws Exception {
		
		String args[] = new String[] { "/usr/bin/dpkg", "-la"};
		List<String> installed = execAndEnlist(args);
		for(String l : installed) {
			if (l!=null && l.length()>2) {
				if (l.substring(2).startsWith(name)) {
					log(LogLevel.INFO, "package '"+name+"' already installed");
					return;
				}
			}
		}
		
		args = new String[] { "/usr/bin/sudo", "/usr/bin/apt", "install", name};

		execPrivilegd(args);
		

	}

	
	public List<String> execAndEnlist(String args[]) {
		ArrayList<String> lines = new ArrayList<>();
		int exitcode = 0;
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(args);
			
			
			
			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
			Thread errthread = errstreamer.launch();
			
			InputStream in = process.getInputStream();
			LineNumberReader reader = new LineNumberReader(new InputStreamReader(in));
			while(true) {
				try {
					String line = reader.readLine();
					if (line == null) {
						break;
					}
					lines.add(line);
				} catch (IOException e) {
					e.printStackTrace();
					break;
				}
			}

			exitcode = process.waitFor();
			errthread.join();
			for(String line : errstreamer.output) {
				BashHelper.printLine(line);
			}
			if (exitcode!=0) {
				System.err.println("Dumping arguments");
				for(int ia=0; ia<args.length; ia++) {
					System.err.println(" ["+ia+"]="+args[ia]);
				}
//				throw new BuildException("cc compile returned "+exitcode);
			}
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return lines;
	}


	
	public void execPrivilegd(String args[]) {
		int exitcode = 0;
		try {
			ProcessBuilder pb = new ProcessBuilder(Arrays.asList(args));
//			pb.redirectInput(Redirect.INHERIT);
//			pb.redirectOutput(Redirect.INHERIT);
//			pb.redirectError(Redirect.INHERIT);
			pb.inheritIO();
			Process process = pb.start();			
//			Runtime runtime = Runtime.getRuntime();
//			Process process = runtime.exec(args);
			
//			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
//			Thread errthread = errstreamer.launch();
//
//			ProcessInputStreamer instreamer = new ProcessInputStreamer(process.getInputStream());
//			Thread inthread = errstreamer.launch();
//
//			
//			final OutputStream outputStream = process.getOutputStream();
//			
//			Thread inForwarderThread = new Thread(new Runnable() {
//				
//				@Override
//				public void run() {
//					byte buf[] = new byte[1024];
//					try {
//						while(true) {
//							int read = System.in.read(buf,0,buf.length);
//							System.out.println(""+new String(buf,0,read));
//							if (read>0) {
//								outputStream.write(buf, 0, read);
//								outputStream.flush();
//							}
//						}
//					} catch(Exception e) {
//						
//					}
//				}
//			});
//			
//			inForwarderThread.start();
			

			exitcode = process.waitFor();
//			errthread.join();
//			inForwarderThread.interrupt();
//			for(String line : errstreamer.output) {
//				BashHelper.printLine(line);
//			}
			if (exitcode!=0) {
				System.err.println("Dumping arguments");
				for(int ia=0; ia<args.length; ia++) {
					System.err.println(" ["+ia+"]="+args[ia]);
				}
//				throw new BuildException("cc compile returned "+exitcode);
			}
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}


	
}
