package net.natpad.json;

public class JsonTypes {

	public static String asString(IJsonItem item) throws JsonException {
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).stringValue();
		} else if (item instanceof JsonString) {
			return ((JsonString) item).value;
		}
		throw new JsonException(""+item+" can't read string value");
	}
	
	public static JsonArray asArray(IJsonItem item) throws JsonException {
		if (item==null || item instanceof JsonArray) {
			return (JsonArray) item;
		}
		throw new JsonException("not an array item="+item);
	}

	public static int asInt(IJsonItem item) throws JsonException {
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).intValue();
		} else if (item instanceof JsonString) {
			return Integer.parseInt(((JsonString) item).value);
		} else if (item == null) {
			return 0;
		}
		throw new JsonException("can't extract int value"+item);
	}

	public static long asLong(IJsonItem item) throws JsonException {
		if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).longValue();
		} else if (item instanceof JsonString) {
			return Long.parseLong(((JsonString) item).value);
		} else if (item == null) {
			return 0l;
		}
		throw new JsonException("can't extract long value"+item);
	}

	public static boolean asBoolean(IJsonItem item) {
		if (item instanceof JsonBoolean) {
			return ((JsonBoolean) item).value;
		} else if (item instanceof IJsonNumber) {
			return ((IJsonNumber) item).intValue()!=0;
		} else if (item instanceof JsonString) {
			return Boolean.parseBoolean(((JsonString) item).value);
		} else if (item == null) {
			return false;
		}
		return false;
	}

}
