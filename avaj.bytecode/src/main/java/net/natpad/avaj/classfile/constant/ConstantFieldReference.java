package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileField;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;


public class ConstantFieldReference implements IConstant {

	static Logger log = Logger.getInstance(Classfile.class);
	
	public final int classIndex;
	public final int nameAndTypeIndex;
	private ConstantClassInfo constantClassInfo;
	private ConstantNameAndType constantNameAndType;
	private DeclarationType declarationType;
	
	public ConstantFieldReference(int classIndex, int nameAndTypeIndex) {
		this.classIndex = classIndex;
		this.nameAndTypeIndex = nameAndTypeIndex;
	}

	private boolean isResolved = false;
	
	@Override
	public boolean isResolved() {
		return isResolved;
	}
	
	@Override
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException {
		log.detail("classIndex="+classIndex);
		constantClassInfo = (ConstantClassInfo) constantPool.getUnresolved(classIndex-1);
		constantNameAndType = (ConstantNameAndType) constantPool.getUnresolved(nameAndTypeIndex-1);
		
		boolean result = true;
		
		if (!constantClassInfo.isResolved()) {
			result = false;
			if (resolveStack.contains(constantClassInfo)) {
				throw new RecursiveConstantsException();
			}
			resolveStack.add(constantClassInfo);
		}
		if (!constantNameAndType.isResolved()) {
			result = false;
			if (resolveStack.contains(constantNameAndType)) {
				throw new RecursiveConstantsException();
			}
			resolveStack.add(constantNameAndType);
		}
		
		if (result) {
			declarationType = ClassfileField.parseDescriptor(constantNameAndType.getTypeDescription());
			isResolved = true;
		}
		return result;
	}
	
//	public String getClassName() {
//		return constantClassInfo.getText();
//	}

	public DeclarationType getFieldSourceDescription() {
		return constantClassInfo.getParsedDescriptor();
	}
	
	public String getFieldName() {
		return constantNameAndType.getName();
	}
	
	
	public DeclarationType getDeclarationType() {
		return declarationType;
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder(TextUtil.getName(getClass())).append("[");
		buf.append("constantClassInfo=").append(constantClassInfo);
		buf.append(", classIndex=").append(classIndex);
		buf.append(", constantNameAndType=").append(constantNameAndType);
		buf.append(", nameAndTypeIndex=").append(nameAndTypeIndex);
		return buf.append("]").toString();
	}
	
}
