package net.natpad.dung.module.model;

public class ModuleFileDescr {

	public ModuleDescr module;

	
//	public List<Template> templates = new ArrayList<Template>();
	
	@Override
	public String toString() {
		return "ModuleFileDescr [module=" + module + "]";
	}
	
	
	public ModuleDescr getModuleDescr() {
		return module;
	}


//	public void addTemplate(Template template) {
//		templates.add(template);
//	}
	
}
