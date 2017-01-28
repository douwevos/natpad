package net.natpad.dung.module.model;

import java.util.ArrayList;
import java.util.List;

public class DependenciesDescr {

	private List<DependencyDescr> addedDeps = new ArrayList<DependencyDescr>();

	public void addModule(String moduleDep) {
		addedDeps.add(new ModuleDependencyDescr(moduleDep));
	}

	public void addPkgConfig(String pkgConfigName) {
		addedDeps.add(new PkgConfigDependencyDescr(pkgConfigName));
	}
	
	public List<DependencyDescr> getAll() {
		return addedDeps;
	}

	
	@Override
	public String toString() {
		return "DependenciesDescr [addedDeps=" + addedDeps + "]";
	}

}
