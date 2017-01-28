package net.natpad.viper.model;

import java.io.IOException;

import net.natpad.caterpillar.CatArray;

public interface VipIMap extends VipIResource {

	
	
	CatArray<VipIResource> enlist(boolean forceRefresh) throws IOException;
	
	
	
	
}
