package net.natpad.avaj.classfile.attribute;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class LineNumberTable implements Iterable<LineNumberTableEntry> {

	
	protected List<LineNumberTableEntry> entries = new ArrayList<LineNumberTableEntry>(); 


	public void add(LineNumberTableEntry entry) {
		entries.add(entry);
	}

	
	public void remove(LineNumberTableEntry entry) {
		entries.remove(entry);
	}
	
	public boolean isEmpty() {
		return entries.isEmpty();
	}
	
	public int count() {
		return entries.size();
	}

	
	@Override
	public Iterator<LineNumberTableEntry> iterator() {
		return entries.iterator();
	}

	
}
