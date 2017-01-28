package net.natpad.cup.export.java;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.config.CupConfiguration;
import net.natpad.cup.export.PredefinedExporter;
import net.natpad.cup.export.java.ast.AstClassName;
import net.natpad.cup.model.bnf.BnfModel;

/**
 * This class handles emitting generated code for the resulting parser. The various parse tables must be constructed,
 * etc. before calling any routines in this class.
 * <p>
 * 
 * Three classes are produced by this code:
 * <dl>
 * <dt>symbol constant class
 * <dd>this contains constant declarations for each terminal (and optionally each non-terminal).
 * <dt>action class
 * <dd>this non-public class contains code to invoke all the user actions that were embedded in the parser
 * specification.
 * <dt>parser class
 * <dd>the specialized parser class consisting primarily of some user supplied general and initialization code, and the
 * parse tables.
 * </dl>
 * <p>
 * 
 * Three parse tables are created as part of the parser class:
 * <dl>
 * <dt>production table
 * <dd>lists the LHS non terminal number, and the length of the RHS of each production.
 * <dt>action table
 * <dd>for each state of the parse machine, gives the action to be taken (shift, reduce, or error) under each lookahead
 * symbol.<br>
 * <dt>reduce-goto table
 * <dd>when a reduce on a given production is taken, the parse stack is popped back a number of elements corresponding
 * to the RHS of the production. This reveals a prior state, which we transition out of under the LHS non terminal
 * symbol for the production (as if we had seen the LHS symbol rather than all the symbols matching the RHS). This table
 * is indexed by non terminal numbers and indicates how to make these transitions.
 * </dl>
 * <p>
 * 
 * In addition to the method interface, this class maintains a series of public global variables and flags indicating
 * how misc. parts of the code and other output is to be produced, and counting things such as number of conflicts
 * detected (see the source code and public variables below for more details).
 * <p>
 * 
 * @see java_cup.main
 * @version last update: 11/25/95
 * @author Scott Hudson
 */

/*
 * Major externally callable routines here include: symbols - emit the symbol constant class parser - emit the parser
 * class In addition the following major internal routines are provided: emit_package - emit a package declaration
 * emit_action_code - emit the class containing the user's actions emit_production_table - emit declaration and init for
 * the production table do_action_table - emit declaration and init for the action table do_reduce_table - emit
 * declaration and init for the reduce-goto table Finally, this class uses a number of public instance variables to
 * communicate optional parameters and flags used to control how code is generated, as well as to report counts of
 * various things (such as number of conflicts detected). These include: prefix - a prefix string used to prefix names
 * that would otherwise "pollute" someone else's name space. package_name - name of the package emitted code is placed
 * in (or null for an unnamed package. symbol_const_class_name - name of the class containing symbol constants.
 * parser_class_name - name of the class for the resulting parser. action_code - user supplied declarations and other
 * code to be placed in action class. parser_code - user supplied declarations and other code to be placed in parser
 * class. init_code - user supplied code to be executed as the parser is being initialized. scan_code - user supplied
 * code to get the next Symbol. start_production - the start production for the grammar. import_list - list of imports
 * for use with action class. num_conflicts - number of conflicts detected. nowarn - true if we are not to issue warning
 * messages. not_reduced - count of number of productions that never reduce. unused_term - count of unused terminal
 * symbols. unused_non_term - count of unused non terminal symbols._time - a series of symbols indicating how long
 * various sub-parts of code generation took (used to produce optional time reports in main).
 */

public class Emit {

	/*-----------------------------------------------------------*/
	/*--- Constructor(s) ----------------------------------------*/
	/*-----------------------------------------------------------*/

	
	public final BnfModel model;

	public final JavaExportInfo exportInfo;
	public final CupConfiguration cupConfiguration;

	
	/** Only constructor is private so no instances can be created. */
	public Emit(BnfModel model, CupConfiguration cupConfiguration, JavaExportInfo exportInfo) {
		this.model = model;
		this.cupConfiguration = cupConfiguration;
		this.exportInfo = exportInfo;
	}

	
	static class DefnameAndClassName {
		public final String defResourceName;
		public final AstClassName astClassName;
		
		public DefnameAndClassName(String defResourceName, AstClassName astClassName) {
			this.defResourceName = defResourceName;
			this.astClassName = astClassName;
		}
	}
	
