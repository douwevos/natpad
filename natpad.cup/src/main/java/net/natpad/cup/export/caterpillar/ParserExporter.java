package net.natpad.cup.export.caterpillar;

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

	public final CaterpillarConfiguration configuration;
	public final BnfModel model;

	public final String n_Sta, n_sta, n_STA;
	
	public final String n_StaParser, n_sta_parser, n_PARSER;

	
	public final String n_StaParserActions, n_sta_parser_actions, n_PARSER_ACTIONS;

	public final File parserCFile;
	public final File parserHFile;
	
	public ParserExporter(BnfModel model, CaterpillarConfiguration configuration) {
		this.configuration = configuration;
		this.model = model;
		n_Sta = configuration.namespacePrefix;			// Sta
		n_STA = n_Sta.toUpperCase();					// STA
		n_sta = n_Sta.toLowerCase();					// sta
		
		n_StaParser = n_Sta+configuration.getParserNameCamel();							// StaParser
		n_sta_parser = n_sta+"_"+configuration.getParserNameLower();					// sta_parser
		n_PARSER = configuration.getParserNameUpper();

		
		n_StaParserActions = n_Sta+configuration.getParserActionsNameCamel();
		n_sta_parser_actions = n_sta+"_"+configuration.getParserActionsNameLower();
		n_PARSER_ACTIONS = configuration.getParserActionsNameUpper();

		parserCFile = new File(configuration.destDir, n_sta+configuration.getParserNameStripper()+".c");
		parserHFile = new File(configuration.destDir, n_sta+configuration.getParserNameStripper()+".h");
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
		
		

//		String mnPre = "Parser";							// Parser
//		String mnPreUp = "PARSER";							// PARSER
//		String mnPreLow = "parser";							// parser

		PrintWriter out = new PrintWriter(parserCFile);
		out.println("#include \""+n_sta+configuration.getParserNameStripper()+".h\"");
		out.println();
		out.println("#include <logging/catlogdefs.h>");
		out.println("#define CAT_LOG_LEVEL CAT_LOG_ERROR");
		out.println("#define CAT_LOG_CLAZZ \""+n_StaParser+"\"");
		out.println("#include <logging/catlog.h>");
		out.println();
		
		/* emit the various tables */
		emitProductionTable(model, out, n_sta_parser);

		doActionTable(out, action_table, n_sta, compact_reduces, n_sta_parser);
		doReduceTable(out, reduce_table, n_sta_parser);

		
		out.println();
		out.println("G_DEFINE_TYPE("+n_StaParser+", "+n_sta_parser+", "+n_STA+"_TYPE_PARSER_BASE)");
		out.println();
		out.println("static void _dispose(GObject *object);");
		out.println("static "+n_Sta+"Token *"+n_sta_parser+"_run_action("+n_Sta+"ParserBase *parser_base, "+n_Sta+"ParserContext *parserContext, int actionId);");
		out.println("static int "+n_sta_parser+"_start_state("+n_Sta+"ParserBase *parser_base);");
		out.println("static int "+n_sta_parser+"_start_production("+n_Sta+"ParserBase *parser_base);");
		out.println("static int "+n_sta_parser+"_eof_symbol("+n_Sta+"ParserBase *parser_base);");
		out.println("static int "+n_sta_parser+"_error_symbol("+n_Sta+"ParserBase *parser_base);");
		out.println("");
		out.println("");
		out.println("static void "+n_sta_parser+"_class_init("+n_StaParser+"Class *clazz) {");
		out.println("	GObjectClass *object_class = G_OBJECT_CLASS(clazz);");
		out.println("	object_class->dispose = _dispose;");
		out.println("");
		out.println("	"+n_Sta+"ParserBaseClass *parser_base_class = "+n_STA+"_PARSER_BASE_CLASS(clazz);");
		out.println("	parser_base_class->start_state = "+n_sta_parser+"_start_state;");
		out.println("	parser_base_class->start_production = "+n_sta_parser+"_start_production;");
		out.println("	parser_base_class->eof_symbol = "+n_sta_parser+"_eof_symbol;");
		out.println("	parser_base_class->error_symbol = "+n_sta_parser+"_error_symbol;");
		out.println("	parser_base_class->run_action = "+n_sta_parser+"_run_action;");
		out.println("}");
		out.println("");
		out.println("static void "+n_sta_parser+"_init("+n_StaParser+" *parser) {");
		out.println("	"+n_Sta+"ParserBase *parser_base = "+n_STA+"_PARSER_BASE(parser);");
		out.println("	parser_base->production_tab = "+n_sta+"_2d_array_new((short *) "+n_sta_parser+"_production_table);");
		out.println("	parser_base->action_tab = "+n_sta+"_2d_array_new((short *) "+n_sta_parser+"_action_table);");
		out.println("	parser_base->reduce_tab = "+n_sta+"_2d_array_new((short *) "+n_sta_parser+"_reduce_table);");
		out.println("	parser_base->error_sync_size = 5;");
		out.println("}");
		out.println("");
		out.println("static void _dispose(GObject *object) {");
//		out.println("	"+fqPre+" *instance = "+n_STA+"_PARSER_CONTEXT(object);");
		out.println("	"+n_Sta+"ParserBase *parser_base = "+n_STA+"_PARSER_BASE(object);");
		out.println("	if (parser_base->production_tab) {");
		out.println("		parser_base->production_tab->data = NULL;");
		out.println("		cat_unref_ptr(parser_base->production_tab);");
		out.println("	}");
		out.println("	if (parser_base->action_tab) {");
		out.println("		parser_base->action_tab->data = NULL;");
		out.println("		cat_unref_ptr(parser_base->action_tab);");
		out.println("	}");
		out.println("	if (parser_base->reduce_tab) {");
		out.println("		parser_base->reduce_tab->data = NULL;");
		out.println("		cat_unref_ptr(parser_base->reduce_tab);");
		out.println("	}");
		out.println("	cat_unref_ptr("+n_STA+"_"+n_PARSER+"(object)->parser_actions);");
		out.println("}");
		out.println("");
		out.println(""+n_StaParser+" *"+n_sta_parser+"_new("+n_Sta+"IScanner *scanner) {");
		out.println("	"+n_StaParser+" *result = g_object_new("+n_STA+"_TYPE_"+n_PARSER+", NULL);");
		out.println("	cat_ref_anounce(result);");
		out.println("	result->parser_actions = "+n_sta_parser_actions+"_new();");
		out.println("	return result;");
		out.println("}");
		out.println("");
//		out.println("/** Instance of action encapsulation class. */");
//		out.println("protected "+nsPre+"Actions actionObject;");
//		out.println("");
//		out.println("/** Action encapsulation object initializer. */");
//		out.println("protected void "+n_sta+"_parser_init_actions() {");
//		out.println("	actionObject = new "+nsPre+"Actions();");
//		out.println("}");
		out.println("");
		out.println("/** Invoke a user supplied parse action. */");
		out.println("static "+n_Sta+"Token *"+n_sta_parser+"_run_action("+n_Sta+"ParserBase *parser_base, "+n_Sta+"ParserContext *parserContext, int actionId) {");
		out.println("	/* call code in generated class */");
		out.println("	"+n_StaParser+" *parser = ("+n_StaParser+" *) parser_base;");
		out.println("	"+n_StaParserActions+" *parser_actions = ("+n_StaParserActions+" *) parser->parser_actions;");
		out.println("	return "+n_sta_parser_actions+"_run_action(parser_actions, parserContext, actionId);");
		out.println("}");
		out.println("");
		out.println("/** Indicates start state. */");
		out.println("static int "+n_sta_parser+"_start_state("+n_Sta+"ParserBase *parser_base) {");
		out.println("	return " + start_st + ";");
		out.println("}");
		out.println("/** Indicates start production. */");
		out.println("static int "+n_sta_parser+"_start_production("+n_Sta+"ParserBase *parser_base) {");
		out.println("	return " + model.getStartProduction().index() + ";");
		out.println("}");
		out.println("");
		out.println("/** <code>EOF</code> Symbol index. */");
		out.println("static int "+n_sta_parser+"_eof_symbol("+n_Sta+"ParserBase *parser_base) {");
		out.println("	return " + model.EOF.index() + ";");
		out.println("}");
		out.println("");
		out.println("/** <code>error</code> Symbol index. */");
		out.println("static int "+n_sta_parser+"_error_symbol("+n_Sta+"ParserBase *parser_base) {");
		out.println("	return " + model.error.index() + ";");
		out.println("}");
		
		out.flush();
		out.close();

		

		
		
		
		
		
		
		
		
		
//		PrintWriter 
		out = new PrintWriter(parserHFile);

		out.println("#ifndef "+n_STA+""+n_PARSER+"_H_");
		out.println("#define "+n_STA+""+n_PARSER+"_H_");
		out.println("");
		out.println("#include <caterpillar.h>");
		out.println("#include \""+n_sta+configuration.getParserActionsNameStripper()+".h\"");
		out.println("#include \"runtime/"+n_sta+"2darray.h\"");
		out.println("#include \"runtime/"+n_sta+"parserbase.h\"");
		out.println("");
		out.println("G_BEGIN_DECLS");
		out.println("");
		out.println("#define "+n_STA+"_TYPE_"+n_PARSER+"            ("+n_sta_parser+"_get_type())");
		out.println("#define "+n_STA+"_"+n_PARSER+"(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), "+n_sta_parser+"_get_type(), "+n_StaParser+"))");
		out.println("#define "+n_STA+"_"+n_PARSER+"_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), "+n_STA+"_TYPE_"+n_PARSER+", "+n_StaParser+"Class))");
		out.println("#define "+n_STA+"_IS_"+n_PARSER+"(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), "+n_STA+"_TYPE_"+n_PARSER+"))");
		out.println("#define "+n_STA+"_IS_"+n_PARSER+"_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), "+n_STA+"_TYPE_"+n_PARSER+"))");
		out.println("#define "+n_STA+"_"+n_PARSER+"_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), "+n_STA+"_TYPE_"+n_PARSER+", "+n_StaParser+"Class))");
		out.println("");
		out.println("typedef struct _"+n_StaParser+"       "+n_StaParser+";");
		out.println("typedef struct _"+n_StaParser+"Class  "+n_StaParser+"Class;");
		out.println("");
		out.println("");
		out.println("struct _"+n_StaParser+" {");
		out.println("	"+n_Sta+"ParserBase parent;");
		out.println("	"+n_StaParserActions+" *parser_actions;");
		out.println("};");
		out.println("");
		out.println("struct _"+n_StaParser+"Class {");
		out.println("	"+n_Sta+"ParserBaseClass parent_class;");
		out.println("};");
		out.println("");
		out.println("GType "+n_sta_parser+"_get_type(void);");
		out.println("");
		out.println(""+n_StaParser+" *"+n_sta_parser+"_new("+n_Sta+"IScanner *scanner);");
		out.println("");
		out.println("G_END_DECLS");
		out.println("");
		out.println("#endif /* "+n_STA+""+n_PARSER+"_H_ */");
		
		
		out.flush();
		out.close();

	}


	/**
	 * Emit the production table.
	 * 
	 * @param out stream to produce output on.
	 */
	private void emitProductionTable(BnfModel model, PrintWriter out, String fqPreLow) {
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
		out.println("const short "+fqPreLow+"_production_table[] = {");
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
	private void doActionTable(PrintWriter out, ParseActionTable act_tab, String nsPreLow, boolean compact_reduces, String fqPreLow) {
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
		out.println("const short "+fqPreLow+"_action_table[] = {");
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
	private void doReduceTable(PrintWriter out, ParseReduceTable red_tab, String fqPreLow) {
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
		out.println("const short "+fqPreLow+"_reduce_table[] = {");
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
			if (configuration.symbolInfo) {
				out.print(prefix+"/* start row "+row+" */");
			}
			out.print("\n"+"/* "+row+" */"+prefix);
			for(int col=0; col<sa[row].length; col++) {
//				if (idx%24==0) {
//						out.print(",");
//					}
//					out.print("\n"+prefix);
//				} else {
				if (idx!=0) {
					out.print(",");
				}
//				}
				out.print(sa[row][col]);
				idx++;
			}
		}
		out.println();
	}



	
}
