package net.natpad.gross.definition;

import java.io.IOException;
import java.io.Reader;

import net.natpad.gross.runtime.Location;
import net.natpad.gross.runtime.Scanner;
import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.SymbolProvider;
import net.natpad.gross.runtime.Terminal;
import net.natpad.gross.runtime.Token;
import net.natpad.gross.runtime.TokenFactory;

public class GrossScanner implements Scanner {

	private final Reader input;
	private final TokenFactory tokenFactory;
	private final int lookahead[] = new int[5]; 
	private int nextChar;
	private int row, column;
	private Location location;
	
	
	private Symbol connectedSymbols[] = new Symbol[26];

	private static final int SYM_END_OF_INPUT = 0;
	private static final int SYM_NUMBER = 1;
	private static final int SYM_RULE_SEPERATOR = 2;
	private static final int SYM_INDEX_MARK = 5;
	private static final int SYM_COMMA = 6;
	private static final int SYM_COLON = 7;
	private static final int SYM_SEMI = 8;
	private static final int SYM_BAR = 9;
	private static final int SYM_EXCL = 10;
	private static final int SYM_STAR = 11;
	private static final int SYM_PLUS = 12;
	private static final int SYM_LPAREN = 13;
	private static final int SYM_RPAREN = 14;
	private static final int SYM_LSQBRACK = 15;
	private static final int SYM_RSQBRACK = 16;
	private static final int SYM_LCUBRACK = 17;
	private static final int SYM_RCUBRACK = 18;

	private static final int SYM_KW_TERMINAL = 19;
	private static final int SYM_KW_CODE = 20;
	private static final int SYM_KW_TYPE = 21;
	private static final int SYM_KW_IMPORTS = 22;
	private static final int SYM_IDENTIFIER = 23;
	private static final int SYM_MULTI_LINE_TEXT = 24;
	private static final int SYM_COLON_DOT = 25;
	
	
	private static Terminal extractSymbol(SymbolProvider symbolProvider, String name) {
		Terminal terminal = symbolProvider.getTerminalByName(name);
		if (terminal==null) {
			throw new RuntimeException("Cannot connect symbol with name:"+name);
		}
		return terminal;
	}
	
	public GrossScanner(TokenFactory tokenFactory, Reader input, SymbolProvider symbolProvider) throws IOException {
		this.tokenFactory = tokenFactory;
		this.input = input;
		for(int idx=0; idx<lookahead.length-1; idx++) {
			lookahead[idx+1] = input.read();
		}
		advance();
		connectedSymbols[SYM_END_OF_INPUT] = extractSymbol(symbolProvider, "⟧");
		connectedSymbols[SYM_NUMBER] = extractSymbol(symbolProvider, "NUMBER");
		connectedSymbols[SYM_RULE_SEPERATOR] = extractSymbol(symbolProvider, "RULE_SEPERATOR");
		connectedSymbols[SYM_INDEX_MARK] = extractSymbol(symbolProvider, "INDEX_MARK");
		connectedSymbols[SYM_COMMA] = extractSymbol(symbolProvider, "COMMA");
		connectedSymbols[SYM_COLON] = extractSymbol(symbolProvider, "COLON");
		connectedSymbols[SYM_SEMI] = extractSymbol(symbolProvider, "SEMI");
		connectedSymbols[SYM_BAR] = extractSymbol(symbolProvider, "BAR");
		connectedSymbols[SYM_EXCL] = extractSymbol(symbolProvider, "EXCL");
		connectedSymbols[SYM_STAR] = extractSymbol(symbolProvider, "STAR");
		connectedSymbols[SYM_PLUS] = extractSymbol(symbolProvider, "PLUS");
		connectedSymbols[SYM_LPAREN] = extractSymbol(symbolProvider, "LPAREN");
		connectedSymbols[SYM_RPAREN] = extractSymbol(symbolProvider, "RPAREN");
		connectedSymbols[SYM_LSQBRACK] = extractSymbol(symbolProvider, "LSQBRACK");
		connectedSymbols[SYM_RSQBRACK] = extractSymbol(symbolProvider, "RSQBRACK");
		connectedSymbols[SYM_LCUBRACK] = extractSymbol(symbolProvider, "LCUBRACK");
		connectedSymbols[SYM_RCUBRACK] = extractSymbol(symbolProvider, "RCUBRACK");
		connectedSymbols[SYM_KW_TERMINAL] = extractSymbol(symbolProvider, "TERMINAL");
		connectedSymbols[SYM_KW_CODE] = extractSymbol(symbolProvider, "CODE");
		connectedSymbols[SYM_KW_TYPE] = extractSymbol(symbolProvider, "TYPE");
		connectedSymbols[SYM_KW_IMPORTS] = extractSymbol(symbolProvider, "IMPORTS");
		connectedSymbols[SYM_IDENTIFIER] = extractSymbol(symbolProvider, "IDENTIFIER");
		connectedSymbols[SYM_MULTI_LINE_TEXT] = extractSymbol(symbolProvider, "CODE_TEXT");
//		connectedSymbols[SYM_COLON_DOT] = extractSymbol(symbolProvider, "COLON_DOT");
	}
	

