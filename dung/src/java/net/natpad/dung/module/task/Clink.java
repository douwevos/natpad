package net.natpad.dung.module.task;

import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import net.natpad.dung.module.DependencyTree;
import net.natpad.dung.module.IDependency;
import net.natpad.dung.module.Module;
import net.natpad.dung.module.dependency.PkgConfig;
import net.natpad.dung.module.model.DependenciesDescr;
import net.natpad.dung.module.model.ExportDescr;
import net.natpad.dung.module.model.ExportLibraryDescr;
import net.natpad.dung.module.model.ModuleDescr;
import net.natpad.dung.module.task.cc.CcArg;
import net.natpad.dung.module.task.cc.CcConfigSet;
import net.natpad.dung.module.task.cc.CcLibraryName;
import net.natpad.dung.module.task.cc.CcLibraryPath;
import net.natpad.dung.module.task.cc.CcLinkerPath;
import net.natpad.dung.module.task.cc.CcLinkerSettings;
import net.natpad.dung.module.task.cc.CcStripArgument;
import net.natpad.dung.module.task.cc.ICcConfigItem;
import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.Context;
import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;
import net.natpad.dung.workspace.Workspace;

public class Clink extends Task {

	private List<FileBundle> srcFileBundles = new ArrayList<FileBundle>();
	private String output;
    protected CcConfigSet linkerConfigSet = new CcConfigSet();


	public void addSrc(FileBundle fileBundle) {
		srcFileBundles.add(fileBundle);
	}
	
    public void addConfigSet(CcConfigSet configSet) {
    	linkerConfigSet.add(configSet);
    }

    public void addArg(String arg) {
    	linkerConfigSet.add(new CcArg(arg));
    }

    
	public void add(PkgConfig path) {
		linkerConfigSet.add(path);
	}
    
    public void add(CcLinkerPath linkerPath) {
    	linkerConfigSet.add(linkerPath);
    }
    
	public void add(CcLibraryPath path) {
		linkerConfigSet.add(path);
	}

    public void add(CcStripArgument strip) {
    	linkerConfigSet.add(strip);
    }


	public void add(CcLibraryName name) {
		linkerConfigSet.add(name);
	}


	
	@Override
	public void runTask(Session session) throws Exception {

		DependenciesDescr dependencies = session.module.moduleFileDescr.getModuleDescr().dependencies;
		
		HashSet<String> moduleNamesConfigured = new HashSet<String>();
		CcConfigSet ccConfigSet = new CcConfigSet();
		
		
		DependencyTree dependencyTree = new DependencyTree(session.module);
		for(int level=0; level<dependencyTree.levelCount(); level++) {
			IDependency[] depsAtLevel = dependencyTree.getAtLevel(level);
			for(IDependency descr : depsAtLevel) {
				log(LogLevel.VERBOSE, "configuring:"+descr.description());
				if (descr instanceof Module) {
					Module module = (Module) descr;
					configure(session.context, module, ccConfigSet, moduleNamesConfigured);
				} else if (descr instanceof PkgConfig) {
					PkgConfig pkgConfig = (PkgConfig) descr;
					configure(ccConfigSet, (PkgConfig) descr);
				} else if (descr instanceof ICcConfigItem) {
					ccConfigSet.addItem((ICcConfigItem) descr);
				}
//				if (descr instanceof PkgConfigDependencyDescr) {
//					configure(ccConfigSet, (PkgConfigDependencyDescr) descr);
//				} else if (descr instanceof ModuleDependencyDescr) {
//					String moduleName = ((ModuleDependencyDescr) descr).moduleName;
////				System.out.println("moduleName="+moduleName);
//					Module module = session.context.moduleMap.get(moduleName);
//					configure(session.context, module, ccConfigSet, moduleNamesConfigured);
//				}
			}
		}
		
		CcLinkerSettings linkerSettings = new CcLinkerSettings();
		configureLinker(session, linkerSettings);
		
		ArrayList<ICcConfigItem> flatten = ccConfigSet.flatten(null);
		for (ICcConfigItem iCcConfigItem : flatten) {
			iCcConfigItem.setup(session, linkerSettings);
		}
		
		
		File moduleBuildDir = session.module.buildDir;

		PathInPath[] scanBundles = session.scanBundles(moduleBuildDir, srcFileBundles, false);

		log(LogLevel.VERBOSE, "## scanBundles="+scanBundles.length);
		for (PathInPath pathInPath : scanBundles) {
			log(LogLevel.VERBOSE, "## pathInPath="+pathInPath);
			if (!pathInPath.isFile) {
				continue;
			}
			String relativePath = pathInPath.relativePath.toString();
			if (relativePath.endsWith(".o") || relativePath.endsWith(".res")) {
				linkerSettings.addObjectFile(relativePath);
			}
		}

		link(session, linkerSettings);
	}

