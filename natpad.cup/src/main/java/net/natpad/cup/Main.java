package net.natpad.cup;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.StringReader;
import java.util.Properties;

import net.natpad.cup.config.ConfigurationReader;
import net.natpad.cup.config.CupConfiguration;
import net.natpad.cup.config.ICupExporter;
import net.natpad.cup.export.java.Emit;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.SymbolPart;
import net.natpad.cup.model.parser.NewLexer;
import net.natpad.cup.state.LalrState;
import net.natpad.cup.state.StateMachine;
import net.natpad.cup.util.TextFile;


/** This class serves as the main driver for the JavaCup system.
 *  It accepts user options and coordinates overall control flow.
 *  The main flow of control includes the following activities: 
 *  <ul>
 *    <li> Parse user supplied arguments and options.
 *    <li> Open output files.
 *    <li> Parse the specification from standard input.
 *    <li> Check for unused terminals, non-terminals, and productions.
 *    <li> Build the state machine, tables, etc.
 *    <li> Output the generated code.
 *    <li> Close output files.
 *    <li> Print a summary if requested.
 *  </ul>
 *
 *  Options to the main program include: <dl>
 *   <dt> -package name  
 *   <dd> specify package generated classes go in [default none]
 *   <dt> -parser name   
 *   <dd> specify parser class name [default "parser"]
 *   <dt> -symbols name  
 *   <dd> specify name for symbol constant class [default "sym"]
 *   <dt> -interface
 *   <dd> emit symbol constant <i>interface</i>, rather than class
 *   <dt> -nonterms      
 *   <dd> put non terminals in symbol constant class
 *   <dt> -expect #      
 *   <dd> number of conflicts expected/allowed [default 0]
 *   <dt> -compact_red   
 *   <dd> compact tables by defaulting to most frequent reduce
 *   <dt> -nowarn        
 *   <dd> don't warn about useless productions, etc.
 *   <dt> -nosummary     
 *   <dd> don't print the usual summary of parse states, etc.
 *   <dt> -progress      
 *   <dd> print messages to indicate progress of the system
 *   <dt> -time          
 *   <dd> print time usage summary
 *   <dt> -dump_grammar  
 *   <dd> produce a dump of the symbols and grammar
 *   <dt> -dump_states   
 *   <dd> produce a dump of parse state machine
 *   <dt> -dump_tables   
 *   <dd> produce a dump of the parse tables
 *   <dt> -dump          
 *   <dd> produce a dump of all of the above
 *   <dt> -debug         
 *   <dd> turn on debugging messages within JavaCup 
 *   <dt> -nopositions
 *   <dd> don't generate the positions code
 *   <dt> -noscanner
 *   <dd> don't refer to java_cup.runtime.Scanner in the parser
 *        (for compatibility with old runtimes)
 *   <dt> -version
 *   <dd> print version information for JavaCUP and halt.
 *   </dl>
 *
 * @author  Frank Flannery
 *          Douwe Vos
 */

public class Main {


	private CupConfiguration cupConfiguration = new CupConfiguration(); 

	public NewLexer lexer = new NewLexer();
	
	private long inputFileLastModTs = System.currentTimeMillis();
	
	protected Properties properties = new Properties();
	
	
	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/
	/**
	 * Only constructor is private, so we do not allocate any instances of this
	 * class.
	 */
	public Main(String argv[]) throws java.io.IOException, java.lang.Exception {
		Emit.reset();
		/* process user options and arguments */
		parse_args(argv);
	}

	
	public void addProperties(Properties propertiese) {
		properties.putAll(propertiese);
	}
		
