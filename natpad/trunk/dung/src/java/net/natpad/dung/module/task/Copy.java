package net.natpad.dung.module.task;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.Session;

public class Copy extends Task {

	private String tofile;
	private String fromfile;
	private String todir;
	private boolean replaceProperties;
	private List<FileBundle> fileBundles = new ArrayList<FileBundle>();

	
	public void addFrom(FileBundle bundle) {
		fileBundles.add(bundle);
	}
	
	@Override
	public void runTask(Session session) throws Exception {
		if (fromfile!=null && !fileBundles.isEmpty()) {
			throw new RuntimeException("either provide a fromfile or provide a 'from' file-bundle");
		}
		
		if (tofile!=null && todir!=null) {
			throw new RuntimeException("either provide a tofile or provide a todir");
		}
		
		if (tofile!=null) {
			copytofile(session);
		} else {
			copytodir(session);
		}
		
	}

	private void copytodir(Session session) throws IOException {
		if (fromfile!=null) {
			/*copy single file to dir*/
			Path fromPath = session.createModulePath(fromfile);
			Path toDirPath = session.createModulePath(todir);

			File srcFile = fromPath.toFile();
			if (!srcFile.isFile()) {
				throw new RuntimeException("Can not copy, not a file:"+srcFile.getAbsolutePath());
			}
			File destDir = toDirPath.toFile();
			if (!(destDir.exists() && destDir.isDirectory())) {
				if (!destDir.mkdirs()) {
					throw new RuntimeException("Could not create directory:"+destDir.getAbsolutePath());
				}
			}
			
			File destFile = new File(destDir, srcFile.getName());
			copyFile(session, srcFile, destFile);
			log(LogLevel.INFO, "Copied 1 file");
		} else {
			File moduleDir = session.module.directory;

			Path toDirPath = session.createModulePath(todir);

			File destDir = toDirPath.toFile();
			if (!(destDir.exists() && destDir.isDirectory())) {
				if (!destDir.mkdirs()) {
					throw new RuntimeException("Could not create directory:"+destDir.getAbsolutePath());
				}
			}
			
			PathInPath[] scanBundles = session.scanBundles(moduleDir, fileBundles, true);
			int count = 0;
			for (PathInPath pathInPath : scanBundles) {
				String relativePath = pathInPath.relativePath.toString();
				if (pathInPath.isFile) {
					File destFile = new File(destDir, relativePath);
					destFile.getParentFile().mkdirs();
					File srcFile = new File(pathInPath.basePath.toFile(), relativePath);
					try {
						log(LogLevel.DEBUG, "Copying "+srcFile.getAbsolutePath()+" to "+destFile.getAbsolutePath());
						copyFile(session, srcFile, destFile);
						count++;
					} catch (IOException e) {
						throw new RuntimeException("could not copy file:"+srcFile.getAbsolutePath()+" to "+destFile.getAbsolutePath(), e);
					}
				} else {
					File destFile = new File(destDir, relativePath);
					log(LogLevel.DEBUG, "Creating dir "+destFile.getAbsolutePath());
					destFile.mkdir();
				}
				
			}
			log(LogLevel.INFO, "Copied "+count+" file"+(count>1 ? "s" : ""));
		}
	}

	private void copytofile(Session session) throws IOException {
		Path fromPath = session.createModulePath(fromfile);
		Path toPath = session.createModulePath(tofile);
		File destFile = toPath.toFile();
		File destDir = destFile.getParentFile();
		if (!(destDir.exists() && destDir.isDirectory())) {
			if (!destDir.mkdirs()) {
				throw new RuntimeException("Could not create directory:"+destDir.getAbsolutePath());
			}
		}

		copyFile(session, fromPath.toFile(), destFile);
		log(LogLevel.INFO, "Copied 1 file");
	}
	
	
	
	private void copyFile(Session session, File src, File dest) throws IOException {
		if (replaceProperties) {
			copyFileReplace(session, src, dest);
		} else {
			copyFilePlain(session, src, dest);
		}
	}

	private void copyFileReplace(Session session, File src, File dest) throws IOException {
		FileInputStream fis = null;
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		FileOutputStream fos = null;
		try { 
			byte buf[] = new byte[0x10000];
			fis = new FileInputStream(src);
			while(true) {
				int cnt = fis.read(buf);
				if (cnt<=0) {
					break;
				}
				
				baos.write(buf, 0, cnt);
			}
			String rawText = new String(baos.toByteArray());
			String finText = session.resolveProperties(rawText);
			byte[] bytes = finText.getBytes();
			
			fos = new FileOutputStream(dest);
			fos.write(bytes, 0, bytes.length);
		} finally {
			if (fis!=null) {
				fis.close();
			}
			if (fos!=null) {
				fos.close();
			}
		}
	}

	
	private void copyFilePlain(Session session, File src, File dest) throws IOException {
		
		FileInputStream fis = null;
		FileOutputStream fos = null;
		try { 
			byte buf[] = new byte[0x10000];
			fis = new FileInputStream(src);
			fos = new FileOutputStream(dest);
			while(true) {
				int cnt = fis.read(buf);
				if (cnt<=0) {
					break;
				}
				
				fos.write(buf, 0, cnt);
			}
		} finally {
			if (fis!=null) {
				fis.close();
			}
			if (fos!=null) {
				fos.close();
			}
		}
				
	}
	
}
