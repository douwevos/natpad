package net.natpad.dung.module.task;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;

public class Md5sums extends Task {

	private String tofile;
	
	private String basepath;
	
	private List<FileBundle> fileBundles = new ArrayList<FileBundle>();
	
	public void addIn(FileBundle bundle) {
		fileBundles.add(bundle);
	}

	
	@Override
	public void runTask(Session session) throws Exception {
		Runtime runtime = Runtime.getRuntime();
		
		List<String> argList = new ArrayList<String>();
		argList.add("/usr/bin/md5sum");

		int count=0;
		File moduleDir = session.module.directory;
		PathInPath[] scanBundles = session.scanBundles(moduleDir, fileBundles, true);
		for (PathInPath pathInPath : scanBundles) {
			String relativePath = pathInPath.relativePath.toString();
			File srcFile = new File(pathInPath.basePath.toFile(), relativePath);
			if (pathInPath.isFile) {
				argList.add(srcFile.getAbsolutePath());
				count++;
			}
		}

		
		Path output = session.createModulePath(tofile);
		
		String[] args = argList.toArray(new String[argList.size()]);
		FileOutputStream out = null;
		try {
			
			
			Process process = runtime.exec(args);


			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			
			
			Thread thread = new ProcessInputStreamer(process.getErrorStream()).launch();
			InputStream in = process.getInputStream();
			byte data[] = new byte[2048];
			while(true) {
				int read = in.read(data);
				if (read<=0) {
					break;
				} else {
					baos.write(data, 0, read);
				}
				
			}
			thread.join();
			int exitCode = process.waitFor();
			if (exitCode!=0) {
				throw new RuntimeException("Error running MD5-sum over files");
			}
			
			String md5Text = new String(baos.toByteArray());
			if (basepath!=null) {
				md5Text = replacePath(md5Text, session.resolveProperties(basepath));
			}
			out = new FileOutputStream(output.toFile());
			out.write(md5Text.getBytes());
			
			log(LogLevel.INFO, "Run MD5-sum over "+count+" file"+(count>1 ? "s" : ""));
		} catch (IOException e) {
			throw new RuntimeException("Error running MD5-sum over files", e);
		} finally {
			if (out !=null) {
				out.close();
			}
		}
	}

	private String replacePath(String md5Text, String basepath) {
		basepath = " "+basepath;
		StringBuilder buf = new StringBuilder();
		int idxa=0;
		while(true) {
			int idxb = md5Text.indexOf(basepath, idxa);
			if (idxb>=0) {
				buf.append(md5Text.substring(idxa, idxb)).append(' ');
				idxa=idxb+basepath.length();
			} else {
				buf.append(md5Text.substring(idxa));
				break;
			}
		}
		return buf.toString();
	}
}