	public void doExport(boolean includeNonTerms, boolean symInterface, ParseActionTable actionTable,
					ParseReduceTable reduceTable, int startStateIndex, boolean optCompactRed) throws IOException {

		SymbolExporter symbolExporter = new SymbolExporter(model, exportInfo);
		symbolExporter.doExport(includeNonTerms, symInterface);
		
		
		ParserExporter parserExporter = new ParserExporter(model, exportInfo);
		parserExporter.doExport(actionTable, reduceTable, startStateIndex, model.getStartProduction(), optCompactRed);

		ActionsExporter actionsExporter = new ActionsExporter(model, exportInfo);
		actionsExporter.doExport(actionTable, reduceTable, startStateIndex, model.getStartProduction(), optCompactRed);
		
		
		
		

		HashMap<String, String> tokenMap = new HashMap<String, String>();
		
		tokenMap.put("org.natpad.cup.runtime.virtual_parse_stack", "org.natpad.cup.runtime.virtual_parse_stack");
		tokenMap.put("Stack", "Stack");
		tokenMap.put("java.util.Stack", "java.util.Stack");
		tokenMap.put("org.natpad.cup.export.java.runtime", exportInfo.runtimePackage.getFullQualifiedName());
		tokenMap.put("java.lang.Exception", "java.lang.Exception");

		
		tokenMap.put("org.natpad.cup.simplecalc.runtime", exportInfo.runtimePackage.getFullQualifiedName());
		tokenMap.put("org.natpad.cup.simplecalc.runtime.LrScanner", exportInfo.scannerRuntimeClass.getFullQualifiedName());
		tokenMap.put("ParserContext", exportInfo.contextRuntimeClass.className);

		tokenMap.put("Symbol", exportInfo.symbolRuntimeClass.className);
		tokenMap.put("org.natpad.cup.runtime.Symbol", exportInfo.symbolRuntimeClass.getFullQualifiedName());
		tokenMap.put("LrParser", exportInfo.parserRuntimeClass.className);
		tokenMap.put("lr_parser", exportInfo.parserRuntimeClass.className);
		tokenMap.put("org.natpad.cup.simplecalc.runtime.LrParser", exportInfo.parserRuntimeClass.getFullQualifiedName());
		tokenMap.put("org.natpad.cup.runtime.lr_parser", exportInfo.parserRuntimeClass.getFullQualifiedName());
		tokenMap.put("Scanner", exportInfo.scannerRuntimeClass.className);
		tokenMap.put("org.natpad.cup.runtime.Symbol", exportInfo.scannerRuntimeClass.getFullQualifiedName());
		tokenMap.put("virtual_parse_stack", exportInfo.virtParStackRuntimeClass.className);
		tokenMap.put("VirtualParseStack", exportInfo.virtParStackRuntimeClass.className);

		
		ArrayList<DefnameAndClassName> exportDefList = new ArrayList<DefnameAndClassName>();
		exportDefList.add(new DefnameAndClassName("Parser.def", exportInfo.parserRuntimeClass));
		exportDefList.add(new DefnameAndClassName("Scanner.def", exportInfo.scannerRuntimeClass));
		exportDefList.add(new DefnameAndClassName("Symbol.def", exportInfo.symbolRuntimeClass));
		exportDefList.add(new DefnameAndClassName("VirtualParseStack.def", exportInfo.virtParStackRuntimeClass));
		exportDefList.add(new DefnameAndClassName("ParserContext.def", exportInfo.contextRuntimeClass));
		
		
		for(DefnameAndClassName defnameAndClassName : exportDefList) {
			InputStream stream = getClass().getResourceAsStream("runtime/"+defnameAndClassName.defResourceName);
			File outDir = exportInfo.destDir;
			AstClassName className = defnameAndClassName.astClassName;
			if (className.astPackage!=null) {
				for(String subname : className.astPackage) {
					outDir = new File(outDir, subname);
				}
			}
			outDir.mkdirs();
			File outFile = new File(outDir, className.className+".java");
			new PredefinedExporter(new InputStreamReader(stream), outFile, tokenMap);
		}
	
	}
	
	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** whether or not to emit code for left and right values */
	public static boolean lr_values() {
		return getOrCreateTls()._lr_values;
	}

	public static void set_lr_values(boolean b) {
		getOrCreateTls()._lr_values = b;
	}

	static ThreadLocal<TLS> tlsInstance = new ThreadLocal<TLS>();
	
	static class TLS {

		/** Package that the resulting code goes into (null is used for unnamed). */
		public String package_name = null;

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** Name of the generated class for symbol constants. */
		public String symbol_const_class_name = "sym";

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** Name of the generated parser class. */
		public String parser_class_name = "parser";

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** User declarations for direct inclusion in user action class. */
		public String action_code = null;

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** User declarations for direct inclusion in parser class. */
		public String parser_code = null;

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** User code for user_init() which is called during parser initialization. */
		public String init_code = null;

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** User code for scan() which is called to get the next Symbol. */
		public String scan_code = null;

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

		/** List of imports (Strings containing class names) to go with actions. */
		public Stack<String> import_list = new Stack<String>();

		/** Do we skip warnings? */
		public boolean nowarn = false;


		/* frankf 6/18/96 */
		protected boolean _lr_values;
		
	}
	
	
	private static TLS getOrCreateTls() {
		TLS tls = tlsInstance.get();
		if (tls==null) {
			tls = new TLS();
			tlsInstance.set(tls);
		}
		return tls;
	}
	
	public static String getScanCode() {
		return getOrCreateTls().scan_code;
	}

	public static void setScanCode(String user_code) {
		getOrCreateTls().scan_code = user_code;
	}

	public static void setParserCode(String user_code) {
		getOrCreateTls().parser_code = user_code;
		
	}

	public static String getParserCode() {
		return getOrCreateTls().parser_code;
	}

	public static String getInitCode() {
		return getOrCreateTls().init_code;
	}

	public static void setInitCode(String user_code) {
		getOrCreateTls().init_code = user_code;
	}

	public static String getActionCode() {
		return getOrCreateTls().action_code;
	}

	public static void setActionCode(String user_code) {
		getOrCreateTls().action_code = user_code;
		
	}

	public static void importListPush(String multipart_name) {
		getOrCreateTls().import_list.push(multipart_name);
	}

	public static void setPackageName(String multipart_name) {
		getOrCreateTls().package_name = multipart_name;		
	}

	public static Stack<String> getImportList() {
		return getOrCreateTls().import_list;
	}

	public static boolean getNowarn() {
		return getOrCreateTls().nowarn;
	}

	public static void reset() {
		tlsInstance.remove();
	}


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

//	/**
//	 * Emit a package spec if the user wants one.
//	 * 
//	 * @param out stream to produce output on.
//	 */
//	protected void emitPackage(PrintWriter out) {
//		/* generate a package spec if we have a name for one */
//		if (package_name != null) {
//			out.println("package " + package_name + ";");
//			out.println();
//		}
//	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */



	



	
}
