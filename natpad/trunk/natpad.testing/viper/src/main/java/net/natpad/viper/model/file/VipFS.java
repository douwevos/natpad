package net.natpad.viper.model.file;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.io.File;
import java.util.concurrent.atomic.AtomicReference;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.CatFixedArray;
import net.natpad.viper.model.access.VipPath;

public class VipFS {

	VipPath path;
	VipFSType typeOfFile;
	boolean isReadable;
	int mode;
	long mtime;
	long length;

	AtomicReference<CatFixedArray<VipFS>> dirContentRef;
	
	
	enum VipFSType {
		VIP_FS_TYPE_PLAIN,
		VIP_FS_TYPE_DIRECTORY,
	}
	

	public VipFS(VipPath path) {
		this.path = path;
		File file = new File(path.toString());
		if (file.isDirectory()) {
			typeOfFile = VipFSType.VIP_FS_TYPE_DIRECTORY;
		} else if (file.isFile()) {
			typeOfFile = VipFSType.VIP_FS_TYPE_PLAIN;
		}
		
		isReadable = file.canRead();
		mode = 0;
		mtime = file.lastModified();
		length = file.length();

		if (typeOfFile==VipFSType.VIP_FS_TYPE_DIRECTORY) {
			dirContentRef = new AtomicReference<CatFixedArray<VipFS>>();
		}
	}
	
	
	
	public long getLastModified() {
		return mtime;
	}

	public long getLength() {
		return length;
	}

	public VipFSType getTypeOfFile() {
		return typeOfFile;
	}

	public VipPath getPath() {
		return path;
	}


	public CatFixedArray<VipFS> getDirContent(boolean forceRefresh) {
		CatFixedArray<VipFS> result = null;
		if (dirContentRef!=null) {
			result = dirContentRef.get();
			if (result==null || forceRefresh) {
				CatArray<VipFS> dir_content = new CatArray<VipFS>();
				String fs_name = path.toString();
				cat_log_debug("fs_name=%s", fs_name);
				File file = new File(fs_name);
				String[] dirlist = file.list();
				if (dirlist!=null) {
					for (String direntry : dirlist) {
						if (direntry.equals("..") || direntry.equals(".")) {
							continue;
						}

						VipPath childPath = path.createChild(direntry);
						VipFS childFs = new VipFS(childPath);
						dir_content.append(childFs);
					}
				}
				result = CatFixedArray.convert(dir_content);
				dirContentRef.set(result);
			}
		}
		return result;
	}



	public boolean isUpToDate(VipFS fs) {
		return (fs.isReadable == isReadable)
				&& (fs.length == length)
				&& (fs.mode == mode)
				&& (fs.mtime == mtime)
				&& (fs.typeOfFile == typeOfFile);
	}

	
}
