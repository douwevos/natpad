package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpMathLogic extends AbstractMnemonic {

	public final Operator operator;
	public final Type type;
	
	public OpMathLogic(Operation operation, Type type, Operator operator, int offset, int cnt) {
		super(operation, offset, cnt);
		this.operator = operator;
		this.type = type;
	}
	

	@Override
	public String toString() {
		return operation.text;
	}
}
