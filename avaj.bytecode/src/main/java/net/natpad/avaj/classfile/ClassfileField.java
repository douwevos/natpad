package net.natpad.avaj.classfile;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Modifiers;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.attribute.AttributeCode;
import net.natpad.avaj.classfile.attribute.AttributeConstantValue;
import net.natpad.avaj.classfile.attribute.AttributeMap;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.classfile.constant.IConstant;
import net.natpad.avaj.classfile.constant.IConstantProvider;


public class ClassfileField {
	public final short accessFlags;
	public final Modifiers modifiers;

	public final short nameIndex;
	public final String name;
	public final short descriptorIndex;
	public final AttributeMap attributeMap;
	
	public final DeclarationType declarationType;
	
	public final IConstant value;
	
	public ClassfileField(IConstantProvider constantProvider, short accessFlags, short nameIndex, short descriptorIndex, AttributeMap attributeMap) {
		this.accessFlags = accessFlags;
		this.nameIndex = nameIndex;
		this.descriptorIndex = descriptorIndex;
		this.attributeMap = attributeMap;
		
		this.modifiers = new Modifiers(accessFlags);
		this.name = constantProvider.getConstantUtf8Text(nameIndex-1);

		String descText = constantProvider.getConstantUtf8Text(descriptorIndex-1);
		declarationType = parseDescriptor(descText);
		
		AttributeConstantValue constantValue = attributeMap.getConstantValue();
		if (constantValue!=null) {
			value = constantProvider.get(constantValue.constantValueIndex-1);
		} else {
			value = null;
		}
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append("FieldInfo[");
		buf.append("accessFlags=").append(accessFlags);
		buf.append(", nameIndex=").append(nameIndex);
		buf.append(", descriptorIndex=").append(descriptorIndex);
		
		
		AttributeCode attributeCode = attributeMap.getAttributeCode();
		if (attributeCode!=null) {
			buf.append(attributeCode);
		}
		
		return buf.append("]").toString();
	}

	
	
	
	public static DeclarationType parseDescriptor(String description) {
		boolean expectRef = false;
		int referenceCount = 0;
		Type dPrimitive = null;
		StringBuilder extBuf = new StringBuilder();
		int idx = 0;
		while(idx<description.length()) {
			boolean storeAndCreateNew = false;
			char ch = description.charAt(idx);
			if (expectRef) {
				if (ch==';') {
					storeAndCreateNew = true;
				} else {
					extBuf.append(ch);
				}
			} else {
				switch(ch) {
					case 'B' : {
						dPrimitive = Type.BYTE;
						storeAndCreateNew = true;
					} break;
					case 'C': {
						dPrimitive = Type.CHAR;
						storeAndCreateNew = true;
					} break;
					case 'D': {
						dPrimitive = Type.DOUBLE;
						storeAndCreateNew = true;
					} break;
					case 'F': {
						dPrimitive = Type.FLOAT;
						storeAndCreateNew = true;
					} break;
					case 'I': {
						dPrimitive = Type.INT;
						storeAndCreateNew = true;
					} break;
					case 'J': {
						dPrimitive = Type.LONG;
						storeAndCreateNew = true;
					} break;
					case 'L': {
						expectRef = true;
					} break;
					case 'S': {
						dPrimitive = Type.SHORT;
						storeAndCreateNew = true;
					} break;
					case 'Z': {
						dPrimitive = Type.BOOLEAN;
						storeAndCreateNew = true;
					} break;
					case 'V': {
						dPrimitive = Type.VOID;
						storeAndCreateNew = true;
					} break;
					case '[': {
						referenceCount++;
					} break;
						
				}
				
			}
			if (storeAndCreateNew) {
				DeclarationType desc = null;
				if (expectRef) {
					Name referenceTypeName = new Name(extBuf.toString(), true);
					desc = new DeclarationType(referenceTypeName, referenceCount);
				} else {
					desc = new DeclarationType(dPrimitive, referenceCount);
				}
				return desc;
			}
			idx++;
		}
		return null;
	}	
}