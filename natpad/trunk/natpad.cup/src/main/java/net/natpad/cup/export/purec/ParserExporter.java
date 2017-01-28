package net.natpad.cup.export.purec;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.ParseActionRow;
import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.state.LalrState;
import net.natpad.cup.state.ParseAction;
import net.natpad.cup.state.ReduceAction;
import net.natpad.cup.state.ShiftAction;

public class ParserExporter {

	public final PureCConfiguration configuration;
	public final BnfModel model;
	
	public ParserExporter(BnfModel model, PureCConfiguration configuration) {
		this.configuration = configuration;
		this.model = model;
	}
	
	
	
	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Emit the parser subclass with embedded tables.
	 * 
	 * @param out stream to produce output on.
	 * @param action_table internal representation of the action table.
	 * @param reduce_table internal representation of the reduce-goto table.
	 * @param start_st start state of the parse machine.
	 * @param start_prod start production of the grammar.
	 * @param compact_reduces do we use most frequent reduce as default?
	 * @throws IOException 
	 */
	public void doExport(ParseActionTable action_table, ParseReduceTable reduce_table, int start_st,
					Production start_prod, boolean compact_reduces) throws IOException {
		
		
		String nsPre = configuration.namespacePrefix;			// Sta
		String nsPreUp = nsPre.toUpperCase();					// STA
		String nsPreLow = nsPre.toLowerCase();					// sta
		
		String fqPre = nsPre+"Parser";							// StaParser
		String fqPreLow = nsPreLow+"_parser";					// sta_parser

//		String mnPre = "Parser";							// Parser
//		String mnPreUp = "PARSER";							// PARSER
//		String mnPreLow = "parser";							// parser

		PrintWriter out = new PrintWriter(new File(configuration.destDir, nsPreLow+"parser.c"));
		out.println("#include \""+nsPreLow+"parser.h\"");
		out.println("#include \""+nsPreLow+"parseractions.h\"");
		out.println();
		
		/* emit the various tables */
		emitProductionTable(model, out, nsPreLow);

		doActionTable(out, action_table, nsPreLow, compact_reduces);
		doReduceTable(out, reduce_table, nsPreLow);

		
		out.println();
		out.println("static int l_class_setup = 0; ");
		out.println("static "+nsPre+"ParserClass l_class; ");
		out.println();
		out.println("static int "+nsPreLow+"_parser_start_state("+nsPre+"ParserBase *parser_base);");
		out.println("static int "+nsPreLow+"_parser_start_production("+nsPre+"ParserBase *parser_base);");
		out.println("static int "+nsPreLow+"_parser_eof_symbol("+nsPre+"ParserBase *parser_base);");
		out.println("static int "+nsPreLow+"_parser_error_symbol("+nsPre+"ParserBase *parser_base);");
		out.println("");
		out.println(""+nsPre+"ParserClass * "+nsPreLow+"_parser_get_type() {");
		out.println("	if (!l_class_setup) {");
		out.println("		l_class_setup = -1;");
		out.println("		"+nsPreLow+"_parser_class_init(&l_class);");
		out.println("	}");
		out.println("	return &l_class;");
		out.println("}");
		out.println("");
		out.println("void "+nsPreLow+"_parser_class_init("+nsPre+"ParserClass *clazz) {");
		out.println("	"+nsPre+"ParserBaseClass *parser_base_class = ("+nsPre+"ParserBaseClass *)(clazz);");
		out.println("	parser_base_class->start_state = "+nsPreLow+"_parser_start_state;");
		out.println("	parser_base_class->start_production = "+nsPreLow+"_parser_start_production;");
		out.println("	parser_base_class->eof_symbol = "+nsPreLow+"_parser_eof_symbol;");
		out.println("	parser_base_class->error_symbol = "+nsPreLow+"_parser_error_symbol;");
		out.println("	parser_base_class->action_cb = ("+nsPre+"ParserActionsCB) "+nsPreLow+"_parser_actions_run_action;");
		out.println("}");
		out.println("");
		out.println("void "+nsPreLow+"_parser_init("+nsPre+"Parser *parser) {");
		out.println("	*((void **) parser) = "+nsPreLow+"_parser_get_type();");
		out.println("	"+nsPre+"ParserBase *parser_base = ("+nsPre+"ParserBase *) (parser);");
		out.println("	"+nsPreLow+"_parser_base_init(parser_base);");
		out.println("	parser_base->production_tab = "+nsPreLow+"_2d_array_new((short *) "+nsPreLow+"_parser_production_table);");
		out.println("	parser_base->action_tab = "+nsPreLow+"_2d_array_new((short *) "+nsPreLow+"_parser_action_table);");
		out.println("	parser_base->reduce_tab = "+nsPreLow+"_2d_array_new((short *) "+nsPreLow+"_parser_reduce_table);");
		out.println("	parser_base->error_sync_size = 5;");
		out.println("}");
		out.println("");
		out.println("void "+nsPreLow+"_parser_dispose("+nsPre+"Parser *parser) {");
//		out.println("	cat_unref_ptr(instance->scanner);");
		out.println("}");
		out.println("");
		out.println(""+nsPre+"Parser *"+nsPreLow+"_parser_new("+nsPre+"ScannerNextTokenCB scanner, void *scanner_data) {");
		out.println("	"+nsPre+"Parser *result = malloc(sizeof("+nsPre+"Parser));");
		out.println("	"+nsPreLow+"_parser_init(result);");
		out.println("	"+nsPre+"ParserBase *parser_base = ("+nsPre+"ParserBase *) (result);");
		out.println("	parser_base->scanner = scanner;");
		out.println("	parser_base->scanner_data = scanner_data;");
		out.println("	return result;");
		out.println("}");
		out.println("");
		out.println("/** Indicates start state. */");
		out.println("static int "+nsPreLow+"_parser_start_state("+nsPre+"ParserBase *parser_base) {");
		out.println("	return " + start_st + ";");
		out.println("}");
		out.println("/** Indicates start production. */");
		out.println("static int "+nsPreLow+"_parser_start_production("+nsPre+"ParserBase *parser_base) {");
		out.println("	return " + model.getStartProduction().index() + ";");
		out.println("}");
		out.println("");
		out.println("/** <code>EOF</code> Symbol index. */");
		out.println("static int "+nsPreLow+"_parser_eof_symbol("+nsPre+"ParserBase *parser_base) {");
		out.println("	return " + model.EOF.index() + ";");
		out.println("}");
		out.println("");
		out.println("/** <code>error</code> Symbol index. */");
		out.println("static int "+nsPreLow+"_parser_error_symbol("+nsPre+"ParserBase *parser_base) {");
		out.println("	return " + model.error.index() + ";");
		out.println("}");
		
		out.flush();
		out.close();

		

		
		
		
		
		
		
		
		
		
//		PrintWriter 
		out = new PrintWriter(new File(configuration.destDir, nsPreLow+"parser.h"));

		out.println("#ifndef "+nsPreUp+"PARSER_H_");
		out.println("#define "+nsPreUp+"PARSER_H_");
		out.println("");
		out.println("#include \""+nsPreLow+"parseractions.h\"");
		out.println("#include \"runtime/"+nsPreLow+"2darray.h\"");
		out.println("#include \"runtime/"+nsPreLow+"parserbase.h\"");
		out.println("");
		out.println("typedef struct s_"+nsPre+"Parser       "+nsPre+"Parser;");
		out.println("typedef struct s_"+nsPre+"ParserClass  "+nsPre+"ParserClass;");
		out.println("");
		out.println("");
		out.println("struct s_"+nsPre+"Parser {");
		out.println("	"+nsPre+"ParserBase parent;");
		out.println("};");
		out.println("");
		out.println("struct s_"+nsPre+"ParserClass {");
		out.println("	"+nsPre+"ParserBaseClass parent_class;");
		out.println("};");
		out.println("");
		out.println(""+nsPre+"ParserClass * "+nsPreLow+"_parser_get_type();");
		out.println("void "+nsPreLow+"_parser_class_init("+nsPre+"ParserClass *clazz);");
		out.println("void "+nsPreLow+"_parser_init("+nsPre+"Parser *parser);");
		out.println("");
		out.println("void "+nsPreLow+"_parser_dispose("+nsPre+"Parser *parser);");
		out.println("");
		out.println(""+nsPre+"Parser *"+nsPreLow+"_parser_new("+nsPre+"ScannerNextTokenCB scanner, void *scanner_data);");
		out.println("");
		out.println("#endif /* "+nsPreUp+"PARSER_H_ */");

		

		
		out.flush();
		out.close();

	}


