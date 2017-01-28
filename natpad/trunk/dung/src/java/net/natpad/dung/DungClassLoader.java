package net.natpad.dung;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class DungClassLoader extends ClassLoader {

	HashMap<String, Class<?>> loadedClasses = new HashMap<String, Class<?>>();

	List<JarFile> classpath = new ArrayList<JarFile>();

	public DungClassLoader() throws IOException {
		// try {
		// jarFile = new
		// JarFile("/home/superman/cpp-workspace/trunk/natpad.cup/dung/8a19ad8209e8da98/natpad-cup.jar");
		// } catch(Throwable t) {
		//
		// }
	}

	@Override
	protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
//		System.out.println("loading-class:" + name);
		Class<?> result = null;
		result = loadedClasses.get(name);
		if (result == null && !classpath.isEmpty()) {
			String filename = name.replace('.', '/') + ".class";
			for (int idx = 0; result == null && idx < classpath.size(); idx++) {
				JarFile jarFile = classpath.get(idx);
				JarEntry jarEntry = jarFile.getJarEntry(filename);
				if (jarEntry != null) {
					try {
						// System.out.println("in jar:"+name);

						InputStream inputStream = jarFile.getInputStream(jarEntry);
						byte buf[] = new byte[4096];
						ByteArrayOutputStream baos = new ByteArrayOutputStream();
						while (true) {
							int cnt = inputStream.read(buf);
							if (cnt <= 0) {
								break;
							}
							baos.write(buf, 0, cnt);
						}
						byte[] classbytes = baos.toByteArray();

						result = super.defineClass(name, classbytes, 0, classbytes.length);
						if (result != null) {
							resolveClass(result);
							loadedClasses.put(name, result);
						}
					} catch (Exception e) {
						System.err.println("could not load class");
						e.printStackTrace();
					}
				}
			}
		}

		if (result == null) {
			result = super.loadClass(name, resolve);
		}

		return result;
	}

	@Override
	public InputStream getResourceAsStream(String name) {
		// System.err.println("get-resource:"+name);
		for (int idx = 0; idx < classpath.size(); idx++) {
			JarFile jarFile = classpath.get(idx);
			JarEntry jarEntry = jarFile.getJarEntry(name);
			if (jarEntry != null) {
				try {
					return jarFile.getInputStream(jarEntry);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		return super.getResourceAsStream(name);
	}

	public void addJarPath(Path jarpath) throws IOException {
//		System.out.println("adding to loader : "+jarpath);
		classpath.add(new JarFile(jarpath.toFile()));
	}

}
