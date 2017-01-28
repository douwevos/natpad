package net.natpad.dung.run;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

import net.natpad.dung.module.DependencyTree;
import net.natpad.dung.module.Module;
import net.natpad.dung.module.ModuleLoader;
import net.natpad.dung.module.dependency.PkgConfig;
import net.natpad.dung.module.dependency.RawDependency;
import net.natpad.dung.module.model.DependencyDescr;
import net.natpad.dung.module.model.ModuleDependencyDescr;
import net.natpad.dung.module.model.ModuleDescr;
import net.natpad.dung.module.model.PkgConfigDependencyDescr;
import net.natpad.dung.module.task.LogLevel;
import net.natpad.dung.workspace.ModuleIgnore;
import net.natpad.dung.workspace.ModulePkgConfig;
import net.natpad.dung.workspace.ModuleRaw;
import net.natpad.dung.workspace.Workspace;

public class Context {

	public final Workspace workspace;
	public final String rootModuleName;
	public final File workspaceWorkDir;
	private ModuleLoader moduleLoader;

	
	private LogLevel logLevel;
	
	public Module rootModule;

	public ModuleMap moduleMap = new ModuleMap();
	public HashMap<String, PkgConfig> pkgConfigMap = new HashMap<String, PkgConfig>(); 
	
	public Context(Workspace workspace, String moduleName, File workspaceWorkDir) throws IOException {
		this.workspace = workspace;
		moduleLoader = new ModuleLoader(workspace);
		rootModuleName = moduleName;
		this.workspaceWorkDir = workspaceWorkDir;
		
	}

	public void setLogLevel(LogLevel logLevel) {
		this.logLevel = logLevel;
	}
	
	public LogLevel getLogLevel() {
		return logLevel;
	}
	
	public void build(List<String> targetsToRun, boolean reverse, boolean nodeps) throws Exception {
		if (targetsToRun.isEmpty()) {
			targetsToRun.add("compile");
			targetsToRun.add("package");
		}
		rootModule = loadModule(rootModuleName, false);
		if (rootModule==null) {
			throw new RuntimeException("could not load main module:"+rootModuleName);
		}

		
		int maxLevel = resolveDependencyTree();

		if (nodeps) {
			Session session = new Session(this, rootModule);
			session.build(targetsToRun);
			return;
		}
		
		new DependencyTree(rootModule);
		for(int idx=0; idx<=maxLevel; idx++) {
//		for(int level=maxLevel; level>=0; level--) {
			int level = reverse ? idx : maxLevel-idx;
			
			List<Module> modulesOnLevel = collectModulesWithLevel(level);
			
			System.out.println();
			System.out.println((char) 27 +"[1;35m" + "building "+modulesOnLevel.size()+" module"+(modulesOnLevel.size()>1 ? "s" : "")+" with level:"+level+ (char) 27 +"[0m");
			System.out.println();
			for (Module module : modulesOnLevel) {
				Session session = new Session(this, module);
				session.build(targetsToRun);
			}
		}
	}
	
