package net.natpad.dung.eprops;

public class EPropertyItem {

	public final String unresolvedValue;
	private String key, value;
	private boolean isResolved;
	
	public EPropertyItem(String key, String unresolvedValue) {
		this.unresolvedValue = unresolvedValue;
		this.key = key;
	}
	
	public boolean isResolved() {
		return isResolved;
	}
	
	public String getKey() {
		return key;
	}
	
	public String getValue() {
		return value;
	}
	
	public void setValue(String value, boolean isResolved) {
		this.value = value;
		this.isResolved = isResolved;
	}
	
	public void reset() {
		this.value = unresolvedValue;
		this.isResolved = false;
	}

	@Override
	public String toString() {
		return "EPropertyItem [unresolvedValue=" + unresolvedValue + ", key=" + key + ", value=" + value
				+ ", isResolved=" + isResolved + "]";
	}
	
	
	
}
