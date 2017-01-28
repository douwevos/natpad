package net.natpad.json;

public class JsonDouble implements IJsonItem, IJsonNumber {

	public final double value;
	
	public JsonDouble(double value) {
		this.value = value;
	}
	
	@Override
	public int intValue() {
		return (int) value;
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
	
	
	@Override
	public String toString() {
		return "JsonDouble["+value+"]";
	}
}
