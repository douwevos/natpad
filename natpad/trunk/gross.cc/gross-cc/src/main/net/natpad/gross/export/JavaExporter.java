package net.natpad.gross.export;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Date;
import java.util.List;

import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.ProductionPart;
import net.natpad.gross.build.SymbolWithType;
import net.natpad.gross.runtime.NonTerminal;
import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.Terminal;

public class JavaExporter {
	
	public final CCStateMachine stateMachine;
	public final AstFile astFile;
	public final String packageName;
	public final String prefix;

	StateMachineEncoder stateMachineEncoder;
	
	public JavaExporter(CCStateMachine stateMachine, AstFile astFile, String packageName, String prefix) {
		this.stateMachine = stateMachine;
		this.astFile = astFile;
		this.packageName = packageName;
		this.prefix = prefix;
		stateMachineEncoder = new StateMachineEncoder(stateMachine);
	}
	
	
	public void runExport(File mbase) throws IOException {
		File base = null;
		if (packageName!=null & !packageName.isEmpty()) {
			String dirName = packageName.replaceAll("\\.", File.separator);
			base = new File(mbase, dirName);
		} else {
			base = mbase;
		}
		base.mkdirs();
		exportStateMachineConfiguration(base);
		exportActions(base);
		
		if (astFile!=null) {
			generateAstFiles(mbase);
		}
		
	}

	private void generateAstFiles(File mbase) throws IOException {
	}


//	private void generateAstFiles(File mbase) throws IOException {
//		String astPackageName = packageName==null ? "ast" : packageName+".ast";
//		String dirName = astPackageName.replaceAll("\\.", File.separator);
//		File base = new File(mbase, dirName);
//		base.mkdirs();
//		
//		List<AstLine> astLines = astFile.astLines.enlist();
//		HashMap<String, TypeDescription> typeMap = new HashMap<String, TypeDescription>();
//		for(AstLine line : astLines) {
//			LineBody lineBody = line.lineBody;
//			if (lineBody.typeDescription!=null) {
//				typeMap.put(line.symbolName, lineBody.typeDescription);
//			}
//		}
//
//
//		for(AstLine line : astLines) {
//			LineBody lineBody = line.lineBody;
//			if (lineBody.typeDescription!=null) {
//				String qfTypeName = lineBody.typeDescription.description;
//				File file = new File(base, qfTypeName+".java");
//				PrintWriter out = new PrintWriter(new FileOutputStream(file, false));
//				out.println("package "+astPackageName+";");
//				out.println("");
//				
//
//				CCModel model = stateMachine.model;
//				
//				out.println("public class "+qfTypeName+" {");
//				
//				
//				HashMap<String, String> fields = new HashMap<String, String>();
//				
//				HashSet<String> constructors = new HashSet<String>();
//				NonTerminalExt lhs = model.getNonTerminalByName(line.symbolName);
//				for(Production prod : lhs) {
//					out.println();
//					StringBuilder constr = new StringBuilder();
//					StringBuilder args = new StringBuilder();
//					boolean hasParams = false;
//					for(int rhsIdx=0; rhsIdx<prod.rhsCount(); rhsIdx++) {
//						ProductionPart rhsAt = prod.rhsAt(rhsIdx);
//						if (rhsAt.label!=null) {
//							if (!hasParams) {
//								hasParams= true;
//							} else {
//								args.append(", ");
//							}
//							
//							
//							String argQName = extractTypeForSymbolName(rhsAt.symbol.name, typeMap);
//
//
//							args.append(argQName+" "+rhsAt.label);
//							
//							constr.append(rhsAt.label+";");
//						}
//					}
//					
//					if (constructors.add(constr.toString())) {
//						out.println("	public "+qfTypeName+"("+args+") {");
//						for(int rhsIdx=0; rhsIdx<prod.rhsCount(); rhsIdx++) {
//							ProductionPart rhsAt = prod.rhsAt(rhsIdx);
//							if (rhsAt.label!=null) {
//								out.println("		this."+rhsAt.label+" = "+rhsAt.label+";");
//								
//								String argQName = extractTypeForSymbolName(rhsAt.symbol.name, typeMap);
//								fields.put(rhsAt.label, argQName);
//							}
//						}
//						
//						out.println("	}");
//					}
//				}
//
//				
//				out.println("");
//				for(String fieldName : fields.keySet()) {
//					String typeName = fields.get(fieldName);
//					out.println("	public "+typeName+" "+fieldName+";");
//				}
//
//				out.println("}");
//
//				
//				out.flush();
//				out.close();
//			}
//		}
//
//		
//	}


//	private String extractTypeForSymbolName(String name, HashMap<String, TypeDescription> typeMap) {
//		String result = null;
//		TypeDescription typeDescription = typeMap.get(name);
//		if (typeDescription!=null) {
//			result = typeDescription.description;
//		}
//		if (result==null || result.isEmpty()) {
//			Symbol symbolByName = stateMachine.model.getSymbolByName(name);
//			if (symbolByName instanceof SymbolWithType) {
//				SymbolWithType swt = (SymbolWithType) symbolByName;
//				result = swt.getType();
//			}
//		}
//		if (result == null || result.isEmpty()) {
//			result = "Object";
//		}
//		return result;
//	}
//

