package net.natpad.avaj.util;

public final class Util {

	public static final <T> boolean nullSafeEquals(T a, T b) {
		return (a==b) || (a!=null && b!=null && a.equals(b));
	}

}
