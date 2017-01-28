package net.natpad.dung.module.task;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;
import net.natpad.dung.workspace.Workspace;


/**
 * Creates an AR archive.
 */
public class Ar extends Task {
	
	private String srcdir;
	private String destfile;
	
    
    @Override
    public void runTask(Session session) throws Exception {
		List<String> argList = new ArrayList<String>();

		String arPath = detectArPath(session);
		File ap = new File(arPath);
		
		argList.add(ap.getAbsolutePath());
		argList.add("-r");
		
		
		Path destPath = session.createBuildPath(destfile);
		argList.add(destPath.toString());

		File destFile = destPath.toFile();
		Long tsDest = null;
		if (destFile.exists()) {
			tsDest = destFile.lastModified();
		}

		Path srcPath = session.createBuildPath(srcdir);
		
//		System.out.println("srcPath="+srcPath);
		
		boolean doRun = tsDest==null;
		
		int fileCount = 0;
		String[] files = session.context.scanDirectory(srcPath.toFile(), false);
		for (String file : files) {
			File sourceFile = new File(file);
			String fullpath = sourceFile.getAbsolutePath();         	
			fileCount++;	
//			log("adding file:"+fullpath, Project.MSG_VERBOSE);
			argList.add(fullpath);

			if (!doRun) {
				if (sourceFile.exists()) {
					doRun = tsDest<sourceFile.lastModified();
				}
			}
		}

		
		if (!doRun) {
			return;
		}
		
		
		log(LogLevel.INFO, "creating "+destPath.getFileName().toString()+" and adding "+fileCount+" files.");
		
		Runtime runtime = Runtime.getRuntime();

		
		
		String[] args = argList.toArray(new String[argList.size()]);
//		for (String string : args) {
//			System.out.println("str:"+string);
//		}
		try {
			Process process = runtime.exec(args);
			new ProcessInputStreamer(process.getErrorStream()).launch();
			new ProcessInputStreamer(process.getInputStream()).launch();
		} catch (IOException e) {
			e.printStackTrace();
		}
	
//		log("created "+destfile.getName()+" added "+fileCount+" file(s)");
	}



	private String detectArPath(Session session) {
		Workspace workspace = session.getWorkspace();
		String arPath = workspace.getProperty("arpath");
		if (arPath!=null) {
			return arPath;
		}
		return "/usr/bin/ar";
	}

    
}
