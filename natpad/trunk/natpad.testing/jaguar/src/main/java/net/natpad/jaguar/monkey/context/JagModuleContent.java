package net.natpad.jaguar.monkey.context;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.CatFixedArray;
import net.natpad.caterpillar.CatFixedString;
import net.natpad.monkey.model.MooIContent;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.ListValue;
import net.natpad.sht.model.Model;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class JagModuleContent implements MooIContent {

	public final static String KEY = "JagModuleContent";
	
	private volatile boolean isFixed = false;

	
	private Model modelCached;
	private JagModuleSettings moduleSettings;

	public JagModuleContent() {
		modelCached = null;
		moduleSettings = new JagModuleSettings();
	}

	public JagModuleContent(JagModuleContent source) {
		modelCached = source.modelCached;
		moduleSettings = source.moduleSettings;
	}

	
	private JagModuleContent ensureEditable() {
		if (isFixed) {
			JagModuleContent copy = new JagModuleContent();
			copy.modelCached = modelCached;
			copy.moduleSettings = moduleSettings;
			return copy;
		}
		return this;
	}
	
	@Override
	public String getKey() {
		return KEY;
	}
	
	@Override
	public void markFixed() {
		isFixed = true;
	}
	
	
	
	public JagModuleContent reconfigure(Model model) {
		if (model==modelCached) {
			return this;
		}
		JagModuleContent result = ensureEditable();
		result.modelCached = model;
		
		result.updateModuleSettings();
		
		return result;
	}
	

	private void updateModuleSettings() {
		CatArray<CatArray<CatFixedString>> sourceFolderList = new CatArray<CatArray<CatFixedString>>();
		if (modelCached!=null) {
			Reference javaRef = modelCached.getFirstReferenceById("java");
			if (javaRef!=null) {
			
				Value value = javaRef.getValue();
				if (value instanceof BlockValue) {
					BlockValue block = (BlockValue) value;
					Reference srcFoldersRef = block.getFirstReferenceById("java_source_folders");
					value = srcFoldersRef.getValue();
					if (value instanceof ListValue) {
						ListValue srcFoldersList = (ListValue) value;
						for (Value lv : srcFoldersList) {
							if (lv instanceof SimpleValue) {
								String folderName = ((SimpleValue) lv).value;
								CatFixedString fname = new CatFixedString(folderName);
								CatArray<CatFixedString> split = fname.split('/', false);
								sourceFolderList.append(split);
							}
						}
					}
				}
			}
		} else {
			// TODO remove config
		}
		CatFixedArray<CatArray<CatFixedString>> fsrclist = CatFixedArray.convert(sourceFolderList);
		moduleSettings = moduleSettings.setSourceFolders(fsrclist);
	}


	public JagModuleSettings getModuleSettings() {
		return moduleSettings;
	}
	

}
