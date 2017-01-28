package net.natpad.avaj.classfile.attribute;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import net.natpad.avaj.log.Logger;


public class AttributeExceptions implements IAttribute {

	public final short exceptionsCount;
	private final short exceptionIndexTable[];
	
	public AttributeExceptions(AttributeInfo info) throws IOException {
		ByteArrayInputStream stream = info.getStream();
		DataInputStream dis = new DataInputStream(stream);
		exceptionsCount = dis.readShort();
		exceptionIndexTable = new short[exceptionsCount];
		for(int idx=0; idx<exceptionsCount; idx++) {
			exceptionIndexTable[idx] = dis.readShort();
		}
	}
	
	public int count() {
		return exceptionIndexTable.length;
	}
	
	public int get(int index) {
		return ((int) (exceptionIndexTable[index])-1) & 0xFFFF;
	}
	
	
	@Override
	public void dump(String prefix, StringBuilder buf) {
		buf.append(prefix).append("AttributExceptions").append(Logger.ln);
	}

	
}
