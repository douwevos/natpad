package net.natpad.cup.export.caterpillar;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Terminal;

public class SymbolExporter {

	public final CaterpillarConfiguration configuration;
	public final BnfModel model;

	public final String n_Sta, n_STA, n_sta;

	public final String n_StaSymbol, n_sta_symbol, n_SYMBOL;

	
	public SymbolExporter(BnfModel model, CaterpillarConfiguration configuration) {
		this.configuration = configuration;
		this.model = model;
		n_Sta = configuration.namespacePrefix;
		n_STA = n_Sta.toUpperCase();
		n_sta = n_Sta.toLowerCase();


		n_StaSymbol = n_Sta+configuration.getSymbolNameCamel();
		n_sta_symbol = n_sta+"_"+configuration.getSymbolNameLower();
		n_SYMBOL = configuration.getSymbolNameUpper();
	}

	public void doExport() throws IOException {

		
		PrintWriter out = new PrintWriter(new File(configuration.destDir, n_sta+configuration.getSymbolNameStripper()+".h"));

		out.println("#ifndef "+n_STA+""+n_SYMBOL+"_H_");
		out.println("#define "+n_STA+""+n_SYMBOL+"_H_");

		if (configuration.symbolInfo) {
			out.println();
			out.println("#define "+n_STA+"_"+n_SYMBOL+"_INFO  -1");
		}

		
		out.println();
		out.println("/* terminals */");
		
		
		List<Terminal> orderTerminals = model.terminals.getOrderedByIndex();
		int mxLength = 0;
		for(Terminal term : orderTerminals) {
			if (term.name().length()>mxLength) {
				mxLength = term.name().length();
			}
		}
		mxLength+=2;
		StringBuilder buf = new StringBuilder(); 
		
		/* walk over the terminals *//* later might sort these */
		for (Terminal term : orderTerminals) {
			buf.setLength(0);
			buf.append(term.name().toUpperCase());
			while(buf.length()<mxLength) {
				buf.append(' ');
			}
			/* output a constant decl for the terminal */
			out.println("#define "+n_STA+"_"+n_SYMBOL+"_TERM_" + buf.toString() + " " + term.index());
		}
		out.println("#define "+n_STA+"_"+n_SYMBOL+"_TERMINAL_COUNT " + orderTerminals.size());

		

		/* do the non terminals if they want them (parser doesn't need them) */
		if (configuration.outputNonTerminals) {
			out.println();
			out.println("  /* non terminals */");
			
			List<NonTerminal> orderNonTerminals = model.nonTerminals.getOrderedByIndex();
			mxLength = 0;
			for(NonTerminal term : orderNonTerminals) {
				if (term.name().length()>mxLength) {
					mxLength = term.name().length();
				}
			}
			mxLength+=2;
			buf = new StringBuilder(); 
			
			/* walk over the terminals *//* later might sort these */
			for (NonTerminal term : orderNonTerminals) {
				buf.setLength(0);
				buf.append(term.name().toUpperCase());
				while(buf.length()<mxLength) {
					buf.append(' ');
				}
				/* output a constant decl for the terminal */
				out.println("#define "+n_STA+"_"+n_SYMBOL+"_NONTERM_" + buf.toString() + " " + term.index());
			}			

			out.println("#define "+n_STA+"_"+n_SYMBOL+"_NONTERMINAL_COUNT " + orderNonTerminals.size());

			
		}

		
		if (configuration.symbolInfo) {
			out.println("const char *"+n_sta_symbol+"_terminal_as_string(int termIndex);");
			if (configuration.outputNonTerminals) {
				out.println("const char *"+n_sta_symbol+"_non_terminal_as_string(int termIndex);");
			}
		}
		
		out.println("#endif /* "+n_STA+""+n_SYMBOL+"_H_ */");
		
		out.flush();
		out.close();

		
		
		if (configuration.symbolInfo) {
			out = new PrintWriter(new File(configuration.destDir, n_sta+configuration.getSymbolNameStripper()+".c"));
			out.println("#include \""+n_sta+configuration.getSymbolNameStripper()+".h\"");
			out.println();
			
			orderTerminals = model.terminals.getOrderedByIndex();

			out.println("static const char *"+n_sta_symbol+"_term_strings[] = {");
			int index = 0;
			for(Terminal term : orderTerminals) {
				if (index!=0) {
					out.println(",");
				}
				out.print("	\""+term.name()+"\"");
				index++;
			}
			out.println();
			out.println("};");
			out.println();


			
			if (configuration.outputNonTerminals) {

				List<NonTerminal> orderNonTerminals = model.nonTerminals.getOrderedByIndex();
	
				out.println("static const char *"+n_sta_symbol+"_non_term_strings[] = {");
				index = 0;
				for(NonTerminal term : orderNonTerminals) {
					if (index!=0) {
						out.println(",");
					}
					out.print("	\""+term.name()+"\"");
					index++;
				}
				out.println();
				out.println("};");
				out.println();
			}
				
			
			
			out.println("const char *"+n_sta_symbol+"_terminal_as_string(int termIndex) {");
			out.println("	if ((termIndex>=0) && (termIndex<"+n_STA+"_"+n_SYMBOL+"_TERMINAL_COUNT)) {");
			out.println("		return "+n_sta_symbol+"_term_strings[termIndex];");
			out.println("	}");
			out.println("	return \"<index-out-of-range>\";");
			out.println("}");
			out.println();
				
			if (configuration.outputNonTerminals) {
				out.println("const char *"+n_sta_symbol+"_non_terminal_as_string(int nonTermIndex) {");
				out.println("	if ((nonTermIndex>=0) && (nonTermIndex<"+n_STA+"_"+n_SYMBOL+"_NONTERMINAL_COUNT)) {");
				out.println("		return "+n_sta_symbol+"_non_term_strings[nonTermIndex];");
				out.println("	}");
				out.println("	return \"<index-out-of-range>\";");
				out.println("}");
				out.println();
			}
			
			out.flush();
			out.close();
		}
		
		
	}
	
}
