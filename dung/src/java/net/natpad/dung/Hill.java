package net.natpad.dung;

import java.io.File;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import net.natpad.dung.WorkspaceHelper.WorkspaceAndName;
import net.natpad.dung.hill.HillInstaller;
import net.natpad.dung.hill.HillRunner;
import net.natpad.dung.workspace.Workspace;


public class Hill {

	
	public static void main(String[] args) {
		Hill hill = new Hill();
		hill.run(args);
	}

	private void run(String[] args) {
		try {
			
			HillRunner hillRunner = parseArgs(args);
			if (hillRunner!=null) {
				hillRunner.run();
			} else {
				printUsage();
			}
			
                } catch (NoCurrentWorkspace e) {
                    System.err.println("No current active workspace. You will need to install and/or select a workspace first.");
                    System.err.println("If you have a <name>.hil file then run 'dung-hill install <name.hill>'");
                    System.exit(-1);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void printUsage() {
		System.out.println("dung-hill <action>");
		System.out.println("  list-modules                       List modules in current workspace");
		System.out.println("  clear-modules                      clear modules in current workspace");
		System.out.println("  install [definition-file]          Remove workspace with the given id");
		System.out.println("  select [id|'name']                 Select the specified workspace as the current");
		System.out.println("  list                               List installed workspaces");
		System.out.println("");
		System.out.println("  show                               Show details of current workspace");
		System.out.println("  remove [id|'name']                 Remove workspace with the given id or name");

	}

	
	private HillRunner parseArgs(String[] args) {
		if (args.length==0) {
			return null;
		}
		HillRunner result = null;
		String fa = args[0];
		String rargs[] = new String[args.length-1];
		System.arraycopy(args, 1, rargs, 0, rargs.length);
		if ("list-modules".equals(fa)) {
			result = new ListModulesRunner();
		} else if ("install".equals(fa)) {
			HillInstaller ir = new HillInstaller();
			result = ir;
		} else if ("list".equals(fa)) {
			ListRunner ir = new ListRunner();
			result = ir;
		} else if ("select".equals(fa)) {
			SelectRunner ir = new SelectRunner();
			result = ir;
		} else if ("clear-modules".equals(fa)) {
			result = new ClearModulesRunner();
		}
		result.parseArgs(rargs);
		return result;
	}

	

	class ListModulesRunner implements HillRunner {
		
		@Override
		public void parseArgs(String[] args) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void run() throws Exception {
			WorkspaceHelper helper = new WorkspaceHelper();
			Workspace workspace = helper.loadWorkspace();
			
			ArrayList<String> enlisted = new ArrayList<String>();
			Path[] modulePaths = workspace.getModulePath();
			for(Path modulePath : modulePaths) {
				enlistModules(modulePath, enlisted);
			}
			
			Collections.sort(enlisted);
			for(String mn : enlisted) {
				System.out.println("  # "+mn);
			}
		}

		
		public void enlistModules(Path modulePath, ArrayList<String> enlisted) {
			File baseDir = modulePath.toFile();
			if (!baseDir.exists()) {
				return;
			}
			
			File[] listFiles = baseDir.listFiles();
			for(File moduleDir : listFiles) {
				if (moduleDir.isDirectory()) {
					enlistModulesInDir(moduleDir, enlisted);
				}
			}
		}


		private void enlistModulesInDir(File moduleDir, ArrayList<String> enlisted) {
			File[] listFiles = moduleDir.listFiles();
			for(File posDunFile : listFiles) {
				if (posDunFile.getName().endsWith(".dun")) {
					String fname = posDunFile.getName();
					fname = fname.substring(0, fname.length()-4);
					if ("build".equals(fname)) {
						enlisted.add(moduleDir.getName());
					} else {
						enlisted.add(moduleDir.getName()+"#"+fname);
					}
				}
			}
		}
		
	}

	static class SelectRunner implements HillRunner {

		String idOrName;
		

		public void parseArgs(String args[]) {
			for(int idx=0; idx<args.length; idx++) {
				String arg = args[idx];
				idOrName = arg;
			}
		}

		@Override
		public void run() throws Exception {
			
			WorkspaceHelper workspaceHelper = new WorkspaceHelper();
			workspaceHelper.select(idOrName);
		}
	}

	
	class ListRunner implements HillRunner {

		public void parseArgs(String args[]) {
		}

		@Override
		public void run() throws Exception {
			
			WorkspaceHelper helper = new WorkspaceHelper();
			List<WorkspaceAndName> enlist = helper.enlist();
			for (WorkspaceAndName workspaceAndName : enlist) {
				StringBuilder buf = new StringBuilder();


				buf.append(workspaceAndName.name);
				while(buf.length()<25) {
					buf.append(' ');
				}
				String id = workspaceAndName.workspace.getId();
				if (id!=null) {
					buf.append(id);
				}

				if (workspaceAndName.isSelected) {
					buf.append('*');
				} else {
					buf.append(' ');
				}

				System.out.println(buf.toString());
			}

		}
	}

	class ClearModulesRunner implements HillRunner {
		
		@Override
		public void parseArgs(String[] args) {
			
		}
		
		@Override
		public void run() throws Exception {
			WorkspaceHelper helper = new WorkspaceHelper();
			Workspace workspace = helper.loadWorkspace();
			ArrayList<File> modDirs = enlistAllModuleDirs(workspace);
			for(File modDir : modDirs) {
				File file = new File(modDir, "dung");
				if (file.exists()) {
					AtomicInteger counter = new AtomicInteger();
					remove(file, counter);
					System.out.println("removed:"+file.getAbsolutePath());
				}
			}
		}

		
		private ArrayList<File> enlistAllModuleDirs(Workspace workspace) {
			ArrayList<File> enlisted = new ArrayList<File>();
			Path[] modulePaths = workspace.getModulePath();
			for(Path modulePath : modulePaths) {
				enlistModuleDirs(modulePath, enlisted);
			}
			return enlisted;
		}
		
		public void enlistModuleDirs(Path modulePath, ArrayList<File> enlisted) {
			File baseDir = modulePath.toFile();
			if (!baseDir.exists()) {
				return;
			}
			
			File[] listFiles = baseDir.listFiles();
			for(File moduleDir : listFiles) {
				if (moduleDir.isDirectory()) {
					enlisted.add(moduleDir);
				}
			}
		}

		private void remove(File file, AtomicInteger counter) {
			if (file.isDirectory()) {
				File[] files = file.listFiles();
				for(File sfile : files) {
					remove(sfile, counter);
				}
			}
			if (file.delete()) {
				counter.incrementAndGet();
			}
		}

		
	}

	
	
}
