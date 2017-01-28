package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpNewArray extends AbstractMnemonic {


	public final Type arrayType;
	public final int refTypeIndex;
	
	public OpNewArray(Operation operation, int offset, int cnt, Type arrayType, int refTypeIndex) {
		super(operation, offset, cnt);
		this.arrayType = arrayType;
		this.refTypeIndex = refTypeIndex;
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		if (arrayType==Type.REFERENCE) {
			fillSpaces(buf).append(refTypeIndex);
		} else {
			fillSpaces(buf);
			switch(arrayType) {
				case BOOLEAN : buf.append("boolean");	break;
				case CHAR : buf.append("char");	break;
				case FLOAT : buf.append("float");	break;
				case DOUBLE : buf.append("double");	break;
				case BYTE : buf.append("byte");	break;
				case SHORT : buf.append("short");	break;
				case INT : buf.append("int");	break;
				case LONG : buf.append("long");	break;
			}
		}
		return buf.toString();
	}
}
