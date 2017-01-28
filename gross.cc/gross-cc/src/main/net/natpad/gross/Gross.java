package net.natpad.gross;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import net.natpad.gross.ast.AstScanner;
import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.parser.Actions;
import net.natpad.gross.build.parser.GrossParserTables;
import net.natpad.gross.definition.GrossScanner;
import net.natpad.gross.definition.bnf.Specification;
import net.natpad.gross.export.Exporter;
import net.natpad.gross.runtime.DefaultLogger;
import net.natpad.gross.runtime.Fork;
import net.natpad.gross.runtime.GrossLogger.Level;
import net.natpad.gross.runtime.FullTokenFactory;
import net.natpad.gross.runtime.Message;
import net.natpad.gross.runtime.Parser;
import net.natpad.gross.runtime.ParserContext;
import net.natpad.gross.runtime.Scanner;
import net.natpad.gross.runtime.StateModel;

public class Gross {

	
	boolean optUpdateOnly = false;
	String grammarFileName;
	String exportOutputPath;
	String prefix;
	Exporter exporter = null;
	DefaultLogger logger = new DefaultLogger(new PrintWriter(System.out));
	String astFileName = null;
	
	public static void main(String[] args) {
		Gross gross = new Gross();
		gross.doRun(args);
	}

	@SuppressWarnings("resource")
	private void doRun(String[] args) {
		parseArgs(args);

		try {
			
			exporter.setOutputPath(exportOutputPath);
			exporter.applyConfiguration();
			File grammarFile = new File(grammarFileName);
			long lastMod = grammarFile.lastModified();
			if (optUpdateOnly) {
				if (!exporter.needsUpdate(lastMod)) {
					System.out.println("no changes in "+grammarFile.getName());
					return;
				}
			}
			
			Specification specification = parseGrammarFile(grammarFile);

			CCModel buildModel = specification.createModel();
			CCStateMachine stateMachine = new CCStateMachine(logger, buildModel);
			
			stateMachine.build();

			AstFile astFile = null;
			if (astFileName!=null) {
				astFile = parseAstFile(astFileName);
			}
			
			exporter.doExport(stateMachine, astFile);
			
//			CaterpillarExporter exporter = new CaterpillarExporter(stateMachine, prefix);
//			exporter.runExport(new File(exportOutputPath));
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

	private AstFile parseAstFile(String astFileName) throws IOException {
		FileReader reader = new FileReader(astFileName);
		net.natpad.ast.build.parser.GrossParserTables parserTables = new net.natpad.ast.build.parser.GrossParserTables();
		StateModel stateModel = new StateModel(parserTables.symbols, parserTables.cfgText, parserTables.stateCount);
		FullTokenFactory tokenFactory = new FullTokenFactory();
		AstScanner scanner = new AstScanner(tokenFactory, reader, stateModel);

		net.natpad.ast.build.parser.Actions actionHandler = new net.natpad.ast.build.parser.Actions();
		
		ParserContext parserContext = new ParserContext(tokenFactory, stateModel, actionHandler);
		return runThroughParser(scanner, parserContext);
	}

	private Specification parseGrammarFile(File grammarFile) throws FileNotFoundException, IOException {
		FileReader reader = new FileReader(grammarFile);
		GrossParserTables parserTables = new GrossParserTables();
		StateModel stateModel = new StateModel(parserTables.symbols, parserTables.cfgText, parserTables.stateCount);

		FullTokenFactory tokenFactory = new FullTokenFactory();
		GrossScanner scanner = new GrossScanner(tokenFactory, reader, stateModel);

		Actions actionHandler = new Actions();
		
		ParserContext parserContext = new ParserContext(tokenFactory, stateModel, actionHandler);

		return runThroughParser(scanner, parserContext);
	}

	private <T> T runThroughParser(Scanner scanner, ParserContext parserContext) {
		Parser parser = new Parser(logger);
		T parserResult = parser.parse(parserContext, scanner);
		List<Fork> acceptedForks = parserContext.getAcceptedForks();
		if (acceptedForks==null || acceptedForks.isEmpty()) {
			if (parserContext.errorForkCount()>0) {
				Fork fork = parserContext.errorForkAt(0);
				if (fork.getMessages()!=null) {
					Message messages = fork.getMessages();
					while(messages!=null) {
						if (messages.location!=null) {
							logger.log(Level.ERROR, "at line:%d, column:%d : %s", (messages.location.beginRow+1),(messages.location.beginColumn+1), messages.text);
						} else {
							logger.log(Level.ERROR, "%s", messages.text);
						}
						messages = messages.parent;
					}
				}
			}
			throw new RuntimeException("Unfixable error");
		}
		Fork fork = acceptedForks.get(0);
		if (fork.getMessages()!=null) {
			Message messages = fork.getMessages();
			while(messages!=null) {
				if (messages.location!=null) {
					logger.log(Level.WARN, "at line:%d, column:%d : %s", (messages.location.beginRow+1),(messages.location.beginColumn+1), messages.text);
				} else {
					logger.log(Level.WARN, "%s", messages.text);
				}
				messages = messages.parent;
			}
		}
		return parserResult;
	}

	private void parseArgs(String[] args) {
		for(int idx=0; idx<args.length; idx++) {
			String arg = args[idx];
			if (arg.startsWith("-")) {
				if (arg.startsWith("--")) {
					if ("--output-path".equals(arg)) {
						idx++;
						exportOutputPath = args[idx];
					} else if ("--update-only".equals(arg)) {
						optUpdateOnly = true;
					} else if ("--ast".equals(arg)) {
						idx++;
						if (idx<args.length) {
							astFileName = args[idx];
						} else {
							throw new RuntimeException("Expected filename follow --ast argument");
						}
					} else if ("--prefix".equals(arg)) {
						idx++;
						prefix = args[idx];
					} else if ("--exporter".equals(arg)) {
						idx++;
						String exporterName = args[idx];
						String fqn = "net.natpad.gross.export."+Character.toUpperCase(exporterName.charAt(0))+exporterName.substring(1);
						try {
							Class<?> forName = Class.forName(fqn);
							exporter = (Exporter) forName.newInstance();
						} catch (Exception e) {
							e.printStackTrace();
							throw new RuntimeException("aborted");
						}
					}

					
				} else if (arg.startsWith("-D")) {
					String propEqVal = arg.substring(2);
					int assIdx = propEqVal.indexOf('=');
					String name = propEqVal.substring(0, assIdx);
					String value = propEqVal.substring(assIdx+1);
					exporter.setProperty(name, value);
					
				} else {
					for(int sidx=1; sidx<arg.length(); sidx++) {
						char optch = arg.charAt(sidx);
						switch(optch) {
							case 'v' : {
								logger.increaseLogLevel();
							} break;
							case 'u' : {
								optUpdateOnly = true;
							} break;


						}
					}
				}
			} else {
				grammarFileName = arg;
				
			}
		}
	}
	
}
