package net.natpad.viper;

public class VipNamePath {

	String entries[];
	
	public VipNamePath(String unparsed) {
		String[] split = unparsed.split("/");
		entries = split;
	}
	
	public VipNamePath(VipNamePath parentPath, String name) {
		entries = new String[parentPath.entries.length+1];
		System.arraycopy(parentPath.entries, 0, entries, 0, parentPath.entries.length);
		entries[parentPath.entries.length] = name;
	}

	public String getTail() {
		return entries.length>0 ? entries[entries.length-1] : "";
	}
	
//	public String asSystemFilename() {
//		StringBuilder buf = new StringBuilder();
//		for(String entry : entries) {
//			buf.append('/').append(entry);
//		}
//		if (buf.length()==0) {
//			buf.append("/");
//		}
//		return buf.toString();
//	}
	
}
