package net.natpad.avaj.classfile;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Modifiers;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.attribute.AttributeCode;
import net.natpad.avaj.classfile.attribute.AttributeMap;
import net.natpad.avaj.classfile.constant.ConstantClassInfo;
import net.natpad.avaj.classfile.constant.ConstantPool;

public class Classfile  {

	int magicNr;
	short minorVersion;
	short majorVersion;
	
	private ConstantPool constantPool = new ConstantPool();
	
	short accessFlags;
	private Modifiers modifiers;
	
	short thisClass;
	private Name mainTypeName;
	
	
	short superClass;
	private Name superTypeName;
	
	short interfaces[];
	
	DeclarationType interfaceList[];
	
	ClassfileField fields[];
	
	ClassfileMethod methodInfos[];
	
	
	AttributeMap attributeMap;

	
	public ConstantPool getConstantPool() {
		return constantPool;
	}

	
	public Name getMainTypeName() {
		if (mainTypeName==null) {
			ConstantClassInfo constantClassInfo = (ConstantClassInfo) constantPool.getResolved(thisClass-1);
			mainTypeName = constantClassInfo.getRefrenceTypeName();
		}
		return mainTypeName;
	}
	
	public Name getSuperTypeName() {
		if (superTypeName==null) {
			ConstantClassInfo constClassInfo = (ConstantClassInfo) constantPool.getResolved(superClass-1);
			superTypeName = constClassInfo.getRefrenceTypeName();
		}
		return superTypeName;
	}
	
	

	public Modifiers getModifiers() {
		if (modifiers==null) {
			modifiers = new Modifiers((short) (accessFlags & 0x411));
		}
		return modifiers;
	}

	
	public ClassfileField[] getFields() {
		return fields;
	}

	public DeclarationType[] getInterfaces() {
		return interfaceList;
	}

	public ClassfileMethod[] getMethods() {
		return methodInfos;
	}

	public byte[] getByteCode() {
		byte[] result = null;
		AttributeCode attributeCode = attributeMap.getAttributeCode();
		if (attributeCode!=null) {
			result = attributeCode.getByteCode();
		}
		return result;
	}
	
	
	public TryCatchList getTryCatchList() {
		return null;
	}

}
