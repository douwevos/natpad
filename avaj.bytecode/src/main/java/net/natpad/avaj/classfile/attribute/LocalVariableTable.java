package net.natpad.avaj.classfile.attribute;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;

public class LocalVariableTable implements Iterable<LocalVariableTableEntry> {

	static Logger log = Logger.getInstance(LocalVariableTable.class);

	
	protected List<LocalVariableTableEntry> entries = new ArrayList<LocalVariableTableEntry>(); 

	
	

	public String findName(int frameIndex, int index) {
		LocalVariableTableEntry entry = findEntry(frameIndex, index);
		if (entry != null) {
			return entry.nameText;
		}
		return null;
	}

	
	public LocalVariableTableEntry findEntry(int frameIndex, int index) {
		LocalVariableTableEntry  result = null;
		for(LocalVariableTableEntry entry : entries) {
			log.detail("localVariable="+entry+", frameIndex="+frameIndex+", index="+TextUtil.hex(index,4));
			if (entry.index==frameIndex && index>=entry.startPc && index<=(entry.startPc+entry.length)) {
				result = entry;
				break;
			}
		}
		return result;
	}

	
	
	public void add(LocalVariableTableEntry entry) {
		entries.add(entry);
	}

	
	public void remove(LocalVariableTableEntry entry) {
		entries.remove(entry);
	}
	
	public boolean isEmpty() {
		return entries.isEmpty();
	}
	
	public int count() {
		return entries.size();
	}

	
	@Override
	public Iterator<LocalVariableTableEntry> iterator() {
		return entries.iterator();
	}

	
}
