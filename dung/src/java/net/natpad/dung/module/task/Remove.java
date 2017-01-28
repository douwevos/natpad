package net.natpad.dung.module.task;

import java.io.File;
import java.nio.file.Path;
import java.util.concurrent.atomic.AtomicInteger;

import net.natpad.dung.run.Session;

public class Remove extends Task {

	String path;
	
	public Remove(String path) {
		this.path = path;
	}
	
	@Override
	public void runTask(Session session) throws Exception {
		Path rpath = session.createModulePath(path);
		File file = rpath.toFile();
		AtomicInteger counter = new AtomicInteger();
		remove(file, counter);
		int nrOfFilesRemoved = counter.get();
		if (nrOfFilesRemoved>1) {
			log(LogLevel.INFO, "Removed "+file.getAbsolutePath()+" including "+(nrOfFilesRemoved-1)+" files");
		} else if (nrOfFilesRemoved>0) {
			log(LogLevel.INFO, "Removed "+file.getAbsolutePath());
		} else {
			log(LogLevel.INFO, "Not removed "+file.getAbsolutePath());
		}
	}

	private void remove(File file, AtomicInteger counter) {
		if (file.isDirectory()) {
			File[] files = file.listFiles();
			for(File sfile : files) {
				remove(sfile, counter);
			}
		}
		if (file.delete()) {
			counter.incrementAndGet();
		}
	}
	
}