	/**
	 * Emit the production table.
	 * 
	 * @param out stream to produce output on.
	 */
	private void emitProductionTable(BnfModel model, PrintWriter out, String nsPreLow) {
		Production all_prods[];

		/* collect up the productions in order */
		all_prods = new Production[model.productions.count()];
		for (Production prod : model.productions) {
			all_prods[prod.index()] = prod;
		}

		// make short[][]
		short[][] prod_table = new short[model.productions.count()][2];
		for (int i = 0; i < model.productions.count(); i++) {
			Production prod = all_prods[i];
			// { lhs symbol , rhs size }
			prod_table[i][0] = (short) prod.lhs().getSymbol().index();
			prod_table[i][1] = (short) prod.rhsLength();
		}
		/* do the top of the table */
		out.println();
		out.println("/** Production table. */");
		out.println("const short "+nsPreLow+"_parser_production_table[] = {");
		doTableAsString(out, prod_table);
		out.println("};");

//		production_table_time = System.currentTimeMillis() - start_time;
	}
	

	
	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Emit the action table.
	 * 
	 * @param out stream to produce output on.
	 * @param act_tab the internal representation of the action table.
	 * @param compact_reduces do we use the most frequent reduce as default?
	 */
	private void doActionTable(PrintWriter out, ParseActionTable act_tab, String nsPreLow, boolean compact_reduces) {
		ParseActionRow row;
		ParseAction act;
		int red;


		/* collect values for the action table */
		short[][] action_table = new short[act_tab.num_states()][];
		/* do each state (row) of the action table */
		for (int i = 0; i < act_tab.num_states(); i++) {
			/* get the row */
			row = act_tab.under_state[i];

			/* determine the default for the row */
			if (compact_reduces)
				row.compute_default();
			else
				row.default_reduce = -1;

			/* make temporary table for the row. */
			short[] temp_table = new short[2 * row.size()];
			int nentries = 0;

			/* do each column */
			for (int j = 0; j < row.size(); j++) {
				/* extract the action from the table */
				act = row.under_term[j];

				/* skip error entries these are all defaulted out */
				if (act.kind() != ParseAction.ERROR) {
					/* first put in the symbol index, then the actual entry */

					/* shifts get positive entries of state number + 1 */
					if (act.kind() == ParseAction.SHIFT) {
						/* make entry */
						temp_table[nentries++] = (short) j;
						temp_table[nentries++] = (short) (((ShiftAction) act).shift_to().index() + 1);
					}

					/* reduce actions get negated entries of production# + 1 */
					else if (act.kind() == ParseAction.REDUCE) {
						/* if its the default entry let it get defaulted out */
						red = ((ReduceAction) act).reduce_with().index();
						if (red != row.default_reduce) {
							/* make entry */
							temp_table[nentries++] = (short) j;
							temp_table[nentries++] = (short) (-(red + 1));
						}
					} else if (act.kind() == ParseAction.NONASSOC) {
						/* do nothing, since we just want a syntax error */
					} else {
						/* shouldn't be anything else */
						throw new FatalCupException("Unrecognized action code " + act.kind() + " found in parse table");
					}
				}
			}

			/* now we know how big to make the row */
			action_table[i] = new short[nentries + 2];
			System.arraycopy(temp_table, 0, action_table[i], 0, nentries);

			/* finish off the row with a default entry */
			action_table[i][nentries++] = -1;
			if (row.default_reduce != -1)
				action_table[i][nentries++] = (short) (-(row.default_reduce + 1));
			else
				action_table[i][nentries++] = 0;
		}

		
		/* do the top of the table */
		out.println();
		out.println("/** Parse-action table. */");
		out.println("const short "+nsPreLow+"_parser_action_table[] = {");
		doTableAsString(out, action_table);
		out.println("};");

//		action_table_time = System.currentTimeMillis() - start_time;
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	
	/**
	 * Emit the reduce-goto table.
	 * 
	 * @param out stream to produce output on.
	 * @param red_tab the internal representation of the reduce-goto table.
	 */
	private void doReduceTable(PrintWriter out, ParseReduceTable red_tab, String nsPreLow) {
		LalrState goto_st;


		/* collect values for reduce-goto table */
		short[][] reduce_goto_table = new short[red_tab.num_states()][];
		/* do each row of the reduce-goto table */
		for (int i = 0; i < red_tab.num_states(); i++) {
			/* make temporary table for the row. */
			short[] temp_table = new short[2 * red_tab.under_state[i].size()];
			int nentries = 0;
			/* do each entry in the row */
			for (int j = 0; j < red_tab.under_state[i].size(); j++) {
				/* get the entry */
				goto_st = red_tab.under_state[i].under_non_term[j];

				/* if we have none, skip it */
				if (goto_st != null) {
					/* make entries for the index and the value */
					temp_table[nentries++] = (short) j;
					temp_table[nentries++] = (short) goto_st.index();
				}
			}
			/* now we know how big to make the row. */
			reduce_goto_table[i] = new short[nentries + 2];
			System.arraycopy(temp_table, 0, reduce_goto_table[i], 0, nentries);

			/* end row with default value */
			reduce_goto_table[i][nentries++] = -1;
			reduce_goto_table[i][nentries++] = -1;
		}

		/* do the top of the table */
		out.println();
		out.println("/** Parse-action table. */");
		out.println("const short "+nsPreLow+"_parser_reduce_table[] = {");
		doTableAsString(out, reduce_goto_table);
		out.println("};");

	}
	
	
	
	
	
	// print a string array encoding the given short[][] array.
	private void doTableAsString(PrintWriter out, short[][] sa) {
		String prefix = "\t";
		
		out.println(prefix+"/* the number of rows */");
		out.println(prefix+sa.length+", ");
	
		  out.print(prefix+"/* the number of columns for each row */");
		for(int row=0; row<sa.length; row++) {
			if (row%24==0) {
				if (row!=0) {
					out.print(",");
				}
				out.print("\n"+prefix);
			} else {
				out.print(", ");
			}
			out.print(sa[row].length);
		}
		out.println(",");

		  out.print(prefix+"/* the raw table data */");
		int idx = 0;
		for(int row=0; row<sa.length; row++) {
			  out.print(prefix+"/* start row "+row+" */");
				out.print("\n"+prefix);
			for(int col=0; col<sa[row].length; col++) {
//				if (idx%24==0) {
//					if (idx!=0) {
//						out.print(",");
//					}
//					out.print("\n"+prefix);
//				} else {
					out.print(", ");
//				}
				out.print(sa[row][col]);
				idx++;
			}
		}
		out.println();
	}



	
}
