package net.natpad.jaguar.viper.model.archive;

import java.io.IOException;
import java.io.InputStream;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;

import net.natpad.viper.VipNamePath;
import net.natpad.viper.model.VipIFile;

public class VipJarFile implements VipIFile {

	VipIFile mainFile;
	VipNamePath entryPath;
	int entryIndex;
	
	public VipJarFile(VipIFile mainFile, VipNamePath entryPath, int entryIndex) {
		this.mainFile = mainFile;
		this.entryPath = entryPath;
		this.entryIndex = entryIndex;
	}
	
	
	public void setEntryIndex(int entryIndex) {
		this.entryIndex = entryIndex;
	}
	
	@Override
	public String getName() {
		return entryPath.getTail();
	}
	
	
	@Override
	public long lastModified() {
		return 0;
	}
	
	@Override
	public long length() {
		return 0;
	}
	
	@Override
	public InputStream openInputStream() throws IOException {
		
		InputStream inputStream = mainFile.openInputStream();
		JarInputStream jarInputStream = new JarInputStream(inputStream);
		JarEntry nextJarEntry = null; 
		for(int idx=entryIndex; idx>=0; idx--) {
			nextJarEntry = jarInputStream.getNextJarEntry();
		}
		if (nextJarEntry!=null) {
			LimitInputStream limitInputStream = new LimitInputStream(jarInputStream, nextJarEntry.getSize());
			return limitInputStream;
		}
		jarInputStream.close();
		return null;
	}
	
	
	
	
}