	public void run() throws Exception {		

		File jsonConfigFile = new File(jsonConfigFileName);
		long newestInModTs = jsonConfigFile.lastModified();
		if (newestInModTs<inputFileLastModTs) {
			newestInModTs = inputFileLastModTs;
		}

		TextFile textFile = new TextFile(properties);
		String jsonText = textFile.readText(jsonConfigFile);
		ConfigurationReader configurationReader = new ConfigurationReader(new StringReader(jsonText));
		cupConfiguration = configurationReader.readConfiguration(cupConfiguration);

		if (optUpdateOnly) {
			boolean shouldUpdate = false;
			System.out.println();
			for(ICupExporter exporter : cupConfiguration.getExporters()) {
				if (exporter.testForChanges(newestInModTs)) {
					shouldUpdate = true;
					break;
				}
			}
			if (!shouldUpdate) {
				System.out.println("No changes detected.");
				return;
			}
		}

		
		
		
		
//		/*
//		 * frankf 6/18/96 hackish, yes, but works
//		 */
//		Emit.set_lr_values(lr_values);
		/* open output files */
		if (printProgress) {
			System.err.println("Opening all files...");
		}
		/* use a buffered version of standard input */
		input_file = new BufferedInputStream(System.in);

		/* parse spec into internal data structures */
		if (printProgress) {
			System.err.println("Parsing specification from standard input...");
		}
		BnfModel model = parseGrammarSpec();

		
		StateMachine stateMachine = null;
//		Emit emit = null;

		/* don't proceed unless we are error free */
		if (lexer.error_count == 0) {
			/* check for unused bits */
			if (printProgress) System.err.println("Checking specification...");
			lexer.warning_count += model.checkUnused();

			/* build the state machine and parse tables */
			if (printProgress) System.err.println("Building parse tables...");
			stateMachine = buildParser(model);
//			stateMachine.dump_tables();
//			emit = new Emit(model);
			
			for(ICupExporter exporter : cupConfiguration.getExporters()) {
				exporter.export(model, stateMachine);
			}
			
//			build_end = System.currentTimeMillis();
//
//			/* output the generated code, if # of conflicts permits */
//			if (Lexer.error_count != 0) {
//				// conflicts! don't emit code, don't dump tables.
//				opt_dump_tables = false;
//			} else { // everything's okay, emit parser.
//				if (print_progress) System.err.println("Writing parser...");
//				emit.doExport(include_non_terms, sym_interface, stateMachine.action_table, stateMachine.reduce_table, start_state.index(),
//								opt_compact_red, suppress_scanner);
//				
//
//				did_output = true;
//			}
		}

		/* do requested dumps */
		if (optDumpGrammar) {
			dump_grammar(model);
		}
		if (optDumpStates && stateMachine != null) {
			dump_machine(stateMachine);
		}
		if (optDumpTables) {
			stateMachine.dump_tables();
		}


		/* close input/output files */
		if (printProgress)
			System.err.println("Closing files...");
		if (input_file != null)
			input_file.close();

//		/* produce a summary if desired */
//		if (!no_summary && stateMachine != null)
//			emit_summary(stateMachine, model, emit, did_output);

		/*
		 * If there were errors during the run, exit with non-zero status
		 * (makefile-friendliness). --CSA
		 */
		if (lexer.error_count != 0)
			System.exit(100);
	}


	// public static Model model = new Model();

	/** Input file. This is a buffered version of System.in. */
	protected BufferedInputStream input_file;
	
	
	protected String jsonConfigFileName = "./etc/parser-config.json";
	
	/*-------------------------*/
	/* Options set by the user */
	/*-------------------------*/
	/** User option -- do we print progress messages. */
	protected boolean printProgress = true;
	/** User option -- do we produce a dump of the state machine */
	protected boolean optDumpStates = false;
	/** User option -- do we produce a dump of the parse tables */
	protected boolean optDumpTables = false;
	/** User option -- do we produce a dump of the grammar */
	protected boolean optDumpGrammar = false;
	/** User option -- do we show timing information as a part of the summary */
	protected boolean optShowTiming = false;
	/** User option -- do we run produce extra debugging messages */
	protected boolean optDoDebug = false;
	/** User option -- do not print a summary. */
	protected boolean noSummary = false;

	
	protected boolean optUpdateOnly = false;

	
	/** User option -- should symbols be put in a class or an interface? [CSA] */
	protected boolean symToInterface = false;

	/**
	 * User option -- should generator suppress references to
	 * java_cup.runtime.Scanner for compatibility with old runtimes?
	 */
	protected boolean suppressScanner = false;

	/*----------------------------------------------------------------------*/
	/* Timing data (not all of these time intervals are mutually exclusive) */
	/*----------------------------------------------------------------------*/


