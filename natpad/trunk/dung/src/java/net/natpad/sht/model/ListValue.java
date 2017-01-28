package net.natpad.sht.model;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Iterator;

public class ListValue extends Value implements Iterable<Value> {

	private ArrayList<Value> list = new ArrayList<Value>();
	
	public void add(Value expr) {
		list.add(expr);
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("{");
		for(Value e : list) {
			if (buf.length()>1) {
				buf.append(',');
			}
			buf.append(e);
		}
		buf.append("}");
		return buf.toString();
	}


	public Value get(int idx) {
		if (idx>=0 && idx<list.size()) {
			return list.get(idx);
		}
		return null;
	}
	
	public int count() {
		return list.size();
	}
	
	
	@Override
	public Iterator<Value> iterator() {
		return list.iterator();
	}
	
	@Override
	public void dump(PrintStream out, String indent) {
		out.println(indent+"List");
		indent = indent+"   ";
		for(Value ref : list) {
			ref.dump(out, indent);
		}
		
	}
	
}
