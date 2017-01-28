package net.natpad.cup.export.java;

import java.io.File;

import net.natpad.cup.export.java.ast.AstQualifiedName;

public class JavaExportConfiguration {

	protected File destDir = new File("./src/generated2/");
	
	protected AstQualifiedName basePackage = new AstQualifiedName("org.natpad.test");
	
	protected String symbolClassName = "TestSymbol";
	protected String parserClassName = "TestParser";
	
	protected boolean exportLeftRight = false;
	
	/** User option -- should symbols be put in a class or an interface? [CSA] */
	public boolean symbolsAsInterface = false;

	/**
	 * User option -- do we compact tables by making most common reduce the
	 * default action
	 */
//	public boolean opt_compact_red = false;
	public boolean compactReduceDefault = false;

	
	public File getDestinationDir() {
		return destDir;
	}
	
	public String getSymbolClassName() {
		return symbolClassName;
	}
	
	public AstQualifiedName getBasePackage() {
		return basePackage;
	}

	public String getParserClassName() {
		return parserClassName;
	}
	
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append("destDir=").append(destDir.getAbsolutePath());
		buf.append(", base-package=").append(basePackage.getFullQualifiedName());
		buf.append(", symbol-class-name=").append(symbolClassName);
		buf.append(", parser-class-name=").append(parserClassName);
		return "JavaExportConfiguration["+buf+"]";
	}
}