	private void configureLinker(Session session, CcLinkerSettings linkerSettings) {
		Workspace workspace = session.getWorkspace();
		String cLinkerPath = workspace.getProperty("clinkerpath");
		log(LogLevel.DEBUG, "cLinkerPath="+cLinkerPath);
		if (cLinkerPath!=null) {
			linkerSettings.setLinkerPath(new File(cLinkerPath));
		}
	}


	
	private void link(Session session, CcLinkerSettings linkerSettings) {
    	ArrayList<ICcConfigItem> flatten = linkerConfigSet.flatten(null);
    	
    	
    	for(ICcConfigItem ccitem : flatten) {
    		ccitem.setup(session, linkerSettings);
    	}
    	
    	ArrayList<String> argList = linkerSettings.getExecAsArgList();
    	
    	
    	
    	Path outPath = session.createBuildPath(output);
    	
    	argList.add("-o"+outPath);
    	

		String[] optflat = argList.toArray(new String[argList.size()]);
		
		if (isEnabled(LogLevel.DEBUG)) {
			StringBuilder b = new StringBuilder();
			for(String opt : optflat) {
				b.append(" "+opt);
			}
			log(LogLevel.DEBUG, "command :: "+b);
		} else {
			log(LogLevel.INFO, "link "+outPath);
		}

		
		
		
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(optflat);
			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
			errstreamer.launch();
			ProcessInputStreamer outstreamer = new ProcessInputStreamer(process.getInputStream());
			outstreamer.launch();
			
			int exitcode = process.waitFor();
			if (exitcode!=0) {
				for(String line : outstreamer.output) {
					System.err.println(line);
				}
				for(String line : errstreamer.output) {
					System.err.println(line);
				}
				System.err.println("Dumping arguments");
				for(int ia=0; ia<optflat.length; ia++) {
					System.err.println(" ["+ia+"]="+optflat[ia]);
				}
				
				throw new RuntimeException("cc compile returned "+exitcode);
				
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}    	
		
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
				
//				for(IDependency other : module.dependencies) {
//					configure(context, other, ccConfigSet, moduleNamesConfigured);
//				}
//			} else {
//				System.out.println("skipping on "+moduleName);
			}
		} else if (dependency instanceof PkgConfig) {
			PkgConfig pkgConfig = (PkgConfig) dependency;
			ccConfigSet.add(pkgConfig);
		}
	}

	
	

    private void configure(Session session, CcConfigSet ccConfigSet) throws IOException {
    	ModuleDescr moduleDescr = session.module.moduleFileDescr.getModuleDescr();
    	
    	
    	
    	ExportDescr export = moduleDescr.export;
    	if (export.headers!=null) {
	    	ExportLibraryDescr libDesc = export.lib;
	    	if (libDesc!=null) {
	    		for(String libName : libDesc.libnames) {
					ccConfigSet.add(new CcLibraryName(libName));
	    		}
	    		for(String path : libDesc.paths) {
	    			Path libPath = session.createBuildPath(path);
	    			CcLibraryPath ccLibPath = new CcLibraryPath(libPath.toString());
					ccConfigSet.add(ccLibPath);
	    		}
	    	}
    	}
	}


	private void configure(CcConfigSet ccConfigSet, PkgConfig pkgConfig) {
		ccConfigSet.add(pkgConfig);
	}
	
}


