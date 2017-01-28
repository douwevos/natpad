package net.natpad.cup.export.caterpillar;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.export.java.Emit;
import net.natpad.cup.model.bnf.ActionDeclaration;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.SymbolPart;

public class ActionsExporter {

	public final CaterpillarConfiguration configuration;
	public final BnfModel model;
	public final String n_Sta, n_sta, n_STA;
	
	public final String n_StaParserActions, n_sta_parser_actions, n_PARSER_ACTIONS;

	public ActionsExporter(BnfModel model, CaterpillarConfiguration configuration) {
		this.configuration = configuration;
		this.model = model;
		n_Sta = configuration.namespacePrefix;			// Sta
		n_STA = n_Sta.toUpperCase();					// STA
		n_sta = n_Sta.toLowerCase();					// sta
		
		n_StaParserActions = n_Sta+configuration.getParserActionsNameCamel();
		n_sta_parser_actions = n_sta+"_"+configuration.getParserActionsNameLower();
		n_PARSER_ACTIONS = configuration.getParserActionsNameUpper();
	}

	public void doExport(ParseActionTable action_table, ParseReduceTable reduce_table, int start_st,
					Production start_prod, boolean compact_reduces) throws IOException {

		String fname = ""+n_sta+configuration.getParserActionsNameStripper();
		
		PrintWriter out = new PrintWriter(new File(configuration.destDir, fname+".c"));
		out.println("#include \""+fname+".h\"");
		out.println();
		if (Emit.getActionCode()!=null) {
			out.println(Emit.getActionCode());
		}
		out.println();
		out.println("#include <logging/catlogdefs.h>");
		out.println("#define CAT_LOG_LEVEL CAT_LOG_ERROR");
		out.println("#define CAT_LOG_CLAZZ \""+n_StaParserActions+"\"");
		out.println("#include <logging/catlog.h>");
		out.println();
		out.println("G_DEFINE_TYPE("+n_StaParserActions+", "+n_sta_parser_actions+", G_TYPE_OBJECT)");
		out.println("");
//		out.println("static void _dispose(GObject *object);");
		out.println("");
		out.println("static void "+n_sta_parser_actions+"_class_init("+n_StaParserActions+"Class *clazz) {");
		out.println("	clazz->run_action = "+n_sta_parser_actions+"_run_action;");
//		out.println("	GObjectClass *object_class = G_OBJECT_CLASS(clazz);");
//		out.println("	object_class->dispose = _dispose;");
		out.println("}");
		out.println("");
		out.println("static void "+n_sta_parser_actions+"_init("+n_StaParserActions+" *parser) {");
		out.println("}");
		out.println("");
//		out.println("static void _dispose(GObject *object) {");
//		out.println("	"+n_StaParserActions+" *instance = "+nsPreUp+"_"+n_PARSER_ACTIONS+"(object);");
//		out.println("}");
//		out.println("");
		out.println(""+n_StaParserActions+" *"+n_sta_parser_actions+"_new() {");
		out.println("	"+n_StaParserActions+" *result = g_object_new("+n_STA+"_TYPE_"+n_PARSER_ACTIONS+", NULL);");
		out.println("	cat_ref_anounce(result);");
		out.println("	return result;");
		out.println("}");
		
		out.println("");
		emitActionCode(out, start_prod);

		out.flush();
		out.close();

		
		
		

		out = new PrintWriter(new File(configuration.destDir, fname+".h"));
		
		out.println("#ifndef "+n_STA+""+n_PARSER_ACTIONS+"_H_");
		out.println("#define "+n_STA+""+n_PARSER_ACTIONS+"_H_");
		out.println("");
		out.println("#include <caterpillar.h>");
		out.println("#include \"runtime/"+n_sta+"token.h\"");
		out.println("#include \"runtime/"+n_sta+"parsercontext.h\"");
		out.println("");
		out.println("G_BEGIN_DECLS");
		out.println("");
		out.println("#define "+n_STA+"_TYPE_"+n_PARSER_ACTIONS+"            ("+n_sta_parser_actions+"_get_type())");
		out.println("#define "+n_STA+"_"+n_PARSER_ACTIONS+"(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), "+n_sta_parser_actions+"_get_type(), "+n_StaParserActions+"))");
		out.println("#define "+n_STA+"_"+n_PARSER_ACTIONS+"_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), "+n_STA+"_TYPE_"+n_PARSER_ACTIONS+", "+n_StaParserActions+"Class))");
		out.println("#define "+n_STA+"_IS_"+n_PARSER_ACTIONS+"(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), "+n_STA+"_TYPE_"+n_PARSER_ACTIONS+"))");
		out.println("#define "+n_STA+"_IS_"+n_PARSER_ACTIONS+"_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), "+n_STA+"_TYPE_"+n_PARSER_ACTIONS+"))");
		out.println("#define "+n_STA+"_"+n_PARSER_ACTIONS+"_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), "+n_STA+"_TYPE_"+n_PARSER_ACTIONS+", "+n_StaParserActions+"Class))");
		out.println("");
		out.println("typedef struct _"+n_StaParserActions+"       "+n_StaParserActions+";");
		out.println("typedef struct _"+n_StaParserActions+"Class  "+n_StaParserActions+"Class;");
		out.println("");
		out.println("struct _"+n_StaParserActions+" {");
		out.println("	GObject parent;");
		out.println("};");
		out.println("");
		out.println("struct _"+n_StaParserActions+"Class {");
		out.println("	GObjectClass parent_class;");
		out.println("	"+n_Sta+"Token *(*run_action)("+n_StaParserActions+" *parser_actions, "+n_Sta+"ParserContext *parser_context, int cup_action_id);");
		out.println("};");
		out.println("");
		out.println("GType "+n_sta_parser_actions+"_get_type(void);");
		out.println("");
		out.println(""+n_StaParserActions+" *"+n_sta_parser_actions+"_new();");
		out.println("");
		out.println(""+n_Sta+"Token *"+n_sta_parser_actions+"_run_action("+n_StaParserActions+" *parser_actions, "+n_Sta+"ParserContext *parser_context, int cup_action_id);");
		out.println("");
		out.println("G_END_DECLS");
		out.println("");
		out.println("#endif /* "+n_STA+""+n_PARSER_ACTIONS+"_H_ */");
		
		
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
		out.print(""+n_Sta+"Token *"+n_sta_parser_actions+"_run_action("+n_StaParserActions+" *parser_actions, "+n_Sta+"ParserContext *parser_context, int cup_action_id) {");
		out.println("	"+n_Sta+"ParserContextClass *context_class = "+n_STA+"_PARSER_CONTEXT_GET_CLASS(parser_context);");

		
		/* declaration of result symbol */
		/*
		 * New declaration!! now return Symbol 6/13/96 frankf
		 */
		out.println("	/* Symbol object for return from actions */");
		out.println("	"+n_Sta+"Token *cup_result = NULL;");
		out.println();

		/* switch top */
		out.println("	/* select the action based on the action number */");
		out.println("	switch(cup_action_id) {");

		/* emit action code for each production as a separate case */
		for (Production prod : model.productions) {
			/* case label */
			out.println("		case " + prod.index() + ": { // " + prod.toSimpleString());

			String codetext = "";
			if (prod.action() != null && prod.action().code_string() != null) {
				codetext = prod.action().code_string();
			}

			boolean createResult = !findCupAssignement(codetext); 

			
			/* create the result symbol */
			
			
			/*
			 * make the variable RESULT which will point to the new Symbol (see below) and be changed by action code
			 * 6/13/96 frankf
			 */
			String stackType = prod.lhs().getSymbol().getStackType();
			if (stackType==null || "Object".equals(stackType)) {
				stackType = "GObject";
			}
			
			if (createResult) {
				out.println("			" +  stackType + " *RESULT = NULL;");
			}

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
					makeDeclaration(declaration, out, codetext);
				}
			}
			
			
			/* if there is an action string, emit it */
			if (codetext.length()>0) {
				String marker = "$cup.create.token$";
				int markeridx = codetext.indexOf(marker);
				if (markeridx>=0) {
					out.println(codetext.substring(0, markeridx));
					codetext = codetext.substring(markeridx+marker.length());
				} else {
					out.println(codetext);
					codetext = "";
				}
			}
				
