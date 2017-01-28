package net.natpad.avaj.classfile.bytecode;


public interface IMnemonic {

	public Operation getOperation();
	
	public int getOffset();
	public int getLength();

	public int getContinuesOffset();
	public int getBranchOffset();
	
	public String toString(IMnemonicBlock imnemonicBlock);
	public short getOppCode();
	
}
