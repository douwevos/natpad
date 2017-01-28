package net.natpad.dung.module.task;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;

public class Dpkgdeb extends Task {

	String in;
	String tofile;
	
	
	@Override
	public void runTask(Session session) throws Exception {
		Runtime runtime = Runtime.getRuntime();

		if (in==null) {
			throw new RuntimeException("'in' is a mandatory argument");
		}
		if (tofile==null) {
			throw new RuntimeException("'tofile' is a mandatory argument");
		}
		
		Path inoutPath = session.createModulePath(in);
		Path outputPath = session.createBuildPath(tofile);
		
		List<String> argList = new ArrayList<String>();
		argList.add("/usr/bin/fakeroot");
		argList.add("/usr/bin/dpkg-deb");
		argList.add("--build");
		argList.add(inoutPath.toString());
		argList.add(outputPath.toString());
		
		String[] args = argList.toArray(new String[argList.size()]);
		try {
			if (isEnabled(LogLevel.VERBOSE)) {
				for(String arg : args) {
					log(LogLevel.VERBOSE, "arg:"+arg );
				}
			}

			Process process = runtime.exec(args);
			Thread thread1 = new ProcessInputStreamer(process.getErrorStream(), System.out).launch();
			Thread thread2 = new ProcessInputStreamer(process.getInputStream(), System.out).launch();
			thread1.join();
			thread2.join();
			int exitCode = process.waitFor();
			if (exitCode!=0) {
				throw new RuntimeException("Error while creating Debian package:"+outputPath);
			}
		} catch (Exception e) {
			e.printStackTrace();
			log(LogLevel.ERROR, "error-"+e.getMessage());
		}	
	}
	
}
