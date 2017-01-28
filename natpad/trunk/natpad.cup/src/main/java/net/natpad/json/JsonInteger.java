package net.natpad.json;

public class JsonInteger implements IJsonItem, IJsonNumber {

	public final int value;
	
	public JsonInteger(int value) {
		this.value = value;
	}
	
	@Override
	public int intValue() {
		return value;
	}
	
	@Override
	public long longValue() {
		return (long) value;
	}
	
	@Override
	public double doubleValue() {
		return (double) value;
	}

	@Override
	public String stringValue() {
		return ""+value;
	}

}
