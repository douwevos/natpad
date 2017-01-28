package net.natpad.jaguar.viper.model.archive;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicReference;

import net.natpad.caterpillar.CatArray;
import net.natpad.viper.VipNamePath;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipIResource;

public class VipJarMap implements VipIMap {


	private VipIFile mainFile;
	private VipNamePath entryPath;

	private AtomicReference<VipIResource[]> children = new AtomicReference<VipIResource[]>();

	
	public VipJarMap(VipIFile mainFile, VipNamePath entryPath, VipIResource[] vipchildren) {
		this.mainFile = mainFile;
		this.entryPath = entryPath;
		children.set(vipchildren);
	}
	
	
	
	
	@Override
	public CatArray<VipIResource> enlist(boolean forceRefresh) throws IOException {
		VipIResource[] vipIResources = children.get();
		CatArray<VipIResource> result = null;
		if (vipIResources == null) {
			result = new CatArray<VipIResource>();
		} else {
			result = new CatArray<VipIResource>(vipIResources);
		}
		return result;
	}
	
	@Override
	public String getName() {
		return entryPath.getTail();
	}
	
	
	@Override
	public long lastModified() {
		// TODO Auto-generated method stub
		return 0;
	}
	
}
