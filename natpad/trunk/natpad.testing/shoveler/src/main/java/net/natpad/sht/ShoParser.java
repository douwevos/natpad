package net.natpad.sht;

import java.io.InputStream;
import java.io.InputStreamReader;

import net.natpad.sht.generated.CupParser;
import net.natpad.sht.generated.runtime.LrParserContext;
import net.natpad.sht.generated.runtime.LrSymbol;
import net.natpad.sht.model.Model;

public class ShoParser {

	
	public Model parse(InputStream stream) throws Exception {
		LrParserContext parserContext = null;
		ShtScanner shtScanner = new ShtScanner(new InputStreamReader(stream));
		
		CupParser cupParser = new CupParser();
		parserContext = new LrParserContext(shtScanner);
		LrSymbol rootToken = cupParser.parse(parserContext);
		
		return rootToken!=null ? (Model) rootToken.value : null;
	}
	
}
