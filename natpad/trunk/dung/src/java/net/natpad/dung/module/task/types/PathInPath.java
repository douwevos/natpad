package net.natpad.dung.module.task.types;

import java.nio.file.Path;

public class PathInPath {

	public final Path basePath;
	public final Path relativePath;
	public final boolean isFile;
	
	public PathInPath(Path basePath, Path relativePath, boolean isFile) {
		this.basePath = basePath;
		this.relativePath = relativePath;
		this.isFile = isFile;
	}

	@Override
	public String toString() {
		return "PathInPath [basePath=" + basePath + ", relativePath=" + relativePath + ", isFile=" + isFile + "]";
	}
	
}
