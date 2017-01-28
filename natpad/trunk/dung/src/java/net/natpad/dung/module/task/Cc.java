package net.natpad.dung.module.task;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import net.natpad.dung.StreamHelper;
import net.natpad.dung.module.IDependency;
import net.natpad.dung.module.Module;
import net.natpad.dung.module.dependency.PkgConfig;
import net.natpad.dung.module.dependency.RawDependency;
import net.natpad.dung.module.model.DependenciesDescr;
import net.natpad.dung.module.model.ExportDescr;
import net.natpad.dung.module.model.ExportHeadersDescr;
import net.natpad.dung.module.model.ModuleDescr;
import net.natpad.dung.module.model.PkgConfigDependencyDescr;
import net.natpad.dung.module.task.cc.CcArg;
import net.natpad.dung.module.task.cc.CcCompilerSettings;
import net.natpad.dung.module.task.cc.CcConfigSet;
import net.natpad.dung.module.task.cc.CcIncludePath;
import net.natpad.dung.module.task.cc.CcLibraryName;
import net.natpad.dung.module.task.cc.CcLibraryPath;
import net.natpad.dung.module.task.cc.CcLinkerPath;
import net.natpad.dung.module.task.cc.CcStripArgument;
import net.natpad.dung.module.task.cc.ICcConfigItem;
import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.Context;
import net.natpad.dung.run.Session;
import net.natpad.dung.workspace.Workspace;

public class Cc extends Task {

	private List<FileBundle> srcFileBundles = new ArrayList<FileBundle>();
    protected CcConfigSet compilerConfigSet = new CcConfigSet();

	
	private String destdir;
	private String depdir;

	
	public void addSrc(FileBundle fileBundle) {
		srcFileBundles.add(fileBundle);
	}
	
	

    public void addConfigSet(CcConfigSet configSet) {
    	compilerConfigSet.add(configSet);
    }

    public void addArg(String arg) {
    	compilerConfigSet.add(new CcArg(arg));
    }

    
	public void add(PkgConfig path) {
		compilerConfigSet.add(path);
	}
    
    public void add(CcLinkerPath linkerPath) {
    	compilerConfigSet.add(linkerPath);
    }
    
	public void add(CcLibraryPath path) {
		compilerConfigSet.add(path);
	}

    public void add(CcStripArgument strip) {
    	compilerConfigSet.add(strip);
    }


	public void add(CcLibraryName name) {
		compilerConfigSet.add(name);
	}

	
	@Override
	public void runTask(Session session) throws Exception {

		DependenciesDescr dependencies = session.module.moduleFileDescr.getModuleDescr().dependencies;
		
		HashSet<String> moduleNamesConfigured = new HashSet<String>();
		CcConfigSet ccConfigSet = new CcConfigSet();
		ccConfigSet.add(compilerConfigSet);
		
		
		for(IDependency dep : session.module.dependencies) {
			configure(session.context, dep, ccConfigSet, moduleNamesConfigured);
		}
		
		
    	CcCompilerSettings compilerSettings = new CcCompilerSettings();
    	configureCompiler(session, compilerSettings);
    	
    	ArrayList<ICcConfigItem> flatten = ccConfigSet.flatten(null);
    	for(ICcConfigItem ccitem : flatten) {
    		ccitem.setup(session, compilerSettings);
    	}

		
		ThreadPool threadPool = new ThreadPool();

		Path pathOut = session.createBuildPath(destdir);
		
		File depsDir = null;
		if (depdir!=null) {
			Path pathDeps = session.createBuildPath(depdir);
			depsDir = pathDeps.toFile();
			depsDir.mkdirs();
		}
		
		File destFile = pathOut.toFile();

		
		File moduleDir = session.getModuleDirectory();
		
		PathInPath[] scanBundles = session.scanBundles(moduleDir, srcFileBundles, true);
		
		for(PathInPath pip : scanBundles) {
			if (!pip.isFile) {
				continue;
			}
			String fn = pip.relativePath.toString();
			String on;
			String dn;
			if (fn.endsWith(".c") || fn.endsWith(".C")) {
				String base = fn.substring(0, fn.length()-2);
				on = base+".o";
				dn = base+".d";
			} else if (fn.endsWith(".cpp") || fn.endsWith(".CPP")) {
				String base = fn.substring(0,fn.length()-4);
				on = base+".o";
				dn = base+".d";
			} else {
				continue;
			}
			

			int idx = fn.lastIndexOf(File.separatorChar);
			String ccname = fn;
			if (idx>=0) {
				ccname = fn.substring(idx+1);
			}
//			System.out.println("fn="+fn);
			File dest = new File(destFile, on);
			dest.getParentFile().mkdirs();
			
			
			
			File srcFile = new File(pip.basePath.toFile(), fn);
			

			File dependenciesFile = null;

			if (depsDir!=null) {
				dependenciesFile = new File(depsDir, dn);
			}
			
			if (dest.exists()) {
				boolean objIsNewer = dest.lastModified()>=srcFile.lastModified();
				if (objIsNewer && isObjNewest(session, dependenciesFile, pip.basePath, dest)) {
					continue;
				}
			}
			
			log(LogLevel.INFO, ccname);
			
			compileFile(threadPool, srcFile, dest, compilerSettings, dependenciesFile);			
		}
		
		threadPool.finish();

		if (threadPool.exitcode != 0) {
			throw new RuntimeException("cc compile returned "
					+ threadPool.exitcode);
		}

	}


