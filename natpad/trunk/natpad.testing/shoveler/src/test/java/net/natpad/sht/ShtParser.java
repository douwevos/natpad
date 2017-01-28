package net.natpad.sht;

import java.io.FileReader;
import java.io.StringReader;

import net.natpad.sht.generated.CupParser;
import net.natpad.sht.generated.runtime.LrParserContext;
import net.natpad.sht.generated.runtime.LrSymbol;
import net.natpad.sht.model.Model;

public class ShtParser {

	
	
	public static void main(String[] args) {
		String txt = "test 3; \n" +
				"old_prop=fine for me\n" +
				"empty { }\n" +
				"valuelist { 'john', 'mark' }\n" +
				"map { test 'v'; bc=200; }";
		txt = "test 3;\nold_prop=fine for me\n empty { }\n valuelist { 'john', 'mark' }\n multilist { test 'help'; dom 'te gek'; }";
		StringReader stringReader = new StringReader(txt);
		try {
//			FileReader fileReader = new FileReader("./src/main/java/net/natpad/sht/config-example.sht");
			FileReader fileReader = new FileReader("/home/superman/cpp-workspace/natpad/natpad.cup/natpad-project.sht");
			
			
//			FileReader fileReader = new FileReader("/home/superman/.natpad/natpad-config.sht");
			LrParserContext parserContext = null;
			ShtScanner shtScanner = new ShtScanner(fileReader);
			
			CupParser cupParser = new CupParser();
			parserContext = new LrParserContext(shtScanner);
			LrSymbol rootToken = cupParser.parse(parserContext);
			
			Model model = (Model) rootToken.value;
			model.dump(System.out);
			System.out.println("model="+model.toString());
			
			System.out.println("done");
		} catch(Exception e) {
//			dumpParserContext(parserContext);
			e.printStackTrace();
		}
	}

	private static void dumpParserContext(LrParserContext parserContext) {
		System.out.println("dumping stack");
		LrSymbol symbol = parserContext.pop();
		while(symbol!=null) {
			System.out.println("symbol="+symbol);
			symbol = parserContext.pop();
		}
	}
	
}


/*



Target(name);


Target("compile") {
	JavaC {}
}






 */