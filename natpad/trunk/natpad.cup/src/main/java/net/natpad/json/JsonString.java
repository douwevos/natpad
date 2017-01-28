package net.natpad.json;

public class JsonString implements IJsonItem {

	public final String value;
	
	public JsonString(String value) {
		this.value = value;
	}
	
	
	@Override
	public String toString() {
		return "JsonString["+value+"]";
	}
}
