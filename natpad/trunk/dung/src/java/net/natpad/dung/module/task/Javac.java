package net.natpad.dung.module.task;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import net.natpad.dung.StreamHelper;
import net.natpad.dung.module.IDependency;
import net.natpad.dung.module.Module;
import net.natpad.dung.module.model.ExportDescr;
import net.natpad.dung.module.model.ExportLibraryDescr;
import net.natpad.dung.module.model.ModuleDescr;
import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.Context;
import net.natpad.dung.run.Session;

public class Javac extends Task {

	private List<FileBundle> srcFileBundles = new ArrayList<FileBundle>();

	private String destdir;


	public void addSrc(FileBundle fileBundle) {
		srcFileBundles.add(fileBundle);
	}


	@Override
	public void runTask(Session session) throws Exception {

		
		HashSet<String> moduleNamesConfigured = new HashSet<String>();
		ArrayList<String> cp = new ArrayList<String>();

		for(IDependency dep : session.module.dependencies) {
			configure(session.context, dep, cp, moduleNamesConfigured);
		}

		Path pathOut = session.createBuildPath(destdir);
		File destDir = pathOut.toFile();

		
//		System.out.println("folder="+path);
		
		
		

		File moduleDir = session.getModuleDirectory();
		PathInPath[] scanBundles = session.scanBundles(moduleDir, srcFileBundles, true);

		StringBuilder sourcePath = new StringBuilder();
		HashSet<String> sourcePathSet = new HashSet<String>();
		
		boolean needsRebuild = false;
		List<String> javaFiles = new ArrayList<String>();
		for(PathInPath pip : scanBundles) {

			if (!pip.isFile) {
				continue;
			}
			String javaFullSrcName = pip.relativePath.toString();
			if (!javaFullSrcName.endsWith(".java")) {
				continue;
			}
			
			
			String baseTxt = pip.basePath.toString();
			log(LogLevel.VERBOSE, "scanned:"+pip);
			if (sourcePathSet.add(baseTxt)) {
				if (sourcePath.length()>0) {
					sourcePath.append(File.pathSeparator);
				}
				sourcePath.append(baseTxt);
				log(LogLevel.DEBUG, "baseTxt:"+baseTxt);
			}
			
			String javaFullClsName = javaFullSrcName.substring(0, javaFullSrcName.length()-5)+".class";
			
			File actualSrcFile = new File(pip.basePath.toFile(), javaFullSrcName);
			File actualDstFile = new File(destDir, javaFullClsName);
			if (actualDstFile.exists()) {
				if (actualSrcFile.lastModified()>actualDstFile.lastModified()) {
					log(LogLevel.VERBOSE, ""+actualDstFile+" is to old");
					needsRebuild = true;
				}
			} else {
				log(LogLevel.VERBOSE, ""+actualDstFile+" does not exist");
				needsRebuild = true;
			}
			javaFiles.add(actualSrcFile.toString());
		}
		
//		String[] scanDirectory = session.context.scanDirectory(file, false);
//		for (String fn : scanDirectory) {
//			if (!fn.endsWith(".java")) {
//				continue;
//			}
//			javaFiles.add(fn);
//		}
//

		if (!needsRebuild) {
			log(LogLevel.DEBUG, "no changes detected skipping java compilation");
			return;
		}

		List<String> argList = new ArrayList<String>();
		argList.add("/usr/bin/javac");
		
		if (!cp.isEmpty()) {
			argList.add("-cp");
			StringBuilder buf = new StringBuilder();
			for(String cpe : cp) {
				if (buf.length()!=0) {
					buf.append(File.pathSeparatorChar);
				}
				buf.append(cpe);
			}
			argList.add(buf.toString());
		}
		
		argList.add("-sourcepath");
		argList.add(sourcePath.toString());
		if (destDir!=null) {
			destDir.mkdirs();
			argList.add("-d");
			argList.add(destDir.getCanonicalPath());
		}

		argList.addAll(javaFiles);
		
		String[] args = argList.toArray(new String[argList.size()]);
		
		log(LogLevel.INFO, "compiling "+argList.size()+" Java source files.");

		StreamHelper streamHelper = (StreamHelper) System.out;
		String prefix = streamHelper.getPrefix();

		ExecAction execAction = new ExecAction(prefix, args);
		int exitcode = execAction.runAction();
		

		if (exitcode != 0) {
			throw new RuntimeException("javac compile returned "
					+ exitcode);
		}

	}



	private void configure(Context context, IDependency dependency, ArrayList<String> cp, HashSet<String> moduleNamesConfigured) throws IOException {
		if (dependency instanceof Module) {
			Module module = (Module) dependency;
//			System.out.println("module="+module);
			String moduleName = module.moduleFileDescr.getModuleDescr().name;
			if (moduleNamesConfigured.add(moduleName)) {
				
				Session session = new Session(context, module);
				configure(session, cp);
				
				for(IDependency other : module.dependencies) {
					configure(context, other, cp, moduleNamesConfigured);
				}
			}
		}
	}
	
    private void configure(Session session, ArrayList<String> cp) throws IOException {
    	ModuleDescr moduleDescr = session.module.moduleFileDescr.getModuleDescr();
    	
    	ExportDescr export = moduleDescr.export;
    	if (export.lib!=null) {
	    	ExportLibraryDescr lib = export.lib;
	    	for(String libFile : lib.paths) {
	    		Path libFullPath = session.createBuildPath(libFile);
	    		String libFull = libFullPath.toString();
	    		if (!cp.contains(libFull)) {
	    			cp.add(libFull);
	    		}
	    	}
    	}
	}

	
}
