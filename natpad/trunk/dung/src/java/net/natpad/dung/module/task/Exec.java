package net.natpad.dung.module.task;

import java.nio.file.Path;
import java.util.ArrayList;

import net.natpad.dung.StreamHelper;
import net.natpad.dung.run.Session;

public class Exec extends Task {

	String executable;
	String dir;
	ArrayList<String> args = new ArrayList<String>();
	ArrayList<String> envs = new ArrayList<String>();
	
	public void addArg(String arg) {
		args.add(arg);
	}

	public void addEnv(String env) {
		envs.add(env);
	}

	@Override
	public void runTask(Session session) throws Exception {
		if (dir==null) {
			dir=".";
		}
		Path path = session.createBuildPath(dir);
		
		String fargs[] = new String[args.size()+1];
		fargs[0] = session.resolveProperties(executable);
		for(int idx=0; idx<args.size(); idx++) {
			fargs[idx+1] = session.resolveProperties(args.get(idx));
		}

		if (isEnabled(LogLevel.DEBUG)) {
			log(LogLevel.DEBUG, "dir "+path.toFile());
			for(String opt : fargs) {
				log(LogLevel.DEBUG, "arg "+opt);
			}
		}

		
		StreamHelper streamHelper = (StreamHelper) System.out;
		String prefix = streamHelper.getPrefix();
		log(LogLevel.INFO, ""+executable);

		
		
		
		String fenvs[] = new String[envs.size()];
		for(int idx=0; idx<fenvs.length; idx++) {
			fenvs[idx] = session.resolveProperties(envs.get(idx));
		}
		
		ExecAction execAction = new ExecAction(prefix, fargs, fenvs, path.toFile());
		int exitCode = execAction.runAction();
		log(LogLevel.DEBUG, "exit-code="+exitCode);
	}
}
