package net.natpad.dung.sht;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import net.natpad.sht.ShtScanner;
import net.natpad.sht.ShtSymbol;
import net.natpad.sht.generated.CupParser;
import net.natpad.sht.generated.runtime.LrParserContext;
import net.natpad.sht.generated.runtime.LrScanner;
import net.natpad.sht.generated.runtime.LrSymbol;
import net.natpad.sht.model.Model;

public class ModelLoader {


	public Model parse(InputStream stream, final boolean reportErrors) throws IOException  {
		MlParserContext parserContext = null;
		ShtScanner shtScanner = new ShtScanner(new InputStreamReader(stream));
		
		CupParser cupParser = new CupParser() {
			
			@Override
			public void reportError(String message, Object info) {
				if (!reportErrors) {
					return;
				}
				System.err.print("reportErrors="+reportErrors);
				System.err.print(message);
				if (info instanceof ShtSymbol) {
					ShtSymbol sym = (ShtSymbol) info;
					System.err.println(" at character " + sym.left + ", row "+(sym.leftRow+1)+" of input");
				} else if (info instanceof LrSymbol) {
					LrSymbol symbol = (LrSymbol) info;
					if (symbol.left != -1)
						System.err.println(" at character " + symbol.left + " of input");
					else
						System.err.println("");
				} else {
					System.err.println("");
				}
				
			}
			
		};
		parserContext = new MlParserContext(shtScanner);
		LrSymbol rootToken;
		try {
			rootToken = cupParser.parse(parserContext);
			return rootToken!=null ? (Model) rootToken.value : null;
		} catch (Exception e) {
			if (reportErrors) {
				e.printStackTrace();
				
				
				LrSymbol peek = parserContext.getCurrent();
				System.out.println(""+peek);
			}
		}
		return null;
	}
	
	
	static class MlParserContext extends LrParserContext {
		
		public MlParserContext(LrScanner scanner) {
			super(scanner);
		}
		
		
		public LrSymbol getCurrent() {
			return cur_token;
		}
		
	}
}
