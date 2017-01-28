package net.natpad.dung.module.task;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import net.natpad.cup.Main;
import net.natpad.dung.run.Session;

public class NatpadCup extends Task {

	public String grammar;
	public String jsonConfig;
	public List<String> extraArgs = new ArrayList<String>();

	public int expect = 0;
	
	public boolean updateOnly;
	
	@Override
	public void runTask(Session session) throws Exception {
		checkParamaters();

		
		ArrayList<String> argList = new ArrayList<String>();

		
		for(String doccArg : extraArgs) {
			argList.add(doccArg);
		}
		
		if (jsonConfig!=null) {
			argList.add("--config");
			Path configPath = session.createModulePath(jsonConfig);
			argList.add(configPath.toString());
		}
		
		if (expect>0) {
			argList.add("-expect");
			argList.add(""+expect);
		}
		
		if (updateOnly) {
			argList.add("--update");
		}
		
		Path grammarPath = session.createModulePath(grammar);
		argList.add(grammarPath.toString());
		
		String args[] = new String[argList.size()];
		
		args = argList.toArray(args);

		try {
			Main main = new Main(args);
			Properties p = new Properties();
			p.setProperty("basedir", session.getModuleDirectory().toString());
//			for(Object key : getProject().getProperties().keySet()) {
//				String skey = key.toString();
//				String val = getProject().getProperty(skey);
//				p.put(skey, val);
//			}
			main.addProperties(p);
			main.run();
		} catch (Exception e) {
			throw new RuntimeException("Unknown error", e);
		}
		
	}
	

	private void checkParamaters() {
		if (grammar==null) {
			throw new RuntimeException("grammar is a mandatory attribute");
		}
//		if (!grammarFile.isFile() || !grammarFile.canRead()) {
//			throw new RuntimeException("grammar-file must be an existing file and readable");
//		}
	}


}