			/*
			 * Create the code that assigns the left and right values of the new Symbol that the production is reducing
			 * to
			 */
			if (createResult) {
	//			if (Emit.lr_values()) {
				if (true) {
					int loffset;
					String leftstring, leftrowstring, rightstring, rightrowstring;
					int roffset = 0;
					rightrowstring = "context_class->getFromTop(parser_context, "+ roffset + ")->right_row";
					rightstring = "context_class->getFromTop(parser_context, "+ roffset + ")->right";
					if (prod.rhsLength() == 0) {
						leftstring = rightstring;
						leftrowstring = rightrowstring;
					} else {
						loffset = prod.rhsLength() - 1;
						leftstring = "context_class->getFromTop(parser_context, "+ loffset + ")->left";
						leftrowstring = "context_class->getFromTop(parser_context, "+ loffset + ")->left_row";
					}
					
					
					out.println("			cup_result  = "+n_sta+"_iscanner_create_token(parser_context->scanner, "+prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/" + ", -1, FALSE, FALSE, FALSE, " + leftstring + ", " + leftrowstring + ", " + rightstring + ", "+rightrowstring+", G_OBJECT(RESULT));"); 
					
//					out.println("			cup_result  = "+nsPreLow+"_token_new_full("
//									+ prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/" + ", FALSE, "
//									+ leftstring + ", " + leftrowstring + ", " + rightstring + ", "+rightrowstring+", G_OBJECT(RESULT));");
				} else {
					out.println("			cup_result = cup_token_new("
									+ prod.lhs().getSymbol().index() + "/*" + prod.lhs().getSymbol().name() + "*/"
									+ ", G_OBJECT(RESULT));");
				}
			}
			
