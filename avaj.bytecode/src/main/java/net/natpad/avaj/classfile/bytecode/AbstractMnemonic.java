package net.natpad.avaj.classfile.bytecode;



public abstract class AbstractMnemonic implements IMnemonic {

	public final Operation operation;
	public final int offset;
	public final int length;
	
	
	public AbstractMnemonic(Operation operation, final int offset, final int length) {
		this.operation = operation;
		this.offset = offset;
		this.length = length;
	}
	
	
	@Override
	public Operation getOperation() {
		return operation;
	}
	
	
	@Override
	public short getOppCode() {
		return (short) operation.opcode;
	}
	
	@Override
	public int getOffset() {
		return offset;
	}
	
	@Override
	public int getContinuesOffset() {
		return offset+length;
	}
	
	@Override
	public int getBranchOffset() {
		return -1;
	}
	
	@Override
	public int getLength() {
		return length;
	}
	
	public static StringBuilder fillSpaces(StringBuilder buf) {
		return fillSpaces(buf, 20);
	}
	
	public static StringBuilder fillSpaces(StringBuilder buf, int pos) {
		while(buf.length()<pos) {
			buf.append(' ');
		}
		return buf;
	}
	
	@Override
	public String toString(IMnemonicBlock imnemonicBlock) {
		return toString();
	}

}
