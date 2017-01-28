package net.natpad.avaj.classfile.attribute;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import net.natpad.avaj.classfile.constant.IConstantProvider;
import net.natpad.avaj.log.Logger;

public class AttributeLineNumberTable implements IAttribute {

	
	private LineNumberTable table;

	public AttributeLineNumberTable(AttributeInfo info, IConstantProvider constantProvider) throws IOException {
		ByteArrayInputStream stream = info.getStream();
		DataInputStream dis = new DataInputStream(stream);
		int tableLength = dis.readUnsignedShort();
		table = new LineNumberTable();
		for(int idx=0; idx<tableLength; idx++) {
			int startPc = dis.readUnsignedShort();
			int lineNumber = dis.readUnsignedShort();
			
			table.add(new LineNumberTableEntry(startPc, lineNumber));
		}
	}

	
	public LineNumberTable getTable() {
		return table;
	}
	
	@Override
	public void dump(String prefix, StringBuilder buf) {
		buf.append(prefix).append("LineNumberTable").append(Logger.ln);
		int idx = 0;
		for(LineNumberTableEntry entry : table) {
			buf.append(prefix).append(" ["+idx+"] ").append(entry).append(Logger.ln);
			idx++;
		}
	}

}
