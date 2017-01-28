package net.natpad.gross.export;

import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.build.CCStateMachine;

public interface Exporter {

	void setOutputPath(String outputPath);
	void setProperty(String name, String value);
	void applyConfiguration();

	boolean needsUpdate(long lastMod);
	
	void doExport(CCStateMachine stateMachine, AstFile astFile);

}
