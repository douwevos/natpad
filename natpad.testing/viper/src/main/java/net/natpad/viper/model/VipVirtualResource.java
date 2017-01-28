package net.natpad.viper.model;

public class VipVirtualResource implements VipIResource {

	private final String name;
	
	public VipVirtualResource(String name) {
		this.name = name;
	}
	
	@Override
	public String getName() {
		return name;
	}
	
	@Override
	public long lastModified() {
		return 0;
	}
	
}