	private void configureCompiler(Session session, CcCompilerSettings compilerSettings) {
		Workspace workspace = session.getWorkspace();
		String ccPath = workspace.getProperty("ccpath");
		log(LogLevel.VERBOSE, "ccPath="+ccPath);
		if (ccPath!=null) {
			compilerSettings.setCompilerPath(new File(ccPath));
		}
	}


	@SuppressWarnings("resource")
	private boolean isObjNewest(Session session, File dependencyFile, Path pathSrc, File dest) {
		if (dependencyFile==null) {
			return true;
		}
		if (!dependencyFile.exists()) {
			return false;
		}
		long lastModifiedDest = dest.lastModified();
		try {
			LineNumberReader lnr = new LineNumberReader(new FileReader(dependencyFile));
			StringBuilder buf = new StringBuilder();
			while(true) {
				String rl = lnr.readLine();
				if (rl==null) {
					break;
				}
				if (rl.trim().isEmpty()) {
					break;
				}
				if (rl.endsWith("\\")) {
					rl = rl.substring(0, rl.length()-1);
				}
				buf.append(rl);
			}
			
			String depLine = buf.toString();
			int indexOf = depLine.indexOf(':');
			if (indexOf>=0) {
				depLine = depLine.substring(indexOf+1);
			}
			String[] paths = depLine.split(" ");
			for (String path : paths) {
				Path fpath = Paths.get(path);
				Path pp = pathSrc.resolve(fpath);
//				System.out.println("pp:"+pp+", dest="+dest.getName());
				File ffile = pp.toFile();
				if (!ffile.exists()) {
					return false;
				}
				if (ffile.lastModified()>lastModifiedDest) {
					return false;
				}
//				fpath.
			}
			
			
			
		} catch (Exception e) {
			return false;
		}
		
		
		return true;
	}


	private void configure(Context context, IDependency dependency, CcConfigSet ccConfigSet, HashSet<String> moduleNamesConfigured) throws IOException {
//		System.out.println("dependency="+dependency);
//		System.out.println("module.moduleFileDescr="+module.moduleFileDescr);
		if (dependency instanceof Module) {
			Module module = (Module) dependency;
//			System.out.println("module="+module);
			String moduleName = module.moduleFileDescr.getModuleDescr().name;
			if (moduleNamesConfigured.add(moduleName)) {
				
				Session session = new Session(context, module);
				configure(session, ccConfigSet);
				
				for(IDependency other : module.dependencies) {
					configure(context, other, ccConfigSet, moduleNamesConfigured);
				}
//			} else {
//				System.out.println("skipping on "+moduleName);
			}
		} else if (dependency instanceof PkgConfig) {
			PkgConfig pkgConfig = (PkgConfig) dependency;
			ccConfigSet.add(pkgConfig);
		} else if (dependency instanceof ICcConfigItem) {
			ccConfigSet.addItem((ICcConfigItem) dependency);
		}
	}

	
	

    private void configure(Session session, CcConfigSet ccConfigSet) throws IOException {
    	ModuleDescr moduleDescr = session.module.moduleFileDescr.getModuleDescr();
    	
    	ExportDescr export = moduleDescr.export;
    	if (export.headers!=null) {
	    	ExportHeadersDescr headers = export.headers;
	    	if (headers!=null) {
		    	CcIncludePath includePaths = new CcIncludePath();
		    	for(String dir : headers.directories) {
		    		Path cPath = session.createModulePath(dir);
		    		includePaths.addPath(cPath.toString());
		    	}
		    	ccConfigSet.add(includePaths);
	    	}
    	}
	}




