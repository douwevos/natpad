package net.natpad.cup.util;

public class TextUtil {

	public static <T> boolean nullSafeEquals(T a, T b) {
		if (a==b) {
			return true;
		}
		if (a==null || b==null) {
			return false;
		}
		return a.equals(b);
	}
	
}
