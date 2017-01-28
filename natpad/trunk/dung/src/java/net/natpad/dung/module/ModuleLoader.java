package net.natpad.dung.module;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import net.natpad.dung.DungClassLoader;
import net.natpad.dung.sht.ModelLoader;
import net.natpad.dung.workspace.Workspace;
import net.natpad.sht.ShoParser;
import net.natpad.sht.model.Model;

public class ModuleLoader {

	public final Workspace workspace;
	
	public ModuleLoader(Workspace workspace) {
		this.workspace = workspace;
	}
	
	public Module loadModule(File moduleDir, File buildFile) throws IOException {
		DungClassLoader loader = new DungClassLoader();
		Module result = null;
		ShoParser shoParser = new ShoParser();
		try {
			ModelLoader modelLoader = new ModelLoader();
			Model model = modelLoader.parse(new FileInputStream(buildFile), true);

//			System.out.println("descr="+moduleDescr);
			
			result = new Module(workspace, moduleDir,  model, loader);
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
}
