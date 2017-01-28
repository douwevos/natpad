package net.natpad.gross.export.glibc;

public class Argument {

	public final ClassName type;
	public final String label;
	
	public Argument(ClassName type, String label) {
		this.type = type;
		this.label = label;
	}
	
	
	
	@Override
	public int hashCode() {
		return label.hashCode()+type.hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof Argument) {
			Argument other = (Argument) obj;
			return other.type.equals(type) && label.equals(other.label);
		}
		return false;
	}
}
