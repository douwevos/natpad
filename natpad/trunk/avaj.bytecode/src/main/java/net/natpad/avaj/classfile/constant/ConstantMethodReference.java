package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;

import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.util.TextUtil;



public class ConstantMethodReference implements IConstant {

	public final int classIndex;
	public final int nameAndTypeIndex;
	private ConstantClassInfo constantClassInfo;
	private ConstantNameAndType constantNameAndType;
	
	private ClassfileMethodHeader methodHeader;
	
	public ConstantMethodReference(int classIndex, int nameAndTypeIndex) {
		this.classIndex = classIndex;
		this.nameAndTypeIndex = nameAndTypeIndex;
	}

	private boolean isResolved = false;
	
	@Override
	public boolean isResolved() {
		return isResolved;
	}
	
//	public String getClassName() {
//		return constantClassInfo.getText();
//	}
	
	public ConstantClassInfo getConstantClassInfo() {
		return constantClassInfo;
	}
	
	public ConstantNameAndType getConstantNameAndType() {
		return constantNameAndType;
	}
	
	
	public ClassfileMethodHeader getMethodHeader() {
		return methodHeader;
	}
	
	
	@Override
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException {
		constantClassInfo = (ConstantClassInfo) constantPool.getUnresolved(classIndex-1);
		boolean result = true;
		if (!constantClassInfo.isResolved()) {
			if (resolveStack.contains(constantClassInfo)) {
				throw new RecursiveConstantsException();
			}
			resolveStack.add(constantClassInfo);
			result = false;
		}
		constantNameAndType = (ConstantNameAndType) constantPool.getUnresolved(nameAndTypeIndex-1);
		if (!constantNameAndType.isResolved()) {
			if (resolveStack.contains(constantNameAndType)) {
				throw new RecursiveConstantsException();
			}
			resolveStack.add(constantNameAndType);
			result = false;
		}
		
		ConstantClassInfo thisClassConstant = constantPool.getThisClassConstant();
		if (!thisClassConstant.isResolved()) {
			if (resolveStack.contains(thisClassConstant)) {
				throw new RecursiveConstantsException();
			}
			resolveStack.add(thisClassConstant);
			result = false;
		}
		
		if (result) {
			String methodName = constantNameAndType.getName();
			methodHeader = new ClassfileMethodHeader(thisClassConstant.getRefrenceTypeName(), methodName, constantNameAndType.getTypeDescription());
			
			isResolved = true;
		}
		return result;
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder(TextUtil.getName(getClass())).append("[");
		buf.append("constantClassInfo=").append(constantClassInfo);
		buf.append(", constantNameAndType=").append(constantNameAndType);
		return buf.append("]").toString();
	}
}
