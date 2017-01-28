package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;

public class OpInvoke extends AbstractMnemonic {

	public static final short OPP_CODE_INTERFACE		= 0xb9;
	public static final short OPP_CODE_SPECIAL			= 0xb7;
	public static final short OPP_CODE_STATIC			= 0xb8;
	public static final short OPP_CODE_VIRTUAL			= 0xb6;
	
	public final int index;
	
	int count;
	
	public OpInvoke(Operation operation, int offset, int index) {
		super(operation, offset, operation==Operation.OP_INVOKEDYNAMIC ? 5 : 3);
		this.index = index;
		this.count = 0;
	}

	public OpInvoke(int offset, int index, int count) {
		super(Operation.OP_INVOKEINTERFACE, offset, 5);
		this.index = index;
		this.count = count;
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		
		fillSpaces(buf).append(index);
		if (operation == Operation.OP_INVOKEINTERFACE) {
			buf.append(", ").append(index);
		}
		return buf.toString();
	}
	
}
