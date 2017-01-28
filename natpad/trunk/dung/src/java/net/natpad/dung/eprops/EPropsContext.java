package net.natpad.dung.eprops;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class EPropsContext {

	
	public final EPropsContext parent;
	
	private Map<String, String> raw = new HashMap<String, String>();


	private int parentVersion[];
	private int myVersion[];
	private int version;
	
	
	
	public EPropsContext(EPropsContext parent) {
		this.parent = parent;
	}
	
	public void put(String key, String value) {
		raw.put(key, value);
		myVersion = null;
	}

	
	
	public Iterable<String> keys() {
		return raw.keySet();
	}

	public String get(String key) {
		return raw.get(key);
	}

	public int[] getVersion() {
		int ps = 0;
		if (parent!=null) {
			int[] newParVer = parent.getVersion();
			if (newParVer!=parentVersion) {
				parentVersion = newParVer;
				myVersion = null;
			}
			ps = newParVer.length;
		}
		
		if (myVersion==null) {
			version++;
			myVersion = new int[ps+1];
			if (parentVersion!=null) {
				System.arraycopy(parentVersion, 0, myVersion, 0, parentVersion.length);
			}
			myVersion[ps] = version;
		}
		
		return myVersion;
	}


}
