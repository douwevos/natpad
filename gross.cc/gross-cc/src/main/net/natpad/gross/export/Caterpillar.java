package net.natpad.gross.export;

import java.io.File;
import java.io.IOException;

import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.export.glibc.ClassName;

public class Caterpillar implements Exporter {

	
	String exportOutputPath = ".";
	String prefix = "Gross";
	
	
	
	File baseFile;
	ClassName actionClassName;
	ClassName configClassName;

	@Override
	public void setOutputPath(String outputPath) {
		this.exportOutputPath = outputPath;
	}
	
	
	@Override
	public void setProperty(String name, String value) {
		if ("prefix".equals(name)) {
			prefix = value;
		} else {
			throw new RuntimeException("unknown property:"+name);
		}
	}
	
	
	public void applyConfiguration() {
		actionClassName = new ClassName(prefix, "Actions", "actions");
		configClassName = new ClassName(prefix, "ParserConfig", "parser_config");
		baseFile = new File(exportOutputPath);
	}
	
	
	@Override
	public boolean needsUpdate(long lastMod) {
		File fc = new File(baseFile, configClassName.createFilename()+".c");
		if (testFileForUpdate(lastMod, fc)) {
			return true;
		}
		fc = new File(baseFile, configClassName.createFilename()+".h");
		if (testFileForUpdate(lastMod, fc)) {
			return true;
		}
		return false;
	}

	private boolean testFileForUpdate(long lastMod, File fc) {
		if (!fc.exists()) {
			return true;
		}
		if (fc.lastModified()<lastMod) {
			return true;
		}
		return false;
	}
	
	@Override
	public void doExport(CCStateMachine stateMachine, AstFile astFile) {
		CaterpillarExporter caterpillarExporter = new CaterpillarExporter(stateMachine, astFile, configClassName, actionClassName);
		try {
			caterpillarExporter.runExport(new File(exportOutputPath));
		} catch (IOException e) {
			e.printStackTrace();
			throw new RuntimeException("aborted");
		}
	}
	
}
