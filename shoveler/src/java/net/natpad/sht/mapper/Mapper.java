package net.natpad.sht.mapper;

import java.io.FileReader;
import java.util.ArrayList;

import net.natpad.sht.ShtScanner;
import net.natpad.sht.generated.CupParser;
import net.natpad.sht.generated.runtime.LrParserContext;
import net.natpad.sht.generated.runtime.LrSymbol;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.ListValue;
import net.natpad.sht.model.Model;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class Mapper {

	Model model;
	
	public Mapper() {
		FileReader fileReader;
		try {
			fileReader = new FileReader("./src/java/net/natpad/sht/config-example.sht");
			LrParserContext parserContext = null;
			ShtScanner shtScanner = new ShtScanner(fileReader);
			
			CupParser cupParser = new CupParser();
			parserContext = new LrParserContext(shtScanner);
			LrSymbol rootToken = cupParser.parse(parserContext);
			
			model = (Model) rootToken.value;
			System.out.println("model="+model.toString());
			
			
			for(Reference reference : model) {
				String id = reference.getId();
				if ("tern".equals(id)) {
					mapTern(reference);
				}
			}
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	
	private String getStringFormBlock(BlockValue block, String id) {
		String result = null;
		Reference nameRef = block.getFirstReferenceById(id);
		if (nameRef!=null) {
			Value nameVal = nameRef.getValue();
			if (nameVal instanceof SimpleValue) {
				SimpleValue sval = (SimpleValue) nameVal;
				result = sval.getValue();
			}
		}
		return result;
	}
	
	
	private void mapTern(Reference reference) {
		System.out.println("mapping tern:"+reference);
		Value value = reference.getValue();
		System.out.println("value:"+value);
		if (value instanceof BlockValue) {
			BlockValue block = (BlockValue) value;
			block = model.resolve(block);
			for(Reference childRef : block) {
				String childId = childRef.getId();
				System.out.println("childId="+childId);
				DocumentClass documentClass = mapTernDocumentClass(childRef);
				System.out.println("document-class="+documentClass);
			}
		}
	}


	private DocumentClass mapTernDocumentClass(Reference reference) {
		DocumentClass result = null;
		Value value = reference.getValue();
		if (value instanceof BlockValue) {
			BlockValue block = (BlockValue) value;
			block = model.resolve(block);
			String name = getStringFormBlock(block, "name");
			String grammar = getStringFormBlock(block, "grammar");
			ArrayList<String> patterns = mapTernDocumentClassPatterns(block);
			result = new DocumentClass(name, grammar, patterns);
		}
		return result;
	}
	
	private ArrayList<String> mapTernDocumentClassPatterns(BlockValue block) {
		ArrayList<String> result = null;
		Reference patternsRef = block.getFirstReferenceById("filename-pattern");
		if (patternsRef!=null) {
			Value value = patternsRef.getValue();
			if (value instanceof ListValue) {
				ListValue listValue = (ListValue) value;
				for(Value sval : listValue) {
					if (sval instanceof SimpleValue) {
						SimpleValue tval = (SimpleValue) sval;
						if (result==null) {
							result = new ArrayList<String>();
						}
						result.add(tval.getValue());
					}
					
				}
			}
		}
		return result;
	}


	public static void main(String[] args) {
		new Mapper();
	}
	
}
