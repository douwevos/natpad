package net.natpad.jaguar.viper.model.archive;

import java.util.HashSet;

import net.natpad.viper.VipNamePath;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipIResource;

class JarNameAndType {

	public final String name;
	public final boolean isMap;
	public HashSet<JarNameAndType> children;
	
	VipIResource resource;
	public int entryIndex = -1;
	
	
	public JarNameAndType(String name, boolean isMap) {
		this.name = name;
		this.isMap = isMap;
		if (isMap) {
			children = new HashSet<JarNameAndType>();
		}
	}
	
	
	@Override
	public int hashCode() {
		return name.hashCode()+(isMap ? 13 : 0);
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof JarNameAndType) {
			JarNameAndType other = (JarNameAndType) obj;
			return isMap==other.isMap && name.equals(other.name);
		}
		return false;
	}
	
	public JarNameAndType add(String pathEntries[], int index) {
		resource = null;
		while(true) {
			if (index>=pathEntries.length) {
				return this;
			}
			String entry = pathEntries[index++];
			if (entry.length()==0) {
				continue;
			}
			boolean isMap = index!=pathEntries.length;
			JarNameAndType nameAndType = new JarNameAndType(entry, isMap);
			if (!children.add(nameAndType)) {
				for(JarNameAndType child : children) {
					if (child.equals(nameAndType)) {
						nameAndType = child;
						break;
					}
				}
			}
			
			return nameAndType.add(pathEntries, index);
		}
	}
	
	public JarNameAndType add(String path, boolean isMap) {
		String[] split = path.split("/");
		if (split == null || split.length==0) {
			return this;
		}
		if (isMap) {
			String[] splitNew = new String[split.length+1];
			System.arraycopy(split, 0, splitNew, 0, split.length);
			splitNew[split.length] = "";
			split = splitNew;

		} else {
			while(split[split.length-1].isEmpty()) {
				if (split.length==1) {
					return this;
				}
				String[] splitNew = new String[split.length-1];
				System.arraycopy(split, 0, splitNew, 0, splitNew.length);
				split = splitNew;
			}
		}
		return add(split,0);
	}
	
	public VipIResource getVipResource(VipIFile mainFile, VipNamePath parentPath) {
		if (resource==null) {
			VipNamePath vipPath = new VipNamePath(parentPath, name);
			if (isMap) {
				VipIResource vipchildren[] = new VipIResource[children.size()];
				int out = 0;
				for(JarNameAndType childNameAndType : children) {
					vipchildren[out++] = childNameAndType.getVipResource(mainFile, vipPath);
				}
				resource = new VipJarMap(mainFile, vipPath, vipchildren);
			} else {
				resource = new VipJarFile(mainFile, vipPath, entryIndex);
			}
		}
		return resource;
	}
	
}