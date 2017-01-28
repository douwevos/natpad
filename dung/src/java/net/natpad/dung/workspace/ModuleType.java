package net.natpad.dung.workspace;

public enum ModuleType {

	PKG_CONFIG(ModulePkgConfig.class),
	IGNORE(ModuleIgnore.class),
	RAW(ModuleRaw.class);
	
	public final Class<? extends Module> moduleClass;
	
	private ModuleType(Class<? extends Module> moduleClass) {
		this.moduleClass = moduleClass;
	}
	
}
