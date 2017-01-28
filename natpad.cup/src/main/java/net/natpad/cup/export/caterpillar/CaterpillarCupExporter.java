package net.natpad.cup.export.caterpillar;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.HashMap;

import net.natpad.cup.config.CupConfiguration;
import net.natpad.cup.config.ICupExporter;
import net.natpad.cup.export.PredefinedExporter;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;
import net.natpad.cup.state.StateMachine;

public class CaterpillarCupExporter implements ICupExporter {

	private static final String predefined[] = { "ccup2darray.c", "ccup2darray.h", "ccupiscanner.c", "ccupiscanner.h",
			"ccupparserbase.c", "ccupparserbase.h", "ccupparsercontext.c", "ccupparsercontext.h", "ccuptoken.c",
			"ccuptoken.h", "ccupvirtualparsestack.c", "ccupvirtualparsestack.h" };

	public final CupConfiguration cupConfiguration;
	public final CaterpillarConfiguration configuration;

	public CaterpillarCupExporter(CupConfiguration cupConfiguration, CaterpillarConfiguration configuration) {
		this.cupConfiguration = cupConfiguration;
		this.configuration = configuration;
	}

	
	@Override
	public void export(BnfModel model, StateMachine stateMachine) {

		Production startProduction = model.getStartProduction();
		SymbolPart rhs = (SymbolPart) startProduction.rhs(0);
		NonTerminal nt = (NonTerminal) rhs.getSymbol();
		String stackType = nt.getStackType();
		if (stackType==null || "Object".equals(stackType)) {
			stackType = "GObject";
		}
		model.START_nt.setStackType(stackType);

		configuration.destDir.mkdirs();

		System.out.println("Exporting Caterpillar: destination-directory=" + configuration.destDir.getAbsolutePath());
		try {
			ParserExporter parserExporter = new ParserExporter(model, configuration);
			parserExporter.doExport(stateMachine.action_table, stateMachine.reduce_table,
					stateMachine.startState.index(), model.getStartProduction(), configuration.compactReduceDefault);

			ActionsExporter actionsExporter = new ActionsExporter(model, configuration);
			actionsExporter.doExport(stateMachine.action_table, stateMachine.reduce_table,
					stateMachine.startState.index(), model.getStartProduction(), configuration.compactReduceDefault);

			SymbolExporter symbolExporter = new SymbolExporter(model, configuration);
			symbolExporter.doExport();

			String prefix = configuration.namespacePrefix.toLowerCase();
			HashMap<String, String> tokenMap = new HashMap<String, String>();
			tokenMap.put("pre", configuration.namespacePrefix.toLowerCase());
			tokenMap.put("PRE", configuration.namespacePrefix.toUpperCase());
			tokenMap.put("Pre", configuration.namespacePrefix);
			tokenMap.put("d", "[%d]");

			tokenMap.put("symbol.name.lower", configuration.getSymbolNameLower());

			
			if (configuration.symbolInfo) {
				tokenMap.put("symbol.info", "true");
			}

			if (configuration.outputNonTerminals) {
				tokenMap.put("output.non.terminals", "true");
			}

			
			if (configuration.extraDebugInfo) {
				tokenMap.put("debug", "true");
			}

			tokenMap.put("debug.level", configuration.debugLevel);

			File runtimeDir = new File(configuration.destDir, "runtime");
			runtimeDir.mkdirs();

			for (String fname : predefined) {
				InputStream stream = this.getClass().getResourceAsStream("runtime/" + fname);
				File outputFile = new File(runtimeDir, prefix + fname.substring(4));
				System.err.println("outputFile=" + outputFile);
				new PredefinedExporter(new InputStreamReader(stream), outputFile, tokenMap);
			}

//			File astDir = new File(configuration.destDir, "ast");
//			astDir.mkdirs();
//
//			int rhsCount = startProduction.rhsLength();
//			for (int idx = 0; idx < rhsCount; idx++) {
//				ProductionPart rhs2 = startProduction.rhs(idx);
//				System.out.println("rhs2=" + rhs2);
//				if (rhs2.isAction()) {
//					continue;
//				}
//				SymbolPart sp = (SymbolPart) rhs2;
//				if (sp.getSymbol().is_non_term()) {
//					System.out.println("sp=" + sp);
//
//					createNonTerminalAstClass(astDir, tokenMap, sp.getSymbol());
//
//				}
//			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public boolean testForChanges(long ts) {
		File runtimeDir = new File(configuration.destDir, "runtime");
		if (!runtimeDir.exists()) {
			return true;
		}
		String prefix = configuration.namespacePrefix.toLowerCase();
		for (String fname : predefined) {
			File outputFile = new File(runtimeDir, prefix + fname.substring(4));
			if (!outputFile.exists() || outputFile.lastModified()<ts) {
				return true;
			}
		}
		
		String n_Sta = configuration.namespacePrefix;			// Sta
		String n_sta = n_Sta.toLowerCase();					// sta
		File file = new File(configuration.destDir, n_sta+configuration.getParserNameStripper()+".c");
		if (!file.exists() || file.lastModified()<ts) {
			return true;
		}
		file = new File(configuration.destDir, n_sta+configuration.getParserNameStripper()+".h");
		if (!file.exists() || file.lastModified()<ts) {
			return true;
		}
		return false;
	}
	

	@SuppressWarnings("resource")
	private void createNonTerminalAstClass(File astDir, HashMap<String, String> tokenMap, Symbol symbol)
			throws IOException {
		String name = symbol.name();
		String fname = tokenMap.get("pre") + name;

		System.out.println("fname=" + fname);

		String[] uName = extractFromUName(tokenMap.get("pre"), tokenMap.get("PRE"), tokenMap.get("Pre"), name);
		
		PrintWriter writer = new PrintWriter(new File(astDir, fname + ".c"));
		writer.println("#include \""+fname+".h\"");
		writer.println();
		writer.println("#include <logging/catlogdefs.h>");
		writer.println("#define CAT_LOG_LEVEL CAT_LOG_WARN");
		writer.println("#define CAT_LOG_CLAZZ \""+uName[5]+"\"");
		writer.println("#include <logging/catlog.h>");
		writer.println("");
		writer.flush();
		writer.close();
	}

	/**
	 * 
	 * string[0] = foo_bar string[1] = FOO_BAR string[2] = FooBar string[3] =
	 * pre_foo_base string[4] = PRE_FOO_BAR string[5] = PreFooBar
	 * 
	 * @param pre
	 * @param PRE
	 * @param Pre
	 * @param uname
	 * @return
	 */
	private String[] extractFromUName(String pre, String PRE, String Pre, String uname) {
		String result[] = new String[6];
		result[0] = uname;
		result[1] = uname.toUpperCase();
		StringBuilder buf = new StringBuilder();
		boolean nextUp = true;
		for (int idx = 0; idx < uname.length(); idx++) {
			char ch = uname.charAt(idx);
			if (ch == '_') {
				nextUp = true;
			} else if (nextUp) {
				nextUp = false;
				buf.append(Character.toUpperCase(ch));
			} else {
				buf.append(Character.toLowerCase(ch));
			}
		}
		result[2] = buf.toString();
		result[3] = pre == null ? result[0] : pre + "_" + result[0];
		result[4] = PRE == null ? result[1] : PRE + "_" + result[1];
		result[5] = Pre == null ? result[2] : Pre + result[2];

		return result;
	}

}
