package net.natpad.cup.model.parser;

import java.io.IOException;
import java.io.Reader;

import net.natpad.cup.model.parser.generated.ParserSymbol;
import net.natpad.cup.model.parser.generated.runtime.LrScanner;
import net.natpad.cup.model.parser.generated.runtime.LrSymbol;

public class CupLexer implements LrScanner {

	private final Reader reader;
	private char lastChar, lookAheadChar;
	private boolean skipSpaces;
	private int row, column;
	
	public CupLexer(Reader reader) throws IOException {
		this.reader = reader;
		skipSpaces = true;
		advance();
		advance();
	}
	
	
	private void advance() throws IOException {
		while(true) {
			if (lastChar==10) {
				row++;
				column=0;
			} else {
				column++;
			}
			
			lastChar = lookAheadChar;
			int rch = reader.read();
			lookAheadChar = (char) rch;
			if (rch==-1) {
				return;
			}
			if (lastChar==13 && lookAheadChar==10) {
				column--;
				continue;
			}
			if (lastChar==13) {
				lastChar=10;
			}
			
			if (skipSpaces) {
				if (lastChar==' ' || lastChar=='\t' || lastChar==10) {
					continue;
				}
				break;
			} else {
				break;
			}
		}
	}
	
	
	
	@Override
	public LrSymbol next_token() throws Exception {
		LrSymbol result = null;
		switch(lastChar) {
			case '.' :  result = new LrSymbol(ParserSymbol.DOT, column, column+1, "."); advance(); return result;
			case ';' :  result = new LrSymbol(ParserSymbol.SEMI, column, column+1, ";"); advance(); return result;
			case ',' :  result = new LrSymbol(ParserSymbol.COMMA, column, column+1, ","); advance(); return result;
			case '*' :  result = new LrSymbol(ParserSymbol.STAR, column, column+1, "*"); advance(); return result;
			case '|' :  result = new LrSymbol(ParserSymbol.BAR, column, column+1, "|"); advance(); return result;
			case '[' :  result = new LrSymbol(ParserSymbol.LBRACK, column, column+1, "["); advance(); return result;
			case ']' :  result = new LrSymbol(ParserSymbol.RBRACK, column, column+1, "]"); advance(); return result;

		}
		return null;
	}
	
}
