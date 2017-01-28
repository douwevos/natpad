package net.natpad.viper.model.scanner;


public interface VipIScanWork {
	public VipIScanWork[] initChildWork(VipMapperRegistry mapperRegistry);
	public void runScan();
}