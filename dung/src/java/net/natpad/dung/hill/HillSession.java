package net.natpad.dung.hill;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.nio.file.Path;
import java.nio.file.Paths;

import net.natpad.dung.WorkspaceHelper;
import net.natpad.dung.eprops.EPropsContext;
import net.natpad.dung.eprops.EPropsResolved;
import net.natpad.dung.eprops.IEPropertyResolver;
import net.natpad.dung.expression.ExpressionScanner;
import net.natpad.dung.expression.generated.DungExprParser;
import net.natpad.dung.expression.generated.runtime.LrParserContext;
import net.natpad.dung.expression.generated.runtime.LrScanner;
import net.natpad.dung.expression.generated.runtime.LrSymbol;
import net.natpad.dung.workspace.Workspace;

public class HillSession implements IEPropertyResolver {

	WorkspaceHelper helper = new WorkspaceHelper();

	public final Workspace workspace;
	
	public EPropsContext properties;
	public EPropsResolved propsResolved;

	
	public HillSession(Workspace workspace) {
		this.workspace = workspace;

		properties = new EPropsContext(null);
		properties.put("workspace.workdir", getWorkDir().toString());
		propsResolved = new EPropsResolved(properties, this);

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
			LrParserContext parserContext =  new HillExprHelper(scanner , this);
			LrSymbol parsed = parser.parse(parserContext);
			return parsed.value==null ? null : parsed.value.toString();
		} catch (Exception e) {
			e.printStackTrace();
		}
		throw new RuntimeException("parser error:"+expressionAsText);
	}


	public File getWorkDir() {
		return helper.getWorkDir(workspace);
	}
	
	
	public Path createWorkPath(String dir) throws IOException {
		dir = resolveProperties(dir);
		File workDirectory = getWorkDir().getCanonicalFile();
		Path path = Paths.get(dir);
		if (!path.isAbsolute()) {
			Path path2 = workDirectory.toPath();
			path = path2.resolve(path).normalize();
		}
		return path;
	}	

	
}
