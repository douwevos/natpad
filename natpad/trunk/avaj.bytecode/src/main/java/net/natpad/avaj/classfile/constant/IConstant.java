package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;


public interface IConstant {


	public boolean isResolved();
	
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException;
}
