package net.natpad.gross.export.glibc;

public class ClassName {

	
	public static final ClassName INT = new ClassName("", "int", "int");
	public static final ClassName LONG_LONG = new ClassName("", "long long", "long long");
	
	public final String camelPrefix;
	public final String camelPostfix;
	public final String lowPrefix;
	public final String lowPostfix;
	public final String upPrefix;
	public final String upPostfix;
	public final boolean isPointerType;
	
	public final String filename;
	
	public ClassName(String camelPrefix, String camelPostfix, String basePostfix) {
		this.camelPrefix = camelPrefix;
		this.camelPostfix = camelPostfix;
		lowPrefix = camelPrefix.toLowerCase();
		lowPostfix = basePostfix.toLowerCase();
		upPrefix = camelPrefix.toUpperCase();
		upPostfix = basePostfix.toUpperCase();
		filename = null;
		isPointerType = true;
	}

	public ClassName(String cn) {
		if (cn.equals("String")) {
			camelPrefix = "Cat";
			camelPostfix = "StringWo";
			lowPrefix = "cat";
			lowPostfix = "string_wo";
			upPrefix = "CAT";
			upPostfix = "STRING_WO";
			filename = "<caterpillar.h>";
			isPointerType = true;
			return;
		}
		int fnidx = cn.lastIndexOf('#');
		String fname = null;
		if (fnidx>0) {
			fname = cn.substring(fnidx+1);
			cn = cn.substring(0, fnidx);
		}
		if (cn.startsWith("-")) {
			cn = cn.substring(1);
			isPointerType = false;
		} else {
			isPointerType = true;
		}
		
		String[] split = cn.split(":");
		String camelPrefix = null;
		String camelPostfix = null;
		String basePostfix = null;
		if (split.length==1) {
			int sidx = 1;
			for(int idx=1; idx<split[0].length(); idx++) {
				char charAt = split[0].charAt(idx);
				if (Character.isUpperCase(charAt)) {
					sidx = idx;
					break;
				}
			}
			camelPrefix = split[0].substring(0, sidx);
			camelPostfix = split[0].substring(sidx);
			basePostfix = convert(camelPostfix);
		} else if (split.length==2) {
			camelPrefix = split[0];
			camelPostfix = split[1];
			basePostfix = convert(camelPostfix);
		} else {
			camelPrefix = split[0];
			camelPostfix = split[1];
			basePostfix = split[2];
		}
		this.camelPrefix = camelPrefix;
		this.camelPostfix = camelPostfix;
		lowPrefix = camelPrefix.toLowerCase();
		lowPostfix = basePostfix.toLowerCase();
		upPrefix = camelPrefix.toUpperCase();
		upPostfix = basePostfix.toUpperCase();
		
		filename = fname;
	}

	// state 0   uppercase
	// state 1   lowercase
	//
	
	private static String convert(String camel) {
		StringBuilder sb = new StringBuilder();
		int state = 0;
		for(int idx=0; idx<camel.length(); idx++) {
			char ch = camel.charAt(idx);
			if (Character.isUpperCase(ch)) {
				if (state == 1) {
					state = 0;
					sb.append("_");
				}
				ch = Character.toLowerCase(ch);
			} else {
				state = 1;
			}
			sb.append(ch);
		}
		return sb.toString();
	}

	public String createFilename() {
		if (filename!=null) {
			return filename;
		}
		return camelPrefix.toLowerCase()+camelPostfix.toLowerCase();
	}

	public String fullLow() {
		if (lowPrefix==null || lowPrefix.isEmpty()) {
			return lowPostfix;
		}
		return lowPrefix+"_"+lowPostfix;
	}
	
	public String fullTypePtr() {
		if (this==INT) {
			return "int ";
		} else if (this==LONG_LONG) {
			return "long long ";
		}
		String result = null;
		if (camelPrefix==null) {
			result = camelPostfix;
		} else {
			result = camelPrefix+camelPostfix;
		}
		return isPointerType ? result+" *" : result+" ";
	}
	
	
	@Override
	public int hashCode() {
		return camelPostfix.hashCode()+camelPrefix.hashCode();
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof ClassName) {
			ClassName other = (ClassName) obj;
			return other.camelPostfix.equals(camelPostfix)
					&& other.camelPrefix.equals(camelPrefix)
					&& other.lowPostfix.equals(lowPostfix);
		}
		return false;
	}
	
	
	@Override
	public String toString() {
		return camelPrefix+camelPostfix;
	}
}




