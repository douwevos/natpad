package net.natpad.avaj.classfile;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.attribute.AttributeMap;
import net.natpad.avaj.classfile.constant.ConstantClassInfo;
import net.natpad.avaj.classfile.constant.ConstantFieldReference;
import net.natpad.avaj.classfile.constant.ConstantInterfaceMethodReference;
import net.natpad.avaj.classfile.constant.ConstantMethodReference;
import net.natpad.avaj.classfile.constant.ConstantNameAndType;
import net.natpad.avaj.classfile.constant.ConstantPool;
import net.natpad.avaj.classfile.constant.ConstantPrimString;
import net.natpad.avaj.classfile.constant.ConstantPrimitive;
import net.natpad.avaj.classfile.constant.ConstantUtfText;
import net.natpad.avaj.classfile.constant.IConstant;
import net.natpad.avaj.classfile.constant.IConstantProvider;
import net.natpad.avaj.log.LogLevel;
import net.natpad.avaj.log.Logger;



public class ClassfileParser {

	static Logger log = Logger.getInstance(Classfile.class);

	
	public static final int CONSTANT_UTF8 = 1;
	public static final int CONSTANT_INTEGER = 3;
	public static final int CONSTANT_FLOAT = 4;
	public static final int CONSTANT_LONG = 5;
	public static final int CONSTANT_DOUBLE = 6;
	public static final int CONSTANT_CLASS = 7;
	public static final int CONSTANT_STRING = 8;
	public static final int CONSTANT_FIELDREF = 9;
	public static final int CONSTANT_METHODREF = 10;
	public static final int CONSTANT_INTERFACEMETHODREF = 11;
	public static final int CONSTANT_NAMEANDTYPE = 12;
	
