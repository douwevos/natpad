package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;

public class OpMonitor extends AbstractMnemonic {

	public static final short OPPCODE_MONITOR_ENTER	= 0xc2;
	public static final short OPPCODE_MONITOR_EXIT	= 0xc3;
	
	
	public OpMonitor(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
	}
	

	@Override
	public String toString() {
	    return operation.text;
	}
	
}
