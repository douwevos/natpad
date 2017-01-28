package net.natpad.json;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

public class JsonObject extends JsonContainer {

	protected final HashMap<String, IJsonItem> map = new HashMap<String, IJsonItem>();
	

	public JsonObject() {
	}

	public JsonObject(HashMap<String, IJsonItem> objectMap) {
		map.putAll(objectMap);
	}

	public IJsonItem set(String key, int value) {
		return map.put(key, new JsonInteger(value));
	}

	public IJsonItem set(String key, long value) {
		return map.put(key, new JsonLong(value));
	}

	public IJsonItem set(String key, String value) {
		return map.put(key, new JsonString(value));
	}

	public IJsonItem set(String key, IJsonItem value) {
		return map.put(key, value);
	}

	public JsonArray getArray(String key) throws JsonException {
		return JsonTypes.asArray(map.get(key));
	}

	public String getString(String key) throws JsonException {
		return JsonTypes.asString(map.get(key));
	}

	public boolean getBoolean(String key) throws JsonException {
		return JsonTypes.asBoolean(map.get(key));
	}
	
	public int getInt(String key) throws JsonException {
		return JsonTypes.asInt(map.get(key));
	}

	public long getLong(String key) throws JsonException {
		return JsonTypes.asLong(map.get(key));
	}

	
	@Override
	public int count() {
		return map.size();
	}
	
	@Override
	public Iterator<IJsonItem> iterator() {
		return map.values().iterator();
	}

	
	public Set<String> keys() {
		return map.keySet();
		
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		for(String key : map.keySet()) {
			IJsonItem val = map.get(key);
			if (buf.length()>0) {
				buf.append(", ");
			}
			buf.append(key).append('=').append(val.toString());
		}
		return "JsonObject[size="+map.size()+", ("+buf+")]";
	}






	
}
