package net.natpad.dung.workspace;

import net.natpad.sht.mapper.MapParent;

@MapParent
public class ModulePkgConfig extends Module {


	public ModulePkgConfig() {
		super.type = ModuleType.PKG_CONFIG;
	}
	
	public String pkgName;

}