	int prevLookahead;
	
	private int nextLookahead() throws IOException {
		System.arraycopy(lookahead, 1, lookahead, 0, lookahead.length-1);
		lookahead[lookahead.length-1] = input.read();
		int result = lookahead[0];
		if (result==0xa) {
			if (prevLookahead==0xd) {
				prevLookahead = 0;
			} else {
				prevLookahead = result;
				row++;
			}
			column = -1;
		} else if (result==0xd) {
			if (prevLookahead==0xa) {
				prevLookahead = 0;
			} else {
				prevLookahead = result;
				row++;
			}
			column = -1;
		} else if (result==-1) {
			return -1;
		} else {
			column++;
			prevLookahead = result;
		}
		return result;
	}
	
	public int advance() throws IOException {
		while(true) {
			nextChar = nextLookahead();
			if (nextChar==0xa || nextChar==0xd) {
				continue;
			} else if (nextChar==-1) {
				return -1;
			}
			break;
		}
		return nextChar;
	}
	
	
	@Override
	public Token next() {
		Token result = null;
		try {
			while(result == null) {
				location = new Location(column, row, column+1, row);
				switch(nextChar) {

					case -1 :
						result = createToken(SYM_END_OF_INPUT, location, "⟧");
						break;
					case '⟶' :
					case '⇒' : {
						result = createToken(SYM_RULE_SEPERATOR, location, "⟶");
						advance();
					} break;

					case '[' : {
						result = createToken(SYM_LSQBRACK, location, "[");
						advance();
					} break;
					case ']' : {
						result = createToken(SYM_RSQBRACK, location, "]");
						advance();
					} break;

					case '｟' : {
						result = scanForCodeText();
					} break;


					case '#' : { 
						result = scanForComment();
					} break;

					
					case '≣' : { 
						result = createToken(SYM_INDEX_MARK, location, ""+nextChar);
						advance();
					} break;
					case ',' : {
						result = createToken(SYM_COMMA, location, ",");
						advance();
					} break;
					case ':' : {
						advance();
//						if (lookahead[0]==':' && lookahead[1]=='=') {
//							advance();
//							advance();
//							result = tokenFactory.createToken(symbolProvider.getTerminalByName("COLON_COLON_EQUALS"), null);
//						} else {
						result = createToken(SYM_COLON, location, ":");
//						}
					} break;
					case ';' : {
						result = createToken(SYM_SEMI, location, ";");
						advance();
					} break;
					case '|' : {
						result = createToken(SYM_BAR, location, "|");
						advance();
					} break;
					case '⊗' : {
						result = createToken(SYM_IDENTIFIER, location, "⊗");
						advance();
					} break;
					case '*' : {
						result = createToken(SYM_STAR, location, "*");
						advance();
					} break;
					case '+' : {
						result = createToken(SYM_PLUS, location, "+");
						advance();
					} break;
					case '?' : {
						result = createToken(SYM_EXCL, location, "?");
						advance();
					} break;
					case '(' : {
						result = createToken(SYM_LPAREN, location, "(");
						advance();
					} break;
					case ')' : {
						result = createToken(SYM_RPAREN, location, ")");
						advance();
					} break;

					case '"' : {
						result = scanForString();
					} break;
					
					default : {
						if (Character.isAlphabetic(nextChar)) {
							result = scanForId();
						} else if (nextChar>='0' && nextChar<='9') {
							result = scanForNumber();
						} else {
							advance();
						}
					}
				}
			}
		} catch(IOException e) {
			throw new RuntimeException(e);
		} catch(Throwable t) {
			throw new RuntimeException("uncaught exception at line:"+(location.beginRow+1)+", column:"+(location.beginColumn+1), t);
		}
		return result;
	}

