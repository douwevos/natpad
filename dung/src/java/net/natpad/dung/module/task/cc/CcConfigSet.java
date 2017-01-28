package net.natpad.dung.module.task.cc;

import java.util.ArrayList;
import java.util.Iterator;

import net.natpad.dung.module.dependency.PkgConfig;
import net.natpad.dung.run.Session;


public class CcConfigSet implements ICcConfigItem, Iterable<ICcConfigItem> {

	protected ArrayList<ICcConfigItem> items = new ArrayList<ICcConfigItem>();

	
	public void addItem(ICcConfigItem item) {
		items.add(item);
	}

	
	public void add(CcConfigSet subset) {
		items.add(subset);
	}

	public void add(PkgConfig path) {
		items.add(path);
	}
	
	public void add(CcCompilerPath path) {
		items.add(path);
	}

	public void add(CcIncludePath path) {
		items.add(path);
	}

	public void add(CcLinkerPath path) {
		items.add(path);
	}

	public void add(CcLibraryPath path) {
		items.add(path);
	}
	
    public void add(CcStripArgument strip) {
    	items.add(strip);
    }


	public void add(CcLibraryName libName) {
		items.add(libName);
	}

	
	public void add(CcArg argument) {
		items.add(argument);
	}

	
	public ArrayList<ICcConfigItem> flatten(ArrayList<ICcConfigItem> flatList) {
//        if (isReference()) {
//            return ((CcConfigSet) getCheckedRef()).flatten(flatList);
//        }

        if (flatList==null) {
			flatList = new ArrayList<ICcConfigItem>();
		}
		for(ICcConfigItem item : items) {
			if (item instanceof CcConfigSet) {
				CcConfigSet ccset = (CcConfigSet) item;
				ccset.flatten(flatList);
			} else {
				flatList.add(item);
			}
		}
		
		return flatList;
	}
	
	 @Override
	public Iterator<ICcConfigItem> iterator() {
		ArrayList<ICcConfigItem> flatlist = flatten(null);
		return flatlist.iterator();
	}
	 
	 
	 
	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
	}
	
	
	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
	}
	
	
}
