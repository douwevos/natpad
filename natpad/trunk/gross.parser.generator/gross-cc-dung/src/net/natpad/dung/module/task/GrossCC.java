package net.natpad.dung.module.task;

import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.run.Session;
import net.natpad.gross.Gross;

public class GrossCC extends Task {

	public String grammar;
	public String ast;
	public String outputPath;
	public String prefix;
	public String exporter;
	public boolean updateOnly;
	public List<String> extraArgs = new ArrayList<String>();

	
	public void addArg(String arg) {
		extraArgs.add(arg);
	}
	
	@Override
	public void runTask(Session session) throws Exception {
		checkParamaters();

		
		ArrayList<String> argList = new ArrayList<String>();

//		argList.add("/usr/bin/gross");
		
		argList.add("--exporter");
		argList.add(exporter);

		
		for(String doccArg : extraArgs) {
			argList.add(doccArg);
		}

		if (updateOnly) {
			argList.add("-u");
		}
		if (outputPath!=null) {
			argList.add("--output-path");
			Path configPath = session.createModulePath(outputPath);
			argList.add(configPath.toString());
		}

		if (ast!=null) {
			argList.add("--ast");
			Path configPath = session.createModulePath(ast);
			argList.add(configPath.toString());
		}
		
		if (prefix!=null) {
			argList.add("-Dprefix="+session.resolveProperties(prefix));
		}

		Path grammarPath = session.createModulePath(grammar);
		argList.add(grammarPath.toString());
		
		String args[] = new String[argList.size()];
		
		args = argList.toArray(args);


//		for(String arg : args) {
//			System.out.println("# arg:"+arg);
//		}
		
		
		try {
//			ExecAction execAction = new ExecAction("gross", args, null, null);
//			int exitCode = execAction.runAction();
//			if (exitCode!=0) {
//				System.err.println("Gross exited with code:"+exitCode);
//			}
			
//			Main main = new Main(args);
//			Properties p = new Properties();
//			p.setProperty("basedir", session.getModuleDirectory().toString());
////			for(Object key : getProject().getProperties().keySet()) {
////				String skey = key.toString();
////				String val = getProject().getProperty(skey);
////				p.put(skey, val);
////			}
//			main.addProperties(p);
//			main.run();
			
			
			Gross.main(args);
			
		} catch (Exception e) {
			System.err.println("Dumping arguments");
			for(int ia=0; ia<args.length; ia++) {
				System.err.println(" ["+ia+"]="+args[ia]);
			}
			throw new RuntimeException("Unknown error", e);
		}
		
	}
	

	private void checkParamaters() {
		if (grammar==null) {
			throw new RuntimeException("grammar is a mandatory attribute");
		}
		if (exporter==null) {
			throw new RuntimeException("exporter is a mandatory attribute");
		}
//		if (!grammarFile.isFile() || !grammarFile.canRead()) {
//			throw new RuntimeException("grammar-file must be an existing file and readable");
//		}
	}


}
