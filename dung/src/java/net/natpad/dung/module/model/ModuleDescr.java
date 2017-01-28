package net.natpad.dung.module.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import net.natpad.dung.eprops.EPropsContext;

public class ModuleDescr {

	public String name;

	
	public DependenciesDescr dependencies;
	public ExportDescr export;

	
	private HashMap<String, TargetDescr> targetMap = new HashMap<>();

	public List<String> modulesForClassloader = new ArrayList<String>();
	
	public final EPropsContext propsContext;
	
	public ModuleDescr() {
		propsContext = new EPropsContext(null);
	}
	
	public void addLoader(String moduleName) {
		modulesForClassloader.add(moduleName);
	}
	
	public void addTarget(String id, TargetDescr target) {
//		System.out.println("adding target:"+id+", target="+target);
		targetMap.put(id, target);
	}

	
	public TargetDescr getTargetDescr(String id) {
		return targetMap.get(id);
	}

	@Override
	public String toString() {
		return "ModuleDescr [name=" + name + ", dependencies="+dependencies+"]";
	}


	public void setProperty(String id, String value) {
		propsContext.put(id, value);
	}

	public List<DependencyDescr> getPlainDependecies() {
		if (dependencies!=null) {
			return dependencies.getAll();
		}
		return null;
	}

}
