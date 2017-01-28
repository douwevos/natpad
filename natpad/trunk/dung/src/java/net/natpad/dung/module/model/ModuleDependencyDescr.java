package net.natpad.dung.module.model;


public class ModuleDependencyDescr implements DependencyDescr {

	public final String moduleName;

	public ModuleDependencyDescr(String moduleName) {
		this.moduleName = moduleName;
	}

	@Override
	public String toString() {
		return "ModuleDependencyDescr [moduleName=" + moduleName + "]";
	}

}
