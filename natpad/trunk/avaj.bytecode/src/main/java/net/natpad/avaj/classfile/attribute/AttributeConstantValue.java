package net.natpad.avaj.classfile.attribute;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import net.natpad.avaj.log.Logger;


public class AttributeConstantValue implements IAttribute {

	public final int constantValueIndex;
	
	public AttributeConstantValue(AttributeInfo info) throws IOException {
		ByteArrayInputStream stream = info.getStream();
		DataInputStream dis = new DataInputStream(stream);
		constantValueIndex = dis.readShort();
	}
	
	
	@Override
	public void dump(String prefix, StringBuilder buf) {
		buf.append(prefix).append("AttributConstantValue").append(Logger.ln);
	}

}