	/**
	 * The main driver for the system.
	 * 
	 * @param argv    an array of strings containing command line arguments.
	 */
	public static void main(String[] args) {
		try {
			Main main = new Main(args);
			main.run();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Print a "usage message" that described possible command line options,
	 * then exit.
	 * 
	 * @param message      a specific error message to preface the usage message by.
	 */
	protected void usage(String message) {
		System.err.println();
		System.err.println(message);
		System.err.println();
		System.err.println("Usage: " + Version.program_name + " [options] [filename]\n"
						+ "  and expects a specification file on standard input if no filename is given.\n"
						+ "  Legal options include:\n"
						+ "    -c --config filename     specify the Json configuration file\n"
						+ "    -u --update    Update if input/config file is/are newer then the output files\n"
						+ "    -package name  specify package generated classes go in [default none]\n"
						+ "    -parser name   specify parser class name [default \"parser\"]\n"
						+ "    -symbols name  specify name for symbol constant class [default \"sym\"]\n"
						+ "    -interface     put symbols in an interface, rather than a class\n"
						+ "    -nonterms      put non terminals in symbol constant class\n"
						+ "    -expect #      number of conflicts expected/allowed [default 0]\n"
						+ "    -compact_red   compact tables by defaulting to most frequent reduce\n"
						+ "    -nowarn        don't warn about useless productions, etc.\n"
						+ "    -nosummary     don't print the usual summary of parse states, etc.\n"
						+ "    -noscanner     don't refer to java_cup.runtime.Scanner\n"
						+ "    -progress      print messages to indicate progress of the system\n"
						+ "    -time          print time usage summary\n"
						+ "    -dump_grammar  produce a human readable dump of the symbols and grammar\n"
						+ "    -dump_states   produce a dump of parse state machine\n"
						+ "    -dump_tables   produce a dump of the parse tables\n"
						+ "    -dump          produce a dump of all of the above\n"
						+ "    -version       print the version information for CUP and exit\n");
		System.exit(1);
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Parse command line options and arguments to set various user-option flags
	 * and variables.
	 * 
	 * @param argv
	 *            the command line arguments to be parsed.
	 */
	protected void parse_args(String argv[]) {
		int len = argv.length;
		int i;

		/* parse the options */
		for (i = 0; i < len; i++) {
			/* try to get the various options */
			if (argv[i].equals("-nonterms")) {
				cupConfiguration.setExportNonTerms(true);
			} else if (argv[i].equals("-expect")) {
				/* must have an arg */
				if (++i >= len || argv[i].startsWith("-") || argv[i].endsWith(".cup"))
					usage("-expect must have a name argument");

				/* record the number */
				try {
					cupConfiguration.setExepectConflicts(Integer.parseInt(argv[i]));
				} catch (NumberFormatException e) {
					usage("-expect must be followed by a decimal integer");
				}
			} else if (argv[i].equals("-nosummary"))
				noSummary = true;
//			else if (argv[i].equals("-nowarn"))
//				Emit.nowarn = true;
			else if (argv[i].equals("-dump_states"))
				optDumpStates = true;
			else if (argv[i].equals("-dump_tables"))
				optDumpTables = true;
			else if (argv[i].equals("-progress"))
				printProgress = true;
			else if (argv[i].equals("-dump_grammar"))
				optDumpGrammar = true;
			else if (argv[i].equals("-dump"))
				optDumpStates = optDumpTables = optDumpGrammar = true;
			else if (argv[i].equals("-time"))
				optShowTiming = true;
			else if (argv[i].equals("-debug"))
				optDoDebug = true;
			/* CSA 23-Jul-1999 */
			else if (argv[i].equals("-version")) {
				System.out.println(Version.title_str);
				System.exit(1);
			} else if (argv[i].equals("--config") || argv[i].equals("-c")) {
				/* must have an arg */
				if (++i >= len) {
					usage("--config must have a file-name argument");
				}
				jsonConfigFileName = argv[i];

			} else if (argv[i].equals("--update") || argv[i].equals("-u")) {
				optUpdateOnly = true;
			} else if (argv[i].charAt(0)=='-') {
				
			}
			/* CSA 24-Jul-1999; suggestion by Jean Vaucher */
			else if (!argv[i].startsWith("-") && i == len - 1) {
				/* use input from file. */
				try {
					File inputFile = new File(argv[i]);
					inputFileLastModTs = inputFile.lastModified();
					System.setIn(new FileInputStream(inputFile));
				} catch (java.io.FileNotFoundException e) {
					usage("Unable to open \"" + argv[i] + "\" for input");
				}
			} else {
				usage("Unrecognized option \"" + argv[i] + "\"");
			}
		}
	}


	/**
	 * Parse the grammar specification from standard input. This produces sets
	 * of terminal, non-terminals, and productions which can be accessed via
	 * static variables of the respective classes, as well as the setting of
	 * various variables (mostly in the emit class) for small user supplied
	 * items such as the code to scan with.
	 */
	public BnfModel parseGrammarSpec() throws java.lang.Exception {

		/* create a parser and parse with it */
		net.natpad.cup.model.parser.generated.Parser parserObj = new net.natpad.cup.model.parser.generated.Parser();
		net.natpad.cup.model.parser.generated.runtime.LrParserContext parseContext  = new net.natpad.cup.model.parser.generated.runtime.LrParserContext(lexer);
		parserObj.lexer = lexer;
		try {
			if (optDoDebug) {
				parserObj.debug_parse(parseContext);
			} else {
				parserObj.parse(parseContext);
			}
			return parserObj.model;
		} catch (Exception e) {
			/*
			 * something threw an exception. catch it and emit a message so we
			 * have a line number to work with, then re-throw it
			 */
			lexer.emit_error("Internal error: Unexpected exception");
			throw e;
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . */
	/* . . Internal Results of Generating the Parser . . */
	/* . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Start state in the overall state machine. */
	protected LalrState start_state;


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Build the (internal) parser from the previously parsed specification.
	 * This includes:
	 * <ul>
	 * <li>Computing nullability of non-terminals.
	 * <li>Computing first sets of non-terminals and productions.
	 * <li>Building the viable prefix recognizer machine.
	 * <li>Filling in the (internal) parse tables.
	 * <li>Checking for unreduced productions.
	 * </ul>
	 */
	public StateMachine buildParser(BnfModel model) {
		/* compute nullability of all non terminals */
		if (optDoDebug || printProgress)
			System.err.println("  Computing non-terminal nullability...");
		model.computeNullability();

		/* compute first sets of all non terminals */
		if (optDoDebug || printProgress) System.err.println("  Computing first sets...");
		model.computeFirstSets();


		/* build the LR viable prefix recognition machine */
		if (optDoDebug || printProgress) System.err.println("  Building state machine...");

		StateMachine stateMachine = new StateMachine(model, model.getStartProduction());
		stateMachine.buildMachine(cupConfiguration);
		return stateMachine;
	}


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Helper routine to optionally return a plural or non-plural ending.
	 * 
	 * @param val
	 *            the numerical value determining plurality.
	 */
	protected  String plural(int val) {
		if (val == 1)
			return "";
		else
			return "s";
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Emit a long summary message to standard error (System.err) which
	 * summarizes what was found in the specification, how many states were
	 * produced, how many conflicts were found, etc. A detailed timing summary
	 * is also produced if it was requested by the user.
	 * 
	 * @param outputProduced    did the system get far enough to generate code.
	 */
	protected void emitSummary(StateMachine stateMachine, BnfModel model, boolean outputProduced) {

		if (noSummary)
			return;

		System.err.println("------- " + Version.title_str + " Parser Generation Summary -------");

		/* error and warning count */
		System.err.println("  " + lexer.error_count + " error" + plural(lexer.error_count) + " and "+ lexer.warning_count + " warning" + plural(lexer.warning_count));

		/* basic stats */
		System.err.print("  " + model.terminals.count() + " terminal" + plural(model.terminals.count()) + ", ");
		System.err.print(model.nonTerminals.count() + " non-terminal" + plural(model.nonTerminals.count()) + ", and ");
		System.err.println(model.productions.count() + " production" + plural(model.productions.count())+ " declared, ");
		System.err.println("  producing " + stateMachine.getStateMap().count() + " unique parse states.");

		/* unused symbols */
		System.err.println("  " + model.unused_term + " terminal" + plural(model.unused_term)+ " declared but not used.");
		System.err.println("  " + model.unused_non_term + " non-terminal" + plural(model.unused_term)+ " declared but not used.");

		/* conflicts */
		System.err.println("  " + model.num_conflicts + " conflict" + plural(model.num_conflicts) + " detected" + " ("+ cupConfiguration.getExpectConflicts() + " expected).");

//		/* code location */
//		if (output_produced)
//			System.err.println("  Code written to \"" + Emit.parser_class_name + ".java\", and \"" + Emit.symbol_const_class_name + ".java\".");
//		else
//			System.err.println("  No code produced.");

		System.err.println("---------------------------------------------------- (" + Version.version_str + ")");
	}



	/**
	 * Helper routine to format a decimal based display of seconds and
	 * percentage of total time given counts of milliseconds. Note: this is
	 * broken for use with some instances of negative time (since we don't use
	 * any negative time here, we let if be for now).
	 * 
	 * @param time_val
	 *            the value being formatted (in ms).
	 * @param total_time
	 *            total time percentages are calculated against (in ms).
	 */
	protected String timestr(long time_val, long total_time) {
		boolean neg;
		long ms = 0;
		long sec = 0;
		long percent10;
		String pad;

		/* work with positives only */
		neg = time_val < 0;
		if (neg)
			time_val = -time_val;

		/* pull out seconds and ms */
		ms = time_val % 1000;
		sec = time_val / 1000;

		/* construct a pad to blank fill seconds out to 4 places */
		if (sec < 10)
			pad = "   ";
		else if (sec < 100)
			pad = "  ";
		else if (sec < 1000)
			pad = " ";
		else
			pad = "";

		/* calculate 10 times the percentage of total */
		percent10 = (time_val * 1000) / total_time;

		/* build and return the output string */
		return (neg ? "-" : "") + pad + sec + "." + ((ms % 1000) / 100) + ((ms % 100) / 10) + (ms % 10) + "sec" + " ("
						+ percent10 / 10 + "." + percent10 % 10 + "%)";
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Produce a human readable dump of the grammar. */
	public void dump_grammar(BnfModel model) {
		System.err.println("===== Terminals =====");
		for (int tidx = 0, cnt = 0; tidx < model.terminals.count(); tidx++, cnt++) {
			System.err.print("[" + tidx + "]" + model.terminals.get(tidx).name() + " ");
			if ((cnt + 1) % 5 == 0)
				System.err.println();
		}
		System.err.println();
		System.err.println();

		System.err.println("===== Non terminals =====");
		for (int nidx = 0, cnt = 0; nidx < model.nonTerminals.count(); nidx++, cnt++) {
			System.err.print("[" + nidx + "]" + model.nonTerminals.get(nidx).name() + " ");
			if ((cnt + 1) % 5 == 0)
				System.err.println();
		}
		System.err.println();
		System.err.println();

		System.err.println("===== Productions =====");
		for (int pidx = 0; pidx < model.productions.count(); pidx++) {
			Production prod = model.productions.get(pidx);
			System.err.print("[" + pidx + "] " + prod.lhs().getSymbol().name() + " ::= ");
			for (int i = 0; i < prod.rhsLength(); i++)
				if (prod.rhs(i).isAction())
					System.err.print("{action} ");
				else
					System.err.print(((SymbolPart) prod.rhs(i)).getSymbol().name() + " ");
			System.err.println();
		}
		System.err.println();
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Produce a (semi-) human readable dump of the complete viable prefix
	 * recognition state machine.
	 */
	public void dump_machine(StateMachine stateMachine) {
		LalrState ordered[] = new LalrState[stateMachine.getStateMap().count()];

		/* put the states in sorted order for a nicer display */
		for (LalrState st : stateMachine.getStateMap()) {
			ordered[st.index()] = st;
		}

		System.err.println("===== Viable Prefix Recognizer =====");
		for (int i = 0; i < stateMachine.getStateMap().count(); i++) {
			if (ordered[i] == start_state)
				System.err.print("START ");
			System.err.println(ordered[i]);
			System.err.println("-------------------");
		}
	}


	/*-----------------------------------------------------------*/

}
