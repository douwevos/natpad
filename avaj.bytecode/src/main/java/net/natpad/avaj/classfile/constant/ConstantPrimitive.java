package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;

public class ConstantPrimitive<P> implements IConstant {

	public final P value;
	
	public ConstantPrimitive(P value) {
		this.value = value;
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
		return "ConstantPrimitive[value="+value+"]";
	}
	
}
