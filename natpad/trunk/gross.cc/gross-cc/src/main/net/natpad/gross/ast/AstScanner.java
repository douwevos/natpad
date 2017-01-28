package net.natpad.gross.ast;

import java.io.IOException;
import java.io.Reader;

import net.natpad.gross.runtime.Location;
import net.natpad.gross.runtime.Scanner;
import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.SymbolProvider;
import net.natpad.gross.runtime.Terminal;
import net.natpad.gross.runtime.Token;
import net.natpad.gross.runtime.TokenFactory;

public class AstScanner implements Scanner {

	public final TokenFactory tokenFactory;
	private final Reader input;
	private final int lookahead[] = new int[5]; 
	private int row, leftColumn, rightColumn;
	private int markRow, markColumn;
	

	private Symbol connectedSymbols[] = new Symbol[26];

	private static final int SYM_END_OF_INPUT = 0;

	private static final int SYM_IDENTIFIER 	= 1;
	private static final int SYM_LSQBRACK   	= 2;
	private static final int SYM_RSQBRACK   	= 3;
	private static final int SYM_LCUBRACK   	= 4;
	private static final int SYM_RCUBRACK   	= 5;
	private static final int SYM_LPAREN     	= 6;
	private static final int SYM_RPAREN     	= 7;
	private static final int SYM_KW_TYPE		= 8;
	private static final int SYM_STRING     	= 9;
	private static final int SYM_KW_EXTENDS 	= 10;
	private static final int SYM_SEMI       	= 11;
	private static final int SYM_KW_FORWARD 	= 12;
	private static final int SYM_KW_AST			= 13;
	private static final int SYM_INDEX_MARK		= 14;
	private static final int SYM_KW_ARGUMENT	= 15;
	private static final int SYM_COMMA			= 16;
	private static final int SYM_KW_METHOD	    = 17;
	private static final int SYM_CODE_TEXT	    = 18;
	private static final int SYM_KW_RETURN	    = 19;
	private static final int SYM_KW_AS		    = 20;
	private static final int SYM_KW_IMPLEMENT   = 21;

	private static Terminal extractSymbol(SymbolProvider symbolProvider, String name) {
		Terminal terminal = symbolProvider.getTerminalByName(name);
		if (terminal==null) {
			throw new RuntimeException("Cannot connect symbol with name:"+name);
		}
		return terminal;
	}

	
	public AstScanner(TokenFactory tokenFactory, Reader input, SymbolProvider symbolProvider) throws IOException {
		this.tokenFactory = tokenFactory;
		this.input = input;
		for(int idx=0; idx<lookahead.length-1; idx++) {
			lookahead[idx+1] = input.read();
		}
		advance();
		connectedSymbols[SYM_END_OF_INPUT] = extractSymbol(symbolProvider, "⟧");
		connectedSymbols[SYM_IDENTIFIER] = extractSymbol(symbolProvider, "IDENTIFIER");
		connectedSymbols[SYM_LSQBRACK] = extractSymbol(symbolProvider, "LSQBRACK");
		connectedSymbols[SYM_RSQBRACK] = extractSymbol(symbolProvider, "RSQBRACK");
		connectedSymbols[SYM_LCUBRACK] = extractSymbol(symbolProvider, "LCUBRACK");
		connectedSymbols[SYM_RCUBRACK] = extractSymbol(symbolProvider, "RCUBRACK");
		connectedSymbols[SYM_LPAREN] = extractSymbol(symbolProvider, "LPAREN");
		connectedSymbols[SYM_RPAREN] = extractSymbol(symbolProvider, "RPAREN");
		connectedSymbols[SYM_KW_TYPE] = extractSymbol(symbolProvider, "TYPE");
		connectedSymbols[SYM_STRING] = extractSymbol(symbolProvider, "STRING");
		connectedSymbols[SYM_KW_EXTENDS] = extractSymbol(symbolProvider, "EXTENDS");
		connectedSymbols[SYM_SEMI] = extractSymbol(symbolProvider, "SEMI");
		connectedSymbols[SYM_KW_FORWARD] = extractSymbol(symbolProvider, "FORWARD");
		connectedSymbols[SYM_KW_AST] = extractSymbol(symbolProvider, "AST");
		connectedSymbols[SYM_INDEX_MARK] = extractSymbol(symbolProvider, "INDEX_MARK");
		connectedSymbols[SYM_KW_ARGUMENT] = extractSymbol(symbolProvider, "ARGUMENT");
		connectedSymbols[SYM_COMMA] = extractSymbol(symbolProvider, "COMMA");
		connectedSymbols[SYM_KW_METHOD] = extractSymbol(symbolProvider, "METHOD");
		connectedSymbols[SYM_CODE_TEXT] = extractSymbol(symbolProvider, "CODE_TEXT");
		connectedSymbols[SYM_KW_RETURN] = extractSymbol(symbolProvider, "RETURN");
		connectedSymbols[SYM_KW_AS] = extractSymbol(symbolProvider, "AS");
		connectedSymbols[SYM_KW_IMPLEMENT] = extractSymbol(symbolProvider, "IMPLEMENT");
	}

	int prevLookahead;
	