/*
/usr/bin/gcc 
/home/superman/cpp-workspace/trunk/natpad.ide/natpad/build/main/natpad-app.o
/home/superman/cpp-workspace/trunk/natpad.ide/natpad/build/main/natpad-window.o
/home/superman/cpp-workspace/trunk/natpad.ide/natpad/build/main/natpad.o
-L/home/superman/cpp-workspace/trunk/natpad.ide/natpad/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/jordanella/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/mule/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/tern/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/jaguar/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/jaguar-decompiler/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/jaguar-bytecode/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/cheetah/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/elk/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/moose-explorer/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/moose/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/viper/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/dragonfly/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/leafhopper/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/cow/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/worm/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/shoveler/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/chameleon/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/caterpillar/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/codeparsing/build
-L/home/superman/cpp-workspace/trunk/natpad.ide/gvlib/build
-lnatpad
-ljordanella
-lmule
-ltern
-ljaguar
-lz
-ljaguar-decompiler
-ljaguar-bytecode
-lcheetah
-lelk
-lmoose-explorer
-lmoose
-lviper
-ldragonfly
-lleafhopper
-lcow
-lworm
-lshoveler
-lchameleon
-lcaterpillar
-lm
-lcodeparsing
-lgvlib
-lgtk-3
-latk-1.0
-lgio-2.0
-lgdk-3
-lpangocairo-1.0
-lgdk_pixbuf-2.0
-lcairo-gobject
-lpango-1.0
-lcairo
-lgobject-2.0
-lglib-2.0
-lX11
-o/home/superman/cpp-workspace/trunk/natpad.ide/natpad/build/natpad
 */




/*

 
/usr/bin/x86_64-w64-mingw32-gcc
/home/superman/work/trunk/natpad.ide/natpad/build/main/natpad-app.o
/home/superman/work/trunk/natpad.ide/natpad/build/main/natpad-window.o
/home/superman/work/trunk/natpad.ide/natpad/build/main/natpad.o
-L/home/superman/work/trunk/natpad.ide/natpad/build
-L/home/superman/work/trunk/natpad.ide/jordanella/build
-L/home/superman/work/trunk/natpad.ide/mule/build
-L/home/superman/work/trunk/natpad.ide/tern/build
-L/home/superman/work/trunk/natpad.ide/jaguar/build
-L/home/superman/work/trunk/natpad.ide/jaguar-decompiler/build
-L/home/superman/work/trunk/natpad.ide/jaguar-bytecode/build
-L/home/superman/work/trunk/natpad.ide/cheetah/build
-L/home/superman/work/trunk/natpad.ide/elk/build
-L/home/superman/work/trunk/natpad.ide/moose-explorer/build
-L/home/superman/work/trunk/natpad.ide/moose/build
-L/home/superman/work/trunk/natpad.ide/viper/build
-L/home/superman/work/trunk/natpad.ide/dragonfly/build
-L/home/superman/work/trunk/natpad.ide/leafhopper/build
-L/home/superman/work/trunk/natpad.ide/cow/build
-L/home/superman/work/trunk/natpad.ide/worm/build
-L/home/superman/work/trunk/natpad.ide/shoveler/build
-L/home/superman/work/trunk/natpad.ide/chameleon/build
-L/home/superman/work/trunk/natpad.ide/caterpillar/build
-L/home/superman/work/trunk/natpad.ide/codeparsing/build
-L/home/superman/work/trunk/natpad.ide/gvlib/build
-L/home/superman/work/trunk/natpad.ide/build/dll/devel/usr/x86_64-w64-mingw32/sys-root/mingw/lib
-lnatpad
-ljordanella
-lmule
-ltern
-ljaguar
-lz
-ljaguar-decompiler
-ljaguar-bytecode
-lcheetah
-lelk
-lmoose-explorer
-lmoose
-lviper
-ldragonfly
-lleafhopper
-lcow
-lworm
-lshoveler
-lchameleon
-lcaterpillar
-lm
-lcodeparsing
-lgvlib
-lgtk-3
-latk-1.0
-lgio-2.0
-lgdk-3
-lpangowin32-1.0
-lpangocairo-1.0
-lcairo-gobject
-lpango-1.0
-lcairo
-lgdk_pixbuf-2.0
-lgobject-2.0
-lglib-2.0
-lintl -mwindows -gstabs -mms-bitfields
-luserenv /home/superman/work/trunk/natpad.ide/build/natpad.res -o/home/superman/work/trunk/natpad.ide/natpad/build/natpad
 

 * */
