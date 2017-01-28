package net.natpad.dung.hill;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.util.Random;

import net.natpad.dung.DungClassLoader;
import net.natpad.dung.Template;
import net.natpad.dung.WorkspaceHelper;
import net.natpad.dung.WorkspaceHelper.WorkspaceAndName;
import net.natpad.dung.hill.model.HillMapper;
import net.natpad.dung.hill.model.WorkspaceDescr;
import net.natpad.dung.hill.model.WorkspaceHolder;
import net.natpad.dung.hill.task.Task;
import net.natpad.dung.sht.ModelLoader;
import net.natpad.dung.workspace.Module;
import net.natpad.dung.workspace.ModulePkgConfig;
import net.natpad.dung.workspace.Workspace;
import net.natpad.dung.workspace.WorkspaceMapper;
import net.natpad.sht.ShtSerializer;
import net.natpad.sht.mapper.Mapper;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.Model;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;

public class HillInstaller implements HillRunner {
	
	WorkspaceHelper workspaceHelper = new WorkspaceHelper();
	
	String definitionArg;
	
	public HillInstaller() {
	}

	
	public void parseArgs(String args[]) {
		for(int idx=0; idx<args.length; idx++) {
			String arg = args[idx];
			definitionArg = arg;
		}
	}
	
	@Override
	public void run() throws Exception {
		if (!definitionArg.endsWith(".hil")) {
			definitionArg = definitionArg + ".hil";
		}
		File file = new File(definitionArg);
		ModelLoader modelLoader = new ModelLoader();
		Model model = modelLoader.parse(new FileInputStream(file), true);
//		System.out.println("model="+model);
		DungClassLoader dungClassLoader = new DungClassLoader();
		Mapper mapper = new HillMapper(dungClassLoader);
		WorkspaceHolder hws = mapper.map(model, WorkspaceHolder.class, null);

		WorkspaceAndName existingWorkspace = workspaceHelper.findByName(hws.workspace.name);
		
		String wsId = null;
		if (existingWorkspace!=null) {
			wsId = existingWorkspace.workspace.id;
			System.out.println("Existing workspace ID found:"+wsId);
		} else {
			Random r = new Random();
			long nextLongA = r.nextLong();
			long nextLongB = r.nextLong();
			String sa = "00000000"+Long.toHexString(nextLongA);
			sa = sa.substring(sa.length()-8);
			String sb = "00000000"+Long.toHexString(nextLongB);
			sb = sb.substring(sb.length()-8);
			wsId = sa+sb;
		}
		
		
		Workspace ws = createFromDescription(hws.workspace, wsId);

		File dungBase = workspaceHelper.getDungHome(true);
		File f = new File(dungBase, hws.workspace.name+".dun");
		WorkspaceMapper workspaceMapper = new WorkspaceMapper();
		model = workspaceMapper.unmap(ws);
		ShtSerializer serializer = new ShtSerializer();
		StringBuilder buf = new StringBuilder();
		serializer.serializeReferences(model, buf);
		FileWriter fw = new FileWriter(f);
		fw.write(buf.toString());
		fw.close();
		System.out.println("Succesfully installed the workspace '"+hws.workspace.name+"'");
		
		if (!workspaceHelper.hasActiveWorspace()) {
                    workspaceHelper.select(hws.workspace.name);
                    System.out.println("Automatically selected workspace '"+hws.workspace.name+"'");
		}
	}


	private Workspace createFromDescription(WorkspaceDescr workspaceDesc, String id) throws Exception {
		Workspace result = new Workspace();
		result.id = id;

		result.references.putAll(workspaceDesc.references);
		result.modules.putAll(workspaceDesc.modules);
		
		for(Reference tref : workspaceDesc.templates) {
			BlockValue bv = (BlockValue) tref.getValue();
			Reference firstIdRef = bv.getFirstReferenceById("id");
			bv.remove(firstIdRef);
			String tname = ((SimpleValue) firstIdRef.getValue()).getValue();
			Template template = new Template(tname);
			for(Reference tsref : bv) {
				template.add(tsref);
			}
			result.templates.put(tname, template);
		}

		HillSession session = new HillSession(result);

		
		for(Task task : workspaceDesc) {
			task.run(session);
		}
		
		
				
		return result;
	}
}