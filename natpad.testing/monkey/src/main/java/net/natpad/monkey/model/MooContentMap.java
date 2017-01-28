package net.natpad.monkey.model;

import java.util.HashMap;
import java.util.Map;

public class MooContentMap implements MooIContentMap {

	Map<String, MooIContent> map = new HashMap<String, MooIContent>();
	
	
	public MooContentMap() {
	}
	
	public MooContentMap(HashMap<String, MooIContent> newContent) {
		for(MooIContent content : newContent.values()) {
			content.markFixed();
			map.put(content.getKey(), content);
		}
		map = newContent;
	}
	
	public MooIContent get(String key) {
		return map.get(key);
	}
	
	
}
