package net.natpad.avaj.classfile.attribute;

import java.io.ByteArrayInputStream;

import net.natpad.avaj.log.Logger;


class AttributeInfo implements IAttribute {

	public final short attributeNameIndex;
	private final byte[] attributeData;
	
	public AttributeInfo(short attributeNameIndex, byte[] attributeData) {
		this.attributeData = attributeData;
		this.attributeNameIndex = attributeNameIndex;
	}
	
	
	
	public byte get(int index) {
		return attributeData[index];
	}
	
	public int count() {
		return attributeData.length;
	}
	
	public ByteArrayInputStream getStream() {
		return new ByteArrayInputStream(attributeData);
	}
	
	
	@Override
	public void dump(String prefix, StringBuilder buf) {
		buf.append(prefix).append("AttributInfo").append(Logger.ln);
	}
}