			if (codetext.length()>0) {
				out.println(codetext);
			}

			
			out.println("		}");

			/* if this was the start production, do action for accept */
			if (prod == start_prod) {
				out.println("		/* ACCEPT */");
				out.println("		"+n_sta+"_parser_context_done_parsing(parser_context);");
			}

			/* code to return lhs symbol */
			if (configuration.symbolInfo) {
				out.println("		cup_result->symbol_text = \""+prod.toSimpleString()+"\";");				
			}
			out.println("		return cup_result;");
			out.println();
		}

		/* end of switch */
//		out.println("			default:");
//		out.println("				throw new Exception(\"Invalid action number found in internal parse table\");");
		out.println("	}");
		out.println("	return cup_result;");

		/* end of method */
		out.println("}");

		out.println();
	}
	
	
	
	private boolean findCupAssignement(String codetext) {
		int idx = 0;
		while(true) {
			idx = codetext.indexOf("cup_result", idx);
			if (idx>=0) {
				idx+=10;
				int idxb = codetext.indexOf("=", idx);
				if (idxb>0) {
					String sp = codetext.substring(idx, idxb).trim();
					if (sp.length()==0) {
						return true;
					}
				}
			} else {
				break;
			}
		}
		return false;
	}

	
	
	
	private boolean findVariableUse(String code, String identifier) {
		int startIdx = 0;
		while(true) {
			int nextIdx = code.indexOf(identifier, startIdx);
			startIdx = nextIdx+1;
			if (nextIdx<0) {
				return false;
			}
			
			if (nextIdx>0) {
				char prechar = code.charAt(nextIdx-1);
				if (Character.isJavaIdentifierStart(prechar)) {
					continue;
				}
			}
			nextIdx+=identifier.length();
			if (nextIdx<code.length()) {
				char postChar = code.charAt(nextIdx);
				if (Character.isJavaIdentifierPart(postChar)) {
					continue;
				}
			}
			return true;
		}
	}
	
	/**
	 * Return label declaration code
	 * 
	 * @param labelname    the label name
	 * @param stack_type   the stack type of label?
	 */
	public void makeDeclaration(ActionDeclaration declaration, PrintWriter out, String actionCode) {
		/* Put in the left/right value labels */
//		if (Emit.lr_values()) {
		boolean leftUsed = false;
		boolean rightUsed = false;
		boolean rowUsed = false;
		boolean mainUsed = false;
		String leftLabel = "cup_" + declaration.label + "left";
		String rowLabel = "cup_" + declaration.label + "row";
		String rightLabel = "cup_" + declaration.label + "right";
		String mainLabel = declaration.label;
		String baseLabel = "cup_" + declaration.label;

		if (true) {
			leftUsed = findVariableUse(actionCode,leftLabel);
			rowUsed = findVariableUse(actionCode,rowLabel);
			rightUsed = findVariableUse(actionCode,rightLabel);
		}
		mainUsed = findVariableUse(actionCode, mainLabel);

		if (leftUsed || rowUsed || rightUsed || mainUsed || findVariableUse(actionCode, baseLabel)) {
			out.println("				"+n_Sta+"Token *" + baseLabel + " = context_class->getFromTop(parser_context, "+declaration.offset+");");
		}
		
		if (leftUsed) {
			out.println("				int " + leftLabel + " = "+baseLabel+"->left;");
		}
		if (rowUsed) {
			out.println("				int " + rowLabel + " = "+baseLabel+"->row;");
		}
		if (rightUsed) {
			out.println("				int " + rightLabel + " = "+baseLabel+"->right;");
		}
		if (mainUsed) {
			/* otherwise, just declare label. */
			String stackType = declaration.stackType==null || "Object".equals(declaration.stackType) ? "GObject" : declaration.stackType;
			out.println("				" + stackType + " *" + mainLabel + " = (" + stackType + " *) ("+baseLabel+"->value);");
		}

	}
	
}
