package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;

public class OpDuplicate extends AbstractMnemonic {

	/* TODO DupType is deprecated ... remove it */
	
	public enum DupType {
		DUP(0x59),
		DUP_X1(0x5a),
		DUP_X2(0x5b),
		DUP2(0x5c),
		DUP2_X1(0x5d),
		DUP2_X2(0x5e);
		
		public short oppcode;
		
		DupType(int oppcode) {
			this.oppcode = (short) oppcode;
		}
		
		static DupType getByOppCode(short oppcode) {
			switch(oppcode) {
				case 0x59 :  return DUP;
				case 0x5a :  return DUP_X1;
				case 0x5b :  return DUP_X2;
				case 0x5c :  return DUP2;
				case 0x5d :  return DUP2_X1;
				case 0x5e :  return DUP2_X2;
			}
			return DUP;
		}
		
	}
	
	
	public DupType dupType;
	
	public OpDuplicate(Operation operation, int offset, int cnt) {
		super(operation, offset, cnt);
		this.dupType = DupType.getByOppCode((short) operation.opcode);
	}
	
	@Override
	public short getOppCode() {
		return dupType.oppcode;
	}
	
	@Override
	public String toString() {
		return ""+operation.text;
	}
	
	
}
