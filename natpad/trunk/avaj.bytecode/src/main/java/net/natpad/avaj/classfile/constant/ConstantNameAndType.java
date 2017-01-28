package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;

import net.natpad.avaj.util.TextUtil;

public class ConstantNameAndType implements IConstant {

	public final int nameIndex, descriptorIndex;
	private ConstantUtfText constantName, constantDescriptor;
	
	public ConstantNameAndType(int nameIndex, int descriptorIndex) {
		this.nameIndex = nameIndex;
		this.descriptorIndex = descriptorIndex;
	}

	public String getName() {
		return constantName.text;
	}
	
	public String getTypeDescription() {
		return constantDescriptor.text;
	}
	
	private boolean isResolved;
	@Override
	public boolean isResolved() {
		return isResolved;
	}

	
	@Override
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException {
		constantName = (ConstantUtfText) constantPool.getUnresolved(nameIndex-1);
		constantDescriptor = (ConstantUtfText) constantPool.getUnresolved(descriptorIndex-1);
		isResolved = true;
		return true;
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder(TextUtil.getName(getClass())).append("[");
		buf.append("constantName=").append(constantName);
		buf.append(", constantDescriptor").append(constantDescriptor);
		return buf.append("]").toString();
	}

}
