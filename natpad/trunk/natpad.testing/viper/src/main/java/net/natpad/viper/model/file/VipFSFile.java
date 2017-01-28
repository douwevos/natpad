package net.natpad.viper.model.file;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.access.VipPath;

public class VipFSFile implements VipIFile {

	private final VipPath path;
	private final VipFS fs;
	
	public VipFSFile(VipFS fs) {
		this.fs = fs;
		path = fs.getPath();
	}
	
	
	@Override
	public String getName() {
		return path.getLeaf();
	}
	
	@Override
	public long lastModified() {
		return fs.getLastModified();
	}
	
	@Override
	public long length() {
		return fs.getLength();
	}
	
	
	
	@Override
	public InputStream openInputStream() throws IOException {
		return new FileInputStream(new File(path.toString()));
	}
	
	
	@Override
	public String toString() {
		return "VipFSFile[name="+getName()+", mtime="+lastModified()+", length="+length()+"]";
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof VipFSFile) {
			VipFSFile other = (VipFSFile) obj;
			return other.fs.isUpToDate(fs);
		}
		return false;
	}

	
}
