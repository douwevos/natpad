package net.natpad.monkey.model;

import java.util.HashMap;

import net.natpad.caterpillar.CatLogger;
import net.natpad.sht.model.Model;

public class ModuleContext {

	protected Model model;
	protected HashMap<String, Object> subContextMap;
	
	
	public ModuleContext() {
		subContextMap = new HashMap<String, Object>();
		model = null;
	}

	private ModuleContext(HashMap<String, Object> configurations) {
		this.subContextMap = configurations;
		model = null;
	}

	
	private ModuleContext cloneContext() {
		ModuleContext result = new ModuleContext(subContextMap);
		result.model = model;
		return result;
	}

	
	public Object getSubContext(String key) {
		return subContextMap.get(key);
	}
	
	
	
	public Model getModel() {
		return model;
	}


	public ModuleContext setModel(Model parse) {
		if (CatLogger.safeEquals(parse, model)) {
			return this;
		}
		ModuleContext result = cloneContext();
		result.model = parse;
		return result;
		
	}


	
	
	
	
}
