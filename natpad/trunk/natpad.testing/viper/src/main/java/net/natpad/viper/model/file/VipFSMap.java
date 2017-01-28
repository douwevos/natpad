package net.natpad.viper.model.file;

import java.io.IOException;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.CatFixedArray;
import net.natpad.caterpillar.CatIIterator;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipIResource;

public class VipFSMap implements VipIMap {

	protected VipFS fs;
	
	public VipFSMap(VipFS fs) {
		this.fs = fs;
	}
	
	@Override
	public String getName() {
		return fs.getPath().getLeaf();
	}
	
	@Override
	public long lastModified() {
		return fs.getLastModified();
	}
	
	@Override
	public CatArray<VipIResource> enlist(boolean forceRefresh) throws IOException {
		CatArray<VipIResource> result = new CatArray<VipIResource>();
		CatFixedArray<VipFS> dirContent = fs.getDirContent(forceRefresh);
		CatIIterator<VipFS> iter = dirContent.iterator();
		while(iter.hasNext()) {
			VipFS childFs = iter.next();
			switch(childFs.getTypeOfFile()) {
				case VIP_FS_TYPE_PLAIN : {
					result.append(new VipFSFile(childFs));
				} break;
				case VIP_FS_TYPE_DIRECTORY : {
					result.append(new VipFSMap(childFs));
				} break;
			}
		}
		return result;
	}
	
	
	@Override
	public String toString() {
		return "VipFSMap[name="+getName()+"]";
	}
	
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof VipFSMap) {
			VipFSMap other = (VipFSMap) obj;
			return other.fs.isUpToDate(fs);
		}
		return false;
	}
}
