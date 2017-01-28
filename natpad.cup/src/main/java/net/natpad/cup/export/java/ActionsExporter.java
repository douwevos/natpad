package net.natpad.cup.export.java;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.export.java.ast.AstClassName;
import net.natpad.cup.export.java.ast.AstJava;
import net.natpad.cup.model.bnf.ActionDeclaration;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;

public class ActionsExporter {

	public final JavaExportInfo exportInfo;
	public final BnfModel model;

	public ActionsExporter(BnfModel model, JavaExportInfo exportInfo) {
		this.exportInfo = exportInfo;
		this.model = model;
	}

	public void doExport(ParseActionTable action_table, ParseReduceTable reduce_table, int start_st,
					Production start_prod, boolean compact_reduces) throws IOException {

		AstJava astJava = new AstJava();
		astJava.setClassName(exportInfo.actionClass);
		
		
		for(String importName : Emit.getImportList()) {
			astJava.addImport(new AstClassName(importName));
		}

		astJava.addImport(exportInfo.symbolRuntimeClass);
		astJava.addImport(exportInfo.contextRuntimeClass);
		
		PrintWriter out = astJava.openClass(exportInfo.destDir);

		/* put out the action code class */
		emitActionCode(astJava, out, start_prod);
		
		astJava.closeClass(out);
	}

	
	
	/**
	 * Emit code for the class holding the actual action code.
	 * 
	 * @param out stream to produce output on.
	 * @param start_prod the start production of the grammar.
	 */
	protected void emitActionCode(AstJava astJava, PrintWriter out, Production start_prod) {

		astJava.increaseIndent();
		astJava.increaseIndent();
		astJava.increaseIndent();
		astJava.increaseIndent();

		/* class header */
		out.println();

		/* user supplied code */
		if (Emit.getActionCode() != null) {
			out.println();
			out.println(Emit.getActionCode());
		}

		/* constructor */
		out.println();
		out.println("	" + exportInfo.actionClass.className + "() {");
		out.println("	}");

		/* action method head */
		out.println();
		out.println("	/** Method with the actual generated action code. */");
		out.print("	public final "+exportInfo.symbolRuntimeClass.className+" runAction("+exportInfo.contextRuntimeClass.className+" "+exportInfo.prefix("Context")+", int " + exportInfo.prefix("ActionId"));
		out.println(") throws Exception {");

		/* declaration of result symbol */
		/*
		 * New declaration!! now return Symbol 6/13/96 frankf
		 */
		out.println("		/* Symbol object for return from actions */");
		out.println("		"+exportInfo.symbolRuntimeClass.className+" " + exportInfo.prefix("Result") + ";");
		out.println();

		/* switch top */
		out.println("		/* select the action based on the action number */");
		out.println("		switch (" + exportInfo.prefix("ActionId") + ") {");

		/* emit action code for each production as a separate case */
		for (Production prod : model.productions) {
			/* case label */
			out.println("			case " + prod.index() + ": { // " + prod.toSimpleString());

			/* create the result symbol */
			/*
			 * make the variable RESULT which will point to the new Symbol (see below) and be changed by action code
			 * 6/13/96 frankf
			 */
			String stackType = prod.lhs().getSymbol().getStackType();
			if (stackType==null) {
				stackType = "Object";
			}
			out.println("				" + stackType + " RESULT = null;");

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
				int index = prod.rhsLength() - i - 1; // last rhs is on top.
				out.println("				" + "// propagate RESULT from " + s.name());
				out.println("				" + "if ( " + exportInfo.prefix("Context")+".getFromTop(" + index + ").value != null ) {");
				out.println("					" + "RESULT = " + exportInfo.prefix("Context")+".getFromTop(" + index + ").value;");
				out.println("				}");
			}

			
			List<ActionDeclaration> declarations = prod.getDeclarations();
			if (declarations!=null) {
				for(ActionDeclaration declaration : declarations) {
					makeDeclaration(declaration, out);
				}
			}
			
			/* if there is an action string, emit it */
			if (prod.action() != null && prod.action().code_string() != null && !prod.action().equals("")) {
				astJava.println(prod.action().code_string());
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
			if (Emit.lr_values()) {
				int loffset;
				String leftstring, rightstring;
				int roffset = 0;
				rightstring = exportInfo.prefix("Context") + ".getFromTop("+ roffset + ").right";
				if (prod.rhsLength() == 0) {
					leftstring = rightstring;
				} else {
					loffset = prod.rhsLength() - 1;
					leftstring = exportInfo.prefix("Context") + ".getFromTop("+ loffset + ").left";
				}
				out.println("				" + exportInfo.prefix("Result") + " = new "+exportInfo.symbolRuntimeClass.className+"("
								+ prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/" + ", "
								+ leftstring + ", " + rightstring + ", RESULT);");
			} else {
				out.println("				" + exportInfo.prefix("Result") + " = new "+exportInfo.symbolRuntimeClass.className+"("
								+ prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/"
								+ ", RESULT);");
			}
			out.println("			}");

			/* if this was the start production, do action for accept */
			if (prod == start_prod) {
				out.println("			/* ACCEPT */");
				out.println("			" + exportInfo.prefix("Context") + ".doneParsing();");
			}

			/* code to return lhs symbol */
			out.println("			return " + exportInfo.prefix("Result") + ";");
			out.println();
		}

		/* end of switch */
		out.println("			default:");
		out.println("				throw new Exception(\"Invalid action number found in internal parse table\");");
		out.println("			}");

		/* end of method */
		out.println("		}");

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
		out.println("				" + exportInfo.symbolRuntimeClass.className + " " + exportInfo.prefix(declaration.label) + " = " + exportInfo.prefix("Context")+".getFromTop("+declaration.offset+");");
		if (Emit.lr_values()) {
			out.println("				int " + declaration.label + "left = "+exportInfo.prefix(declaration.label)+".left;");
			out.println("				int " + declaration.label + "right = "+exportInfo.prefix(declaration.label)+".right;");
		}

		/* otherwise, just declare label. */
		String stackType = declaration.stackType==null ? "Object" : declaration.stackType;
		out.println("				" + stackType + " " + declaration.label + " = (" + stackType + ") "+exportInfo.prefix(declaration.label)+".value;");

	}
	
}