	private void configure(CcConfigSet ccConfigSet, PkgConfigDependencyDescr descr) {
   		PkgConfig pkgConfig = new PkgConfig();
		pkgConfig.setpackage(descr.packageName);
		ccConfigSet.add(pkgConfig);
	}




	private void compileFile(ThreadPool threadPool, File source, File dest, CcCompilerSettings compilerSettings, File dependencies) {
//        log("source="+source.getAbsolutePath(), Project.MSG_VERBOSE);
		
		dest.getParentFile().mkdirs();

		List<String> argList = new ArrayList<String>();
		argList.addAll(compilerSettings.getExecAsArgList());
		
		argList.add(source.getAbsolutePath());
		argList.add("-o");
		argList.add(dest.getAbsolutePath());
		if (dependencies!=null) {
			argList.add("-MP");
			argList.add("-MMD");
			argList.add("-MF");
			dependencies.getParentFile().mkdirs();
			argList.add(dependencies.getAbsolutePath());
		}
		
		String[] args = argList.toArray(new String[argList.size()]);


		if (isEnabled(LogLevel.DEBUG)) {
			for(String opt : args) {
				log(LogLevel.DEBUG, "arg "+opt);
			}
		}

		StreamHelper streamHelper = (StreamHelper) System.out;
		String prefix = streamHelper.getPrefix();
		
		threadPool.postAction(new ExecAction(prefix, args));
    }    
    
	
	@Override
	public String toString() {
		return "Cc [sourceFolders=" + srcFileBundles + "]";
	}

	
	
	

    static class ActionHandler implements Runnable {
    	
    	Object notifyme;
    	ExecAction action;
    	boolean stopit = false;
    	int exitCode;
    	
    	public ActionHandler(Object notifyme) {
    		this.notifyme = notifyme;
    		exitCode = 0;
    	}
    	
    	public synchronized void postAction(ExecAction action) {
    		this.action = action;
    		notifyAll();
    	}
    	
    	public void run() {
    		ExecAction torun = null;
    		boolean donotify = false;
    		while(!stopit) {
    			synchronized(this) {
					if (action!=null) {
						torun = action;
					} else { 
						try {
							wait(2000);
						} catch (InterruptedException e) {
						}
    				}
    			}
    			if (torun!=null) {
   					exitCode = torun.runAction();
    				torun = null;
	    			synchronized(this) {
						action = null;
						donotify = true;
	    			}
				}
    			
    			if (donotify) {
    				synchronized (notifyme) {
    					notifyme.notifyAll();
					}
    			}
    		}
    	}
    }
        

    static class ThreadPool {

    	ArrayList<ExecAction> actionQueue = new ArrayList<ExecAction>();
    	ActionHandler handlers[];
    	int exitcode;
    	
    	public ThreadPool() {
    		Runtime runtime = Runtime.getRuntime();
            int nrOfProcessors = runtime.availableProcessors();

            handlers = new ActionHandler[nrOfProcessors+2];
//            handlers = new ActionHandler[1];
            
    		for(int idx=0; idx<handlers.length; idx++) {
    			handlers[idx] = new ActionHandler(this);
    			new Thread(handlers[idx]).start();
    		}
		}
    	
    	
    	public void postAction(ExecAction action) {
    		while(true) {
    			synchronized (this) {
    				for(int idx=0; idx<handlers.length; idx++) {
    					synchronized (handlers[idx]) {
    						if (handlers[idx].exitCode!=0) {
    							exitcode = handlers[idx].exitCode;
    							return;
    						}
							if (handlers[idx].action == null) {
								handlers[idx].postAction(action);
								return;
							}
						} 
    				}
    				try {
						this.wait(5000);
					} catch (InterruptedException e) {
					}
				}
    		}
    	}
    	
    	
    	public void finish() {
    		boolean keep_alive = true;
    		while(keep_alive) {
    			synchronized(this) {
    				keep_alive = false;
    				for(int idx=0; idx<handlers.length; idx++) {
    					synchronized (handlers[idx]) {
    						if (handlers[idx].exitCode!=0) {
    							exitcode = handlers[idx].exitCode;
    						}
    						
							if (handlers[idx].action != null) {
								keep_alive = true;
							} else {
								handlers[idx].stopit = true;
							}
						} 
    				}
    				if (keep_alive) {
	    				try {
							this.wait(5000);
						} catch (InterruptedException e) {
						}
    				}
				}
    		}
    	}
    	
    }
	
}