	private void exportActions(File base) throws FileNotFoundException {
		File symbolsFile = new File(base, "Actions.java");
		PrintWriter out = new PrintWriter(new FileOutputStream(symbolsFile, false));
		if (packageName!=null) {
			out.println("package "+packageName+";");
		}
		out.println("import net.natpad.gross.runtime.Symbol;");
		out.println("import net.natpad.gross.runtime.Terminal;");
		out.println("import net.natpad.gross.runtime.NonTerminal;");
		out.println("import net.natpad.gross.runtime.Token;");
		out.println("import net.natpad.gross.runtime.Fork;");
		out.println("import net.natpad.gross.runtime.StackEntry;");
		out.println("import net.natpad.gross.runtime.ParserContext;");
		out.println("import net.natpad.gross.runtime.ReduceLayout;");

		
		final CCModel model = stateMachine.model;
		String headerCodeText = model.getHeaderCodeText();
		if (headerCodeText!=null) {
			out.println(headerCodeText);
		}
		
		out.println();
		String dateAsStr = new SimpleDateFormat("yyyy.MM.dd G 'at' HH:mm:ss z").format(new Date());
		out.println("/* This file was generated by Gross-CC on "+dateAsStr+" */");
		out.println("public class Actions implements ParserContext.ReduceAction {");
		out.println();

		out.println("	public <T> T extract(Fork fork, ReduceLayout reduceLayout, int offset) {");
		out.println("		T result = null;");
		out.println("		StackEntry fe = fork.getEntry(reduceLayout.calculateStackOffset(offset));");
		out.println("		if (fe!=null) {");
		out.println("			result = (T) fe.lookahead.getValue();");
		out.println("		}");
		out.println("		return result;");
		out.println("	}");

		out.println();
		
		out.println("	public Object doAction(Fork fork, ReduceLayout reduceLayout, int actionId) {");
		out.println("		Object result = null;");
		out.println("		switch(actionId) {");
		
		

		int aidx=0;
		for(Production prod : stateMachineEncoder.actionIndexList) {
			
			
			StringBuilder buf = new StringBuilder();
			buf.append(prod.lhs.name);
			buf.append(" ⟶");
			for(ProductionPart p : prod) {
				buf.append(' ').append(p.symbol.name);
				if (p.label!=null) {
					buf.append(':').append(p.label);
				}
			}
			
			
			out.println("			case "+aidx+" : { /* "+buf+" */");

			
			List<ProductionPart> excluded = prod.getExcluded();
			if (excluded!=null) {
				for(ProductionPart p : excluded) {
					if (p.label!=null) {
						String type = ((SymbolWithType) p.symbol).getType();
						if (type==null) {
							type = "Object";
						}
						out.println("				"+type+" "+p.label+" = null;  /* was optional */");
					}
				}
			}

			
			int idx = 0;
			for(ProductionPart p : prod) {
				if (p.label!=null) {
					String type = ((SymbolWithType) p.symbol).getType();
					if (type==null) {
						type = "Object";
					}
					out.println("				"+type+" "+p.label+" = extract(fork, reduceLayout, "+idx+");");
				}
				idx++;
			}
			

			String actionCode = prod.getActionCode();
			if (actionCode!=null) {
				out.println(""+actionCode);
			}
			aidx++;
			out.println("			} break;");
		}

		
		out.println("		}");
		out.println("		return result;");
		out.println("	}");

		
		out.println("}");
		out.flush();
		out.close();
	}

	
	private void exportStateMachineConfiguration(File base) throws FileNotFoundException {
		File symbolsFile = new File(base, "GrossParserTables.java");
		PrintWriter out = new PrintWriter(new FileOutputStream(symbolsFile, false));
		if (packageName!=null) {
			out.println("package "+packageName+";");
		}
		out.println("import net.natpad.gross.runtime.Symbol;");
		out.println("import net.natpad.gross.runtime.Terminal;");
		out.println("import net.natpad.gross.runtime.NonTerminal;");

		out.println("");
		String dateAsStr = new SimpleDateFormat("yyyy.MM.dd G 'at' HH:mm:ss z").format(new Date());
		out.println("/* This file was generated by Gross-CC on "+dateAsStr+" */");
		out.println("public class GrossParserTables {");
		out.println("");
		
		final CCModel model = stateMachine.model;
		
		
		Collection<Symbol> allSymbols = model.allSymbols();
		int max= 0;
		for (Symbol symbol : allSymbols) {
			if (symbol.id>max) {
				max = symbol.id;
			}
		}
		Symbol listed[] = new Symbol[max+1];
		for (Symbol symbol : allSymbols) {
			listed[symbol.id] = symbol;
		}
		
		
		
		out.println("	public final Symbol symbols[] = new Symbol[] {");
		
		boolean isFirst = true;
		for (Symbol symbol : listed) {
			if (symbol instanceof NonTerminal) {
				out.println("		"+(isFirst ? "  " : ", ")+"new NonTerminal(\""+symbol.name+"\", "+symbol.id.intValue()+")");
			} else if (symbol instanceof Terminal) {
				out.println("		"+(isFirst ? "  " : ", ")+"new Terminal(\""+symbol.name+"\", "+symbol.id.intValue()+")");
			} else {
				out.println("		"+(isFirst ? "  " : ", ")+"null");
			}
			isFirst = false;
		}
		out.println("	};");
		
		out.println();
		out.println("	public final int stateCount = "+stateMachine.stateCount()+";");
		out.println("	public final String cfgText =");
		String cfgText = stateMachineEncoder.createStateConfigurationText();
		for(int idx=0; idx<cfgText.length(); idx += 100) {
			int e = idx+100;
			if (e>cfgText.length()) {
				e=cfgText.length();
			}
			out.println("				"+(idx==0 ? "  " : "+ ")+"\""+cfgText.substring(idx, e)+"\"");
		}
		out.println("			;");
		out.println();
		out.println("}");
		out.flush();
		out.close();
	}
	
}
