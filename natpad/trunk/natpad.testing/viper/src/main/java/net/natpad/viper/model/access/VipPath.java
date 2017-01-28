package net.natpad.viper.model.access;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;
import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.CatFixedArray;
import net.natpad.caterpillar.CatFixedString;
import net.natpad.caterpillar.CatIIterator;

public class VipPath {

	private boolean isCanonical;
	int hashCode;
	CatFixedArray<String> entries;
	
	public VipPath(CatFixedString fullPath) {
		cat_log_debug("fp=%s", fullPath);
		CatArray<CatFixedString> splitted = fullPath.split('/', true);
		CatArray<String> result = new CatArray<String>();
		
		boolean is_canonical = true;
		int hash_code = 0;
		int idx;
		boolean is_first = true;
		for(idx=0; idx<splitted.size(); idx++) {
			String sub = splitted.get(idx).toString();
			cat_log_debug("sub=%s", sub);
			if (sub.length()==0) {
				splitted.remove(idx, null);
				idx--;
				is_first = false;
				continue;
			}
			result.add(sub);

			if (sub.equals(".")) {
				is_canonical = false;
			} else if (sub.equals("..")) {
				is_canonical = false;
			} else if (is_first) {
				sub = ".";
				result.insert(0, sub);
			}

			hash_code = hash_code*7+sub.hashCode();
			is_first = false;
		}

		entries = CatFixedArray.convert(result);
		isCanonical = is_canonical;
		this.hashCode = hash_code;
	}

	

	public VipPath(CatFixedArray<String> entries) {
		this.entries = entries;
		boolean is_canonical = true;
		CatIIterator<String> iter = entries.iterator();
		int hash_code = 0;
		while(iter.hasNext()) {
			String sub = iter.next();
			if (sub.equals(".")) {
				is_canonical = false;
			} else if (sub.equals("..")) {
				is_canonical = false;
			}
			hash_code = hash_code*7+sub.hashCode();
		}
		isCanonical = is_canonical;
		hashCode = hash_code;
	}

	
	
	public int count() {
		return entries.size();
	}
	
	public String get(int index) {
		return entries.get(index);
	}

	
	public VipPath createChild(String direntry) {
		CatFixedArray<String> newEntries = entries.append(direntry);
		return new VipPath(newEntries);
	}



	public String getLeaf() {
		return entries.size()>0 ? entries.getLast() : null;
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		boolean is_first = true;
		for (String sub : entries) {
			if (is_first) {
				is_first = false;
				if (sub.equals(".") || (sub.equals(".."))) {
				} else {
					buf.append('/');
				}
			} else {
				buf.append('/');
			}
			buf.append(sub);
		}
		if (buf.length()==0 && is_first) {
			buf.append('/');
		}
		return buf.toString();
	}



	
}
