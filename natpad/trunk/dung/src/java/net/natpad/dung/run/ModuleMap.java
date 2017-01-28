package net.natpad.dung.run;

import java.util.Collection;
import java.util.HashMap;

import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.StringLiteral;
import net.natpad.dung.module.Module;

public class ModuleMap implements IExpressionValue {

	public HashMap<String, Module> moduleMap = new HashMap<String, Module>();

	public Collection<Module> values() {
		return moduleMap.values();
	}

	public void add(String moduleName, Module module) {
		moduleMap.put(moduleName, module);
		
	}

	public Module get(String moduleName) {
		return moduleMap.get(moduleName);
	} 
	
	@Override
	public IExpressionValue getById(Object id) {
		if (id instanceof StringLiteral) {
			return moduleMap.get(((StringLiteral) id).text);
		}
		return null;
	}
}
