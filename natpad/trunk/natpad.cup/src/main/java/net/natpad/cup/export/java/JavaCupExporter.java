package net.natpad.cup.export.java;

import net.natpad.cup.config.CupConfiguration;
import net.natpad.cup.config.ICupExporter;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.state.StateMachine;

public class JavaCupExporter implements ICupExporter {

	public final JavaExportConfiguration exportConfiguration;
	public final CupConfiguration cupConfigurarion;
	public final JavaExportInfo exportInfo;
	
	public JavaCupExporter(CupConfiguration cupConfigurarion, JavaExportConfiguration exportConfiguration) {
		this.cupConfigurarion = cupConfigurarion;
		this.exportConfiguration = exportConfiguration;
		exportInfo = new JavaExportInfo(exportConfiguration);
	}

	
	@Override
	public void export(BnfModel model, StateMachine stateMachine) {
		Emit emit = new Emit(model, cupConfigurarion, exportInfo);

		try {
			emit.doExport(cupConfigurarion.getExportNonTerms(), exportConfiguration.symbolsAsInterface, stateMachine.action_table, stateMachine.reduce_table, stateMachine.startState.index(),
							exportConfiguration.compactReduceDefault);
		} catch(Exception e) {
			e.printStackTrace();
		}
		
	}
	
	
	@Override
	public boolean testForChanges(long ts) {
		return true;
	}
	
}
