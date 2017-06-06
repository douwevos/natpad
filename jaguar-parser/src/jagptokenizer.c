///*
//   File:    jagptokenizer.c
//   Project: jaguar-parser
//   Author:  Douwe Vos
//   Date:    May 29, 2017
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2017 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "jagptokenizer.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_ALL
//#define CAT_LOG_CLAZZ "JagPTokenizer"
//#include <logging/catlog.h>
//
//struct _JagPTokenizerPrivate {
//
//	/** Allow binary literals.
//	 */
//	gboolean allowBinaryLiterals;
//
//	/** Allow underscores in literals.
//	 */
//	gboolean allowUnderscoresInLiterals;
//
//	/** The source language setting.
//	 */
//	Source source;
//
//	/** The log to be used for error reporting.
//	 */
//	Log log;
//
//	/** The token factory. */
//	Tokens tokens;
//
//	/** The token kind, set by nextToken().
//	 */
//	JagPTokenKind tk;
//
//	/** The token's radix, set by nextToken().
//	 */
//	int radix;
//
//	/** The token's name, set by nextToken().
//	 */
//	JagPName *name;
//
//	/** The position where a lexical error occurred;
//	 */
//	int errPos = -1;
//
//	/** The Unicode reader (low-level stream reader).
//	 */
//	UnicodeReader reader;
//
//	ScannerFactory fac;
//};
//
//static void l_stringable_iface_init(CatIStringableInterface *iface);
//
//G_DEFINE_TYPE_WITH_CODE(JagPTokenizer, jagp_tokenizer, G_TYPE_OBJECT,
//		G_ADD_PRIVATE(JagPTokenizer)
//		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void jagp_tokenizer_class_init(JagPTokenizerClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void jagp_tokenizer_init(JagPTokenizer *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	JagPTokenizer *instance = JAGP_TOKENIZER(object);
////	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(instance);
//	G_OBJECT_CLASS(jagp_tokenizer_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(jagp_tokenizer_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//JagPTokenizer *jagp_tokenizer_new() {
//	JagPTokenizer *result = g_object_new(JAGP_TYPE_TOKENIZER, NULL);
//	cat_ref_anounce(result);
//	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(result);
////	G_OBJECT_construct((GObject *) result);
//	return result;
//}
//
//
//
//#define SCANNER_DEBUG  FALSE
//
//
//
//static boolean hexFloatsWork = hexFloatsWork();
//
//static boolean hexFloatsWork() {
//	try {
//		Float.valueOf("0x1.0p1");
//		return true;
//	} catch (NumberFormatException ex) {
//		return false;
//	}
//}
//
//
// JavaTokenizer(ScannerFactory fac, UnicodeReader reader) {
//	this.fac = fac;
//	this.log = fac.log;
//	this.tokens = fac.tokens;
//	this.source = fac.source;
//	this.reader = reader;
//	this.allowBinaryLiterals = source.allowBinaryLiterals();
//	this.allowUnderscoresInLiterals = source.allowUnderscoresInLiterals();
//}
//
///** Report an error at the given position using the provided arguments.
// */
//void lexError(int pos, String key, Object... args) {
//	log.error(pos, key, args);
//	tk = TokenKind.ERROR;
//	errPos = pos;
//}
//
///** Read next character in character or string literal and copy into sbuf.
// */
//void scanLitChar(JagPTokenizer *tokenizer, int pos) {
//	if (reader.ch == '\\') {
//		if (reader.peekChar() == '\\' && !reader.isUnicode()) {
//			reader.skipChar();
//			reader.putChar('\\', true);
//		} else {
//			reader.scanChar();
//			switch (reader.ch) {
//			case '0': case '1': case '2': case '3':
//			case '4': case '5': case '6': case '7':
//				char leadch = reader.ch;
//				int oct = reader.digit(pos, 8);
//				reader.scanChar();
//				if ('0' <= reader.ch && reader.ch <= '7') {
//					oct = oct * 8 + reader.digit(pos, 8);
//					reader.scanChar();
//					if (leadch <= '3' && '0' <= reader.ch && reader.ch <= '7') {
//						oct = oct * 8 + reader.digit(pos, 8);
//						reader.scanChar();
//					}
//				}
//				reader.putChar((char)oct);
//				break;
//			case 'b':
//				reader.putChar('\b', true); break;
//			case 't':
//				reader.putChar('\t', true); break;
//			case 'n':
//				reader.putChar('\n', true); break;
//			case 'f':
//				reader.putChar('\f', true); break;
//			case 'r':
//				reader.putChar('\r', true); break;
//			case '\'':
//				reader.putChar('\'', true); break;
//			case '\"':
//				reader.putChar('\"', true); break;
//			case '\\':
//				reader.putChar('\\', true); break;
//			default:
//				lexError(reader.bp, "illegal.esc.char");
//			}
//		}
//	} else if (reader.bp != reader.buflen) {
//		reader.putChar(true);
//	}
//}
//
//void scanDigits(JagPTokenizer *tokenizer, int pos, int digitRadix) {
//	char saveCh;
//	int savePos;
//	do {
//		if (reader.ch != '_') {
//			reader.putChar(false);
//		} else {
//			if (!allowUnderscoresInLiterals) {
//				lexError(pos, "unsupported.underscore.lit", source.name);
//				allowUnderscoresInLiterals = true;
//			}
//		}
//		saveCh = reader.ch;
//		savePos = reader.bp;
//		reader.scanChar();
//	} while (reader.digit(pos, digitRadix) >= 0 || reader.ch == '_');
//	if (saveCh == '_')
//		lexError(savePos, "illegal.underscore");
//}
//
///** Read fractional part of hexadecimal floating point number.
// */
//void scanHexExponentAndSuffix(JagPTokenizer *tokenizer, int pos) {
//	if (reader.ch == 'p' || reader.ch == 'P') {
//		reader.putChar(true);
//		skipIllegalUnderscores();
//		if (reader.ch == '+' || reader.ch == '-') {
//			reader.putChar(true);
//		}
//		skipIllegalUnderscores();
//		if (reader.digit(pos, 10) >= 0) {
//			scanDigits(pos, 10);
//			if (!hexFloatsWork)
//				lexError(pos, "unsupported.cross.fp.lit");
//		} else
//			lexError(pos, "malformed.fp.lit");
//	} else {
//		lexError(pos, "malformed.fp.lit");
//	}
//	if (reader.ch == 'f' || reader.ch == 'F') {
//		reader.putChar(true);
//		tk = TokenKind.FLOATLITERAL;
//		radix = 16;
//	} else {
//		if (reader.ch == 'd' || reader.ch == 'D') {
//			reader.putChar(true);
//		}
//		tk = TokenKind.DOUBLELITERAL;
//		radix = 16;
//	}
//}
//
///** Read fractional part of floating point number.
// */
//void scanFraction(JagPTokenizer *tokenizer, int pos) {
//	skipIllegalUnderscores();
//	if (reader.digit(pos, 10) >= 0) {
//		scanDigits(pos, 10);
//	}
//	int sp1 = reader.sp;
//	if (reader.ch == 'e' || reader.ch == 'E') {
//		reader.putChar(true);
//		skipIllegalUnderscores();
//		if (reader.ch == '+' || reader.ch == '-') {
//			reader.putChar(true);
//		}
//		skipIllegalUnderscores();
//		if (reader.digit(pos, 10) >= 0) {
//			scanDigits(pos, 10);
//			return;
//		}
//		lexError(pos, "malformed.fp.lit");
//		reader.sp = sp1;
//	}
//}
//
///** Read fractional part and 'd' or 'f' suffix of floating point number.
// */
//void scanFractionAndSuffix(JagPTokenizer *tokenizer, int pos) {
//	radix = 10;
//	scanFraction(pos);
//	if (reader.ch == 'f' || reader.ch == 'F') {
//		reader.putChar(true);
//		tk = TokenKind.FLOATLITERAL;
//	} else {
//		if (reader.ch == 'd' || reader.ch == 'D') {
//			reader.putChar(true);
//		}
//		tk = TokenKind.DOUBLELITERAL;
//	}
//}
//
///** Read fractional part and 'd' or 'f' suffix of floating point number.
// */
//void scanHexFractionAndSuffix(JagPTokenizer *tokenizer, int pos, boolean seendigit) {
//	radix = 16;
//	Assert.check(reader.ch == '.');
//	reader.putChar(true);
//	skipIllegalUnderscores();
//	if (reader.digit(pos, 16) >= 0) {
//		seendigit = true;
//		scanDigits(pos, 16);
//	}
//	if (!seendigit)
//		lexError(pos, "invalid.hex.number");
//	else
//		scanHexExponentAndSuffix(pos);
//}
//
//void skipIllegalUnderscores(JagPTokenizer *tokenizer) {
//	if (reader.ch == '_') {
//		lexError(reader.bp, "illegal.underscore");
//		while (reader.ch == '_')
//			reader.scanChar();
//	}
//}
//
///** Read a number.
// *  @param radix  The radix of the number; one of 2, 8, 10, 16.
// */
//void scanNumber(JagPTokenizer *tokenizer, int pos, int radix) {
//	// for octal, allow base-10 digit in case it's a float literal
//	this.radix = radix;
//	int digitRadix = (radix == 8 ? 10 : radix);
//	int firstDigit = reader.digit(pos, Math.max(10, digitRadix));
//	boolean seendigit = firstDigit >= 0;
//	boolean seenValidDigit = firstDigit >= 0 && firstDigit < digitRadix;
//	if (seendigit) {
//		scanDigits(pos, digitRadix);
//	}
//	if (radix == 16 && reader.ch == '.') {
//		scanHexFractionAndSuffix(pos, seendigit);
//	} else if (seendigit && radix == 16 && (reader.ch == 'p' || reader.ch == 'P')) {
//		scanHexExponentAndSuffix(pos);
//	} else if (digitRadix == 10 && reader.ch == '.') {
//		reader.putChar(true);
//		scanFractionAndSuffix(pos);
//	} else if (digitRadix == 10 &&
//			   (reader.ch == 'e' || reader.ch == 'E' ||
//				reader.ch == 'f' || reader.ch == 'F' ||
//				reader.ch == 'd' || reader.ch == 'D')) {
//		scanFractionAndSuffix(pos);
//	} else {
//		if (!seenValidDigit) {
//			switch (radix) {
//			case 2:
//				lexError(pos, "invalid.binary.number");
//				break;
//			case 16:
//				lexError(pos, "invalid.hex.number");
//				break;
//			}
//		}
//		if (reader.ch == 'l' || reader.ch == 'L') {
//			reader.scanChar();
//			tk = TokenKind.LONGLITERAL;
//		} else {
//			tk = TokenKind.INTLITERAL;
//		}
//	}
//}
//
///** Read an identifier.
// */
//void scanIdent(JagPTokenizer *tokenizer) {
//	boolean isJavaIdentifierPart;
//	char high;
//	reader.putChar(true);
//	do {
//		switch (reader.ch) {
//		case 'A': case 'B': case 'C': case 'D': case 'E':
//		case 'F': case 'G': case 'H': case 'I': case 'J':
//		case 'K': case 'L': case 'M': case 'N': case 'O':
//		case 'P': case 'Q': case 'R': case 'S': case 'T':
//		case 'U': case 'V': case 'W': case 'X': case 'Y':
//		case 'Z':
//		case 'a': case 'b': case 'c': case 'd': case 'e':
//		case 'f': case 'g': case 'h': case 'i': case 'j':
//		case 'k': case 'l': case 'm': case 'n': case 'o':
//		case 'p': case 'q': case 'r': case 's': case 't':
//		case 'u': case 'v': case 'w': case 'x': case 'y':
//		case 'z':
//		case '$': case '_':
//		case '0': case '1': case '2': case '3': case '4':
//		case '5': case '6': case '7': case '8': case '9':
//			break;
//		case '\u0000': case '\u0001': case '\u0002': case '\u0003':
//		case '\u0004': case '\u0005': case '\u0006': case '\u0007':
//		case '\u0008': case '\u000E': case '\u000F': case '\u0010':
//		case '\u0011': case '\u0012': case '\u0013': case '\u0014':
//		case '\u0015': case '\u0016': case '\u0017':
//		case '\u0018': case '\u0019': case '\u001B':
//		case '\u007F':
//			reader.scanChar();
//			continue;
//		case '\u001A': // EOI is also a legal identifier part
//			if (reader.bp >= reader.buflen) {
//				name = reader.name();
//				tk = tokens.lookupKind(name);
//				return;
//			}
//			reader.scanChar();
//			continue;
//		default:
//			if (reader.ch < '\u0080') {
//				// all ASCII range chars already handled, above
//				isJavaIdentifierPart = false;
//			} else {
//				if (Character.isIdentifierIgnorable(reader.ch)) {
//					reader.scanChar();
//					continue;
//				} else {
//					int codePoint = reader.peekSurrogates();
//					if (codePoint >= 0) {
//						if (isJavaIdentifierPart = Character.isJavaIdentifierPart(codePoint)) {
//							reader.putChar(true);
//						}
//					} else {
//						isJavaIdentifierPart = Character.isJavaIdentifierPart(reader.ch);
//					}
//				}
//			}
//			if (!isJavaIdentifierPart) {
//				name = reader.name();
//				tk = tokens.lookupKind(name);
//				return;
//			}
//		}
//		reader.putChar(true);
//	} while (true);
//}
//
///** Return true if reader.ch can be part of an operator.
// */
//gboolean isSpecial(JagPTokenizer *tokenizer, char ch) {
//	switch (ch) {
//	case '!': case '%': case '&': case '*': case '?':
//	case '+': case '-': case ':': case '<': case '=':
//	case '>': case '^': case '|': case '~':
//	case '@':
//		return true;
//	default:
//		return false;
//	}
//}
//
///** Read longest possible sequence of special characters and convert
// *  to token.
// */
//void scanOperator(JagPTokenizer *tokenizer) {
//	while (true) {
//		reader.putChar(false);
//		Name newname = reader.name();
//		TokenKind tk1 = tokens.lookupKind(newname);
//		if (tk1 == TokenKind.IDENTIFIER) {
//			reader.sp--;
//			break;
//		}
//		tk = tk1;
//		reader.scanChar();
//		if (!isSpecial(reader.ch)) break;
//	}
//}
//
///** Read token.
// */
//Token readToken(JagPTokenizer *tokenizer) {
//
//	reader.sp = 0;
//	name = null;
//	radix = 0;
//
//	int pos = 0;
//	int endPos = 0;
//	List<Comment> comments = null;
//
//	try {
//		loop: while (true) {
//			pos = reader.bp;
//			switch (reader.ch) {
//			case ' ': // (Spec 3.6)
//			case '\t': // (Spec 3.6)
//			case FF: // (Spec 3.6)
//				do {
//					reader.scanChar();
//				} while (reader.ch == ' ' || reader.ch == '\t' || reader.ch == FF);
//				processWhiteSpace(pos, reader.bp);
//				break;
//			case LF: // (Spec 3.4)
//				reader.scanChar();
//				processLineTerminator(pos, reader.bp);
//				break;
//			case CR: // (Spec 3.4)
//				reader.scanChar();
//				if (reader.ch == LF) {
//					reader.scanChar();
//				}
//				processLineTerminator(pos, reader.bp);
//				break;
//			case 'A': case 'B': case 'C': case 'D': case 'E':
//			case 'F': case 'G': case 'H': case 'I': case 'J':
//			case 'K': case 'L': case 'M': case 'N': case 'O':
//			case 'P': case 'Q': case 'R': case 'S': case 'T':
//			case 'U': case 'V': case 'W': case 'X': case 'Y':
//			case 'Z':
//			case 'a': case 'b': case 'c': case 'd': case 'e':
//			case 'f': case 'g': case 'h': case 'i': case 'j':
//			case 'k': case 'l': case 'm': case 'n': case 'o':
//			case 'p': case 'q': case 'r': case 's': case 't':
//			case 'u': case 'v': case 'w': case 'x': case 'y':
//			case 'z':
//			case '$': case '_':
//				scanIdent();
//				break loop;
//			case '0':
//				reader.scanChar();
//				if (reader.ch == 'x' || reader.ch == 'X') {
//					reader.scanChar();
//					skipIllegalUnderscores();
//					scanNumber(pos, 16);
//				} else if (reader.ch == 'b' || reader.ch == 'B') {
//					if (!allowBinaryLiterals) {
//						lexError(pos, "unsupported.binary.lit", source.name);
//						allowBinaryLiterals = true;
//					}
//					reader.scanChar();
//					skipIllegalUnderscores();
//					scanNumber(pos, 2);
//				} else {
//					reader.putChar('0');
//					if (reader.ch == '_') {
//						int savePos = reader.bp;
//						do {
//							reader.scanChar();
//						} while (reader.ch == '_');
//						if (reader.digit(pos, 10) < 0) {
//							lexError(savePos, "illegal.underscore");
//						}
//					}
//					scanNumber(pos, 8);
//				}
//				break loop;
//			case '1': case '2': case '3': case '4':
//			case '5': case '6': case '7': case '8': case '9':
//				scanNumber(pos, 10);
//				break loop;
//			case '.':
//				reader.scanChar();
//				if (reader.digit(pos, 10) >= 0) {
//					reader.putChar('.');
//					scanFractionAndSuffix(pos);
//				} else if (reader.ch == '.') {
//					int savePos = reader.bp;
//					reader.putChar('.'); reader.putChar('.', true);
//					if (reader.ch == '.') {
//						reader.scanChar();
//						reader.putChar('.');
//						tk = TokenKind.ELLIPSIS;
//					} else {
//						lexError(savePos, "illegal.dot");
//					}
//				} else {
//					tk = TokenKind.DOT;
//				}
//				break loop;
//			case ',':
//				reader.scanChar(); tk = TokenKind.COMMA; break loop;
//			case ';':
//				reader.scanChar(); tk = TokenKind.SEMI; break loop;
//			case '(':
//				reader.scanChar(); tk = TokenKind.LPAREN; break loop;
//			case ')':
//				reader.scanChar(); tk = TokenKind.RPAREN; break loop;
//			case '[':
//				reader.scanChar(); tk = TokenKind.LBRACKET; break loop;
//			case ']':
//				reader.scanChar(); tk = TokenKind.RBRACKET; break loop;
//			case '{':
//				reader.scanChar(); tk = TokenKind.LBRACE; break loop;
//			case '}':
//				reader.scanChar(); tk = TokenKind.RBRACE; break loop;
//			case '/':
//				reader.scanChar();
//				if (reader.ch == '/') {
//					do {
//						reader.scanCommentChar();
//					} while (reader.ch != CR && reader.ch != LF && reader.bp < reader.buflen);
//					if (reader.bp < reader.buflen) {
//						comments = addComment(comments, processComment(pos, reader.bp, CommentStyle.LINE));
//					}
//					break;
//				} else if (reader.ch == '*') {
//					boolean isEmpty = false;
//					reader.scanChar();
//					CommentStyle style;
//					if (reader.ch == '*') {
//						style = CommentStyle.JAVADOC;
//						reader.scanCommentChar();
//						if (reader.ch == '/') {
//							isEmpty = true;
//						}
//					} else {
//						style = CommentStyle.BLOCK;
//					}
//					while (!isEmpty && reader.bp < reader.buflen) {
//						if (reader.ch == '*') {
//							reader.scanChar();
//							if (reader.ch == '/') break;
//						} else {
//							reader.scanCommentChar();
//						}
//					}
//					if (reader.ch == '/') {
//						reader.scanChar();
//						comments = addComment(comments, processComment(pos, reader.bp, style));
//						break;
//					} else {
//						lexError(pos, "unclosed.comment");
//						break loop;
//					}
//				} else if (reader.ch == '=') {
//					tk = TokenKind.SLASHEQ;
//					reader.scanChar();
//				} else {
//					tk = TokenKind.SLASH;
//				}
//				break loop;
//			case '\'':
//				reader.scanChar();
//				if (reader.ch == '\'') {
//					lexError(pos, "empty.char.lit");
//					reader.scanChar();
//				} else {
//					if (reader.ch == CR || reader.ch == LF)
//						lexError(pos, "illegal.line.end.in.char.lit");
//					scanLitChar(pos);
//					if (reader.ch == '\'') {
//						reader.scanChar();
//						tk = TokenKind.CHARLITERAL;
//					} else {
//						lexError(pos, "unclosed.char.lit");
//					}
//				}
//				break loop;
//			case '\"':
//				reader.scanChar();
//				while (reader.ch != '\"' && reader.ch != CR && reader.ch != LF && reader.bp < reader.buflen)
//					scanLitChar(pos);
//				if (reader.ch == '\"') {
//					tk = TokenKind.STRINGLITERAL;
//					reader.scanChar();
//				} else {
//					lexError(pos, "unclosed.str.lit");
//				}
//				break loop;
//			default:
//				if (isSpecial(reader.ch)) {
//					scanOperator();
//				} else {
//					boolean isJavaIdentifierStart;
//					int codePoint = -1;
//					if (reader.ch < '\u0080') {
//						// all ASCII range chars already handled, above
//						isJavaIdentifierStart = false;
//					} else {
//						codePoint = reader.peekSurrogates();
//						if (codePoint >= 0) {
//							if (isJavaIdentifierStart = Character.isJavaIdentifierStart(codePoint)) {
//								reader.putChar(true);
//							}
//						} else {
//							isJavaIdentifierStart = Character.isJavaIdentifierStart(reader.ch);
//						}
//					}
//					if (isJavaIdentifierStart) {
//						scanIdent();
//					} else if (reader.digit(pos, 10) >= 0) {
//						scanNumber(pos, 10);
//					} else if (reader.bp == reader.buflen || reader.ch == EOI && reader.bp + 1 == reader.buflen) { // JLS 3.5
//						tk = TokenKind.EOF;
//						pos = reader.buflen;
//					} else {
//						String arg;
//
//						if (codePoint >= 0) {
//							char high = reader.ch;
//							reader.scanChar();
//							arg = String.format("\\u%04x\\u%04x", (int) high, (int)reader.ch);
//						} else {
//							arg = (32 < reader.ch && reader.ch < 127) ?
//											String.format("%s", reader.ch) :
//											String.format("\\u%04x", (int)reader.ch);
//						}
//						lexError(pos, "illegal.char", arg);
//						reader.scanChar();
//					}
//				}
//				break loop;
//			}
//		}
//		endPos = reader.bp;
//		switch (tk.tag) {
//			case DEFAULT: return new Token(tk, pos, endPos, comments);
//			case NAMED: return new NamedToken(tk, pos, endPos, name, comments);
//			case STRING: return new StringToken(tk, pos, endPos, reader.chars(), comments);
//			case NUMERIC: return new NumericToken(tk, pos, endPos, reader.chars(), radix, comments);
//			default: throw new AssertionError();
//		}
//	}
//	finally {
//		if (SCANNER_DEBUG) {
//				System.out.println("nextToken(" + pos
//								   + "," + endPos + ")=|" +
//								   new String(reader.getRawCharacters(pos, endPos))
//								   + "|");
//		}
//	}
//}
////where
//	List<Comment> addComment(List<Comment> comments, Comment comment) {
//		return comments == null ?
//				List.of(comment) :
//				comments.prepend(comment);
//	}
//
///** Return the position where a lexical error occurred;
// */
//int errPos(JagPTokenizer *tokenizer) {
//	return errPos;
//}
//
///** Set the position where a lexical error occurred;
// */
//void errPos(JagPTokenizer *tokenizer, int pos) {
//	errPos = pos;
//}
//
///**
// * Called when a complete comment has been scanned. pos and endPos
// * will mark the comment boundary.
// */
//Tokens.Comment processComment(JagPTokenizer *tokenizer, int pos, int endPos, CommentStyle style) {
//	if (SCANNER_DEBUG)
//		System.out.println("processComment(" + pos
//						   + "," + endPos + "," + style + ")=|"
//						   + new String(reader.getRawCharacters(pos, endPos))
//						   + "|");
//	char[] buf = reader.getRawCharacters(pos, endPos);
//	return new BasicComment<>(new UnicodeReader(fac, buf, buf.length), style);
//}
//
///**
// * Called when a complete whitespace run has been scanned. pos and endPos
// * will mark the whitespace boundary.
// */
//void processWhiteSpace(JagPTokenizer *tokenizer, int pos, int endPos) {
//	if (SCANNER_DEBUG)
//		System.out.println("processWhitespace(" + pos
//						   + "," + endPos + ")=|" +
//						   new String(reader.getRawCharacters(pos, endPos))
//						   + "|");
//}
//
///**
// * Called when a line terminator has been processed.
// */
//void processLineTerminator(JagPTokenizer *tokenizer, int pos, int endPos) {
//	if (SCANNER_DEBUG)
//		System.out.println("processTerminator(" + pos
//						   + "," + endPos + ")=|" +
//						   new String(reader.getRawCharacters(pos, endPos))
//						   + "|");
//}
//
///** Build a map for translating between line numbers and
// * positions in the input.
// *
// * @return a LineMap */
//Position.LineMap getLineMap(JagPTokenizer *tokenizer) {
//	return Position.makeLineMap(reader.getRawCharacters(), reader.buflen, false);
//}
//
//
/////**
////* Scan a documentation comment; determine if a deprecated tag is present.
////* Called once the initial /, * have been skipped, positioned at the second *
////* (which is treated as the beginning of the first line).
////* Stops positioned at the closing '/'.
////*/
////static class BasicComment<U extends UnicodeReader> implements Comment {
////
////	CommentStyle cs;
////	U comment_reader;
////
////	boolean deprecatedFlag = false;
////	boolean scanned = false;
////
////	BasicComment(U comment_reader, CommentStyle cs) {
////		this.comment_reader = comment_reader;
////		this.cs = cs;
////	}
////
////	String getText() {
////		return null;
////	}
////
////	int getSourcePos(int pos) {
////		return -1;
////	}
////
////	CommentStyle getStyle() {
////		return cs;
////	}
////
////	boolean isDeprecated() {
////		if (!scanned && cs == CommentStyle.JAVADOC) {
////			scanDocComment();
////		}
////		return deprecatedFlag;
////	}
////
////	@SuppressWarnings("fallthrough")
////	void scanDocComment() {
////		try {
////			boolean deprecatedPrefix = false;
////
////			comment_reader.bp += 3; // '/**'
////			comment_reader.ch = comment_reader.buf[comment_reader.bp];
////
////			forEachLine:
////			while (comment_reader.bp < comment_reader.buflen) {
////
////				// Skip optional WhiteSpace at beginning of line
////				while (comment_reader.bp < comment_reader.buflen && (comment_reader.ch == ' ' || comment_reader.ch == '\t' || comment_reader.ch == FF)) {
////					comment_reader.scanCommentChar();
////				}
////
////				// Skip optional consecutive Stars
////				while (comment_reader.bp < comment_reader.buflen && comment_reader.ch == '*') {
////					comment_reader.scanCommentChar();
////					if (comment_reader.ch == '/') {
////						return;
////					}
////				}
////
////				// Skip optional WhiteSpace after Stars
////				while (comment_reader.bp < comment_reader.buflen && (comment_reader.ch == ' ' || comment_reader.ch == '\t' || comment_reader.ch == FF)) {
////					comment_reader.scanCommentChar();
////				}
////
////				deprecatedPrefix = false;
////				// At beginning of line in the JavaDoc sense.
////				if (!deprecatedFlag) {
////					String deprecated = "@deprecated";
////					int i = 0;
////					while (comment_reader.bp < comment_reader.buflen && comment_reader.ch == deprecated.charAt(i)) {
////						comment_reader.scanCommentChar();
////						i++;
////						if (i == deprecated.length()) {
////							deprecatedPrefix = true;
////							break;
////						}
////					}
////				}
////
////				if (deprecatedPrefix && comment_reader.bp < comment_reader.buflen) {
////					if (Character.isWhitespace(comment_reader.ch)) {
////						deprecatedFlag = true;
////					} else if (comment_reader.ch == '*') {
////						comment_reader.scanCommentChar();
////						if (comment_reader.ch == '/') {
////							deprecatedFlag = true;
////							return;
////						}
////					}
////				}
////
////				// Skip rest of line
////				while (comment_reader.bp < comment_reader.buflen) {
////					switch (comment_reader.ch) {
////						case '*':
////							comment_reader.scanCommentChar();
////							if (comment_reader.ch == '/') {
////								return;
////							}
////							break;
////						case CR: // (Spec 3.4)
////							comment_reader.scanCommentChar();
////							if (comment_reader.ch != LF) {
////								continue forEachLine;
////							}
////						/* fall through to LF case */
////						case LF: // (Spec 3.4)
////							comment_reader.scanCommentChar();
////							continue forEachLine;
////						default:
////							comment_reader.scanCommentChar();
////					}
////				} // rest of line
////			} // forEachLine
////			return;
////		} finally {
////			scanned = true;
////		}
////	}
////}
////
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	JagPTokenizer *instance = JAGP_TOKENIZER(self);
//	JagPTokenizerPrivate *priv = jagp_tokenizer_get_instance_private(instance);
//	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
//
//	cat_string_wo_format(append_to, "%s[%p]", iname, self);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/
//
