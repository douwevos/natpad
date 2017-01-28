package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpLoadIndex extends AbstractMnemonic {

	
	public final Type loadType;
	public final int index;
	public final boolean is_wide;
	
	public OpLoadIndex(Operation operation, int offset, int index, boolean is_wide) {
		super(operation, offset, is_wide ? 3 : 2);
		switch(operation) {
			case OP_ILOAD : loadType = Type.INT; break;
			case OP_LLOAD : loadType = Type.LONG; break;
			case OP_FLOAD : loadType = Type.FLOAT; break;
			case OP_DLOAD : loadType = Type.DOUBLE; break;
			case OP_ALOAD : loadType = Type.REFERENCE; break;
			default : loadType = Type.INT; break;
		}
		this.index = index;
		this.is_wide = is_wide;
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf).append(index);
		return buf.toString();
	}
}
