package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpValueConvert extends AbstractMnemonic {

	public final Type source, destination;
	
	
	public OpValueConvert(Operation operation, Type source, Type destination, int offset, int cnt) {
		super(operation, offset, cnt);
		this.source = source;
		this.destination = destination;
	}
	
	
	@Override
	public String toString() {
		return operation.text;
	}
}
