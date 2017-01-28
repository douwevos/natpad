package net.natpad.cup.export.caterpillar;

import java.io.File;

public class CaterpillarConfiguration {

	protected File destDir = new File("./src/generated/caterpillar");

	
	
	
	/**
	 * User option -- do we compact tables by making most common reduce the
	 * default action
	 */
//	public boolean opt_compact_red = false;
	public boolean compactReduceDefault = false;

	
	
	public String namespacePrefix = "NatpadCup";

	public CNameTupple parserName = null;
	
	public CNameTupple parserActionsName = null;

	public CNameTupple symbolName = null;

	
	public boolean outputNonTerminals = true;

	public boolean extraDebugInfo = false;

	public boolean symbolInfo = false;



	public String debugLevel = "WARN";
	
	
	
	
	public String getParserNameUpper() {
		if (parserName!=null && parserName.upperName!=null) {
			return parserName.upperName;
		}
		return "PARSER";
	}

	public String getParserNameLower() {
		if (parserName!=null && parserName.lowerName!=null) {
			return parserName.lowerName;
		}
		return "parser";
	}

	
	public String getParserNameCamel() {
		if (parserName!=null && parserName.camelName!=null) {
			return parserName.camelName;
		}
		return "Parser";
	}
	
	public String getParserNameStripper() {
		String res = getParserNameLower();
		res = res.replaceAll("_", "");
		return res;
	}

	
	
	public String getParserActionsNameUpper() {
		if (parserActionsName!=null && parserActionsName.upperName!=null) {
			return parserActionsName.upperName;
		}
		return "PARSER_ACTIONS";
	}

	public String getParserActionsNameLower() {
		if (parserActionsName!=null && parserActionsName.lowerName!=null) {
			return parserActionsName.lowerName;
		}
		return "parser_actions";
	}

	
	public String getParserActionsNameCamel() {
		if (parserActionsName!=null && parserActionsName.camelName!=null) {
			return parserActionsName.camelName;
		}
		return "ParserActions";
	}
	
	public String getParserActionsNameStripper() {
		String res = getParserActionsNameLower();
		res = res.replaceAll("_", "");
		return res;
	}

	
	
	
	

	public String getSymbolNameUpper() {
		if (symbolName!=null && symbolName.upperName!=null) {
			return symbolName.upperName;
		}
		return "SYMBOL";
	}

	public String getSymbolNameLower() {
		if (symbolName!=null && symbolName.lowerName!=null) {
			return symbolName.lowerName;
		}
		return "symbol";
	}

	
	public String getSymbolNameCamel() {
		if (symbolName!=null && symbolName.camelName!=null) {
			return symbolName.camelName;
		}
		return "Symbol";
	}
	
	public String getSymbolNameStripper() {
		String res = getSymbolNameLower();
		res = res.replaceAll("_", "");
		return res;
	}

}
