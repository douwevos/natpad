package net.natpad.monkey.model;

import java.util.HashMap;
import java.util.Map;

public class MooEditableContentMap implements MooIContentMap {

	HashMap<String, MooIContent> map = new HashMap<String, MooIContent>();
	
	
	public MooEditableContentMap(Map<String, MooIContent> sourceMap) {
		if (sourceMap!=null) {
			map.putAll(sourceMap);
		}
	}
	
	
	public MooIContent get(String key) {
		return map.get(key);
	}

	
	public void set(MooIContent content) {
		map.put(content.getKey(), content);
	}


	public MooContentMap finalizeResult() {
		return new MooContentMap(map);
	}
	
}
