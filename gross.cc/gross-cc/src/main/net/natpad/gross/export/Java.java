package net.natpad.gross.export;

import java.io.File;
import java.io.IOException;

import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.build.CCStateMachine;

public class Java implements Exporter {


	String packageName = "";
	String prefix = "Gross";
	String outputPath;
	
	@Override
	public void setOutputPath(String outputPath) {
		this.outputPath = outputPath;
	}

	
	
	@Override
	public void setProperty(String name, String value) {
		if ("prefix".equals(name)) {
			prefix = value;
		} else if ("package".equals(name)) {
			packageName = value;
		} else {
			throw new RuntimeException("unknown property:"+name);
		}
	}

	@Override
	public void applyConfiguration() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public boolean needsUpdate(long lastMod) {
		// TODO Auto-generated method stub
		return true;
	}

	
	@Override
	public void doExport(CCStateMachine stateMachine, AstFile astFile) {
		JavaExporter javaExporter = new JavaExporter(stateMachine, astFile, packageName, prefix);
		File base = new File(outputPath);
		try {
			javaExporter.runExport(base);
		} catch (IOException e) {
			e.printStackTrace();
			throw new RuntimeException("aborted");
		}
	}

}