	private List<Module> collectModulesWithLevel(int level) {
		ArrayList<Module> result = new ArrayList<Module>();
		
		for(Module module : moduleMap.values()) {
			if (module.getLevel()==level) {
				result.add(module);
			}
		}
		
		return result;
	}

	
	private int resolveDependencyTree() {
		ArrayList<Module> work = new ArrayList<Module>();
		moduleMap.add(rootModuleName, rootModule);
		work.add(rootModule);
		while(!work.isEmpty()) {
			Module module = work.remove(work.size()-1);
			
			ModuleDescr moduleDescr = module.moduleFileDescr.getModuleDescr();
//			System.out.println("handling:"+name);
			List<DependencyDescr> plainDependecies = moduleDescr.getPlainDependecies();
			if (plainDependecies!=null) {
				for(DependencyDescr depDesc : plainDependecies) {
					if (depDesc instanceof ModuleDependencyDescr) {
						ModuleDependencyDescr modDepDesc = (ModuleDependencyDescr) depDesc;
						net.natpad.dung.workspace.Module wsModule = workspace.modules.get(modDepDesc.moduleName);
						if (wsModule !=null) {
							if (wsModule instanceof ModulePkgConfig) {
								ModulePkgConfig mph = (ModulePkgConfig) wsModule;
								String packageName = mph.pkgName;
								PkgConfig pkgConfig = pkgConfigMap.get(packageName);
								if (pkgConfig==null) {
									pkgConfig = new PkgConfig();
									pkgConfig.setpackage(packageName);
									pkgConfigMap.put(packageName, pkgConfig);
								}
								module.addDependency(pkgConfig);
							} else if (wsModule instanceof ModuleIgnore) {
								/* yes we do really ignore this one */
							} else if (wsModule instanceof ModuleRaw) {
								ModuleRaw moduleRaw = (ModuleRaw) wsModule;
								module.addDependency(new RawDependency(moduleRaw));
							} else {
								throw new RuntimeException("type of workspace module unknown:"+wsModule);
							}
						} else {
							Module childModule = moduleMap.get(modDepDesc.moduleName);
							if (childModule==null) {
								childModule = loadModule(modDepDesc.moduleName, false);
								if (childModule==null) {
									throw new RuntimeException("could not load module "+modDepDesc.moduleName+" referenced by "+moduleDescr.name);
								}
								moduleMap.add(modDepDesc.moduleName, childModule);
								work.add(childModule);
							}
							module.addDependency(childModule);
						}
	//					System.out.println("adding child:"+modDepDesc.moduleName);
					} else if (depDesc instanceof PkgConfigDependencyDescr) {
						String packageName = ((PkgConfigDependencyDescr) depDesc).packageName;
						PkgConfig pkgConfig = pkgConfigMap.get(packageName);
						if (pkgConfig==null) {
							pkgConfig = new PkgConfig();
							pkgConfig.setpackage(packageName);
							pkgConfigMap.put(packageName, pkgConfig);
						}
						module.addDependency(pkgConfig);
					}
	
				}
			}
		}
		
		int maxLevel = rootModule.setLevel(0, new HashSet<Module>());
		System.out.println("max module depth="+maxLevel);
		return maxLevel;
	}

	
	private Module loadModule(String moduleName, boolean full) {
		int dotidx = moduleName.lastIndexOf('#');
		String fileName = "build.dun";
		if (dotidx>0) {
			fileName = moduleName.substring(dotidx+1)+".dun";
			moduleName = moduleName.substring(0, dotidx);
		}
		
		File cwd = new File(".");
		File moduleDir = new File(cwd, moduleName);
		if (moduleDir!=null && moduleDir.exists()) {
			File buildFile = new File(moduleDir, fileName);
			if (buildFile.exists()) {
				try {
					return moduleLoader.loadModule(moduleDir, buildFile);
				} catch (Exception e) {
					e.printStackTrace();
					return null;
				}
			} 
		}
		Path []paths = workspace.getModulePath();
		for (Path path : paths) {
			File wsDir = path.toFile();
//			System.out.println("testing path:"+path);
			moduleDir = new File(wsDir, moduleName);
			if (moduleDir!=null && moduleDir.exists()) {
				File buildFile = new File(moduleDir, fileName);
//				ModuleLoader moduleLoader = new ModuleLoader();
//				return moduleLoader.loadModule(wsDir, buildFile, full);

				try {
					return moduleLoader.loadModule(moduleDir, buildFile);
				} catch (Exception e) {
					e.printStackTrace();
					return null;
				}

				
			
			}
		}
		return null;
	}

	
	
	public String[] scanDirectory(File directory, boolean relative) {
		List<String> result = new ArrayList<String>();
		Path basePath = Paths.get(directory.getAbsolutePath());
		scanDirectoryRecursive(basePath, directory, result, relative);
		String r[] = new String[result.size()];
		r = result.toArray(r);
		return r;
	}

	private void scanDirectoryRecursive(Path basePath, File dir2scan, List<String> output, boolean relative) {
		File[] listFiles = dir2scan.listFiles();
		if (listFiles!=null) {
			for (File file : listFiles) {
				if (file.isDirectory()) {
					scanDirectoryRecursive(basePath, file, output, relative);
				} else {
					String absolutePath = file.getAbsolutePath();
					if (relative) {
						Path relativized = basePath.relativize(Paths.get(absolutePath));
						absolutePath = relativized.toString();
					}
					output.add(absolutePath);
				}
			}
		}
	}

	public String readFile(File file) throws Exception {
		FileInputStream inStream = new FileInputStream(file);
		byte data[] = new byte[(int) file.length()];
		int cnt = inStream.read(data);
		inStream.close();
		return new String(data, 0, cnt);
	}

	
	
}