	private int nextLookahead() throws IOException {
		leftColumn = rightColumn;
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
			rightColumn = -1;
		} else if (result==0xd) {
			if (prevLookahead==0xa) {
				prevLookahead = 0;
			} else {
				prevLookahead = result;
				row++;
			}
			rightColumn = -1;
		} else if (result==-1) {
			return -1;
		} else {
			rightColumn++;
			prevLookahead = result;
		}
		return result;
	}

	
	public int advance() throws IOException {
		int nextChar = 0;
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

	
	
	public Token next() {
		Token result = null;
		try {
			while(result == null) {
				mark();
				int nextChar = lookahead[0];
				switch(nextChar) {
	
					case -1 :
						result = createBasicToken(SYM_END_OF_INPUT, 1);
						break;

					case '[' :
						result = createBasicToken(SYM_LSQBRACK, 1);
						break;

					case ']' :
						result = createBasicToken(SYM_RSQBRACK, 1);
						break;

					case '{' :
						result = createBasicToken(SYM_LCUBRACK, 1);
						break;

					case '}' :
						result = createBasicToken(SYM_RCUBRACK, 1);
						break;

					case '(' :
						result = createBasicToken(SYM_LPAREN, 1);
						break;

					case ')' :
						result = createBasicToken(SYM_RPAREN, 1);
						break;

					case ';' :
						result = createBasicToken(SYM_SEMI, 1);
						break;

					case ',' :
						result = createBasicToken(SYM_COMMA, 1);
						break;

					case '≣' :
						result = createBasicToken(SYM_INDEX_MARK, 1);
						break;

					case '"' :
						result = scanForString();
						break;
						
					case '｟' : {
						result = scanForCodeText();
					} break;


					default : {
						if (Character.isAlphabetic(nextChar)) {
							result = scanForId();
//						} else if (nextChar>='0' && nextChar<='9') {
//							result = scanForNumber();
						} else {
							advance();
						}
					}
				}
			}
		} catch(IOException e) {
			throw new RuntimeException(e);
		} catch(Throwable t) {
			throw new RuntimeException("uncaught exception at line:"+(markRow+1)+", column:"+(markColumn+1), t);
		}
		return result;
	}



	private void mark() {
		markColumn = leftColumn;
		markRow = row;
	}

	
	private Token createToken(int connSymId, int column, int row, Object value) {
		Symbol symbol = connectedSymbols[connSymId];
		Location tokenLoc = new Location(markColumn, markRow, column, row);
		return tokenFactory.createToken(symbol, value, tokenLoc, symbol.name);
	}

	private Token createBasicToken(int connSymId, int nrOfChars) throws IOException {
		StringBuilder buf = new StringBuilder();
		for(int idx=0; idx<nrOfChars-1; idx++) {
			buf.append((char) lookahead[0]);
			nextLookahead();
		}
		buf.append((char) lookahead[0]);
		Location tokenLoc = new Location(markColumn, markRow, rightColumn, row);
		nextLookahead();
		Symbol symbol = connectedSymbols[connSymId];
		return tokenFactory.createToken(symbol, buf.toString(), tokenLoc, symbol.name);
	}


	private Token scanForId() throws IOException {
		StringBuilder buf = new StringBuilder();
		buf.append((char) lookahead[0]);
		int nch = lookahead[1];
		while((Character.isAlphabetic(nch) || nch=='_' || Character.isDigit(nch))) {
			buf.append((char) nch);
			nextLookahead();
			nch = lookahead[1];
		}
		String idText = buf.toString();
		int connSymId = SYM_IDENTIFIER;
		if ("extends".equals(idText)) {
			connSymId = SYM_KW_EXTENDS;
		} else if ("type".equals(idText)) {
			connSymId = SYM_KW_TYPE;
		} else if ("forward".equals(idText)) {
			connSymId = SYM_KW_FORWARD;
		} else if ("ast".equals(idText)) {
			connSymId = SYM_KW_AST;
		} else if ("argument".equals(idText)) {
			connSymId = SYM_KW_ARGUMENT;
		} else if ("method".equals(idText)) {
			connSymId = SYM_KW_METHOD;
		} else if ("return".equals(idText)) {
			connSymId = SYM_KW_RETURN;
		} else if ("as".equals(idText)) {
			connSymId = SYM_KW_AS;
		} else if ("implement".equals(idText)) {
			connSymId = SYM_KW_IMPLEMENT;
		}
		Token result = createToken(connSymId, rightColumn, row, idText);
		advance();
		return result;
	}


	private Token scanForString() throws IOException {
		StringBuilder buf = new StringBuilder();
		char markChar = (char) lookahead[0];
		int nch = lookahead[1];
		boolean escaped = false;
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
		Token result = createToken(SYM_STRING, rightColumn, row, buf.toString());
		advance();
		return result;
	}

	private Token scanForCodeText() throws IOException {
		StringBuilder buf = new StringBuilder();
		int nch = lookahead[1];
		while(nch!=-1) {
			if (nch=='｠') {
				nextLookahead();
				break;
			}
			buf.append((char) nch);
			nextLookahead();
			nch = lookahead[1];
		}
		Token result = createToken(SYM_CODE_TEXT, rightColumn, row, buf.toString());
		advance();
		return result;
	}

	
}
