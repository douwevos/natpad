package net.natpad.sht;

import java.io.IOException;
import java.io.Reader;

import net.natpad.sht.generated.Symbol;
import net.natpad.sht.generated.runtime.LrScanner;
import net.natpad.sht.generated.runtime.LrSymbol;

public class ShtScanner implements LrScanner {

	private final Reader reader;
	private int lastChar;
	private int row, prow;
	private int column, pcolumn;
	private int markRow;
	private int markColumn;
	
	StringBuilder line = new StringBuilder();
	
	public ShtScanner(Reader reader) throws IOException {
		this.reader = reader;
		advance();
	}
	
	
	public void advance() throws IOException {
		int llc = lastChar;
		lastChar = reader.read();

		if ((lastChar==10 && llc==13) || (lastChar==13 && llc==10)) {
			lastChar = 0;
			advance();
			return;
		}
		prow = row;
		pcolumn = column;
		column++;
		
		if (lastChar==10 || lastChar==13 || lastChar==-1) {
			if (lastChar!=-1) {
				row++;
//				System.out.println(""+row+", line="+line.toString());
			}
			line.setLength(0);
			column=0;
		} else {
			line.append((char) lastChar);
		}
	}
	
	
	@Override
	public LrSymbol next_token() throws Exception {
		LrSymbol symbol = next_token2();
//		System.out.println("symbol="+symbol+" : "+symbol.value);
		return symbol;
	}
	
	public ShtSymbol next_token2() throws Exception {
		ShtSymbol result = null;
		
		
		while(result==null) {
	
			markRow = row;
			markColumn = column;
			
			switch(lastChar) {
				case -1 : result = new ShtSymbol(Symbol.EOF, markColumn, markRow, column, row, null); break;
				case '#' : skipLineComment();
					continue;
				case '{' : result = new ShtSymbol(Symbol.LBRACE, markColumn, markRow, column+1, row, "{"); advance(); break;
				case '}' : result = new ShtSymbol(Symbol.RBRACE, markColumn, markRow, column+1, row, "}"); advance(); break;
				case '(' : result = new ShtSymbol(Symbol.LPARREN, markColumn, markRow, column+1, row, "("); advance(); break;
				case ')' : result = new ShtSymbol(Symbol.RPARREN, markColumn, markRow, column+1, row, ")"); advance(); break;
				case ';' : result = new ShtSymbol(Symbol.SEMICOLON, markColumn, markRow, column+1, row, ";"); advance(); break;
				case ',' : result = new ShtSymbol(Symbol.COMMA, markColumn, markRow, column+1, row, ","); advance(); break;
				case '\'' : result = readString(); break;
				case '"' : result = readString(); break;
				case '=' : result = readAsigneValueNewLine(); break;
			
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
					result = new ShtSymbol(Symbol.error, markColumn, markRow, column, row, null);
					break;
				}
			}
		}
		
		
		return result;
	}

	private void skipLineComment() {
		int row2 = row;
		while(row==row2 && lastChar!=-1) {
			try {
				advance();
			} catch (IOException e) {
				e.printStackTrace();
				return;
			}
		}
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
	
	private ShtSymbol readNumber() throws IOException {
		StringBuilder buf = new StringBuilder();
		readDigits(buf);
		NumberScanState state;
		switch(lastChar) {
			case '.' : state = NumberScanState.DIGITS_DOT; break;
			case 'E' :
			case 'e' : state = NumberScanState.DIGITS_EXP; break;
			default : {
				return new ShtSymbol(Symbol.NUMBER, markColumn, markRow, pcolumn+1, prow, buf.toString());
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
				return new ShtSymbol(Symbol.NUMBER, markColumn, markRow, pcolumn+1, prow, buf.toString());
			}
		}
		
		
		if (lastChar=='-') {
			buf.append((char) lastChar);
			advance();
		}
		readDigits(buf);
		return new ShtSymbol(Symbol.NUMBER, markColumn, markRow, pcolumn+1, prow, buf.toString());
	}

	
	private ShtSymbol readAsigneValueNewLine() throws IOException {
		advance();
		StringBuilder buf = new StringBuilder();
		outer:
		while(true) {
			switch(lastChar) {
				case -1 :
				case 13 :
				case 10 :
					pcolumn = column;
					column = 0;
					prow = row;
					row++;
					break outer;
			}
			buf.append((char) lastChar);
			advance();
			
		}
		return new ShtSymbol(Symbol.ASSIGN_VALUE_EOL, markColumn, markRow, pcolumn+1, prow, buf.toString());
	}


	private ShtSymbol readString() throws IOException {
		char mark = (char) lastChar;
		boolean escaped = false;
		advance();
		ShtSymbol result = null;
		StringBuilder buf = new StringBuilder();
		while(true) {
			if (lastChar==-1) {
				result = new ShtSymbol(Symbol.STRING, markColumn, markRow, pcolumn+1, prow, buf.toString());
				break;
			}
			if (!escaped) {
				if (lastChar==mark) {
					advance();
					result = new ShtSymbol(Symbol.STRING, markColumn, markRow, pcolumn+1, prow, buf.toString());
					break;
				} else if (lastChar=='\\') {
					escaped = true;
					advance();
					continue;
				}
				buf.append((char) lastChar);
			} else {
				switch(lastChar) {
					case 'e' : { buf.append((char) 27); } break;
					case '\\' : { buf.append((char) '\\'); } break;
					case 'n' : { buf.append((char) '\n'); } break;
					case 'r' : { buf.append((char) '\r'); } break;
					case 'f' : { buf.append((char) '\f'); } break;
					case 't' : { buf.append((char) '\t'); } break;
					case '\'' : { buf.append((char) '\''); } break;
					case '\"' : { buf.append((char) '\"'); } break;
					case 'v' : { buf.append((char) 11); } break;
					case 'a' : { buf.append((char) 7); } break;
					case 'b' : { buf.append((char) 8); } break;
					case '0' : { buf.append((char) 0); } break;
					default : {
						buf.append((char) '\\');
						buf.append((char) lastChar);
					}
				}
			}
			advance();
			escaped = false;
		}
		return result;
	}


	private ShtSymbol readIdentifier() throws IOException {
		StringBuilder buf = new StringBuilder();
		buf.append((char) lastChar);
		advance();
		while(true) {
			if (Character.isLetterOrDigit(lastChar) || lastChar=='_' || lastChar=='.' || lastChar=='-') {
				buf.append((char) lastChar);
			} else {
				break;
			}
			advance();
		}
		String idtext = buf.toString();
		if ("nil".equals(idtext)) {
			return new ShtSymbol(Symbol.NIL, markColumn, markRow, pcolumn+1, prow, buf.toString());
		}
		
		return new ShtSymbol(Symbol.ID, markColumn, markRow, pcolumn+1, prow, buf.toString());
	}

	
}
