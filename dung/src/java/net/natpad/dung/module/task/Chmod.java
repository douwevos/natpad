package net.natpad.dung.module.task;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.BashHelper;
import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;

public class Chmod extends Task {

	public String permissions;
	private List<FileBundle> fileBundles = new ArrayList<FileBundle>();
	
	public void addIn(FileBundle bundle) {
		fileBundles.add(bundle);
	}
	
	
	@Override
	public void runTask(Session session) throws Exception {
		String rperms = session.resolveProperties(permissions);

		List<String> argList = new ArrayList<String>();
		argList.add("/bin/chmod");
		argList.add(rperms);

		int count=0;
		PathInPath[] scanBundles = session.scanBundles(session.module.directory, fileBundles, true);
		for (PathInPath pathInPath : scanBundles) {
			if (pathInPath.isFile) {
				String relativePath = pathInPath.relativePath.toString();
				File srcFile = new File(pathInPath.basePath.toFile(), relativePath);
				
				argList.add(srcFile.getAbsolutePath());
				count++;
			}
		}
		
		
		String[] args = argList.toArray(new String[argList.size()]);
		if (isEnabled(LogLevel.VERBOSE)) {
			for (String arg : args) {
				log(LogLevel.VERBOSE, ":"+arg);
			}
		}
		try {
			

			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(args);
			
			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
			Thread thread1 = errstreamer.launch();
			ProcessInputStreamer outstreamer = new ProcessInputStreamer(process.getInputStream());
			Thread thread2 = outstreamer.launch();
			thread1.join();
			thread2.join();
			int exitCode = process.waitFor();
			for(String line : outstreamer.output) {
				BashHelper.printLine(line);
//				System.err.println(line);
			}
			for(String line : errstreamer.output) {
				BashHelper.printLine(line);
			}
			if (exitCode!=0) {
				
				throw new RuntimeException("Error running chmod over files");
			}
			
			log(LogLevel.INFO, "Changed permissions of "+count+" file"+(count>1 ? "s" : ""));
		} catch (IOException e) {
			throw new RuntimeException("Error running chmod over files", e);
		}
		
	}
	
}
