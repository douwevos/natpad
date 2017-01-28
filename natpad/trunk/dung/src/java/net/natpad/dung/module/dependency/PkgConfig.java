package net.natpad.dung.module.dependency;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.StringLiteral;
import net.natpad.dung.module.IDependency;
import net.natpad.dung.module.task.cc.CcCompilerSettings;
import net.natpad.dung.module.task.cc.CcLinkerSettings;
import net.natpad.dung.module.task.cc.ICcConfigItem;
import net.natpad.dung.run.ProcessInputStreamer;
import net.natpad.dung.run.Session;
import net.natpad.dung.workspace.Workspace;


public class PkgConfig implements ICcConfigItem, IDependency {

	private String pkgname;
	private String pkgConfigSysRoot = null;
	private String pkgConfigPath = null;
//	private boolean pkgnameResolved = false;

	public PkgConfig() {
		super();
	}
	
	public void setpackage(String pkgname) {
		this.pkgname = pkgname;
	}
	
	public void setPkgConfigSysRoot(String configSysRoot) {
		pkgConfigSysRoot = configSysRoot;
	}

	public void setPkgConfigPath(String configPath) {
		pkgConfigPath = configPath;
	}

	
	private String executeAndGetOutput(List<String> argList, String envargs[]) {
		Runtime runtime = Runtime.getRuntime();
		String[] args = argList.toArray(new String[argList.size()]);
		try {
//			for(String argje : args) {
//				log("arg:"+argje);
//			}
//			for(String argje : envargs) {
//				log("env:"+argje);
//			}
			Process process = runtime.exec(args, envargs);
			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
			Thread errthread = errstreamer.launch();
			ProcessInputStreamer outstreamer = new ProcessInputStreamer(process.getInputStream());
			Thread outthread = outstreamer.launch();
			int exitcode = process.waitFor();

			errthread.join();
			outthread.join();
			
			
			for(String line : errstreamer.output) {
				System.err.println(line);
				System.err.println("error: "+line);
			}

			
			StringBuilder buf = new StringBuilder();
			for(String line : outstreamer.output) {
				if (buf.length()>0) {
					buf.append("\n");
				}
				buf.append(line);
			}
			if (exitcode!=0) {
				System.out.println(""+buf);
				throw new RuntimeException("pkg-config failed with exitcode:"+exitcode+", pkgname="+pkgname);
			}
//			log("buf.toString()="+buf.toString());
			return buf.toString();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}


	
	
	private String callPkgConfig(Session session, List<String> arguments) {
		ArrayList<String> envs = new ArrayList<String>();
		if (pkgConfigSysRoot != null) {
			envs.add("PKG_CONFIG_SYSROOT_DIR=" + pkgConfigSysRoot);
//			System.out.println("export PKG_CONFIG_SYSROOT_DIR=" + pkgConfigSysRoot);
		} else {
			Workspace workspace = session.getWorkspace();
			String ppkgConfigSysRoot = workspace.getProperty("PkgConfigSysRoot");
			if (ppkgConfigSysRoot != null) {
				envs.add("PKG_CONFIG_SYSROOT_DIR=" + ppkgConfigSysRoot);
//				System.out.println("export PKG_CONFIG_SYSROOT_DIR=" + ppkgConfigSysRoot);
			}
		}
		if (pkgConfigPath != null) {
			envs.add("PKG_CONFIG_PATH=" + pkgConfigPath);
//			System.out.println("export PKG_CONFIG_PATH=" + pkgConfigPath);
		} else { 
			Workspace workspace = session.getWorkspace();
			String ppkgConfigPath = workspace.getProperty("PkgConfigPath");
			if (ppkgConfigPath != null) {
				envs.add("PKG_CONFIG_PATH=" + ppkgConfigPath);
//				System.out.println("export PKG_CONFIG_PATH=" + ppkgConfigPath);
			}
		}
		String envargs[] = new String[envs.size()];
		envs.toArray(envargs);

		String pkgConfigBin = null;
//		if (getProject()!=null) {
//			pkgConfigBin = getProject().getProperty("pkg.config.bin");
//		}
		if (pkgConfigBin == null) {
			pkgConfigBin = "/usr/bin/pkg-config";
		}
		if (!new File(pkgConfigBin).exists()) {
			throw new RuntimeException("pkg-config path : "+pkgConfigBin+" does not exist");
		}
		
		List<String> argList = new ArrayList<String>();
		argList.add(pkgConfigBin);
		argList.addAll(arguments);
//		String p= "";
//		for(String s : argList) {
//			p = p + " " +s;
//		}
//		System.out.println("PkgConfig executing:"+p);
		String res = executeAndGetOutput(argList, envargs);
//		log("result:"+res);
		return res;
	}
	
	
	
	@SuppressWarnings("unchecked")
	private void parseCompilerFlags(CcCompilerSettings compilerSettings, String pkgtext) {
		
		String[] split = pkgtext.split(" ");
		
		for(String iitem : split) {
			if (iitem.startsWith("-I")) {
//				Path path = new Path(getProject());
//				PathElement pathElement = path.createPathElement();
//				pathElement.setLocation(new File(iitem.substring(2)));
//				
//				for(String ip : path.list()) {
				String ip = iitem.substring(2);
					compilerSettings.addArgumentOnce("-I"+ip, false);
//				}
//				log("adding:"+iitem.substring(2), Project.MSG_VERBOSE);
			} else {
				iitem = iitem.trim();
				if (iitem.length()>0) {
					compilerSettings.addArgument(iitem);
				}
			}
		}
	}

	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
//        if (isReference()) {
//            ((PkgConfig) getCheckedRef()).setup(compilerSettings);
//        }
//		
		List<String> argList = new ArrayList<String>();
		argList.add("--cflags");
		argList.add(pkgname);
		
		String pkgtext = callPkgConfig(session, argList);
		parseCompilerFlags(compilerSettings, pkgtext);
		
	}
	
	

	@SuppressWarnings("unchecked")
	private void parseLinkerFlags(CcLinkerSettings linkerSettings, String pkgtext) {
		String[] list = pkgtext.split(" ");
		for(String iitem : list) {
			if (iitem.startsWith("-l")) {
				linkerSettings.addLibName(iitem.substring(2));
			} else if (iitem.startsWith("-L")) {
				
				linkerSettings.addLibSearchPath(iitem.substring(2));
//				log("adding:"+iitem.substring(2), Project.MSG_VERBOSE);
			}
		}
	}

	
	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
//        if (isReference()) {
//            ((PkgConfig) getCheckedRef()).setup(linkerSettings);
//        }
		List<String> argList = new ArrayList<String>();
		argList.add("--libs");
//		if (linkStatic) {
//		argList.add("--static");
//	}
		argList.add(pkgname);
		
		String pkgtext = callPkgConfig(session, argList);
		parseLinkerFlags(linkerSettings, pkgtext);
	}
	
	public IDependency[] children() {
		return null;
	};
	
	
	@Override
	public IExpressionValue getById(Object id) {
		if (id instanceof String) {
			String st = (String) id;
			if ("package".equals(st)) {
				return new StringLiteral(pkgname);
			}
		}
		return null;
	}
	
	@Override
	public String description() {
		return "PkgConfig[package="+pkgname+"]";
	}
}
