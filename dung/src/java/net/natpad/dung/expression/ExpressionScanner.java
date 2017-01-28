package net.natpad.dung.expression;

import java.io.IOException;
import java.io.Reader;

import net.natpad.dung.expression.generated.Symbol;
import net.natpad.dung.expression.generated.runtime.LrScanner;
import net.natpad.dung.expression.generated.runtime.LrSymbol;


public class ExpressionScanner implements LrScanner {


	private final Reader reader;
	private int lastChar;
	private int row, prow;
	private int column, pcolumn;
	private int markRow;
	private int markColumn;
	
	public ExpressionScanner(Reader reader) throws IOException {
		this.reader = reader;
		advance();
	}
	
	
	public void advance() throws IOException {
		prow = row;
		pcolumn = column;
		lastChar = reader.read();
		column++;
		if (lastChar==10 || lastChar==13 || lastChar==-1) {
			row++;
			column=0;
		}
	}
	
	@Override
	public LrSymbol next_token() throws Exception {
		LrSymbol symbol = next_token2();
//		System.out.println("symbol="+symbol+" : "+symbol.value);
		return symbol;
	}
	
	public LrSymbol next_token2() throws Exception {
		LrSymbol result = null;
		
		
		while(result==null) {
	
			markRow = row;
			markColumn = column;
			
			switch(lastChar) {
				case -1 : result = new LrSymbol(Symbol.EOF, markColumn, column, null); break;
				case '[' : result = new LrSymbol(Symbol.LBRACK, markColumn, column+1, "["); advance(); break;
				case ']' : result = new LrSymbol(Symbol.RBRACK, markColumn, column+1, "]"); advance(); break;
				case '.' : result = new LrSymbol(Symbol.DOT, markColumn, column+1, "("); advance(); break;
//				case ')' : result = new LrSymbol(Symbol.RPARREN, markColumn, column+1, ")"); advance(); break;
//				case ';' : result = new LrSymbol(Symbol.SEMICOLON, markColumn, column+1, ";"); advance(); break;
//				case ',' : result = new LrSymbol(Symbol.COMMA, markColumn, column+1, ","); advance(); break;
				case '"' :
				case '\'' : result = readString(); break;
			
				case '0' : case '1' : case '2' : case '3' : case '4' :
				case '5' : case '6' : case '7' : case '8' : case '9' : {
					return readNumber();
				}
				
				default : {
					if (Character.isLetter(lastChar)) {
						return readIdentifier();
					}
					if (Character.isWhitespace(lastChar)) {
						advance();
						break;
					}
					result = new LrSymbol(Symbol.error, markColumn, column, null);
					break;
				}
			}
		}
		
		
		return result;
	}

	private boolean readDigits(StringBuilder buf) throws IOException {
		boolean result = false;
		while(true) {
			if (Character.isDigit(lastChar)) {
				buf.append((char) lastChar);
				result = true;
			} else {
				break;
			}
			advance();
		}
		return result;
	}

	enum NumberScanState {
		DIGITS_DOT,
		DIGITS_EXP,
		DIGITS_DOT_OPTDIGITS_EXP,
	}
	
	private LrSymbol readNumber() throws IOException {
		StringBuilder buf = new StringBuilder();
		readDigits(buf);
		NumberScanState state;
		switch(lastChar) {
			case '.' : state = NumberScanState.DIGITS_DOT; break;
			case 'E' :
			case 'e' : state = NumberScanState.DIGITS_EXP; break;
			default : {
				return new LrSymbol(Symbol.NUMBER, markColumn, pcolumn+1, buf.toString());
			}
		}
		buf.append((char) lastChar);
		advance();
		
		if (state==NumberScanState.DIGITS_DOT) {
			readDigits(buf);
			if (lastChar=='e' || lastChar=='E') {
				buf.append((char) lastChar);
				state = NumberScanState.DIGITS_DOT_OPTDIGITS_EXP;
				advance();
			} else {
				return new LrSymbol(Symbol.NUMBER, markColumn, pcolumn+1, buf.toString());
			}
		}
		
		
		if (lastChar=='-') {
			buf.append((char) lastChar);
			advance();
		}
		readDigits(buf);
		return new LrSymbol(Symbol.NUMBER, markColumn, pcolumn+1, buf.toString());
	}

	

	private LrSymbol readString() throws IOException {
		char mark = (char) lastChar;
		boolean escaped = false;
		advance();
		LrSymbol result = null;
		StringBuilder buf = new StringBuilder();
		while(true) {
			if (lastChar==mark) {
				if (!escaped) {
					advance();
					result = new LrSymbol(Symbol.STRING, markColumn, pcolumn+1, buf.toString());
					break;
				}
			} else if (lastChar=='\\') {
				if (!escaped) {
					escaped = true;
					advance();
					continue;
				}
			} else if (lastChar==-1) {
				result = new LrSymbol(Symbol.STRING, markColumn, pcolumn+1, buf.toString());
				break;
			}
			if (escaped) {
				switch(lastChar) {
					case 'e' : { buf.append((char) 27); } break;
					case '\\' : { buf.append((char) '\\'); } break;
					case 'n' : { buf.append((char) '\n'); } break;
					case 'r' : { buf.append((char) '\r'); } break;
					case 'f' : { buf.append((char) '\f'); } break;
					case 't' : { buf.append((char) '\t'); } break;
					case 'v' : { buf.append((char) 11); } break;
					case 'a' : { buf.append((char) 7); } break;
					case 'b' : { buf.append((char) 8); } break;
					case '0' : { buf.append((char) 0); } break;
					default : {
						buf.append((char) '\\');
						buf.append((char) lastChar);
					}
				}
			} else {
				buf.append((char) lastChar);
			}
			advance();
			escaped = false;
		}
		return result;
	}


	private LrSymbol readIdentifier() throws IOException {
		StringBuilder buf = new StringBuilder();
		buf.append((char) lastChar);
		advance();
		while(true) {
			if (Character.isLetterOrDigit(lastChar) || lastChar=='_') {
				buf.append((char) lastChar);
			} else {
				break;
			}
			advance();
		}
		String idtext = buf.toString();
		if ("nil".equals(idtext)) {
			return new LrSymbol(Symbol.NIL, markColumn, pcolumn+1, buf.toString());
		}
		
		return new LrSymbol(Symbol.ID, markColumn, pcolumn+1, buf.toString());
	}


}
