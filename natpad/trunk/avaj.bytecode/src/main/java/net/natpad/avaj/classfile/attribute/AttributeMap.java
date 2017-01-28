package net.natpad.avaj.classfile.attribute;

import java.io.DataInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import net.natpad.avaj.classfile.constant.IConstantProvider;
import net.natpad.avaj.log.Logger;

public class AttributeMap {

	private Map<String, IAttribute> map = new HashMap<String, IAttribute>();

	
	public AttributeMap(DataInputStream dis, IConstantProvider constantProvider, int cnt) throws IOException {
		for(int idx=0; idx<cnt; idx++) {
			AttributeInfo attributeInfo = readAttributeInfo(dis);
			IAttribute iattr = attributeInfo;
			String attributeName = constantProvider.getConstantUtf8Text(attributeInfo.attributeNameIndex-1);
			if (attributeName.equals("Code")) {
				iattr = new AttributeCode(attributeInfo, constantProvider);
			} else if (attributeName.equals("ConstantValue")) {
				iattr = new AttributeConstantValue(attributeInfo);
			} else if (attributeName.equals("Exceptions")) {
				iattr = new AttributeExceptions(attributeInfo);
			} else if (attributeName.equals("LocalVariableTable")) {
				iattr = new AttributeLocalVariableTable(attributeInfo, constantProvider);
			} else if (attributeName.equals("LineNumberTable")) {
				iattr = new AttributeLineNumberTable(attributeInfo, constantProvider);
			}
			
			map.put(attributeName, iattr);
		}
	}

	private AttributeInfo readAttributeInfo(DataInputStream dis) throws IOException {
		short attributeNameIndex = dis.readShort();
		int length = dis.readInt();
		byte attributeData[] = new byte[length];
		dis.read(attributeData);
		return new AttributeInfo(attributeNameIndex, attributeData);
	}

	public IAttribute get2(String name) {
		return map.get(name);
	}
	

	public AttributeCode getAttributeCode() {
		return (AttributeCode) map.get("Code");
	}
	
	public AttributeConstantValue getConstantValue() {
		return (AttributeConstantValue) map.get("ConstantValue");
	}

	public AttributeExceptions getAttributeExceptions() {
		return (AttributeExceptions) map.get("Exceptions");
	}

	
	public void dump(String prefix, StringBuilder buf) {
		if (map.isEmpty()) {
			buf.append(prefix).append("dumping attributes:EMPTY").append(Logger.ln);
			return;
		}
		buf.append(prefix).append("dumping attributes:size="+map.size()).append(Logger.ln);
		buf.append(prefix).append("--------------------------------------------------").append(Logger.ln);
		int attrIndex = 0;
		for(String attrName : map.keySet()) {
			buf.append(prefix).append("   "+(attrIndex++)).append(" :: ").append(attrName).append(Logger.ln);
			IAttribute attribute = map.get(attrName);
			attribute.dump(prefix+"      ", buf);
		}
		buf.append(prefix).append("--------------------------------------------------").append(Logger.ln);
	}

}
