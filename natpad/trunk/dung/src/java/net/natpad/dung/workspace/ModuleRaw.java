package net.natpad.dung.workspace;

import net.natpad.sht.mapper.MapParent;

@MapParent
public class ModuleRaw extends Module {

	public ModuleRaw() {
		super.type = ModuleType.RAW;
	}
	
	public String libPath;
	public String libName;
	public String headerPath;

}
