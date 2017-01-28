package net.natpad.json;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.util.ArrayList;

public class JsonParser {

	public final Reader reader;
	protected int nextChar;
	
	public JsonParser(Reader reader) {
		this.reader = reader;
	}
	
	enum Status {
		JSON_MAIN,
		JSON_DONE,
		JSON_STRING,
		JSON_NUMBER,
		JSON_OBJECT, 
		JSON_OBJECT_KEY_COLON_VALUE,
		JSON_OBJECT_END, 
		JSON_OBJECT_DOT_KEY,
		JSON_ARRAY, 
		JSON_ARRAY_DOT_VALUE,
		JSON_ARRAY_VALUE,
		JSON_TEXT, 
	}
	
	
	private final static String TXTHEX = "0123456789abcdefABCDEF";
	private final static String TXTNUMBERCHARS = "eE.-+0123456789";
	private final static String TXTNONIDENTIFIERS = ";,}{[]()";
	
	public int readUnicode() throws JsonException, IOException {
		int val = 0;
		for(int idx=4; idx>0; idx--) {
			int indexOf = TXTHEX.indexOf(nextChar);
			if (indexOf<0) {
				throw new JsonException("invalid unicode");
			}
			if (indexOf>15) {
				indexOf-=6;
			}
			val = (val<<4) + indexOf;
			advance();
		}
		return val;
	}
	
