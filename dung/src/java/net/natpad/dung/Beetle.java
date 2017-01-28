package net.natpad.dung;

import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.module.task.LogLevel;
import net.natpad.dung.run.Context;
import net.natpad.dung.workspace.Workspace;

public class Beetle {

	
	public Beetle() {
	}
	
	
	public static void main(String[] args) {
		StreamHelper.setup();
		
		Beetle beetle = new Beetle();
		beetle.run(args);
	}


	private void run(String[] args) {
		
		try {
			
			BeetleRunner beetleRunner = parseArgs(args);
			if (beetleRunner!=null) {
				beetleRunner.run();
			} else {
				printUsage();
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


	private BeetleRunner parseArgs(String[] args) {
		if (args.length==0) {
			return null;
		}
		BeetleRunner result = null;
			BeetleTargetRunner btRunner = new BeetleTargetRunner();
			for(int idx=0; idx<args.length; idx++) {
				String arg = args[idx];
				if (arg.startsWith("-")) {
					for(int oidx=1; oidx<arg.length(); oidx++) {
						char oc = arg.charAt(oidx);
						switch(oc) {
						case 'v' : {
							btRunner.increaseLogLevel();
						} break;
						case 'r' : {
							btRunner.setRunReverseDependencies(true);
						} break;
						case 'n' : {
							btRunner.setNoDeps(true);
						} break;
						}
					}
				} else {
					if (btRunner.module==null) {
						btRunner.module = arg;
					} else {
						btRunner.addTarget(arg);
					}
				}
			}
			result = btRunner;
		return result;
	}
	
	
	private void printUsage() {
		System.out.println("dung <module> [target]");
		System.out.println("dung <action> [...]");
		System.out.println("");
		System.out.println("  action");
		System.out.println("     -v         show debug");
		System.out.println("     -r         run dependencies reverse");
		System.out.println("     -n         do not run dependencies");

	}

	static abstract class BeetleRunner {

		public BeetleRunner() {
		}
		
		LogLevel logLevel = LogLevel.INFO;
		
		abstract void run() throws Exception;

		void increaseLogLevel() {
			switch(logLevel) {
			case INFO : logLevel = LogLevel.DEBUG; break;
			case DEBUG : logLevel = LogLevel.VERBOSE; break;
			}
		}
	}
	
	static class BeetleTargetRunner extends BeetleRunner {
		
		String module;
		List<String> targetsToRun = new ArrayList<>();
		boolean reverse = false;
		boolean nodeps = false;
		
		public BeetleTargetRunner() {
			super();
		}

		
		public void setNoDeps(boolean b) {
			nodeps = b;
			
		}


		public void setRunReverseDependencies(boolean b) {
			reverse = b;
		}


		void addTarget(String target) {
			targetsToRun.add(target);
		}

		
		@Override
		public void run() throws Exception {
			WorkspaceHelper helper = new WorkspaceHelper();
			Workspace workspace = helper.loadWorkspace();
			Context context = new Context(workspace, module, helper.getWorkDir(workspace));
			context.setLogLevel(logLevel);
			context.build(targetsToRun, reverse, nodeps);
		}
	}
	
}
