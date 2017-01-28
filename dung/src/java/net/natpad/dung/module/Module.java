package net.natpad.dung.module;

import java.io.File;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

import net.natpad.dung.DungClassLoader;
import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.StringLiteral;
import net.natpad.dung.module.model.ModuleFileDescr;
import net.natpad.dung.module.model.TargetDescr;
import net.natpad.dung.run.Context;
import net.natpad.dung.run.Session;
import net.natpad.dung.workspace.Workspace;
import net.natpad.sht.model.Model;

public class Module implements IDependency, IExpressionValue {

	private final DungClassLoader classloader;
	public final File directory;
	public final Model model;
	public final ModuleMapper mapper;
	public final File buildDir;
	public final ExportInfo exportInfo; 

	public ModuleFileDescr moduleFileDescr;
	public boolean fullyMapped;
	private int level;

	public HashMap<String, Target> targetMap = new HashMap<String, Target>();

	public final List<IDependency> dependencies = new ArrayList<IDependency>();
	
	public Module(Workspace workspace, File directory, Model model, DungClassLoader classloader) throws Exception {
		this.directory = directory.getCanonicalFile();
		this.model = model;
		this.classloader = classloader;
		mapper = new ModuleMapper(classloader, workspace);
		moduleFileDescr = mapper.map(model, ModuleFileDescr.class, null);
		exportInfo = moduleFileDescr.module.export==null ? null : new ExportInfo(moduleFileDescr.module.export);
		fullyMapped=false;
		File locDungDir = new File(directory, "dung");
		buildDir = new File(locDungDir, workspace.id).getCanonicalFile();
	}
	
	public void ensureFullyMapped(Context context) throws Exception {
		if (!fullyMapped) {
			/* add libraries to class loader */
//			System.out.println("moduleFileDescr.module.modulesForClassloader="+moduleFileDescr.module.modulesForClassloader);
			for(String clm : moduleFileDescr.module.modulesForClassloader) {
				Module module = context.moduleMap.get(clm);
//				System.out.println("clm="+clm);
				if (module == null) {
					throw new RuntimeException("module can not be added to loader since it does not exist:"+clm);
				}
				List<String> paths = module.moduleFileDescr.module.export.lib.paths;
//				System.out.println("clm.paths="+paths);
				if (paths!=null &&  !paths.isEmpty()) {
					Session session = new Session(context, module);
					for(String path : paths) {
						Path jarpath = session.createBuildPath(path);
						classloader.addJarPath(jarpath);
					}
				}
				
			}
			
			
			mapper.resolveTargets(moduleFileDescr.getModuleDescr());
			fullyMapped = true;
		}
	}
	
	public int setLevel(int level, HashSet<Module> parentSet) {
		if (this.level>level) {
			return level;
		}
		if (!parentSet.add(this)) {
			throw new RuntimeException("Circulair module dependency detected");
		}
		int maxLevel = level;
		this.level = level;
		for(IDependency dependency : dependencies) {
			if (dependency instanceof Module) {
				Module sub = (Module) dependency;
				int subLevel = sub.setLevel(level+1, parentSet);
				if (subLevel>maxLevel) {
					maxLevel = subLevel;
				}
			}
		}
		parentSet.remove(this);
		return maxLevel;
	}
	
	public int getLevel() {
		return level;
	}
	
	public void addDependency(IDependency dependency) {
		dependencies.add(dependency);
	}
	
	
	@Override
	public IDependency[] children() {
		if (dependencies==null || dependencies.isEmpty()) {
			return null;
		}
		IDependency[] result = new IDependency[dependencies.size()];
		return dependencies.toArray(result);
	}
	
	public Target getTarget(String name) {
		Target target = targetMap.get(name);
		if (target==null) {
			TargetDescr targetDescr = moduleFileDescr.getModuleDescr().getTargetDescr(name);
			if (targetDescr!=null) {
				target = new Target(targetDescr);
				targetMap.put(name, target);
			}
		}
		return target;
	}

	@Override
	public IExpressionValue getById(Object id) {
		if (id instanceof String) {
			String st = (String) id;
			if ("builddir".equals(st)) {
				return new StringLiteral(buildDir.toString());
			} else if ("directory".equals(st)) {
				return new StringLiteral(directory.toString());
			} else if ("export".equals(st)) {
				return exportInfo;
			}
		}
		return null;
	}
	
	@Override
	public String description() {
		return "Module[name="+moduleFileDescr.getModuleDescr().name+"]";
	}
	
	@Override
	public String toString() {
		return "Module [directory=" + directory + ", moduleFileDescr=" + moduleFileDescr + ", level=" + level + ", dependencies="+dependencies+"]";
	}

}
