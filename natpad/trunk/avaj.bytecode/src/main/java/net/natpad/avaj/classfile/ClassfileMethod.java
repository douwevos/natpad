package net.natpad.avaj.classfile;

import net.natpad.avaj.ast.Modifiers;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.attribute.AttributeCode;
import net.natpad.avaj.classfile.attribute.AttributeExceptions;
import net.natpad.avaj.classfile.attribute.AttributeLineNumberTable;
import net.natpad.avaj.classfile.attribute.AttributeLocalVariableTable;
import net.natpad.avaj.classfile.attribute.AttributeMap;
import net.natpad.avaj.classfile.attribute.AttributeCode.AttributeCodeException;
import net.natpad.avaj.classfile.constant.ConstantClassInfo;
import net.natpad.avaj.classfile.constant.IConstantProvider;


public class ClassfileMethod {

	public final IConstantProvider constantProvider;
	
	public final short accessFlags;
	public final Modifiers modifiers;
	public final short nameIndex;
	public final short descriptorIndex;
	public final AttributeMap attributeMap;

	public final String methodName;
	
	public final ClassfileMethodHeader methodHeader;
	
	
	private TryCatchList tryCatchList;
	
	public ClassfileMethod(Name mainTypeName, IConstantProvider constantProvider, short accessFlags, short nameIndex, short descriptorIndex, AttributeMap attributeMap) {
		this.constantProvider = constantProvider;
		this.accessFlags = accessFlags;
		this.nameIndex = nameIndex;
		this.descriptorIndex = descriptorIndex;
		this.attributeMap = attributeMap;
		
		modifiers = new Modifiers(accessFlags);
		
		methodName = constantProvider.getConstantUtf8Text(nameIndex-1);

		String methodDescriptorText = constantProvider.getConstantUtf8Text(descriptorIndex-1);
		methodHeader = new ClassfileMethodHeader(mainTypeName, methodName, methodDescriptorText);

		AttributeExceptions attributeExceptions = attributeMap.getAttributeExceptions();
		
		ParsedExcpetions parsedExcpetions = null;
		
		if (attributeExceptions!=null) {
			parsedExcpetions = new ParsedExcpetions();
			int exceptionCount = attributeExceptions.count();
			for(int idx=0; idx<exceptionCount; idx++) {
				int poolIndex = attributeExceptions.get(idx);
				ConstantClassInfo constantClassInfo = (ConstantClassInfo) constantProvider.get(poolIndex);
				parsedExcpetions.add(constantClassInfo.getRefrenceTypeName());
			}
		}
		methodHeader.setParsedExceptions(parsedExcpetions);
	}


	public String getMethodName() {
		return methodName;
	}
	
	
	
	public AttributeLocalVariableTable getLocalVariableTable() {
		AttributeLocalVariableTable result = null;
		AttributeCode attributeCode = attributeMap.getAttributeCode();
		if (attributeCode!=null) {
			result = attributeCode.getLocalVariableTable();
		}
		return result;
	}


	public AttributeLineNumberTable getLineNumberTable() {
		AttributeLineNumberTable result = null;
		AttributeCode attributeCode = attributeMap.getAttributeCode();
		if (attributeCode!=null) {
			result = attributeCode.getLineNumberTable();
		}
		return result;
	}
	
	
	public byte[] getBytecode() {
		byte[] byteCode = null;
		AttributeCode attributeCode = attributeMap.getAttributeCode();
		if (attributeCode!=null) {
			byteCode = attributeCode.getByteCode();
		}
		return byteCode;
	}
	

	public TryCatchList getTryCatchList() {
		if (tryCatchList==null) {
			AttributeCode attributeCode = attributeMap.getAttributeCode();
			if (attributeCode!=null) {
				AttributeCodeException[] tryCatchTable = attributeCode.getTryCatchTable();
				tryCatchList = new TryCatchList(tryCatchTable, constantProvider);
			}
		}
		return tryCatchList;
	}

	
}