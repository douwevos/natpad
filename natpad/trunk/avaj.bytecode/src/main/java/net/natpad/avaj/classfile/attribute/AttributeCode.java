package net.natpad.avaj.classfile.attribute;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import net.natpad.avaj.classfile.bytecode.MnemonicBlock;
import net.natpad.avaj.classfile.constant.IConstantProvider;
import net.natpad.avaj.log.Logger;



public class AttributeCode implements IAttribute {

	static Logger log = Logger.getInstance(IAttribute.class);

	
	short maxStack, maxLocals;
	byte bytecode[];
	
	AttributeCodeException exceptionTable[];
	
	AttributeMap attributeMap;
	
	public AttributeCode(AttributeInfo info, IConstantProvider constantProvider) throws IOException {
		ByteArrayInputStream stream = info.getStream();
		DataInputStream dis = new DataInputStream(stream);
		maxStack = dis.readShort();
		maxLocals = dis.readShort();
		int codeLength = dis.readInt();
		bytecode = new byte[codeLength];
		dis.read(bytecode);
		
		int exceptionTableLength = (int) dis.readShort() & 0xFFFF;
		exceptionTable = new AttributeCodeException[exceptionTableLength];
		for(int idx=0; idx<exceptionTableLength; idx++) {
			short startPc = dis.readShort();
			short endPc = dis.readShort();
			short handlerPc = dis.readShort();
			short catchType = dis.readShort();
			exceptionTable[idx] = new AttributeCodeException(startPc, endPc, handlerPc, catchType);
		}
		
		short attributeCount = dis.readShort();
		attributeMap = new AttributeMap(dis, constantProvider, attributeCount);
	}
	
	
	public byte[] getByteCode() {
		byte result[] = new byte[bytecode.length];
		System.arraycopy(bytecode, 0, result, 0, result.length);
		return result;
	}
	
	public static class AttributeCodeException {

		public final short startPc;
		public final short endPc;
		public final short handlerPc;
		public final short catchType;
		
		public AttributeCodeException(short startPc, short endPc, short handlerPc, short catchType) {
			this.startPc = startPc;
			this.endPc = endPc;
			this.handlerPc = handlerPc;
			this.catchType = catchType;
        }
		
	}


	public AttributeCodeException[] getTryCatchTable() {
		return exceptionTable;
	}
	
	
	@Override
	public void dump(String prefix, StringBuilder buf) {
		prefix = prefix+"   ";
		
		StringBuilder bytesAsHexText = Logger.dumpByteArray(prefix, bytecode, 0, 4, 32, false);
		buf.append(bytesAsHexText).append(Logger.ln);
		
		MnemonicBlock mnemonicBlock = new MnemonicBlock(bytecode);
		buf.append(mnemonicBlock.dump(prefix));
		buf.append(Logger.ln);
		
		attributeMap.dump(prefix, buf);
	}


	public AttributeLocalVariableTable getLocalVariableTable() {
		if (attributeMap!=null) {
			return (AttributeLocalVariableTable) attributeMap.get2("LocalVariableTable");
		}
		return null;
	}


	public AttributeLineNumberTable getLineNumberTable() {
		if (attributeMap!=null) {
			return (AttributeLineNumberTable) attributeMap.get2("LineNumberTable");
		}
		return null;
	}
	
}
