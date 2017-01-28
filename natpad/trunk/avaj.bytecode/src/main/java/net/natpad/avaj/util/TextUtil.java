package net.natpad.avaj.util;



public class TextUtil {

	
	public static String getName(Class<?> clazz, String strip) {
		String name = getName(clazz);
		if (name.startsWith(strip)) {
			name = name.substring(strip.length());
		}
		return name;
	}

	
	public static String getName(Class<?> clazz) {
		String canonicalName = clazz.getCanonicalName();
		int idx = canonicalName.lastIndexOf('.');
		if (idx>=0) {
			return canonicalName.substring(idx+1);
		}
		return canonicalName;
	}

	public static final String TXT_HEX = "0123456789ABCDEF";

	
	public static final String hex(long val, int digits) {
		StringBuilder buf = new StringBuilder();
		for(int idx=0; (digits>0 && idx<digits) || (digits<=0 && val!=0); idx++) {
			buf.append(TXT_HEX.charAt((int) (val&0xf)));
			val = val>>>4;
		}
		if (buf.length()==0) {
			buf.append('0');
		} else {
			buf.reverse();
		}
		return buf.toString();
	}


	public static String convertUtfNameToFQN(String utfName) {
		StringBuilder buf = new StringBuilder(utfName);
		for(int idx=0; idx<buf.length(); idx++) {
			if (buf.charAt(idx)=='/') {
				buf.setCharAt(idx, '.');
			}
		}
		return buf.toString();
	}
	
	
}
