package net.natpad.viper.model;

import java.io.IOException;
import java.io.InputStream;

public interface VipIFile extends VipIResource {

	public InputStream openInputStream() throws IOException;

	public long length();

	
}
