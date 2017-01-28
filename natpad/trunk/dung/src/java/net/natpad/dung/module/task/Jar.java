package net.natpad.dung.module.task;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.Manifest;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.BashHelper;
import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;

/**
 * Creates an JAR archive.
 */
public class Jar extends Task {
	
	private String destfile;
	private List<FileBundle> fileBundles = new ArrayList<FileBundle>();
	
	String manifestFile;
	
	Manifest manifest;
	
    public void addIn(FileBundle bundle) {
    	fileBundles.add(bundle);
    }
    
    @Override
    public void runTask(Session session) throws Exception {
		List<String> argList = new ArrayList<String>();

		argList.add("/usr/bin/jar");
		StringBuilder buf = new StringBuilder();
		Path destPath = session.createBuildPath(destfile);
		File destFilet = destPath.toFile();
		if (destFilet.exists()) {
			boolean delete = destFilet.delete();
			if (delete) {
				log(LogLevel.INFO, "deleted jar:"+destPath);
			}
			destFilet = null;
			destFilet = destPath.toFile();
		}
		
		
		buf.append("cf");
		if (manifestFile!=null) {
			buf.append("m");
		}
		argList.add(buf.toString());
		
		argList.add(destPath.toString());

		File destFile = destFilet;
		Long tsDest = null;
		if (destFile.exists()) {
			tsDest = destFile.lastModified();
		}

		if (manifestFile!=null) {
			Path mainfestPath = session.createModulePath(manifestFile);
			argList.add(mainfestPath.toString());
		}

		
//		Path srcPath = session.createBuildPath(srcdir);

		File moduleBuildDir = session.module.buildDir;
		
		
//		System.out.println("srcPath="+srcPath);
		
		boolean doRun = tsDest==null;
		
		int fileCount = 0;
		PathInPath[] scanBundles = session.scanBundles(moduleBuildDir, fileBundles, true);
		String lastDir = null;
		for (PathInPath pathInPath : scanBundles) {
			if (!pathInPath.isFile) {
				continue;
			}
			
//			System.out.println("pathInPath::"+pathInPath);
			if (!pathInPath.basePath.toString().equals(lastDir)) {
				lastDir = pathInPath.basePath.toString();
			}
			argList.add("-C");
			argList.add(lastDir);
//			File sourceFile = new File(file);
//			String fullpath = sourceFile.getAbsolutePath();         	
			fileCount++;	
////			log("adding file:"+fullpath, Project.MSG_VERBOSE);
			argList.add(pathInPath.relativePath.toString());
//
//			if (!doRun) {
//				if (sourceFile.exists()) {
//					doRun = tsDest<sourceFile.lastModified();
//				}
//			}
		}

		
		
		if (!doRun) {
			return;
		}
		
		
		log(LogLevel.INFO, "creating jar and adding "+fileCount+" files.");
		
		Runtime runtime = Runtime.getRuntime();

		
		
		String[] args = argList.toArray(new String[argList.size()]);
//		for (String string : args) {
//			System.out.print(" "+string);
//		}
		try {
			Process process = runtime.exec(args);
			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
			Thread thread1 = errstreamer.launch();
			ProcessInputStreamer outstreamer = new ProcessInputStreamer(process.getInputStream());
			Thread thread2 = outstreamer.launch();
			int waitFor = process.waitFor();
			thread1.join();
			thread2.join();
			for(String line : outstreamer.output) {
				BashHelper.printLine(line);
//				System.err.println(line);
			}
			for(String line : errstreamer.output) {
				BashHelper.printLine(line);
			}
			if (waitFor!=0) {
				throw new RuntimeException("could not create jar:"+destfile);
			}
			int cnt = 5;
			while(!destFile.exists()) {
				Thread.sleep(200);
				if (cnt-->0) {
					System.out.print(".");
				}
			}
			if (cnt!=5) {
				System.out.println();
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	
//		log("created "+destfile.getName()+" added "+fileCount+" file(s)");
	}

	
}
