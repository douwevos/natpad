package net.natpad.cup.export.purec;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Terminal;

public class SymbolExporter {

	public final PureCConfiguration configuration;
	public final BnfModel model;
	
	public SymbolExporter(BnfModel model, PureCConfiguration configuration) {
		this.configuration = configuration;
		this.model = model;
	}

	public void doExport() throws IOException {

		String Prefix = configuration.namespacePrefix;
		String PREFIX = Prefix.toUpperCase();
		String prefix = Prefix.toLowerCase();
		
		PrintWriter out = new PrintWriter(new File(configuration.destDir, prefix+"symbol.h"));

		out.println("#ifndef "+PREFIX+"SYMBOL_H_");
		out.println("#define "+PREFIX+"SYMBOL_H_");

		
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
			out.println("#define "+PREFIX+"_TERM_" + buf.toString() + " " + term.index());
		}

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
				out.println("#define "+PREFIX+"_NONTERM_" + buf.toString() + " " + term.index());
			}			
			
		}

		
		if (configuration.extraDebugInfo) {
			out.println();
			out.println("const char *"+prefix+"_terminal_as_string(int termIndex);");
			out.println("const char *"+prefix+"_non_terminal_as_string(int termIndex);");
		}
		
		out.println("#endif /* "+PREFIX+"SYMBOL_H_ */");
		
		out.flush();
		out.close();

		
		
		if (configuration.extraDebugInfo) {
			out = new PrintWriter(new File(configuration.destDir, prefix+"symbol.c"));
			out.println("#include \""+prefix+"symbol.h\"");
			out.println();
			
			orderTerminals = model.terminals.getOrderedByIndex();

			out.println("static const char *"+prefix+"_term_strings[] = {");
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


			
			
			List<NonTerminal> orderNonTerminals = model.nonTerminals.getOrderedByIndex();

			out.println("static const char *"+prefix+"_non_term_strings[] = {");
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
			
			
			
			
			
			
			out.println("const char *"+prefix+"_terminal_as_string(int termIndex) {");
			out.println("	return "+prefix+"_term_strings[termIndex];");
			out.println("}");
			out.println();
			
			
			out.println("const char *"+prefix+"_non_terminal_as_string(int termIndex) {");
			out.println("	return "+prefix+"_non_term_strings[termIndex];");
			out.println("}");
			out.println();

			
			
			out.flush();
			out.close();
		}
		
		
	}
	
}
