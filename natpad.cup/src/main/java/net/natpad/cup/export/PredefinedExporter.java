package net.natpad.cup.export;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.util.HashMap;
import java.util.HashSet;

public class PredefinedExporter {

	final HashMap<String, String> tokenMap;
	Writer out;
	HashSet<String> unresolved = new HashSet<String>();
	
	public PredefinedExporter(File inputFile, File outputFile, HashMap<String, String> tokenMap) throws IOException {
		this(new FileReader(inputFile), outputFile, tokenMap);
	}

	public PredefinedExporter(Reader inputFile, File outputFile, HashMap<String, String> tokenMap) throws IOException {
		this.tokenMap = tokenMap;
		out = new FileWriter(outputFile);
		String rawText = readTextFile(inputFile);
		parse(rawText);
		out.flush();
		out.close();
		if (!unresolved.isEmpty()) {
			for(String unr : unresolved) {
				System.out.println("unresolved: tokenMap.put(\""+unr+"\", );");
			}
			System.exit(1);
		}
	}
	
	
	
	private void parse(String rawText) throws IOException {
		int idxA = 0;
		int idxB = 0;
		int idxLast = 0;
		while(true) {
			idxA = rawText.indexOf("[%", idxLast);
			if (idxA>=0) {
				idxB = rawText.indexOf("%]", idxA+2);
				if (idxB>=0) {
					out.write(rawText.substring(idxLast, idxA));
					String token = rawText.substring(idxA+2, idxB);
//					System.err.println("token="+token);
					if (token.startsWith("if-set:")) {
						String filterName = token.substring("if-set:".length());
						if (tokenMap.containsKey(filterName)) {
							idxLast = idxB+2;
						} else {
							String filterEnd = "[%end-if:"+filterName+"%]";
							idxLast = rawText.indexOf(filterEnd, idxB+2);
							if (idxLast>=0) {
								idxLast += filterEnd.length();
							} else {
								addUnresolved(filterEnd);
								idxLast = idxB+2;
							}
						}
					} else if (token.startsWith("if-not-set:")) {
						String filterName = token.substring("if-not-set:".length());
						if (!tokenMap.containsKey(filterName)) {
							idxLast = idxB+2;
						} else {
							String filterEnd = "[%end-if:"+filterName+"%]";
							idxLast = rawText.indexOf(filterEnd, idxB+2);
							if (idxLast>=0) {
								idxLast += filterEnd.length();
							} else {
								
								addUnresolved(filterEnd);
								idxLast = idxB+2;
							}
						}
					} else if (token.startsWith("end-if:")) {
						idxLast = idxB+2;
					} else {
						out.write(resolve(token));
						idxLast = idxB+2;
					}
				} else {
					out.write(rawText.substring(idxLast));
					break;
				}
			} else {
				out.write(rawText.substring(idxLast));
				break;
			}
		}
		
	}



	private String resolve(String token) {
		String result = tokenMap.get(token);
		if (result==null) {
			addUnresolved(token);
			result = "[%"+token+":%]";
		}
		return result;
	}



	private void addUnresolved(String token) {
		unresolved.add(token);
	}



	String readTextFile(Reader fileReader) throws IOException {
		char cbuf[] = new char[4096];
		StringBuilder buf = new StringBuilder();
		while(true) {
			int read = fileReader.read(cbuf);
			if (read<=0) {
				break;
			}
			buf.append(cbuf, 0, read);
		}
		return buf.toString();
	}
	
	
	
}
