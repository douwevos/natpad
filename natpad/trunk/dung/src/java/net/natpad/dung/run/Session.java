package net.natpad.dung.run;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.eprops.EPropsContext;
import net.natpad.dung.eprops.EPropsResolved;
import net.natpad.dung.eprops.IEPropertyResolver;
import net.natpad.dung.expression.ExpressionScanner;
import net.natpad.dung.expression.generated.DungExprParser;
import net.natpad.dung.expression.generated.runtime.LrParserContext;
import net.natpad.dung.expression.generated.runtime.LrScanner;
import net.natpad.dung.expression.generated.runtime.LrSymbol;
import net.natpad.dung.module.Module;
import net.natpad.dung.module.Target;
import net.natpad.dung.module.task.types.FileBundle;
import net.natpad.dung.module.task.types.PathInPath;
import net.natpad.dung.workspace.Workspace;

public class Session implements IEPropertyResolver {

	public final Context context;
	public final Module module;
	
//	private final File buildDir;

	public EPropsContext properties;
	public EPropsResolved propsResolved;
	
	public Session(Context context, Module module) throws IOException {
		this.context = context;
		this.module = module;
//		buildDir = new File(module.directory, "bob").getCanonicalFile();
		properties = new EPropsContext(module.moduleFileDescr.getModuleDescr().propsContext);
		properties.put("workspace.workdir", context.workspaceWorkDir.toString());
		propsResolved = new EPropsResolved(properties, this);
	}

	public File getModuleDirectory() {
		return module.directory;
	}

//	public File getModuleBuildDirectory() {
//		return buildDir;
//	}

	
	
	public void build(List<String> targetsToRun) throws Exception {
		for (String target : targetsToRun) {
			runTarget(target);
		}
	}

	public Workspace getWorkspace() {
		return context.workspace;
	}


	private void runTarget(String name) throws Exception {
		try {
			module.ensureFullyMapped(context);
		} catch(Throwable t) {
			throw new RuntimeException("While mapping module "+module.moduleFileDescr.module.name, t);
		}
		Target target = module.getTarget(name);
		if (target==null) {
			return;
		}
		System.out.println();
		System.out.println((char) 27 +"[1;36m" + module.moduleFileDescr.getModuleDescr().name+"."+name + (char) 27 +"[0m" );
		target.run(this);
	}



	public Path createModulePath(String dir) throws IOException {
		dir = resolveProperties(dir);
		File moduleDirectory = getModuleDirectory().getCanonicalFile();
		Path path = Paths.get(dir);
		if (!path.isAbsolute()) {
			Path path2 = moduleDirectory.toPath();
			path = path2.resolve(path).normalize();
		}
		return path;
	}	
	
	public Path createBuildPath(String dir) {
		dir = resolveProperties(dir);
		Path pathOut = Paths.get(dir);
		if (!pathOut.isAbsolute()) {
			File baseBuildDir = module.buildDir;
			baseBuildDir.mkdir();
			Path path2 = baseBuildDir.toPath();
			pathOut = path2.resolve(pathOut).normalize();
		}
		return pathOut;
	}



	public PathInPath[] scanBundles(File defaultDir, List<FileBundle> fileBundles, boolean relative) {
		List<PathInPath> result = new ArrayList<PathInPath>();
		for (FileBundle fileBundle : fileBundles) {
//			System.out.println("fileBundle="+fileBundle);
			scanBundle(fileBundle, result, defaultDir, relative);
		}
		return result.toArray(new PathInPath[result.size()]);
	}
	
	
	private void scanBundle(FileBundle fileBundle, List<PathInPath> result, File defaultDir, boolean relative) {
		String rpath = resolveProperties(fileBundle.path);
//		System.out.println("rpath="+rpath);
		Path bundlePath = Paths.get(rpath);
		if (!bundlePath.isAbsolute()) {
			Path path2 = defaultDir.toPath();
			bundlePath = path2.resolve(bundlePath).normalize();
		}
		File file = bundlePath.toFile();
//		System.out.println("scanning file:"+file);
		if (file.isFile()) {
//			if (fileBundle.match(""));
		} else {
			scanBundleDirectory(fileBundle, result, bundlePath, file, relative);
		}
	}



	private void scanBundleDirectory(FileBundle fileBundle, List<PathInPath> result, Path bundlePath, File dir2scan, boolean relative) {
		File[] listFiles = dir2scan.listFiles();
		if (listFiles!=null) {
			for (File file : listFiles) {
				Path relativized = Paths.get(file.getAbsolutePath());
				Path base = null;
				if (relative) {
					relativized = bundlePath.relativize(relativized);
					base = bundlePath;
				}
				if (!fileBundle.match(base, relativized, file)) {
					continue;
				}
				
				if (file.isDirectory()) {
					result.add(new PathInPath(base, relativized, false));
					scanBundleDirectory(fileBundle, result, bundlePath, file, relative);
				} else {
					result.add(new PathInPath(base, relativized, true));
				}
			}
		}
	}



	public String[] scanDirectory(File directory, boolean relative) {
		List<String> result = new ArrayList<String>();
		Path basePath = Paths.get(directory.getAbsolutePath());
		scanDirectoryRecursive(basePath, directory, result, relative);
		String r[] = new String[result.size()];
		r = result.toArray(r);
		return r;
	}

	private void scanDirectoryRecursive(Path basePath, File dir2scan, List<String> output, boolean relative) {
		File[] listFiles = dir2scan.listFiles();
		if (listFiles!=null) {
			for (File file : listFiles) {
				if (file.isDirectory()) {
					scanDirectoryRecursive(basePath, file, output, relative);
				} else {
					String absolutePath = file.getAbsolutePath();
					if (relative) {
						Path relativized = basePath.relativize(Paths.get(absolutePath));
						absolutePath = relativized.toString();
					}
					output.add(absolutePath);
				}
			}
		}
	}

	public String resolveProperties(String text) {
		String result = propsResolved.replaceProperties(text);
//		System.out.println("resolveProperties:text="+text+", result="+result);
		return result;
	}

	
	@Override
	public String resolveExpression(String expressionAsText) {
		DungExprParser parser = new DungExprParser();
		LrScanner scanner;
		try {
			scanner = new ExpressionScanner(new StringReader(expressionAsText));
			LrParserContext parserContext =  new DungExprHelper(scanner , this);
			LrSymbol parsed = parser.parse(parserContext);
			return parsed.value==null ? null : parsed.value.toString();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		throw new RuntimeException("parser error:"+expressionAsText);
	}

	
}
