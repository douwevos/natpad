package net.natpad.jaguar.monkey.context;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.CatFixedArray;
import net.natpad.caterpillar.CatFixedString;

public class JagModuleSettings {

	private CatFixedArray<CatArray<CatFixedString>> sourceFolders;
	

	public JagModuleSettings() {
		sourceFolders = new CatFixedArray<CatArray<CatFixedString>>();
	}
	
	private JagModuleSettings(JagModuleSettings source) {
		sourceFolders = source.sourceFolders;
	}
	
	
	
	public CatFixedArray<CatArray<CatFixedString>> getSourceFolders() {
		return sourceFolders;
	}
	
	
	public JagModuleSettings setSourceFolders(CatFixedArray<CatArray<CatFixedString>> sourceFolders) {
		JagModuleSettings result = new JagModuleSettings(this);
		result.sourceFolders = sourceFolders;
		return result;
	}

	
	@Override
	public boolean equals(Object obj) {
		if (obj == this) {
			return true;
		}
		if (obj instanceof JagModuleSettings) {
			JagModuleSettings other = (JagModuleSettings) obj;
			return other.sourceFolders.equals(sourceFolders);
		}
		return false;
	}

	
}
