package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;

public class OpLoadFastIndex extends AbstractMnemonic {

	
	public final Type loadType;
	public final int index;
	
	public OpLoadFastIndex(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
		switch(operation) {
			case OP_ILOAD_0 : loadType = Type.INT; 	index=0; break;
			case OP_ILOAD_1 : loadType = Type.INT; 	index=1; break;
			case OP_ILOAD_2 : loadType = Type.INT; 	index=2; break;
			case OP_ILOAD_3 : loadType = Type.INT; 	index=3; break;
			case OP_LLOAD_0 : loadType = Type.LONG;	index=0; break;
			case OP_LLOAD_1 : loadType = Type.LONG;	index=1; break;
			case OP_LLOAD_2 : loadType = Type.LONG;	index=2; break;
			case OP_LLOAD_3 : loadType = Type.LONG;	index=3; break;
			case OP_FLOAD_0 : loadType = Type.FLOAT;	index=0; break;
			case OP_FLOAD_1 : loadType = Type.FLOAT;	index=1; break;
			case OP_FLOAD_2 : loadType = Type.FLOAT;	index=2; break;
			case OP_FLOAD_3 : loadType = Type.FLOAT;	index=3; break;
			case OP_DLOAD_0 : loadType = Type.DOUBLE;	index=0; break;
			case OP_DLOAD_1 : loadType = Type.DOUBLE;	index=1; break;
			case OP_DLOAD_2 : loadType = Type.DOUBLE;	index=2; break;
			case OP_DLOAD_3 : loadType = Type.DOUBLE;	index=3; break;
			case OP_ALOAD_0 : loadType = Type.REFERENCE;	index=0; break;
			case OP_ALOAD_1 : loadType = Type.REFERENCE;	index=1; break;
			case OP_ALOAD_2 : loadType = Type.REFERENCE;	index=2; break;
			case OP_ALOAD_3 : loadType = Type.REFERENCE;	index=3; break;
			default : loadType = Type.INT; index=0; break;
		}
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		return buf.toString();
	}
	
}
