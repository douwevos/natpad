package net.natpad.dung.module.task;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;

public class Cstrip extends Task {

	private final String path;
	
	public Cstrip(String path) {
		this.path = path;
	}
	
	
	@Override
	public void runTask(Session session) throws Exception {
		Path pathToStripable = session.createModulePath(path);
		
		
		Runtime runtime = Runtime.getRuntime();
		
		List<String> argList = new ArrayList<String>();
		argList.add("/usr/bin/strip");
		
		argList.add(pathToStripable.toString());
		
		String[] args = argList.toArray(new String[argList.size()]);
		try {
			Process process = runtime.exec(args);
			Thread thread1 = new ProcessInputStreamer(process.getErrorStream()).launch();
			Thread thread2 = new ProcessInputStreamer(process.getInputStream()).launch();
			int waitFor = process.waitFor();
			thread1.join();
			thread2.join();
			if (waitFor!=0) {
				throw new RuntimeException("Could not strip:"+pathToStripable);
			}
			log(LogLevel.INFO, "Stripped '"+pathToStripable.getFileName()+"'");
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
}