	public ClassfileParser() {
	}
	
	
	public Classfile doParse(InputStream in) throws IOException {
		DataInputStream dis = new DataInputStream(in);
		Classfile classFile = new Classfile();
		
		ConstantPool constantPool = classFile.getConstantPool();
		classFile.magicNr = dis.readInt();
		classFile.minorVersion = dis.readShort();
		classFile.majorVersion = dis.readShort();
		short constantPoolCount = dis.readShort();
		
		for(int idx=1; idx<constantPoolCount; idx++) {
			byte type = dis.readByte();
			IConstant result = null;
			switch(type) {
				case CONSTANT_UTF8 : {
					result = new ConstantUtfText(dis.readUTF());
				} break;
				case CONSTANT_INTEGER : {
					result = new ConstantPrimitive<Integer>(dis.readInt());
				} break;
				case CONSTANT_FLOAT : {
					result = new ConstantPrimitive<Float>(dis.readFloat());
				} break;
				case CONSTANT_LONG : {
					result = new ConstantPrimitive<Long>(dis.readLong());
					constantPool.add(result);
					idx++;
				} break;
				case CONSTANT_DOUBLE : {
					result = new ConstantPrimitive<Double>(dis.readDouble());
					constantPool.add(result);
					idx++;
				} break;
				case CONSTANT_CLASS : {
					result = new ConstantClassInfo(dis.readShort());
				} break;
				case CONSTANT_STRING : {
					result = new ConstantPrimString(dis.readShort());
				} break;
				case CONSTANT_FIELDREF : {
					short classIndex = dis.readShort();
					short nameAndTypeIndex = dis.readShort();
					result = new ConstantFieldReference(classIndex, nameAndTypeIndex);
				} break;
				case CONSTANT_METHODREF : {
					short classIndex = dis.readShort();
					short nameAndTypeIndex = dis.readShort();
					result = new ConstantMethodReference(classIndex, nameAndTypeIndex);
				} break;
				case CONSTANT_INTERFACEMETHODREF : {
					short classIndex = dis.readShort();
					short nameAndTypeIndex = dis.readShort();
					result = new ConstantInterfaceMethodReference(classIndex, nameAndTypeIndex);
				} break;
				case CONSTANT_NAMEANDTYPE : {
					short nameIndex = dis.readShort();
					short descriptorIndex = dis.readShort();
					result = new ConstantNameAndType(nameIndex, descriptorIndex);
				} break;
			}
			constantPool.add(result);
		}


		
		classFile.accessFlags = dis.readShort();
		classFile.thisClass = dis.readShort();
		classFile.superClass = dis.readShort();
		short interfaceCount = dis.readShort();
		constantPool.setThisClassPoolIndex(classFile.thisClass);
		constantPool.dump();

		classFile.interfaces = new short[interfaceCount];
		classFile.interfaceList = new DeclarationType[interfaceCount];
		for(int idx=0; idx<interfaceCount; idx++) {
			int sit = dis.readUnsignedShort();
			classFile.interfaces[idx] = (short) sit;
			ConstantClassInfo constClassInfo = (ConstantClassInfo) classFile.getConstantPool().getResolved(sit-1);
			classFile.interfaceList[idx] = constClassInfo.getParsedDescriptor();
		}
		
		short fieldCount = dis.readShort();
		classFile.fields = new ClassfileField[fieldCount];
		for(int idx=0; idx<fieldCount; idx++) {
			classFile.fields[idx] = readField(dis, constantPool);
		}
		
		
		short methodCount = dis.readShort();
		classFile.methodInfos = new ClassfileMethod[methodCount];
		for(int idx=0; idx<methodCount; idx++) {
			classFile.methodInfos[idx] = readMethod(dis, classFile.getMainTypeName(), constantPool);
		
			log.debug("method:"+classFile.methodInfos[idx].methodName);
			
			if (log.logEnabbledFor(LogLevel.DETAIL)) {
				StringBuilder buf = new StringBuilder();
				classFile.methodInfos[idx].attributeMap.dump("  ", buf);
				log.detail(buf.toString());
			}
			
			log.debug("end method:"+classFile.methodInfos[idx].methodName);

			
		}
		
		short attributeCount = dis.readShort();
		classFile.attributeMap = new AttributeMap(dis, constantPool, attributeCount);
		
		if (log.logEnabbledFor(LogLevel.DETAIL)) {
			StringBuilder buf = new StringBuilder();
			classFile.attributeMap.dump("", buf);
			log.detail(buf.toString());
		}
			
		return classFile;
	}
	
//	private ParsedClass createClassDefinition(Classfile classFile) {
//
//		ConstantPool constantPool = classFile.getConstantPool();
//		
//		constantPool.resolveAll(classFile.thisClass);
//		constantPool.dump();
//
//
//		
//		ParsedClass parsedClass = new ParsedClass(classFile.getMainTypeName(), classFile.getSuperTypeName(), classFile.getModifiers(), constantPool, null, null);
//		
//		ConstantClassInfo constClassInfo;
//		if (classFile.interfaces!=null && classFile.interfaces.length>0) {
//			for(short sit : classFile.interfaces) {
//				constClassInfo = (ConstantClassInfo) constantPool.getResolved(sit-1);
//				parsedClass.add(constClassInfo.getRefrenceTypeName());
//			}
//			
//		}
//		
////		TypeDefinition classDef = new TypeDefinition(typeReference, modifiers, constantPool, bytecode);
//		for(ClassfileMethod methodInfo : classFile.methodInfos) {
//			parsedClass.add(methodInfo);
//		}
//		
//		
//		
//		
//		int idx = 0;
//		for(ClassfileField fieldInfoItem : classFile.fields) {
//			
////			ParsedField parsedField = new ParsedField(fieldInfoItem.modifiers, fieldInfoItem.declarationType, fieldInfoItem.name, fieldInfoItem.value);
//			parsedClass.add(fieldInfoItem);
//			log.trace(idx+" :: "+fieldInfoItem);
//		}
//		
//		return parsedClass;
//	}


	private ClassfileMethod readMethod(DataInputStream dis, Name mainTypeName, IConstantProvider constantProvider) throws IOException {
		short accessFlags = dis.readShort();
		short nameIndex = dis.readShort();
		short descriptorIndex = dis.readShort();
		short attrCount = dis.readShort();
		AttributeMap attrMap = new AttributeMap(dis, constantProvider, attrCount);
		return new ClassfileMethod(mainTypeName, constantProvider, accessFlags, nameIndex, descriptorIndex, attrMap);
	}
	

	public ClassfileField readField(DataInputStream dis, IConstantProvider constantProvider) throws IOException {
		short accessFlags = dis.readShort();
		short nameIndex = dis.readShort();
		short descriptorIndex = dis.readShort();
		short attrCount = dis.readShort();
		AttributeMap attrMap = new AttributeMap(dis, constantProvider, attrCount);
		return new ClassfileField(constantProvider, accessFlags, nameIndex, descriptorIndex, attrMap);
	}
	

}
