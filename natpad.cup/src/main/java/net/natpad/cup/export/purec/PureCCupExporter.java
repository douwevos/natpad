package net.natpad.cup.export.purec;

import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;

import net.natpad.cup.config.CupConfiguration;
import net.natpad.cup.config.ICupExporter;
import net.natpad.cup.export.PredefinedExporter;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.state.StateMachine;

public class PureCCupExporter implements ICupExporter {

	private static final String predefined[] = {
		"ccup2darray.c",
		"ccup2darray.h",
		"ccupiscanner.c",
		"ccupiscanner.h",
		"ccupparserbase.c",
		"ccupparserbase.h",
		"ccupparsercontext.c",
		"ccupparsercontext.h",
		"ccuptoken.c",
		"ccuptoken.h",
		"ccupvector.c",
		"ccupvector.h",
		"ccupvirtualparsestack.c",
		"ccupvirtualparsestack.h"
	};
	
	public final CupConfiguration cupConfiguration;
	public final PureCConfiguration configuration;
	
	public PureCCupExporter(CupConfiguration cupConfiguration, PureCConfiguration configuration) {
		this.cupConfiguration = cupConfiguration;
		this.configuration = configuration;
	}
	
	@Override
	public void export(BnfModel model, StateMachine stateMachine) {
		
		configuration.destDir.mkdirs();
		
		System.out.println("Exporting Caterpillar: destination-directory="+configuration.destDir.getAbsolutePath());
		try {
			ParserExporter parserExporter = new ParserExporter(model, configuration);
			parserExporter.doExport(stateMachine.action_table, stateMachine.reduce_table, stateMachine.startState.index(), model.getStartProduction(), configuration.compactReduceDefault);

		
			ActionsExporter actionsExporter = new ActionsExporter(model, configuration);
			actionsExporter.doExport(stateMachine.action_table, stateMachine.reduce_table, stateMachine.startState.index(), model.getStartProduction(), configuration.compactReduceDefault);

			SymbolExporter symbolExporter = new SymbolExporter(model, configuration);
			symbolExporter.doExport();

			
			
			String prefix = configuration.namespacePrefix.toLowerCase();
			HashMap<String, String> tokenMap = new HashMap<String, String>();
			tokenMap.put("pre", configuration.namespacePrefix.toLowerCase());
			tokenMap.put("PRE", configuration.namespacePrefix.toUpperCase());
			tokenMap.put("Pre", configuration.namespacePrefix);
			tokenMap.put("d", "[%d]");
			
			if (configuration.extraDebugInfo) {
				tokenMap.put("debug", "true");
			}
			tokenMap.put("debug.level", configuration.debugLevel); 
			
			File runtimeDir = new File(configuration.destDir, "runtime");
			runtimeDir.mkdirs();
			
			for(String fname : predefined) {
				InputStream stream = this.getClass().getResourceAsStream("runtime/"+fname);
				File outputFile = new File(runtimeDir, prefix+fname.substring(4));
				System.err.println("outputFile="+outputFile);
				new PredefinedExporter(new InputStreamReader(stream), outputFile, tokenMap);
			}
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	@Override
	public boolean testForChanges(long ts) {
		return true;
	}
	
}
