package net.natpad.jaguar.viper.model.archive;

import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.scanner.VipIScanWork;
import net.natpad.viper.model.scanner.VipMapperRegistry;

public class ScanJarFileWork implements VipIScanWork {

	
	private VipNode mainNode;
	
	public ScanJarFileWork(VipNode vipJarFileNode) {
		this.mainNode = vipJarFileNode;
	}
	
	@Override
	public VipIScanWork[] initChildWork(VipMapperRegistry creator) {
		return null;
	}

	@Override
	public void runScan() {
	}

}
