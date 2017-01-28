package net.natpad.cup.export.purec;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.model.bnf.ActionDeclaration;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;

public class ActionsExporter {

	public final PureCConfiguration configuration;
	public final BnfModel model;
	public final String nsPre;
	public final String nsPreUp;
	public final String nsPreLow;

	public ActionsExporter(BnfModel model, PureCConfiguration configuration) {
		this.configuration = configuration;
		this.model = model;
		nsPre = configuration.namespacePrefix;			// Sta
		nsPreUp = nsPre.toUpperCase();					// STA
		nsPreLow = nsPre.toLowerCase();					// sta
	}

	public void doExport(ParseActionTable action_table, ParseReduceTable reduce_table, int start_st,
					Production start_prod, boolean compact_reduces) throws IOException {

		String fname = ""+nsPreLow+"parseractions";							// parser
		
		PrintWriter out = new PrintWriter(new File(configuration.destDir, fname+".c"));
		out.println("#include \""+fname+".h\"");
		out.println();
		emitActionCode(out, start_prod);
		out.flush();
		out.close();

		
		
		

		out = new PrintWriter(new File(configuration.destDir, fname+".h"));
		
		out.println("#ifndef "+nsPreUp+"PARSERACTIONS_H_");
		out.println("#define "+nsPreUp+"PARSERACTIONS_H_");
		out.println("");
		out.println("#include <stdlib.h>");
		out.println("#include \"runtime/"+nsPreLow+"token.h\"");
		out.println("#include \"runtime/"+nsPreLow+"parsercontext.h\"");
		out.println("");
		out.println(""+nsPre+"Token *"+nsPreLow+"_parser_actions_run_action("+nsPre+"ParserContext *parser_context, int cup_action_id);");
		out.println("");
		out.println("#endif /* "+nsPreUp+"PARSERACTIONS_H_ */");
		
		
		out.flush();
		out.close();
		
	}
		
		
	/**
	 * Emit code for the class holding the actual action code.
	 * 
	 * @param out stream to produce output on.
	 * @param start_prod the start production of the grammar.
	 */
	protected void emitActionCode(PrintWriter out, Production start_prod) {

		/* action method head */
//		out.println("	/** Method with the actual generated action code. */");
		out.print(""+nsPre+"Token *"+nsPreLow+"_parser_actions_run_action("+nsPre+"ParserContext *parser_context, int cup_action_id) {");

		/* declaration of result symbol */
		/*
		 * New declaration!! now return Symbol 6/13/96 frankf
		 */
		out.println("	/* Symbol object for return from actions */");
		out.println("	"+nsPre+"Token *cup_result = NULL;");
		out.println();

		/* switch top */
		out.println("	/* select the action based on the action number */");
		out.println("	switch(cup_action_id) {");

		/* emit action code for each production as a separate case */
		for (Production prod : model.productions) {
			/* case label */
			out.println("		case " + prod.index() + ": { // " + prod.toSimpleString());

			/* create the result symbol */
			/*
			 * make the variable RESULT which will point to the new Symbol (see below) and be changed by action code
			 * 6/13/96 frankf
			 */
			String stackType = prod.lhs().getSymbol().getStackType();
			if (stackType==null) {
				stackType = "void";
			}
			
			out.println("			" +  stackType + " *RESULT = NULL;");

			/*
			 * Add code to propagate RESULT assignments that occur in action code embedded in a production (ie,
			 * non-rightmost action code). 24-Mar-1998 CSA
			 */
			for (int i = 0; i < prod.rhsLength(); i++) {
				// only interested in non-terminal symbols.
				if (!(prod.rhs(i) instanceof SymbolPart))
					continue;
				Symbol s = ((SymbolPart) prod.rhs(i)).getSymbol();
				if (!(s instanceof NonTerminal))
					continue;
				// skip this non-terminal unless it corresponds to
				// an embedded action production.
				if (((NonTerminal) s).is_embedded_action == false)
					continue;
				// OK, it fits. Make a conditional assignment to RESULT.
//				int index = prod.rhsLength() - i - 1; // last rhs is on top.
//				out.println("				" + "// propagate RESULT from " + s.name());
//				out.println("				" + "if ( " + exportInfo.prefix("Context")+".getFromTop(" + index + ").value != null ) {");
//				out.println("					" + "RESULT = " + exportInfo.prefix("Context")+".getFromTop(" + index + ").value;");
//				out.println("				}");
			}
//
//			
			List<ActionDeclaration> declarations = prod.getDeclarations();
			if (declarations!=null) {
				for(ActionDeclaration declaration : declarations) {
					makeDeclaration(declaration, out);
				}
			}
			
			/* if there is an action string, emit it */
			if (prod.action() != null && prod.action().code_string() != null && !prod.action().equals("")) {
				out.println(prod.action().code_string());
//				out.println(prod.action().code_string());
			}

			/*
			 * here we have the left and right values being propagated. must make this a command line option. frankf
			 * 6/18/96
			 */

			/*
			 * Create the code that assigns the left and right values of the new Symbol that the production is reducing
			 * to
			 */
//			if (Emit.lr_values()) {
			if (true) {
				int loffset;
				String leftstring, rightstring, rowstring;
				int roffset = 0;
				rowstring = ""+nsPreLow+"_parser_context_get_from_top(parser_context, "+ roffset + ")->row";
				rightstring = ""+nsPreLow+"_parser_context_get_from_top(parser_context, "+ roffset + ")->right";
				if (prod.rhsLength() == 0) {
					leftstring = rightstring;
				} else {
					loffset = prod.rhsLength() - 1;
					leftstring = ""+nsPreLow+"_parser_context_get_from_top(parser_context, "+ loffset + ")->left";
					rowstring = ""+nsPreLow+"_parser_context_get_from_top(parser_context, "+ loffset + ")->row";
				}
				out.println("			cup_result  = "+nsPreLow+"_token_new("
								+ prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/" + ", "
								+ leftstring + ", " + rightstring + ", "+rowstring+", RESULT);");
			} else {
				out.println("			cup_result = cup_token_new("
								+ prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/"
								+ ", RESULT);");
			}
			out.println("		}");

			/* if this was the start production, do action for accept */
			if (prod == start_prod) {
				out.println("		/* ACCEPT */");
				out.println("		"+nsPreLow+"_parser_context_done_parsing(parser_context);");
			}

			/* code to return lhs symbol */
			if (configuration.extraDebugInfo) {
				out.println("		cup_result->symbol_text = \""+prod.toSimpleString()+"\";");				
			}
			out.println("		return cup_result;");
			out.println();
		}

		/* end of switch */
//		out.println("			default:");
//		out.println("				throw new Exception(\"Invalid action number found in internal parse table\");");
		out.println("	}");

		/* end of method */
		out.println("}");

		out.println();
	}
	
	
	
	/**
	 * Return label declaration code
	 * 
	 * @param labelname    the label name
	 * @param stack_type   the stack type of label?
	 * @author frankf
	 */
	public void makeDeclaration(ActionDeclaration declaration, PrintWriter out) {
		/* Put in the left/right value labels */
		out.println("				"+nsPre+"Token *cup_" + declaration.label + " = "+nsPreLow+"_parser_context_get_from_top(parser_context, "+declaration.offset+");");
//		if (Emit.lr_values()) {
		if (true) {
			out.println("				int cup_" + declaration.label + "left = cup_"+declaration.label+"->left;");
			out.println("				int cup_" + declaration.label + "row = cup_"+declaration.label+"->row;");
			out.println("				int cup_" + declaration.label + "right = cup_"+declaration.label+"->right;");
		}

		/* otherwise, just declare label. */
		String stackType = declaration.stackType==null ? "void" : declaration.stackType;
		out.println("				" + stackType + " *" + declaration.label + " = (" + stackType + " *) (cup_"+declaration.label+"->value);");

	}
	
}
