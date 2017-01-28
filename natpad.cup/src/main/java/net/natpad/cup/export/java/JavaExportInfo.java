package net.natpad.cup.export.java;

import java.io.File;

import net.natpad.cup.export.java.ast.AstClassName;
import net.natpad.cup.export.java.ast.AstQualifiedName;

public class JavaExportInfo {

	public final File destDir;
	
	public final AstQualifiedName basePackage;

	public final AstClassName symbolClass;
	public final AstClassName parserClass;

	public final AstQualifiedName runtimePackage;
	public final AstClassName parserRuntimeClass;
	public final AstClassName scannerRuntimeClass;
	public final AstClassName symbolRuntimeClass;
	public final AstClassName virtParStackRuntimeClass;
	public final AstClassName actionClass;
	public final AstClassName contextRuntimeClass;
	
	
	public JavaExportInfo(JavaExportConfiguration configuration) {
		destDir = configuration.getDestinationDir();
		basePackage = configuration.getBasePackage();
		symbolClass = createClass(basePackage, configuration.getSymbolClassName());
		parserClass = createClass(basePackage, configuration.getParserClassName());
		actionClass = createClass(basePackage, "Actions");

		runtimePackage = new AstQualifiedName(basePackage.getFullQualifiedName()+".runtime");
		parserRuntimeClass = createClass(runtimePackage, "LrParser");
		scannerRuntimeClass = createClass(runtimePackage, "LrScanner");
		symbolRuntimeClass = createClass(runtimePackage, "LrSymbol");
		virtParStackRuntimeClass = createClass(runtimePackage, "LrVirtualParseStack");
		contextRuntimeClass = createClass(runtimePackage, "LrParserContext");
	}
	
	public AstClassName createClass(AstQualifiedName packageName, String className) {
		if (className.lastIndexOf('.')>=0) {
			return new AstClassName(className);
		} else {
			return new AstClassName(packageName, className);
		}
	}
	
	
	protected String prefix(String txt) {
		return "cup"+txt;
	}

}
