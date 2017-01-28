package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;

public class ConstantPrimString implements IConstant {

	public final int nameIndex;
	private ConstantUtfText constantUtfText;
	
	public ConstantPrimString(int nameIndex) {
		this.nameIndex = nameIndex;
	}
	
	private boolean isResolved = false;
	
	@Override
	public boolean isResolved() {
		return isResolved;
	}
	
	public String getText() {
		return constantUtfText.text;
	}
	
	@Override
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException {
		constantUtfText = (ConstantUtfText) constantPool.getUnresolved(nameIndex-1);
		isResolved = true;
		return true;
	}
	
	@Override
	public String toString() {
		return "ConstantPrimString[constantUtfText="+constantUtfText+", nameIndex="+nameIndex+"]";
	}
	
}