	public IJsonItem parse() throws IOException, JsonException {
		StringBuilder buf = new StringBuilder();
		Status state = Status.JSON_MAIN;
		boolean doAdvance = true;
		ArrayList<Status> statusStack = new ArrayList<Status>();
		statusStack.add(Status.JSON_DONE);
		ArrayList<Object> valueStack = new ArrayList<Object>();
//		int count = 20;
		while(true) {
//			if (count--<0) {
//				return null;
//			}
//			System.out.println("state="+state+", next_char="+nextChar+"["+(char) nextChar+"]");
			if (doAdvance) {
				advance();
				doAdvance = false;
			}
			switch(state) {
				case JSON_MAIN : {
					switch(nextChar) {
						case '"' : {
							state = Status.JSON_STRING;
						} break;
						
						case '0' : 
						case '1' : 
						case '2' : 
						case '3' : 
						case '4' : 
						case '5' : 
						case '6' : 
						case '7' : 
						case '8' : 
						case '9' : 
						case '-' : {
							state = Status.JSON_NUMBER;
						} break;
						
						case '{' : {
							state = Status.JSON_OBJECT;
							advance();
							valueStack.add(new JsonObject());
						} break;

						case '[' : {
							state = Status.JSON_ARRAY;
							advance();
							valueStack.add(new JsonArray());
						} break;

						case ' ' :
						case '\t' :
						case '\n' :
						case '\0' : {
							advance();
						} break;

						case -1 : {
							state = statusStack.remove(statusStack.size()-1);
						} break;
						
						default : {
							if (Character.isJavaIdentifierPart(nextChar)) {
								state = Status.JSON_TEXT;
							} else {
								throw new JsonException("invalid char:"+nextChar);
							}
						}
					}
				} break;
				
				case JSON_DONE : {
					if (valueStack.isEmpty()) {
						return null;
					}
					return (IJsonItem) valueStack.get(valueStack.size()-1);
				}
				
				case JSON_TEXT : {
					buf.setLength(0);
					while(TXTNONIDENTIFIERS.indexOf(nextChar)<0 && nextChar!=' ' && nextChar!='\t' && nextChar!='\n' && nextChar!='\0' && nextChar!=-1) {
						buf.append((char) nextChar);
						advance();
					}
					String identifier = buf.toString().trim();
					if ("true".equalsIgnoreCase(identifier)) {
						valueStack.add(new JsonBoolean(true));
					} else if ("false".equalsIgnoreCase(identifier)) {
						valueStack.add(new JsonBoolean(false));
					} else {
						valueStack.add(new JsonString(identifier));
					}
					state = statusStack.remove(statusStack.size()-1);
				} break;
				
				case JSON_STRING : {
					boolean inPrefix = false;
					buf.setLength(0);
					while(true) {
						advance();
						if (inPrefix) {
							switch(nextChar) {
								case -1 : {
									throw new JsonException("unterminated escape code in unterminated string");
								}
								case '"' : {
									buf.append('"');
								} break;
								case '\\' : {
									buf.append('\\');
								} break;
								case '/' : {
									buf.append('/');
								} break;
								case 'b' : {
									buf.append('\b');
								} break;
								case 'f' : {
									buf.append('\f');
								} break;
								case 'n' : {
									buf.append('\n');
								} break;
								case 'r' : {
									buf.append('\r');
								} break;
								case 't' : {
									buf.append('\t');
								} break;
								case 'u' : {
									advance();
									int code = readUnicode();
									buf.append((char) code);
								} break;
								default :
									throw new JsonException("invalid escape code");
							}
							inPrefix = false;
						} else {
							if (nextChar=='\\') {
								inPrefix = true;
							} else if (nextChar=='"') {
								advance();
								break;
							} else {
								buf.append((char) nextChar);
							}
						}
					}
					state = statusStack.remove(statusStack.size()-1);
					valueStack.add(new JsonString(buf.toString()));
				} break;
				
				case JSON_NUMBER : {
					
					buf.setLength(0);
					boolean isFloat = false;
					while(true) {
						int indexOf = TXTNUMBERCHARS.indexOf(nextChar);
						if (indexOf<0) {
							break;
						} else if (indexOf<3) {
							isFloat = true;
						}
						buf.append((char) nextChar); 
						advance();
					}
					if (isFloat) {
						double parseDouble = Double.parseDouble(buf.toString());
						valueStack.add(new JsonDouble(parseDouble));		
					} else {
						long parseLong = Long.parseLong(buf.toString());
						valueStack.add(new JsonLong(parseLong));		
					}
					state = statusStack.remove(statusStack.size()-1);
				} break;
				
				case JSON_OBJECT : {
					skipSpaces();
					if (nextChar=='}') {
						advance();
						state = statusStack.remove(statusStack.size()-1);
						break;
					}
					state = Status.JSON_OBJECT_DOT_KEY;
				}

				case JSON_OBJECT_DOT_KEY : {
					buf.setLength(0);
					while(true) {
						if (nextChar==':') {
							advance();
							break;
						} else if (nextChar<0) {
							throw new JsonException("expected colon preceding a identifier");
						} else if (TXTNONIDENTIFIERS.indexOf(nextChar)>=0) {
							throw new JsonException("invalid identifier character:"+(char) nextChar);
						}
						buf.append((char) nextChar);
						advance();
					}
					String key = buf.toString().trim();
					if (key.isEmpty()) {
						throw new JsonException("empty key");
					}
					valueStack.add(key);
					statusStack.add(Status.JSON_OBJECT_KEY_COLON_VALUE);
					state = Status.JSON_MAIN;
				} break;
				
				case JSON_OBJECT_KEY_COLON_VALUE : {
					IJsonItem value = (IJsonItem) valueStack.remove(valueStack.size()-1);
					String key = (String) valueStack.remove(valueStack.size()-1);
					JsonObject obj = (JsonObject) valueStack.get(valueStack.size()-1);
					obj.set(key, value);
					
					skipSpaces();
					switch(nextChar) {
						case '}' : {
							state = statusStack.remove(statusStack.size()-1);
							advance();
						} break;
						
						case ',' : {
							advance();
							state = Status.JSON_OBJECT_DOT_KEY;
						} break;
					}
				} break;
			
				
				case JSON_ARRAY : {
					skipSpaces();
					if (nextChar==']') {
						advance();
						state = statusStack.remove(statusStack.size()-1);
						break;
					}
					statusStack.add(Status.JSON_ARRAY_VALUE);
					state = Status.JSON_MAIN;
				} break;

				case JSON_ARRAY_VALUE : {
					IJsonItem value = (IJsonItem) valueStack.remove(valueStack.size()-1);
					JsonArray jsonArray = (JsonArray) valueStack.get(valueStack.size()-1);
					jsonArray.add(value);
					
					skipSpaces();
					switch(nextChar) {
						case ']' : {
							state = statusStack.remove(statusStack.size()-1);
							advance();
						} break;
						
						case ',' : {
							advance();
							statusStack.add(Status.JSON_ARRAY_VALUE);
							state = Status.JSON_MAIN;
						} break;
						
						default : {
							throw new JsonException("invalid charachter:"+(char) nextChar);
						}
					}
				} break;
							
				
			}
		}
	}
	
	
	private void skipSpaces() throws IOException {
		while(true) {
			if (nextChar==' ' || nextChar=='\t' || nextChar=='\n' || nextChar=='\0') {
				advance();
			} else {
				break;
			}
		}
	}

	private void advance() throws IOException {
		nextChar = reader.read();
	}


	public static void main(String[] args) {
		try {
			JsonParser jsonParser = new JsonParser(new StringReader("\"this is a test\""));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("321"));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("321.5"));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("{  }"));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("{ val : 3 }"));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("{ val : { val : 3, val2 : 5} }"));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("[]"));
			System.out.println("jsonItem="+jsonParser.parse());

			jsonParser = new JsonParser(new StringReader("[ 3, 5, 6, \"trdy\", true]"));
			System.out.println("jsonItem="+jsonParser.parse());

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
}
