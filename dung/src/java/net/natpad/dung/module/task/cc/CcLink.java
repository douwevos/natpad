package net.natpad.dung.module.task.cc;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class CcLink  {
//
//	private Path src;
//	private File output;
//    protected CcConfigSet linkerConfigSet = new CcConfigSet();
//
//	
//
//	public void setSrcdir(Path srcDir) {
//		if (src == null) {
//			src = srcDir;
//		} else {
//			src.append(srcDir);
//		}
//	}
//
//	public Path getSrcdir() {
//		return src;
//	}
//
//	public void setOutput(File output) {
//		this.output = output;
//	}
//
//	public File getOutput() {
//		return output;
//	}
//
//	
//
//    public void add(CcConfigSet configSet) {
//    	linkerConfigSet.add(configSet);
//    }
//
//    public void add(CcArg arg) {
//    	linkerConfigSet.add(arg);
//    }
//
//    
//	public void add(PkgConfig path) {
//		linkerConfigSet.add(path);
//	}
//    
//    public void add(CcLinkerPath linkerPath) {
//    	linkerConfigSet.add(linkerPath);
//    }
//    
//	public void add(CcLibraryPath path) {
//		linkerConfigSet.add(path);
//	}
//
//    public void add(CcStripArgument strip) {
//    	linkerConfigSet.add(strip);
//    }
//
//
//	public void add(CcLibraryName name) {
//		linkerConfigSet.add(name);
//	}
//
//	
//	@Override
//	public void execute() throws BuildException {
//		// checkParameters();
//
//    	CcLinkerSettings linkerSettings = new CcLinkerSettings();
//
//		// scan source directories and dest directory to build up
//		// compile lists
//		String[] list = src.list();
//		for (int i = 0; i < list.length; i++) {
//			File srcDir = getProject().resolveFile(list[i]);
//			if (!srcDir.exists()) {
//				throw new BuildException("srcdir \"" + srcDir.getPath()
//						+ "\" does not exist!", getLocation());
//			}
//
//			DirectoryScanner ds = this.getDirectoryScanner(srcDir);
//			String[] files = ds.getIncludedFiles();
//			for (int idx = 0; idx < files.length; idx++) {
//	    		File sourceFile = new File(srcDir, files[idx]);
//				try {
//					linkerSettings.addObjectFile(sourceFile.getCanonicalPath());
//				} catch (IOException e) {
//					throw new BuildException(e);
//				}
//			}
//		}
//		
//
//		link(linkerSettings);
//	}
//
//	private void link(CcLinkerSettings linkerSettings) {
//    	ArrayList<ICcConfigItem> flatten = linkerConfigSet.flatten(null);
//    	
//    	
//    	for(ICcConfigItem ccitem : flatten) {
//    		ccitem.setup(linkerSettings);
//    	}
//    	
//    	ArrayList<String> argList = linkerSettings.getExecAsArgList();
//    	
//    	
//    	
//    	
//    	argList.add("-o"+output.getAbsolutePath());
//    	
//
//		String[] optflat = argList.toArray(new String[argList.size()]);
//		StringBuilder b = new StringBuilder();
//		for(String opt : optflat) {
//			b.append(" "+opt);
//		}
//		log("::"+b);
//		
//		try {
//			Runtime runtime = Runtime.getRuntime();
//			Process process = runtime.exec(optflat);
//			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
//			errstreamer.launch();
//			ProcessInputStreamer outstreamer = new ProcessInputStreamer(process.getInputStream());
//			outstreamer.launch();
//			
//			int exitcode = process.waitFor();
//			if (exitcode!=0) {
//				for(String line : outstreamer.output) {
//					System.err.println(line);
//				}
//				for(String line : errstreamer.output) {
//					System.err.println(line);
//				}
//				System.err.println("Dumping arguments");
//				for(int ia=0; ia<optflat.length; ia++) {
//					System.err.println(" ["+ia+"]="+optflat[ia]);
//				}
//				
//				throw new BuildException("cc compile returned "+exitcode);
//				
//			}
//		} catch (InterruptedException e) {
//			e.printStackTrace();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}    	
//		
//	}
//
}