	private Token createToken(int connSymId, Location tokenLoc, Object value) {
		Symbol symbol = connectedSymbols[connSymId];
		return tokenFactory.createToken(symbol, value, tokenLoc, symbol.name);
	}

	private Token scanForNumber() throws IOException {
		
		int lastCol = column+1;
		long lastRow = row;
		long result = 0;
		while(nextChar>='0' && nextChar<='9' && row==lastRow) {
			result = result*10 + (nextChar-'0');
			lastCol = column+1;
			lastRow = row;
			advance();
		}
		return createToken(SYM_NUMBER, location.setEnd(lastCol, lastRow), Long.valueOf(result));
	}

	private Token scanForCodeText() throws IOException {
		int lastCol = column;
		long lastRow = row;
		int nextLookahead = nextLookahead();
		StringBuilder buf = new StringBuilder();
		while(nextLookahead!=-1 && nextLookahead!='｠') {
			buf.append((char) nextLookahead);
			lastCol = column+1;
			lastRow = row;
			nextLookahead = nextLookahead();
		}
		if (nextLookahead=='｠') {
			lastCol++;
		}
		advance();
		return createToken(SYM_MULTI_LINE_TEXT, location.setEnd(lastCol, lastRow), buf.toString());
	}

	private Token scanForComment() throws IOException {
		int lastCol = column;
		long lastRow = row;
		int nextLookahead = nextLookahead();
		StringBuilder buf = new StringBuilder();
		while(nextLookahead!=-1 && row==lastRow) {
			buf.append((char) nextLookahead);
			lastCol = column+1;
			lastRow = row;
			nextLookahead = nextLookahead();
		}
//		if (nextLookahead=='｠') {
//			lastCol++;
//		}
		advance();
		return null;
//		return createToken(SYM_MULTI_LINE_TEXT, location.setEnd(lastCol, lastRow), buf.toString());
	}

	
	private Token scanForId() throws IOException {
		int lastCol = column+1;
		long lastRow = row;
		StringBuilder buf = new StringBuilder();
		while((Character.isAlphabetic(nextChar) || nextChar=='_' || Character.isDigit(nextChar)) &&  row==lastRow) {
			buf.append((char) nextChar);
			lastCol = column+1;
			lastRow = row;
			advance();
		}
		String idText = buf.toString();
		int connSymId = SYM_IDENTIFIER;
		if ("terminal".equals(idText)) {
			connSymId = SYM_KW_TERMINAL;
		} else if ("code".equals(idText)) {
			connSymId = SYM_KW_CODE;
		} else if ("type".equals(idText)) {
			connSymId = SYM_KW_TYPE;
		} else if ("imports".equals(idText)) {
			connSymId = SYM_KW_IMPORTS;
		}
		return createToken(connSymId, location.setEnd(lastCol, lastRow), idText);
	}

	private Token scanForString() throws IOException {
		StringBuilder buf = new StringBuilder();
		char markChar = (char) lookahead[0];
		int nch = lookahead[1];
		boolean escaped = false;
		int endRow = row;
		while(nch!=-1) {
			if (nch==markChar && !escaped) {
				nextLookahead();
				break;
			} else if (nch=='\\') {
				escaped = !escaped;
			}
			buf.append((char) nch);
			nextLookahead();
			nch = lookahead[1];
		}
		
		Token result = createToken(SYM_IDENTIFIER, location.setEnd(column, endRow), buf.toString());
		advance();
		return result;
	}

	
}
