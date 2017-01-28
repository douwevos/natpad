package net.natpad.json;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class JsonArray extends JsonContainer {

	protected final List<IJsonItem> list = new ArrayList<IJsonItem>();
	
	public JsonArray() {
	}

	public int count() {
		return list.size();
	}
	
	public IJsonItem get(int index) {
		return list.get(index);
	}

	public int getInt(int index) throws JsonException {
		IJsonItem item = list.get(index);
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).intValue();
		} else if (item instanceof JsonString) {
			return Integer.parseInt(((JsonString) item).value);
		}
		throw new JsonException(""+item+" can't read int value");
	}

	public long getLong(int index) throws JsonException {
		IJsonItem item = list.get(index);
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).longValue();
		} else if (item instanceof JsonString) {
			return Long.parseLong(((JsonString) item).value);
		}
		throw new JsonException(""+item+" can't read long value");
	}

	public double getDouble(int index) throws JsonException {
		IJsonItem item = list.get(index);
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).doubleValue();
		} else if (item instanceof JsonString) {
			return Double.parseDouble(((JsonString) item).value);
		}
		throw new JsonException(""+item+" can't read long value");
	}

	public String getString(int index) throws JsonException {
		IJsonItem item = list.get(index);
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).stringValue();
		} else if (item instanceof JsonString) {
			return ((JsonString) item).value;
		}
		throw new JsonException(""+item+" can't read long value");
	}

	
	public void add(IJsonItem item) {
		list.add(item);		
	}
	
	public void add(long val) {
		list.add(new JsonLong(val));
	}

	public void add(int val) {
		list.add(new JsonInteger(val));
	}

	public void add(double val) {
		list.add(new JsonDouble(val));
	}

	public void add(String val) {
		list.add(new JsonString(val));
	}
	
	
	@Override
	public Iterator<IJsonItem> iterator() {
		return list.iterator();
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		for(IJsonItem item : list) {
			if (buf.length()>0) {
				buf.append(", ");
			}
			buf.append(item);
		}
		return "JsonArray["+buf+"]";
	}
}
