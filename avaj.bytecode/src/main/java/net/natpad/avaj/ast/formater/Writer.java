package net.natpad.avaj.ast.formater;

import java.util.ArrayList;

public class Writer {

	ArrayList<String> lines = new ArrayList<String>();
	StringBuilder lastLine = new StringBuilder();
	
	private String lastIndexText = "";
	private String indentText = "";
	private int indent;

	public void setAtLeastLineNr(int lineNumber) {
		if (lineNumber>=0) {
			while(lines.size()<lineNumber) {
				storeAndClearLine();
			}
		}
	}
	
	
//	public Writer append(String text) {
//		int lastIndex = 0;
//		for(int idx=0; idx<text.length(); idx++) {
//			boolean lineBreak = text.charAt(idx)<32;
//			if (lineBreak) {
//				lastLine.append(text.substring(lastIndex, idx));
//				storeAndClearLine();
//				lastIndex = idx+1;
//			}
//		}
//		lastLine.append(text.substring(lastIndex, text.length()));
//		return this;
//	}
	
	public Writer print(String text) {
		int lastIndex = 0;
		for(int idx=0; idx<text.length(); idx++) {
			boolean lineBreak = text.charAt(idx)<32;
			if (lineBreak) {
				lastLine.append(text.substring(lastIndex, idx));
				storeAndClearLine();
				lastIndex = idx+1;
			}
		}
		lastLine.append(text.substring(lastIndex, text.length()));
		return this;
	}

	public void clear() {
		lines.clear();
		lastLine.setLength(0);
		
		lastIndexText = "";
		indentText = "";
		indent = 0;
	}
	
	private void storeAndClearLine() {
		lines.add(lastIndexText+lastLine.toString());
		lastIndexText = indentText;
		lastLine.setLength(0);
	}
	
	public void increaseIndent() {
		indent++;
		rebuildIndent();
	}
	
	public void decreaseIndent() {
		indent--;
		rebuildIndent();
	}

	public String toString() {
		StringBuilder buf = new StringBuilder();
		for(int idx=0; idx<lines.size(); idx++) {
			String line = lines.get(idx);
//			buf.append(""+idx+":").append(line).append("\n");
			buf.append(line).append("\n");
		}
		if (lastLine.length()>0) {
			buf.append(lastLine);
		}
		
		return buf.toString();
	}
	
	private void rebuildIndent() {
		StringBuilder newIndent = new StringBuilder();
		for(int idx=0; idx<indent; idx++) {
			newIndent.append("    ");
		}
		indentText = newIndent.toString();
		if (lastLine.length()==0) {
			lastIndexText = indentText;
		}
	}
	
}
