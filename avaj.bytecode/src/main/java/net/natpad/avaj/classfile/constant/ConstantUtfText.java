package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;


public class ConstantUtfText implements IConstant {

	
	public final String text;
	
	public ConstantUtfText(String text) {
		this.text = text;
	}
	
	@Override
	public boolean isResolved() {
		return true;
	}
	
	
	@Override
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException {
		return true;
	}
	
	
	@Override
	public String toString() {
		return "ConstantUtfText<text="+text+">"; 
	}
}
