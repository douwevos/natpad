package net.natpad.jaguar.viper.model.archive;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashSet;
import java.util.jar.JarInputStream;
import java.util.zip.ZipEntry;

import net.natpad.caterpillar.CatArray;
import net.natpad.viper.VipNamePath;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipIResource;

public class VipJarRoot implements VipIMap, VipIFile {

	
	VipIFile jar_file;
	
	public VipJarRoot(VipIFile jar_file) {
		this.jar_file = jar_file;
	}
	
	
	@Override
	public String getName() {
		return jar_file.getName();
	}
	
	@Override
	public long lastModified() {
		return jar_file.lastModified();
	}
	
	@Override
	public long length() {
		return jar_file.length();
	}
	
	@Override
	public InputStream openInputStream() throws IOException {
		return jar_file.openInputStream();
	}
	
	@Override
	public CatArray<VipIResource> enlist(boolean forceRefresh) throws IOException {
		
		CatArray<VipIResource> result = new CatArray<VipIResource>();
		JarNameAndType root = new JarNameAndType("<root>", true);

		JarInputStream jarInputStream = null;
		try {
			InputStream inputStream = jar_file.openInputStream();
			jarInputStream = new JarInputStream(inputStream);

			int entryIndex = 0;
			while(true) {
				ZipEntry nextEntry = jarInputStream.getNextEntry();
				if (nextEntry==null) {
					break;
				}
				String entryName = nextEntry.getName();
				JarNameAndType nameAndType = root.add(entryName, nextEntry.isDirectory());
				nameAndType.entryIndex = entryIndex;
				entryIndex++;

			}
			
			VipNamePath thisVipPath = new VipNamePath("");
			HashSet<String> mapsAdded = new HashSet<String>();
			HashSet<String> filesAdded = new HashSet<String>();
			for(JarNameAndType nameAndType : root.children) {
				if (nameAndType.isMap) {
					if (!mapsAdded.contains(nameAndType.name)) {
						mapsAdded.add(nameAndType.name);
						VipIResource sub_map_resource = nameAndType.getVipResource(jar_file, thisVipPath);
						result.add(sub_map_resource);
					}
				} else {
					if (!filesAdded.contains(nameAndType.name)) {
						filesAdded.add(nameAndType.name);
						VipIResource sub_map_resource = nameAndType.getVipResource(jar_file, thisVipPath);
						result.add(sub_map_resource);
					}
				}
			}
			
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (jarInputStream!=null) {
				jarInputStream.close();
			}
		}
		
		return result;
	}
	
}
